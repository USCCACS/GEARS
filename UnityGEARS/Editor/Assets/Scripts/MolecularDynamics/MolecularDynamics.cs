using UnityEngine;
using System.Collections;
using System;


public class MolecularDynamics : MonoBehaviour
{

  int NMAX = 10000;  /* Maximum number of atoms which can be simulated */
  double RCUT = 2.5;     /* Potential cut-off length */
  double PI = 3.141592653589793;
  /* Constants for the random number generator */
  double D2P31M = 2147483647.0;
  double DMUL = 16807.0;
  System.Random rand;

  /* Input parameters (read from an input file in this order) *******************/

  public int[] InitUcell = { 5, 5, 5 };   /* Number of unit cells */
  public double Density = 0.08;     /* Number density of atoms (in reduced unit) */
  public double InitTemp = 0.1;    /* Starting temperature (in reduced unit) */
  private double DeltaT = 0.1;      /* Size of a time step (in reduced unit) */

  /* Constants ******************************************************************/

  double[] Region;  /* MD box lengths */
  double[] RegionH; /* Half the box lengths */
  double DeltaTH;    /* Half the time step */
  double slowDownFactor;
  double Uc, Duc;    /* Potential cut-off parameters */

  /* Variables ******************************************************************/

  int nAtom;            /* Number of atoms */
  double[,] r;          /* r[i][0|1|2] is the x|y|z coordinate of atom i */
  double[,] rv;         /* Atomic velocities */
  double[,] ra;         /* Acceleration on atoms */
  double kinEnergy;     /* Kinetic energy */
  double potEnergy;     /* Potential energy */
  double totEnergy;     /* Total energy */
  double temperature;   /* Current temperature */
  int stepCount;        /* Current time step */

  /* Unity Variables for rendering and manipulation */
  public GameObject baseParticle;
  public GameObject[] particles;

  public bool updated = true;
  public bool play = true;

  public MolecularDynamics() {

  }

  // Use this for initialization
  void Start() {
    InitParams();
    InitConf();
    ComputeAccel();
    slowDownFactor = 0.3f;
    
    /* This section handles spawning the particles into the game world as GameObjects so that they can be rendered */
    particles = new GameObject[nAtom];
    for (int i = 0; i < nAtom; ++i) {
      Vector3 position = new Vector3(System.Convert.ToSingle(r[i, 0]), System.Convert.ToSingle(r[i, 1]), System.Convert.ToSingle(r[i, 2]));

      GameObject newParticle = ((GameObject)Instantiate(baseParticle, position, transform.rotation));
      newParticle.transform.parent = this.transform;
      particles[i] = newParticle;
    }
  }

  // Update is called once per frame
  void Update() {
    DeltaT = Time.deltaTime * slowDownFactor;
    DeltaTH = 0.5 * DeltaT;

    SingleStep();

    /* Here is where the rendered positions get updated */
    for (int i = 0; i < nAtom; ++i) {
      Vector3 newPosition = new Vector3(System.Convert.ToSingle(r[i, 0]), System.Convert.ToSingle(r[i, 1]), System.Convert.ToSingle(r[i, 2]));
      particles[i].transform.position = transform.position + newPosition;
    }

    //Debug.Log("Position: " +  Math.Round(r[0,0], 2) + " " +  Math.Round(r[0,1], 2) + " " +  Math.Round(r[0,2], 2));
    //Debug.Log("Velocity: " +  Math.Round(rv[0,0], 2) + " " +  Math.Round(rv[0,1], 2) + " " +  Math.Round(rv[0,2], 2));
    //Debug.Log("Accel: " + Math.Round(ra[0,0], 2) + " " +  Math.Round(ra[0,1], 2) + " " +  Math.Round(ra[0,2], 2));
  }

  /*----------------------------------------------------------------------------*/
  void InitParams() {
    /*------------------------------------------------------------------------------
      Initializes parameters.
    ------------------------------------------------------------------------------*/
    int k;
    double rr, ri2, ri6, r1;

    r = new double[NMAX, 3];
    rv = new double[NMAX, 3];
    ra = new double[NMAX, 3];

    Region = new double[3];
    RegionH = new double[3];

    /*InitUcell = new int[3];
    for (int i = 0; i < 3; ++i) InitUcell[i] = 2;*/

    rand = new System.Random();

    /* Computes basic parameters */
    DeltaT = Time.deltaTime * slowDownFactor;
    DeltaTH = 0.5 * DeltaT * slowDownFactor;
    for (k = 0; k < 3; k++) {
      Region[k] = InitUcell[k] / Math.Pow(Density / 4.0, 1.0 / 3.0);
      RegionH[k] = 0.5 * Region[k];
    }

    /* Constants for potential truncation */
    rr = RCUT * RCUT; ri2 = 1.0 / rr; ri6 = ri2 * ri2 * ri2; r1 = Math.Sqrt(rr);
    Uc = 4.0 * ri6 * (ri6 - 1.0);
    Duc = -48.0 * ri6 * (ri6 - 0.5) / r1;
  }

