using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.SceneManagement;

public class SceneControl : MonoBehaviour {

    public static string defaultScene = "ClickMe";
    public List<string> sceneNames = new List<string>(new string[]
    {defaultScene, "Interactive Viewer", "VFC", "LennardJones", "KineticMonteCarlo"});
    private List<string> scenesInBuild;


    // Use this for initialization
    void Start () {
        scenesInBuild = new List<string>();
        for (int i = 1; i < SceneManager.sceneCountInBuildSettings; i++)
        {
            string scenePath = SceneUtility.GetScenePathByBuildIndex(i);
            int lastSlash = scenePath.LastIndexOf("/");
            scenesInBuild.Add(scenePath.Substring(lastSlash + 1, scenePath.LastIndexOf(".") - lastSlash - 1));
        }
    }
	
	// Update is called once per frame
	void Update () {
		if (Input.GetKeyDown(KeyCode.Keypad0))
        {
            ChangeScene(0);
        }
        else if (Input.GetKeyDown(KeyCode.Keypad1))
        {
            ChangeScene(1);
        }
        else if (Input.GetKeyDown(KeyCode.Keypad2))
        {
            ChangeScene(2);
        }
        else if (Input.GetKeyDown(KeyCode.Keypad3))
        {
            ChangeScene(3);
        }
        else if (Input.GetKeyDown(KeyCode.Keypad4))
        {
            ChangeScene(4);
        }
    }

    private bool SceneExists(int nameIndex)
    {
        if (nameIndex < sceneNames.Count)
            return scenesInBuild.Contains(sceneNames[nameIndex]);
        else
            return false;
    }
    
    private void ChangeScene(int sceneIndex)
    {
        if (SceneExists(sceneIndex))
        {
            SceneManager.LoadSceneAsync(sceneNames[sceneIndex], LoadSceneMode.Single);
        } else
        {
            SceneManager.LoadSceneAsync(defaultScene, LoadSceneMode.Single);
        }
    }
}
