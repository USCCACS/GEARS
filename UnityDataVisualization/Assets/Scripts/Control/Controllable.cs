using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace CACS
{
  public class Controllable : MonoBehaviour
  {

    public enum Info { POSITION, ANGLES, SCALE };
    protected Dictionary<Info, Vector3> information;

    /*Initial state (for reset)*/
    private Vector3 initialPos;
    private Vector3 initialRot;
    private Vector3 initialScale;

    /*Effective macros*/
    private float MIN_CONTROLLABLE_SIZE = 0.5f;
    private float MAX_CONTROLLABLE_SIZE = 50f;

    void Start() {
      information = new Dictionary<Info, Vector3>();

      initialPos = transform.position;
      initialRot = transform.eulerAngles;
      initialScale = transform.localScale;
    }

    // Update is called once per frame
    void Update() {
    }

    /* -------------------------------------------------------------------------------
     * Essential tranformation methods
     * -------------------------------------------------------------------------------*/
    public virtual void rotate(float x, float y, float sensitivity) {
      Vector3 angularMomentumVector = Vector3.down * x + Vector3.right * y;
      transform.RotateAround(transform.position, angularMomentumVector,
                              angularMomentumVector.magnitude * sensitivity);
    }

    public virtual void translate(Vector3 dir, float sensitivity) {
    }

    public virtual void scaleSize(float s) {
      Vector3 newSize = transform.localScale + (s * new Vector3(1, 1, 1));
      bool isAboveLowerBound = (newSize.x > MIN_CONTROLLABLE_SIZE) &&
        (newSize.y > MIN_CONTROLLABLE_SIZE) && (newSize.z > MIN_CONTROLLABLE_SIZE);
      bool isBelowUpperBound = (newSize.x < MAX_CONTROLLABLE_SIZE) &&
        (newSize.x < MAX_CONTROLLABLE_SIZE) && (newSize.x < MAX_CONTROLLABLE_SIZE);

      if (isAboveLowerBound && isBelowUpperBound)
        transform.localScale = newSize;
      else if (isAboveLowerBound && !isBelowUpperBound)
        transform.localScale = MAX_CONTROLLABLE_SIZE * new Vector3(1, 1, 1);
      else if (!isAboveLowerBound && isBelowUpperBound)
        transform.localScale = MIN_CONTROLLABLE_SIZE * new Vector3(1, 1, 1);
    }

    public virtual void reset() {
      transform.position = initialPos;
      transform.eulerAngles = initialRot;
      transform.localScale = initialScale;
    }
    /*--------------------------------------------------------------------------------*/

    /*Info/data to be displayed to users*/
    public virtual Dictionary<Info, Vector3> getControllableInfo() {
      information[Info.POSITION] = transform.position;
      information[Info.ANGLES] = transform.eulerAngles;
      information[Info.SCALE] = transform.localScale;
      return information;
    }
  }

}
