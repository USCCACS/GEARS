#pragma strict

var viewingWidth = 0.01;
var started = false;

function OnTriggerStay (col : Collider) {
	//Debug.Log("Trigger Stay", gameObject);
	var normal : Vector3;
	var position : Vector3;
	var distance : float;
	
	normal = col.gameObject.transform.up;
	position = col.gameObject.transform.position;
	distance = col.gameObject.GetComponent.<BoxCollider>().bounds.extents.y;
	
	//Debug.Log("Collider Distance: " + distance);
	
	
    //Change Input to the shaders
	GetComponent.<Renderer>().sharedMaterial.SetVector("_Normal", normal);
	GetComponent.<Renderer>().sharedMaterial.SetVector("_PlanePosition", position);
	GetComponent.<Renderer>().sharedMaterial.SetFloat("_Distance", viewingWidth);

}

function OnTriggerExit (col : Collider) {
	//Debug.Log("Trigger Exit", gameObject);

	GetComponent.<Renderer>().sharedMaterial.SetVector("_Normal", Vector4(0,0,0,0));
	GetComponent.<Renderer>().sharedMaterial.SetVector("_PlanePosition", Vector4(0,0,0,0));
	GetComponent.<Renderer>().sharedMaterial.SetFloat("_Distance", 0);

}

function Start() {

}

function Update() {

}