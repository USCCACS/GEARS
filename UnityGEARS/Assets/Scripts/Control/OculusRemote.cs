using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class OculusRemote : MonoBehaviour {

	// Use this for initialization
	void Start () {
		
	}
	
	// Update is called once per frame
	void Update () {
        OVRInput.Update();
		if (OVRInput.Get(OVRInput.Button.DpadLeft))
        {
            SceneManager.LoadSceneAsync("TellurineFun", LoadSceneMode.Single);
        } else if (OVRInput.Get(OVRInput.Button.DpadRight))
        {
            SceneManager.LoadSceneAsync("ExfoliationFun", LoadSceneMode.Single);
        }
	}
}
