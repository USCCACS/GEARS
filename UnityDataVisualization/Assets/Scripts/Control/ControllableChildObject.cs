using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace CACS
{
  public class ControllableChildObject : ControllableObject
  {

    List<Material> materials;

    // Use this for initialization
    void Start() {
      Material[] mats = GetComponent<MeshRenderer>().sharedMaterials;
      materials = new List<Material>();

      foreach (Material mat in gameObject.GetComponent<MeshRenderer>().sharedMaterials) {
        materials.Add(mat);
      }
    }

    // Update is called once per frame
    void Update() {

    }

    /* -----------------------------------------------------------
     * Trigger/Collision Detection
     * -----------------------------------------------------------*/
    void OnTriggerStay(Collider col) {
      Vector3 normal;
      Vector3 position;
      float viewingDepth;

      normal = col.gameObject.transform.up;
      position = col.gameObject.transform.position;
      viewingDepth = col.GetComponent<ControllablePlane>().getViewingDepth();

      setShaderParams(normal, position, viewingDepth);
    }

    void OnTriggerExit(Collider col) {
      setShaderParams(new Vector4(0, 0, 0, 0), new Vector4(0, 0, 0, 0), 0);
    }

    private void setShaderParams(Vector3 normal, Vector3 pos, float viewingDepth) {
      foreach (Material mat in materials) {
        mat.SetVector("_Normal", normal);
        mat.SetVector("_PlanePosition", pos);
        mat.SetFloat("_Distance", viewingDepth);
      }
    }
    /*------------------------------------------------------------*/
  }

}
