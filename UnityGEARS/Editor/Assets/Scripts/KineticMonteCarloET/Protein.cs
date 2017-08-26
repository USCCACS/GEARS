/**************************************************************
 * Disclaimer: This code is terribly written and commented. 
 * I apologize for the poor choice of variable and function names. 
 **************************************************************/

using UnityEngine;
using UnityEngine.UI;
using System.Collections.Generic;
using System.Threading;

public class Protein : MonoBehaviour {

	/*Input files for the protein*/
	public string tm_filename = "Input/rhop.txt";
	public string location_filename;// = "Input/hemes.xyz";

	/*Event related variables (used to render events and alter speeds)*/
	private KineticMonteCarlo.Event currentEvent = KineticMonteCarlo.Event.NULLEVENT;
	public Slider slider;
	private float sliderSpeed = 0.01f;
	public static double frameDuration = 1e-6;

	/*Heme and game object variables*/
	public GameObject spawnHeme;
	private List<Heme> my_babies;
	public Vector3 center;
	private TransitionMatrix tm;

	/*Starting position for the reset button*/
	private Vector3 startPos;
	private Vector3 startRot;

	// Use this for initialization
	void Start () {
		initHemes (location_filename);
		tm = new TransitionMatrix (tm_filename, my_babies.ToArray());
		Thread thread = new Thread (tm.run);
		thread.Start ();
	}
	
	// Update is called once per frame
	void Update () {
		//Update the position of the protein
		center = Vector3.zero;
		for (int i=0; i<my_babies.Count; ++i) {
			center += my_babies[i].transform.position;
		}
		center /= my_babies.Count;

        // Update the slider value if someone pressed an arrow key
        if (Input.GetKeyDown(KeyCode.LeftArrow))
            slider.value -= 0.1f;
        else if (Input.GetKeyDown(KeyCode.RightArrow))
            slider.value += 0.1f;

		//Calculate the rendering speed
		double adjustment = Mathf.Pow(10, slider.value) * frameDuration;
		//Debug.Log("Delay: " + currentEvent.getDelay () + "\nAdjustment: " + adjustment + "\nFrame Duration: " + frameDuration);

		//Render events depending on their delay and the slider position
		if (slider.value == -2) {
      //Stop the animation
      return;
		} else if (currentEvent.getDelay () < adjustment) {
			//Do the animation
			currentEvent.renderEvent (tm);
			currentEvent = tm.dequeueEvent ();
		} else {
			//skip this frame
			currentEvent.setDelay (currentEvent.getDelay () - adjustment);
		}
	}

  public void initHemes(string filename) {
    my_babies = new List<Heme>();
    center = new Vector3(0, 0, 0);
    XYZFile file = new XYZFile(filename);

    XYZFile.Frame hemeCoords = file.Frames[0];            //there should only be 1 frame in this particular file
    foreach (KeyValuePair<string, Vector3> coord in hemeCoords.Positions) {
      Heme newHeme = ((GameObject)Instantiate(spawnHeme, transform.position + coord.Value, transform.rotation)).GetComponent<Heme>();
      newHeme.transform.SetParent(transform);
      center += newHeme.transform.position;
      my_babies.Add(newHeme);
    }

    center /= my_babies.Count;
    startPos = center;
    startRot = transform.eulerAngles;
  }

  public Vector3 getCenter() {
    return center;
  }

  public void resetPosition() {
    transform.position = startPos;
    transform.eulerAngles = startRot;
  }

  public void slowDownAnimation() {
    slider.value -= sliderSpeed;
  }
  public void speedUpAnimation() {
    slider.value += sliderSpeed;
  }
}
