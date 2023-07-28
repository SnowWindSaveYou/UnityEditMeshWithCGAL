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


        public Vector3[] m_orignal_vertices;
        public int[] m_orignal_triangles;


        public Vector3[] m_work_vertices;
        public int[] m_work_triangles;

        public int[] m_selected_vertices;


        NativeArray<Vector3> m_vertices;
        NativeArray<int> m_triangles;

        Mesh m_refrencedMesh;


        // init data in cpp side
        unsafe public void Init(Mesh mesh)
        {
            m_refrencedMesh = mesh;
            m_orignal_vertices = mesh.vertices;
            m_orignal_triangles = mesh.triangles;

            NativeArray<Vector3>.Copy(mesh.vertices, m_vertices);
            NativeArray<int>.Copy(mesh.triangles, m_triangles);

            IntPtr a = (IntPtr) m_vertices.GetUnsafePtr();
            
        }
        public void Release()
        {
            // release from cpp
            m_ptr = IntPtr.Zero;
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
        public void RefreshMesh()
        {
            m_refrencedMesh.SetTriangles(m_work_triangles,0);
            m_refrencedMesh.SetVertices(m_work_vertices);
        }
    }
}