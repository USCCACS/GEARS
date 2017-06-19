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
    //viewingWidth = 10;
}

function Update() {
    if (Input.GetKey(KeyCode.Space)) {
        started = !started;
        Debug.Log("You pressed Space");
    }

    if (viewingWidth > 0.15 && started == true) {
        viewingWidth -= 2*Time.deltaTime;
        if (viewingWidth < 0.15)
            viewingWidth = 0.15;
        Debug.Log("Decreasing viewing width");
    } else if (viewingWidth < 10 && started == false) {
        viewingWidth += 2*Time.deltaTime;
        if (viewingWidth > 10)
            viewingWidth = 10;
        Debug.Log("Increasing viewing width");
    }
}