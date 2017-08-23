using UnityEngine;
using System;
using System.Collections.Generic;
using System.Threading;

public class TransitionMatrix {

    private long time = 0;
	private int nHemes;
	private Heme[] hemes;
    private double[,] transfer;     	/*Stores rates of electron transfer from heme to heme*/
    private double[] injection;         /*Stores rates of injection for each heme*/
    private double[] ejection;          /*Stores rates of ejection for each heme*/
	private bool[] animationState;		/*boolean area to store state of animation in the case of a buffer flush*/
	private bool[] currentState;		/*boolean array to determine heme occupancy*/

	private Queue<KineticMonteCarlo.Event> buffer;			/*contains all the events to be rendered*/

	private System.Random rand;

    /*Constructor*/
    public TransitionMatrix(string input_file, Heme[] hemes)
    {
		this.hemes = hemes;
		nHemes = hemes.Length;
        initialize_data(input_file);
    }

    /* Methods */
    public bool initialize_data(string input_file)
    {
        try
        {
            System.IO.StreamReader sr = new System.IO.StreamReader(input_file);
            string[] line;

            /*If error occured*/
            if (sr == null) return false;

            /*Declare total number of hemes in simulation
            and assign member variables */
            transfer = new double[nHemes, nHemes];
            injection = new double[nHemes];
            ejection = new double[nHemes];
			currentState = new bool[nHemes];
			animationState = new bool[nHemes];

			rand = new System.Random();

            /*Start assigning rate values*/
            for (int i = 0; i < nHemes; i++)
            {
                for (int j = 0; j < nHemes; j++)
                {
                    if (i == j) transfer[i,j] = 0;
                    else
                    {
                        line = sr.ReadLine().Split(new[] {' '});
                        if (line == null) return false;

                        int row = int.Parse(line[0]) - 1;
                        int col = int.Parse(line[1]) - 1;
                        double k = Double.Parse(line[2]);
                        transfer[row, col] = k;
                    }
                }
            }

            /*Assign occupied booleans and injection/ejection rates*/
            for (int i=0; i < nHemes; i++)
            {
                injection[i] = 0;
                ejection[i] = 0;
				currentState[i] = false;
            }

            //sample injection/ejection rates for debugging
            injection[0] = 150000;
            ejection[nHemes - 1] = 150000;

			//initialize buffer queue
			buffer = new Queue<KineticMonteCarlo.Event>();

        }
        catch (Exception e)
        {
            Debug.Log(e.ToString());
			return false;
        }

        return true;
    }

	public void run()
	{
		while (true) {
			Monitor.Enter (buffer);
			if(buffer.Count < 500) {
				Monitor.Exit (buffer);
				runIteration();
			}
			else {
				Monitor.Wait (buffer);
				Monitor.Exit (buffer);
			}
		}
	}

	/*Runs a single transfer sequence of the KMC*/
	public void runIteration() {

			double totalRate = 0;
		
			/*Build queue of valid Events*/
			Queue<KineticMonteCarlo.Event> events = new Queue<KineticMonteCarlo.Event> ();
			for (int i = 0; i < nHemes; ++i) {
				//if the ith heme is occupied, then ejection and transfer are possible
				if (currentState [i]) {
					//queue up an ejection from this heme if it is possible (i.e. non-zero rate)
					if (ejection [i] != 0) {
						events.Enqueue (new KineticMonteCarlo.EjectionEvent (ejection [i], i));
						totalRate += ejection [i];
					}
					//iterate through all hemes
					for (int j = 0; j < nHemes; ++j)
					//if a transfer from heme i to heme j is possible, queue up the corresponding transfer event
						if (transfer [i, j] != 0) {
							events.Enqueue (new KineticMonteCarlo.TransferEvent (transfer [i, j], i, j));
							totalRate += transfer [i, j];
						}
				}
			//if the ith heme is unoccupied, the only possible event is an injection
			else {
					//if the injection rate is non-zero, queue up the corresponding injection event
					if (injection [i] != 0) {
						events.Enqueue (new KineticMonteCarlo.InjectionEvent (injection [i], i));
						totalRate += injection [i];
					}
				}
			}
		
			/*Select which Event to run*/
			double eventRand = rand.NextDouble () * totalRate; //select a random number between 0 and the total accrued rate of all queued events
			double accumulatedRate = 0;
			double eventDelay = -Math.Log (((double)rand.Next ()) / ((double)Int32.MaxValue)) / totalRate; //calculate how much time this event will take up


            //iterate through the events and sum their rates until the total sum exceeds eventRand.
            //The event at which this happens is the selected event.
            KineticMonteCarlo.Event e;
			for (int i = events.Count; i > 0; --i) {
				e = events.Dequeue ();
				accumulatedRate += e.getRate ();
				if (accumulatedRate >= eventRand) {
					e.setDelay (eventDelay);
					e.processEvent (this);
					lock(buffer) {
						buffer.Enqueue(e);
					}
					break;
				}
			}

			/*Data Collection
			for (int i = 0; i < n; ++i) {
				if (currentState) {
					totalOccupationTime [i] += eventDelay;
					++totalOccupationIterations [i];
				}
			}*/
		
	}

    /*Accessors*/
    public double getTransferRate(int from_heme, int to_heme)
    {
		/*New Method (Buffer Based)*/
		if(currentState[from_heme]==true && currentState[to_heme]==false)
			return transfer[from_heme, to_heme];
		else
			return 0;

		/*Old Method (Frame Based)
        if(hemes[from_heme].isOccupied() && !hemes[to_heme].isOccupied())
            return transfer[from_heme, to_heme];
        else
            return 0;*/
    }

	public void setCurrentState(int heme, bool val)
	{
		lock (currentState) {
			currentState [heme] = val;
		}
	}

	public void setAnimationState(int heme, bool val)
	{
		lock (animationState) {
			animationState[heme] = val;
		}
	}

	public KineticMonteCarlo.Event dequeueEvent()
	{
        KineticMonteCarlo.Event e;
		lock (buffer) {
			if (buffer.Count > 0) {
				e = buffer.Dequeue ();
			}
			else {
				e = KineticMonteCarlo.Event.NULLEVENT;
			}

			Monitor.Pulse (buffer);
		}

		return e;
	}

	/*Resets state of hemes when a user changes parameters mid-simulation*/
	public void clearBuffer() {
		lock (currentState) {
			lock (animationState) {
				lock(buffer) {
					buffer.Clear ();

					for(int i = 0; i < nHemes; ++i) {
						currentState[i] = animationState[i];
					}

					Monitor.Pulse (buffer);
				}
			}
		}
	}

    public double getTimeElapsed()
    {
        return time;
    }

    public int getNumHemes() {
        return nHemes;
    }

	public Heme getHemeObject(int heme) {
		return hemes [heme];
	}

    // Use this for initialization
    void Start () {

    }

    // Update is called once per frame
    void Update () {

    }
}