  /*----------------------------------------------------------------------------*/
  void InitConf() {
    /*------------------------------------------------------------------------------
      r are initialized to face-centered cubic (fcc) lattice positions.
      rv are initialized with a random velocity corresponding to Temperature.
    ------------------------------------------------------------------------------*/
    double[] c = new double[3];
    double[] gap = new double[3];
    double[] e = new double[3];
    double[] vSum = new double[3];
    double vMag;
    int j, n, k, nX, nY, nZ;
    double seed;
    /* FCC atoms in the original unit cell */
    double[,] origAtom = {{0.0, 0.0, 0.0}, {0.0, 0.5, 0.5},
                           {0.5, 0.0, 0.5}, {0.5, 0.5, 0.0}};

    /* Sets up a face-centered cubic (fcc) lattice */
    for (k = 0; k < 3; k++) {
      gap[k] = Region[k] / InitUcell[k];
    }
    nAtom = 0;
    for (nZ = 0; nZ < InitUcell[2]; nZ++) {
      c[2] = nZ * gap[2];
      for (nY = 0; nY < InitUcell[1]; nY++) {
        c[1] = nY * gap[1];
        for (nX = 0; nX < InitUcell[0]; nX++) {
          c[0] = nX * gap[0];
          for (j = 0; j < 4; j++) {
            for (k = 0; k < 3; k++) {
              r[nAtom, k] = c[k] + gap[k] * origAtom[j, k];
            }
            ++nAtom;
          }
        }
      }
    }

    /* Generates random velocities */
    seed = 13597.0;
    vMag = Math.Sqrt(3 * InitTemp);
    for (k = 0; k < 3; k++) vSum[k] = 0.0;
    for (n = 0; n < nAtom; n++) {
      //RandVec3(ref e, ref seed);
      e[0] = rand.NextDouble(); e[1] = rand.NextDouble(); e[2] = rand.NextDouble();
      for (k = 0; k < 3; k++) {
        rv[n, k] = vMag * e[k];
        vSum[k] = vSum[k] + rv[n, k];
      }
    }

    /* Makes the total momentum zero */
    for (k = 0; k < 3; k++) vSum[k] = vSum[k] / nAtom;
    for (n = 0; n < nAtom; n++) for (k = 0; k < 3; k++) rv[n, k] = rv[n, k] - vSum[k];

  }

  /*----------------------------------------------------------------------------*/
  void ComputeAccel() {
    /*------------------------------------------------------------------------------
      Acceleration, ra, are computed as a function of atomic coordinates, r,
      using the Lennard-Jones potential.  The sum of atomic potential energies,
      potEnergy, is also computed.
    ------------------------------------------------------------------------------*/
    double[] dr = new double[3];
    double f, fcVal, rrCut, rr, ri2, ri6, r1;
    int j1, j2, n, k;

    rrCut = RCUT * RCUT;
    for (n = 0; n < nAtom; n++)  {
      for (k = 0; k < 3; k++) {
        ra[n, k] = 0.0;
      }
    }
    potEnergy = 0.0;

    /* Doubly-nested loop over atomic pairs */
    for (j1 = 0; j1 < nAtom - 1; j1++) {
      for (j2 = j1 + 1; j2 < nAtom; j2++) {
        /* Computes the squared atomic distance */
        for (rr = 0.0, k = 0; k < 3; k++) {
          dr[k] = r[j1, k] - r[j2, k];
          /* Chooses the nearest image */
          dr[k] = dr[k] - SignR(RegionH[k], dr[k] - RegionH[k])
                        - SignR(RegionH[k], dr[k] + RegionH[k]);
          rr = rr + dr[k] * dr[k];
        }
        /* Computes acceleration & potential within the cut-off distance */
        if (rr < (rrCut)) {
          ri2 = 1.0 / rr; ri6 = ri2 * ri2 * ri2; r1 = Math.Sqrt(rr);
          fcVal = 48.0 * ri2 * ri6 * (ri6 - 0.5) + Duc / r1;
          for (k = 0; k < 3; k++) {
            f = fcVal * dr[k];
            ra[j1, k] = ra[j1, k] + f;
            ra[j2, k] = ra[j2, k] - f;
          }
          potEnergy = potEnergy + 4.0 * ri6 * (ri6 - 1.0) - Uc - Duc * (r1 - RCUT);
        }
      }



      /***************************************************************************
       *Compute force due to Leap Hands ******************************************
       **************************************************************************
      double[,] leapPosition = new double[2,3];
      if (leapHands[0] != null) {
        leapPosition[0,0] = _pinchDetectors[0].Position.x;
        leapPosition[0,1] = _pinchDetectors[0].Position.y;
        leapPosition[0,2] = _pinchDetectors[0].Position.z;
        Debug.DrawLine(new Vector3(0,0,0), _pinchDetectors[0].Position, Color.white);
      }
      if (leapHands[1] != null) {
        leapPosition[1,0] = _pinchDetectors[1].Position.x;
        leapPosition[1,1] = _pinchDetectors[1].Position.y;
        leapPosition[1,2] = _pinchDetectors[1].Position.z;
        Debug.DrawLine(new Vector3(0,0,0), _pinchDetectors[1].Position, Color.green);
      }

      for (int l=0; l<2; ++l) {
        if (_pinchDetectors[l] != null && _pinchDetectors[l].IsActive) {
          for (rr = 0.0, k = 0; k < 3; k++) {
            dr[k] = r[j1, k] - leapPosition[l, k];
            /* Chooses the nearest image *//*
            dr[k] = dr[k] - SignR(RegionH[k], dr[k] - RegionH[k])
                          - SignR(RegionH[k], dr[k] + RegionH[k]);
            rr = rr + dr[k] * dr[k];
          }
          /* Computes acceleration & potential within the cut-off distance *//*
          if (rr < (rrCut*0.1)) {
            ri2 = 1.0 / rr; ri6 = ri2 * ri2 * ri2; r1 = Math.Sqrt(rr);
            fcVal = 48.0 * ri2 * ri6 * (ri6 - 0.5) + Duc / r1;
            for (k = 0; k < 3; k++) {
              f = fcVal * dr[k] * leapForceDamper;
              ra[j1, k] = ra[j1, k] + f;
              ra[j2, k] = ra[j2, k] - f;
            }
            potEnergy = potEnergy + 4.0 * ri6 * (ri6 - 1.0) - Uc - Duc * (r1 - RCUT);
          }
        }
      }*/
    }
  }

