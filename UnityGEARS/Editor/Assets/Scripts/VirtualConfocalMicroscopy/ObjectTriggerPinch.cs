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
		if (pinch_left.IsHolding || pinch_right.IsHolding)
        {
            viewingWidth = 10000.0f;
        } else
        {
            viewingWidth = 0.05f;
        }
	}

    public PinchDetector pinch_left;
    public PinchDetector pinch_right;
    float viewingWidth = 0.05f;
    bool started = false;

    public void OnTriggerStay(Collider col)
    {
        //Debug.Log("Trigger Stay", gameObject);
        Vector3 normal;
        Vector3 position;
        float distance;

        normal = col.gameObject.transform.up;
        position = col.gameObject.transform.position;
        distance = col.gameObject.GetComponent<BoxCollider>().bounds.extents.y;

        //Debug.Log("Collider Distance: " + distance);



        GetComponent<Renderer>().sharedMaterial.SetVector("_Normal", normal);
        GetComponent<Renderer>().sharedMaterial.SetVector("_PlanePosition", position);
        GetComponent<Renderer>().sharedMaterial.SetFloat("_Distance", viewingWidth);

    }

    public void OnTriggerExit(Collider col)
    {
        //Debug.Log("Trigger Exit", gameObject);

        GetComponent<Renderer>().sharedMaterial.SetVector("_Normal", new Vector4(0f, 0f, 0f, 0f));
        GetComponent<Renderer>().sharedMaterial.SetVector("_PlanePosition", new Vector4(0f, 0f, 0f, 0f));
        GetComponent<Renderer>().sharedMaterial.SetFloat("_Distance", 0.0f);

    }

}
