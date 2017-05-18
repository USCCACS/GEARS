using UnityEngine;
using System.Collections;


namespace CACS
{
  public class ControllableObject : Controllable
  {

    private float visibleAlpha = 1.0f;
    private float transparentAlpha = 0.0f;

    // Use this for initialization
    void Start() {

    }

    // Update is called once per frame
    void Update() {

    }

    /* -----------------------------------------------------------
     * Accessors & Mutators
     * -----------------------------------------------------------*/
    public float getVisibiltyLevel() {
      return visibleAlpha;
    }

    public float getTransparencyLevel() {
      return transparentAlpha;
    }

    public void setVisibiltyLevel(float val) {
      visibleAlpha = val;
    }

    public void setTransparencyLevel(float val) {
      transparentAlpha = val;
    }
    /*------------------------------------------------------------*/
  }

}