  /*----------------------------------------------------------------------------*/
  private void SingleStep() {
    /*------------------------------------------------------------------------------
      r & rv are propagated by DeltaT in time using the velocity-Verlet method.
    ------------------------------------------------------------------------------*/
    int n, k;

    HalfKick(); /* First half kick to obtain v(t+Dt/2) */
    for (n = 0; n < nAtom; n++) /* Update atomic coordinates to r(t+Dt) */
      for (k = 0; k < 3; k++) r[n, k] = r[n, k] + DeltaT * rv[n, k];
    ApplyBoundaryCond();
    ComputeAccel(); /* Computes new accelerations, a(t+Dt) */
    HalfKick(); /* Second half kick to obtain v(t+Dt) */
  }

  /*----------------------------------------------------------------------------*/
  void HalfKick() {
    /*------------------------------------------------------------------------------
      Accelerates atomic velocities, rv, by half the time step.
    ------------------------------------------------------------------------------*/
    int n, k;
    for (n = 0; n < nAtom; n++)
      for (k = 0; k < 3; k++) rv[n, k] = rv[n, k] + DeltaTH * ra[n, k];
  }

  /*----------------------------------------------------------------------------*/
  void ApplyBoundaryCond() {
    /*------------------------------------------------------------------------------
      Applies periodic boundary conditions to atomic coordinates.
    ------------------------------------------------------------------------------*/
    int n, k;
    for (n = 0; n < nAtom; n++)
      for (k = 0; k < 3; k++)
        r[n, k] = r[n, k] - SignR(RegionH[k], r[n, k])
                          - SignR(RegionH[k], r[n, k] - Region[k]);
  }

  /*----------------------------------------------------------------------------*/
  void EvalProps() {
    /*------------------------------------------------------------------------------
      Evaluates physical properties: kinetic, potential & total energies.
    ------------------------------------------------------------------------------*/
    double vv;
    int n, k;

    kinEnergy = 0.0;
    for (n = 0; n < nAtom; n++) {
      vv = 0.0;
      for (k = 0; k < 3; k++)
        vv = vv + rv[n, k] * rv[n, k];
      kinEnergy = kinEnergy + vv;
    }
    kinEnergy *= (0.5 / nAtom);
    potEnergy /= nAtom;
    totEnergy = kinEnergy + potEnergy;
    temperature = kinEnergy * 2.0 / 3.0;

    /* Print the computed properties */
    Debug.Log(stepCount * DeltaT + " " + temperature + " " + potEnergy + " " + totEnergy);
  }



  /* Useful Side Functions ********************************************/
  double SignR(double v, double x) { if (x > 0) return v; else return -v; }
  double Dmod(double a, double b) {
    int n;
    n = (int)(a / b);
    return (a - b * n);
  }
  double RandR(ref double seed) {
    seed = Dmod(seed * DMUL, D2P31M);
    return (seed / D2P31M);
  }
  void RandVec3(ref double[] p, ref double seed) {
    double x = 0, y = 0, s = 2.0;
    while (s > 1.0) {
      x = 2.0 * RandR(ref seed) - 1.0; y = 2.0 * RandR(ref seed) - 1.0; s = x * x + y * y;
    }
    p[2] = 1.0 - 2.0 * s; s = 2.0 * Math.Sqrt(1.0 - s); p[0] = s * x; p[1] = s * y;
  }
}
