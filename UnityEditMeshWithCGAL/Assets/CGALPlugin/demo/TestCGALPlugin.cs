using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using Unity.VisualScripting;
using UnityEngine;

public class TestCGALPlugin : MonoBehaviour
{
    MeshFilter meshFilter;
    public Transform testTransform;

    public bool interactWithA;


    NativeArray<Vector3> nPoints;
    NativeArray<int> nIndexs;

    Vector3[] vertices;
    int[] triangles;

    // Start is called before the first frame update
    void Start()
    {
        meshFilter = this.GetComponent<MeshFilter>();
        SetMeshToCPP();
    }
    private void OnDestroy()
    {
        nPoints.Dispose();
        nIndexs.Dispose();
    }

    unsafe public void SetMeshToCPP()
    {
         vertices = meshFilter.mesh.vertices;
         triangles = meshFilter.mesh.triangles;
        //CGALMeshEditPluginDLL.InitMesh(vertices, vertices.Count() * 3, triangles, triangles.Count());

        nPoints = new NativeArray<Vector3>(vertices.Count(), Allocator.Persistent);
        nIndexs = new NativeArray<int>(triangles.Count(), Allocator.Persistent);
        NativeArray<Vector3>.Copy(vertices, nPoints);
        NativeArray<int>.Copy(triangles, nIndexs);
        CGALMeshEditPluginDLL.InitMesh((IntPtr)nPoints.GetUnsafePtr(), nPoints.Count() * 3, (IntPtr)nIndexs.GetUnsafePtr(), nIndexs.Count());

    }

    // Update is called once per frame
    void Update()
    {
        if (Input.GetMouseButtonDown(0))
        {
            Ray ray = Camera.main.ScreenPointToRay(Input.mousePosition);

            Vector3 localStart = this.transform.worldToLocalMatrix.MultiplyPoint( ray.GetPoint(0));
            Vector3 localStartDir = this.transform.worldToLocalMatrix.MultiplyVector(ray.direction);
            Vector3 pos = CGALMeshEditPluginDLL.rayInteract(localStart, localStartDir);
            Debug.Log("pos " + pos);
            CGALMeshEditPluginDLL.raiseVertices(pos, 0.25f);
            CGALMeshEditPluginDLL.updateVertexPosition();

            //meshFilter.mesh.SetVertices(vertices);
            meshFilter.mesh.SetVertices(nPoints);
            testTransform.localPosition = pos;

        }
    }
}
