using UnityEngine;
using System.Collections;

public class Heme : MonoBehaviour {
    private bool occupied;
    private bool update;

	// Use this for initialization
	void Start () {
        occupied = false;
        update = false;
        GetComponent<Renderer>().material.color = Color.black;
	}

	// Update is called once per frame
	void Update () {
        if(update) {
            update = false;

            if(occupied) {
                GetComponent<Renderer>().material.color = Color.red;
            }
            else {
                GetComponent<Renderer>().material.color = Color.black;
            }
        }
	}

    public void setOccupied(bool val) {
        occupied = val;
        update = true;
    }

    public bool isOccupied() {
        return occupied;
    }
}
