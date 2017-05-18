using UnityEngine;
using System.Collections;

namespace CACS
{
  public class ControllablePlane : Controllable
  {

    private float viewingDepth = 0.8f;

    // Use this for initialization
    void Start() {
    }

    // Update is called once per frame
    void Update() {
    }

    /* -----------------------------------------------------------
     * Accessors & Mutators
     * -----------------------------------------------------------*/
    public float getViewingDepth() {
      return viewingDepth;
    }

    public void setViewingDepth(float val) {
      viewingDepth = val;
    }

    public void incrementViewingDepth(float val) {
      viewingDepth += val;
    }

    public void setPlaneTransparency(float val) {
      Color temp = transform.GetComponent<Renderer>().sharedMaterial.color;
      temp.a = val;
      transform.GetComponent<Renderer>().sharedMaterial.SetColor("_Color", temp);
    }
    /*------------------------------------------------------------*/


    /* -----------------------------------------------------------
     * Transformation Methods
     * -----------------------------------------------------------*/
    public override void translate(Vector3 dir, float sensitivity) {
      transform.Translate(0, 0, Time.deltaTime * sensitivity, Space.Self);
    }
    /*------------------------------------------------------------*/

  }
}

