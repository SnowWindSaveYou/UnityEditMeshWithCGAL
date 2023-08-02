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


    
    [PluginFunctionAttr("InitPlugin")]
    public static InitPlugin initPlugin = null;
    public delegate int InitPlugin();

    [PluginFunctionAttr("DisposePlugin")]
    public static DisposePlugin disposePlugin = null;
    public delegate int DisposePlugin();


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


}


[PluginAttr("CPPEDITMESHPLUGIN")]
public static class CPDLL_MESHSTORAGE
{
    public class CPDLL_HitInfo
    {
        Vector3 position;
        Vector3 normal;
        int hitIdx;
    }


    [PluginFunctionAttr("AddMesh")]
    public static AddMesh addMesh = null;
    public delegate int AddMesh(IntPtr vertices, int vertexCount, IntPtr indices, int indexCount);

    [PluginFunctionAttr("SetCurrentMesh")]
    public static SetCurrentMesh setCurrentMesh = null;
    public delegate void SetCurrentMesh(int idx);


    [PluginFunctionAttr("MergeCloseVertices")]
    public static MergeCloseVertices mergeCloseVertices = null;
    public delegate void MergeCloseVertices(ref int vertexCount, ref int indexCount);


    [PluginFunctionAttr("GetMeshCountsInfo")]
    public static GetMeshCountsInfo getMeshCountsInfo = null;
    public delegate void GetMeshCountsInfo(ref int vertexCount, ref int indexCount);


    [PluginFunctionAttr("UpdateWorkMesh")]
    public static UpdateWorkMesh updateWorkMesh = null;
    public delegate void UpdateWorkMesh();



    //raycast


}


[PluginAttr("CPPEDITMESHPLUGIN")]
public static class CPDLL_EDITPROCESS
{
    [PluginFunctionAttr("SetRayInfo")]
    public static SetRayInfo setRayInfo = null;
    public delegate void SetRayInfo(Vector3 localPos, Vector3 localDir);

    [PluginFunctionAttr("ProcessToolBegin")]
    public static ProcessToolBegin processToolBegin = null;
    public delegate void ProcessToolBegin();

    [PluginFunctionAttr("ProcessToolEnd")]
    public static ProcessToolEnd processToolEnd = null;
    public delegate void ProcessToolEnd();

    [PluginFunctionAttr("ProcessToolUpdate")]
    public static ProcessToolUpdate processToolUpdate = null;
    public delegate void ProcessToolUpdate();


}