using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using Leap.Unity;

public class ObjectTriggerPinch : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        if (pinch_left.DidStartPinch || pinch_right.DidStartPinch)
        { 
            viewingWidth = vcmOn ? planeThicknessOff : planeThicknessOn;
            vcmOn = !vcmOn;
        }
	}

    public PinchDetector pinch_left;
    public PinchDetector pinch_right;
    public static float planeThicknessOn = 0.05f;
    public static float planeThicknessOff = 100000f;
    private bool vcmOn = true;
    float viewingWidth = planeThicknessOn;
    bool started = false;

    public void OnTriggerStay(Collider col)
    {
        Vector3 normal;
        Vector3 position;
        float distance;

        normal = col.gameObject.transform.up;
        position = col.gameObject.transform.position;
        distance = col.gameObject.GetComponent<BoxCollider>().bounds.extents.y;

        GetComponent<Renderer>().sharedMaterial.SetVector("_Normal", normal);
        GetComponent<Renderer>().sharedMaterial.SetVector("_PlanePosition", position);
        GetComponent<Renderer>().sharedMaterial.SetFloat("_Distance", viewingWidth);

    }

    public void OnTriggerExit(Collider col)
    {
        GetComponent<Renderer>().sharedMaterial.SetVector("_Normal", new Vector4(1f, 1f, 1f, 1f));
        GetComponent<Renderer>().sharedMaterial.SetVector("_PlanePosition", new Vector4(0f, 0f, 0f, 0f));
        GetComponent<Renderer>().sharedMaterial.SetFloat("_Distance", vcmOn ? 0.0f : planeThicknessOff);
    }

}
