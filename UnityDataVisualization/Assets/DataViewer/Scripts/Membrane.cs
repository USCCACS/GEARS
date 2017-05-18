using UnityEngine;
using System.Collections;

public class Membrane : MonoBehaviour {

	/*Starting position for the reset button*/
	private Vector3 startPos;
	private Vector3 startRot;

	Transform[] mat;

	// Resets the position and rotation of the complex to their initial states
	public void resetPosition(){
		transform.position = startPos;
		transform.eulerAngles = startRot;
	}

	// Use this for initialization
	void Start () {
		startPos = transform.position;
		startRot = transform.eulerAngles;
		/*
		foreach (Transform grandChild in transform.GetChild (1)) {
			grandChild.gameObject.AddComponent<MembranePiece>();
			MeshCollider mc = grandChild.gameObject.AddComponent<MeshCollider>();
			mc.convex = true;
			mc.isTrigger = true;
		}
		*/
	}
	
	// Update is called once per frame
	void Update () {
	}
}
