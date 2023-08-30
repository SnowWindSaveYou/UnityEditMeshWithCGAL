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
        public int[] m_work_indices;
        public int m_work_vertices_count;
        public int m_work_indices_count;

        public int[] m_selected_vertices;


        NativeArray<Vector3> m_vertices;
        NativeArray<int> m_indices;


        Mesh m_refrencedMesh;
        public Material m_material;

        public Transform testTrans;

        // init data in cpp side
        unsafe public void Init(Mesh mesh)
        {
            m_refrencedMesh = mesh;
            m_orignal_vertices = mesh.vertices;
            m_orignal_triangles = mesh.triangles;

            m_vertices = new NativeArray<Vector3>(m_orignal_vertices, Allocator.Persistent);
            m_indices = new NativeArray<int>(m_orignal_triangles, Allocator.Persistent);
            m_work_vertices_count = m_vertices.Count();
            m_work_indices_count = m_indices.Count();

            Debug.Log(string.Format("mesh info: vtx {0} tri {1}", m_vertices.Count(), m_indices.Count()));

            m_idx = CPDLL_MESHSTORAGE.addMesh((IntPtr)m_vertices.GetUnsafePtr(), m_vertices.Count() * 3, (IntPtr)m_indices.GetUnsafePtr(), m_indices.Count());

            MergeSeem();

        }
        public void Release()
        {
            // release from cpp
            m_ptr = IntPtr.Zero;
            m_vertices.Dispose();
            m_indices.Dispose();
        }

        public void SetMeshData(Vector3[] vertices, int[] triangles)
        {
            m_work_vertices = vertices;
            m_work_indices = triangles;
            m_work_vertices_count = vertices.Count();
            m_work_indices_count = triangles.Count();

        }

        // reset mesh to original state
        public void ResetMeshData()
        {
            if (m_work_vertices.Length != m_orignal_vertices.Length)
                m_work_vertices = new Vector3[m_orignal_vertices.Length];
            if (m_work_indices.Length != m_orignal_triangles.Length)
                m_work_indices = new int[m_orignal_triangles.Length];
            m_work_vertices_count = m_orignal_vertices.Count();
            m_work_indices_count = m_orignal_triangles.Count();

            m_orignal_vertices.CopyTo(m_work_vertices, 0);
            m_orignal_triangles.CopyTo(m_work_indices, 0);

        }

        // get new mesh data from cpp
        public void RefreshMeshVertices()
        {
            CGALMeshEditPluginDLL.updateVertexPosition();
            m_refrencedMesh.SetVertices(m_vertices,0,m_work_vertices_count);
            m_refrencedMesh.RecalculateNormals();
        }

        unsafe public void MergeSeem()
        {
            //int verticesCount = 0, indexCount = 0;
            CPDLL_MESHSTORAGE.mergeCloseVertices(ref m_work_vertices_count, ref m_work_indices_count);
            Debug.Log(string.Format("v:{0} i:{1}", m_work_vertices_count, m_work_indices_count));
            CPDLL_MESHSTORAGE.updateWorkMesh((IntPtr)m_vertices.GetUnsafePtr(), (IntPtr)m_indices.GetUnsafePtr());

            for (int i = 0; i < m_work_indices_count; i++)
            {
                if (m_indices[i] > m_work_vertices_count)
                {
                    Debug.Log(m_indices[i] + "," + m_vertices[m_indices[i]]);
                }
            }

            m_refrencedMesh.Clear();
            m_refrencedMesh.SetVertices(m_vertices, 0, m_work_vertices_count);
            m_refrencedMesh.SetIndices(m_indices, 0, m_work_indices_count, MeshTopology.Triangles, 0);//
            m_refrencedMesh.RecalculateNormals();

        }


        
        #region [ Life Cycle]
        public void OnSelected()
        {

        }

        public void OnWithDraw()
        {

        }

        public void Start()
        {
            var meshFilter = this.GetComponent<MeshFilter>();
            m_material = this.GetComponent<MeshRenderer>().sharedMaterial;
            Debug.Log(meshFilter);

            this.Init(meshFilter.mesh);
        }
        private void OnDestroy()
        {
            Release();
        }

        #endregion
    }
}