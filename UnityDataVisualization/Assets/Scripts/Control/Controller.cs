  using UnityEngine;
using UnityEngine.UI;
using System;
using System.Collections;
using System.Collections.Generic;

namespace CACS
{
  public abstract class Controller : MonoBehaviour
  {

    /*Member variables for access to in-game objects*/
    public List<GameObject> viewingObjects;
    public GameObject viewingPlane;
    public GameObject mainCamera;
        public Slider slider;

    protected List<ControllableObject> objects;
    protected ControllablePlane plane;
    protected ControllableCamera camera;

    /*Variables for adjustable values*/
    public float rotationSensitivity = 1.0f;
    public float zoomSensitivity = 0.5f;
    public float panSensitivity = 0.75f;

    // Use this for initialization
    protected virtual void Start() {
      Debug.Log("Controller Initializaing");
      objects = new List<ControllableObject>();
      objects.Add(viewingObjects[0].GetComponent<ControllableObject>());
      plane = viewingPlane.GetComponent<ControllablePlane>();
      camera = mainCamera.GetComponent<ControllableCamera>();

      Debug.Log("Membrane Active, Tag: " + viewingObjects[0].activeInHierarchy + ", " + viewingObjects[0].tag);
      //updateObjects();
    }

    // Update is called once per frame
    void Update() {

    }

    /* -------------------------------------------------------------------------------
     * Controller Initilaization Methods
     * -------------------------------------------------------------------------------*/
    public void addObjects(GameObject[] inputObjects) {
      for (int i = 0; i < inputObjects.Length; ++i) {
        viewingObjects.Add(inputObjects[i]);
        objects.Add(viewingObjects[i].GetComponent<ControllableObject>());
      }
    }

    public void addObject(GameObject inputObject) {
      viewingObjects.Add(inputObject);
      objects.Add(viewingObjects[viewingObjects.Count - 1].GetComponent<ControllableObject>());
    }

    public void updateObjects() {
      for (int i = 0; i < viewingObjects.Count; ++i) {
        if (viewingObjects[i] != null)
          objects.Add(viewingObjects[i].GetComponent<ControllableObject>());
      }
    }
    /* -------------------------------------------------------------------------------*/

    /* -------------------------------------------------------------------------------
     * Input Processing
     * -------------------------------------------------------------------------------*/
    protected abstract void getInput();
    protected abstract void resetInput();
    protected abstract void processInput(Controllable puppet);

    /* -------------------------------------------------------------------------------*/

    /* -------------------------------------------------------------------------------*/

    /* -------------------------------------------------------------------------------
     * Manual Control
     * -------------------------------------------------------------------------------*/
    protected void rotate(float x, float y, Controllable puppet) {
      Transform t = puppet.transform;
      Vector3 ang_mom = Vector3.down * x + Vector3.right * y;
      //t.transform.RotateAround (t.position, Vector3.down, x * Time.deltaTime * rotationSensitivity);
      //t.transform.RotateAround (t.position, Vector3.right, y * Time.deltaTime * rotationSensitivity);
      t.transform.RotateAround(t.position, ang_mom, ang_mom.magnitude * rotationSensitivity);
    }
    /* -------------------------------------------------------------------------------*/

  }
}
