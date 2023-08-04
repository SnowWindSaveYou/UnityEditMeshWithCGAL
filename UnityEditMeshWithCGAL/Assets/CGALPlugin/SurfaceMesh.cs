using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using Unity.Collections;
using Unity.Collections.LowLevel.Unsafe;
using UnityEngine;

namespace CGALPlugin
{
    public class SurfaceMesh : MonoBehaviour
    {
        IntPtr m_ptr = IntPtr.Zero;
        public IntPtr ptr { get => m_ptr; }
        public int m_idx;


        public Vector3[] m_orignal_vertices;
        public int[] m_orignal_triangles;


        public Vector3[] m_work_vertices;
        public int[] m_work_triangles;

        public int[] m_selected_vertices;


        NativeArray<Vector3> m_vertices;
        NativeArray<int> m_triangles;


        Mesh m_refrencedMesh;


        public Transform testTrans;

        // init data in cpp side
        unsafe public void Init(Mesh mesh)
        {
            m_refrencedMesh = mesh;
            m_orignal_vertices = mesh.vertices;
            m_orignal_triangles = mesh.triangles;

            m_vertices = new NativeArray<Vector3>(m_orignal_vertices,Allocator.Persistent);
            m_triangles = new NativeArray<int>(m_orignal_triangles, Allocator.Persistent);

            Debug.Log(string.Format("mesh info: vtx {0} tri {1}",m_vertices.Count(), m_triangles.Count()));

            m_idx = CPDLL_MESHSTORAGE.addMesh((IntPtr)m_vertices.GetUnsafePtr(), m_vertices.Count()*3, (IntPtr)m_triangles.GetUnsafePtr(), m_triangles.Count());

            MergeSeem();

        }
        public void Release()
        {
            // release from cpp
            m_ptr = IntPtr.Zero;
            m_vertices.Dispose();
            m_triangles.Dispose();
        }

        public void SetMeshData(Vector3[] vertices, int[] triangles)
        {
            m_work_vertices = vertices;
            m_work_triangles = triangles;
            
        }

        // reset mesh to original state
        public void ResetMeshData()
        {
            if (m_work_vertices.Length != m_orignal_vertices.Length)
                m_work_vertices = new Vector3[m_orignal_vertices.Length];
            if (m_work_triangles.Length != m_orignal_triangles.Length)
                m_work_triangles = new int[m_orignal_triangles.Length];
            m_orignal_vertices.CopyTo(m_work_vertices,0);
            m_orignal_triangles.CopyTo(m_work_triangles, 0);


        }

        // get new mesh data from cpp
        public void RefreshMeshVertices()
        {
            //m_refrencedMesh.SetTriangles(m_work_triangles,0);
            //m_refrencedMesh.SetVertices(m_work_vertices);
            //m_refrencedMesh.SetIndices(m_triangles,MeshTopology.Triangles,0);
            CGALMeshEditPluginDLL.updateVertexPosition();
            m_refrencedMesh.SetVertices(m_vertices);
            m_refrencedMesh.RecalculateNormals();
        }

        unsafe public void MergeSeem()
        {
            int verticesCount = 0, indexCount = 0;
            CPDLL_MESHSTORAGE.mergeCloseVertices(ref verticesCount, ref indexCount);
            Debug.Log(string.Format("v:{0} i:{1}", verticesCount, indexCount));
            CPDLL_MESHSTORAGE.updateWorkMesh((IntPtr)m_vertices.GetUnsafePtr(), (IntPtr)m_triangles.GetUnsafePtr() );

            for(int i=0;i< indexCount; i++)
            {
                if (m_triangles[i]> verticesCount)
                {
                    Debug.Log(m_triangles[i]+","+ m_vertices[m_triangles[i]]);
                }
            }

            m_refrencedMesh.Clear();
            m_refrencedMesh.SetVertices(m_vertices,0, verticesCount);
            m_refrencedMesh.SetIndices(m_triangles,0, indexCount, MeshTopology.Triangles,0);//
            m_refrencedMesh.RecalculateNormals();

        }

        public void Start()
        {
            var meshFilter = this.GetComponent<MeshFilter>();
            Debug.Log(meshFilter);

            this.Init(meshFilter.mesh);
        }
        private void OnDestroy()
        {
            Release();
        }
    }
}