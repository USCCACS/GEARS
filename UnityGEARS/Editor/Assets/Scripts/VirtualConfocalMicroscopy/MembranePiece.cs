using UnityEngine;
using System.Collections;

//http://answers.unity3d.com/questions/217078/changing-color-based-on-z-position.html

public class MembranePiece : MonoBehaviour {

	public float transparent = 0.05f;
	public float visible = 1.0f;
	public float visible_locked = 0.8f;
	private bool locked = false;

	/*
	void OnCollisionEnter (Collision col){
		makeVisible ();
	}
	void OnCollisionStay (Collision col){
	}
	void OnCollisionExit (Collision col){
		makeTransparent ();
	}*/

	void OnTriggerEnter (Collider col){
		if (!locked)
			setOpacity (visible);
	}
	/*void OnTriggerStay (Collider col){
		if (Input.GetMouseButtonDown(0)) {
			if (locked)
				setOpacity(visible);
			else 
				setOpacity (visible_locked);

			locked = !locked;
		}
	}*/
	void OnTriggerExit (Collider col){
		if (!locked)
			setOpacity (transparent);
	}

	private void setOpacity(float o){
		foreach (Material mat in transform.GetComponent<MeshRenderer>().materials) {
			Color c = mat.color;
			c.a = o;
			mat.color = c;
		}
	}

	// Use this for initialization
	void Start () {
		setOpacity(transparent);
	}
	
	// Update is called once per frame
	void Update () {
	
	}
}
