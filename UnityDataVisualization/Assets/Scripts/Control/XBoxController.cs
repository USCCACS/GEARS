using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;

namespace CACS
{
  public class XBoxController : Controller
  {
    // XBox Controller Specific Inputs
    private float scrollchange = 0;
    private float left_horiz = 0;
    private float right_vert = 0;
    private float right_horiz = 0;
    private float left_trig = 0;
    private float right_trig = 0;

    // To account for loose/used joysticks
    private float looseJoystickParam = 0.2f;

    // Use this for initialization
    new void Start() {
      base.Start();
    }

    // Update is called once per frame
    void Update() {
      getInput();
            //processInput(objects[0]);
            //processInput(plane);
            processInput();
      resetInput();
            if (Input.GetButton("RightBumper"))
            {
                slider.value = slider.value + 0.01f;
                Debug.Log("You pressed the right bumper");
            } else if (Input.GetButton("LeftBumper"))
            {
                slider.value = slider.value - 0.01f;
                Debug.Log("You pressed the left bumper");
            }
    }

    /* -------------------------------------------------------------------------------
     * Input Handling
     * -------------------------------------------------------------------------------*/
    protected override void getInput() {
      //Get Controller Inputs
      scrollchange = Input.GetAxis("LeftJoyVert");
      left_horiz = Input.GetAxis("LeftJoyHoriz");

      right_vert = Input.GetAxis("RightJoyVert");
      right_horiz = Input.GetAxis("RightJoyHoriz");

      left_trig = Input.GetAxis("LeftTrigger");
      right_trig = Input.GetAxis("RightTrigger");
    }

    protected override void resetInput() {
      scrollchange = 0f;
      left_horiz = 0f;

      right_vert = 0f;
      right_horiz = 0f;

      left_trig = 0f;
      right_trig = 0f;
    }

    protected override void processInput(Controllable puppet) {
      if (puppet != null) {
        if (Math.Abs(right_horiz) > looseJoystickParam || Math.Abs(right_vert) > looseJoystickParam) {
          puppet.rotate(right_horiz, right_vert, rotationSensitivity);
        }
        if (Math.Abs(scrollchange) > looseJoystickParam) {
          puppet.scaleSize(scrollchange * zoomSensitivity);
        }
      }
    }

        protected void processInput()
        {
            if (Math.Abs(left_horiz) > looseJoystickParam)
            {
                transform.RotateAround(transform.position, Vector3.up, left_horiz);
            }
        }

        private void deltaTimeInputs() {
      scrollchange = scrollchange * Time.deltaTime;
      left_horiz *= Time.deltaTime;

      right_vert *= Time.deltaTime;
      right_horiz *= Time.deltaTime;

      left_trig *= Time.deltaTime;
      right_trig *= Time.deltaTime;
    }
    /* -------------------------------------------------------------------------------*/
  }

}
