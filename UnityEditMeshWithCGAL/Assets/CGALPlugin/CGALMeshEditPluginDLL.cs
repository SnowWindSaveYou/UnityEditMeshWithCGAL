using System;
using System.Collections;
using System.Collections.Generic;
using Unity.Collections;
using UnityEngine;


using Utility.fts;
using static CGALMeshEditPluginDLL;


// ------------------------------------------------------------------------
// Auto Lookup
//
// Requires 'NativePluginLoader' object to exist in scene
// ------------------------------------------------------------------------
//[PluginAttr("UnityMeshEditPlugin")]
[PluginAttr("CPPEDITMESHPLUGIN")]
public static class CGALMeshEditPluginDLL 
{
    [PluginFunctionAttr("simple_func")]
    public static simple_func testDll = null;
    public delegate int simple_func();

    //[PluginFunctionAttr("TestParam")]
    //public static TestParam testParam = null;
    //public delegate int TestParam(IntPtr param);


    [PluginFunctionAttr("initMesh")]
    public static initMesh InitMesh = null;
    //public delegate void initMesh(NativeArray< Vector3> vertices, int vertexCount, NativeArray<int> indices, int indexCount);
    //public delegate void initMesh(Vector3[] vertices, int vertexCount, int[] indices, int indexCount);
    public delegate void initMesh(IntPtr vertices, int vertexCount, IntPtr indices, int indexCount);

    [PluginFunctionAttr("RayInteract")]
    public static RayInteract rayInteract = null;
    public delegate Vector3 RayInteract(Vector3 localPos, Vector3 localDir);

    [PluginFunctionAttr("RaiseVertices")]
    public static RaiseVertices raiseVertices = null;
    public delegate void RaiseVertices(Vector3 localPos, float radius);

    [PluginFunctionAttr("UpdateVertexPosition")]
    public static UpdateVertexPosition updateVertexPosition = null;
    public delegate void UpdateVertexPosition();

    #region[ surface mesh]

    // init mesh

    // update mesh

    // drag mesh

    #endregion

}
