#include "..//include/mesh_storage.h"

void MeshStorage::SetMesh(float* vertices, int vertexCount, int* indices, int indexCount) {
	m_work_triangles = indices;
	m_indexCount = indexCount;
	m_work_vertices = vertices;
	m_vertexCount = vertexCount;


	// 添加顶点
	std::vector<Mesh::Vertex_index> vertexIndices;
	vertexIndices.reserve(m_vertexCount / 3);
	for (int i = 0; i < m_vertexCount; i += 3)
	{
		Point3 p(m_work_vertices[i], m_work_vertices[i + 1], m_work_vertices[i + 2]);
		auto vid = m_mesh->add_vertex(p);
		vertexIndices.push_back(vid);
	}


	// 添加三角形面片
	for (int i = 0; i < m_indexCount; i += 3)
	{
		Mesh::Vertex_index v1 = vertexIndices[m_work_triangles[i]];
		Mesh::Vertex_index v2 = vertexIndices[m_work_triangles[i + 1]];
		Mesh::Vertex_index v3 = vertexIndices[m_work_triangles[i + 2]];
		if (v1 == v2 || v2 == v3 || v1 == v3)
			continue;
		m_mesh->add_face(v1, v2, v3);
	}

	PMP::build_AABB_tree(*m_mesh, *m_tree);
}

void MeshStorage::UpdateWorkMeshVertices() { // not update tropo
	int i = 0;
	for (auto p : m_mesh->points()) {
		int pid = i;
		float a = m_work_vertices[pid];
		float b = m_work_vertices[pid + 1];
		float c = m_work_vertices[pid + 2];
		m_work_vertices[pid] = (float)CGAL::to_double(p.x());
		m_work_vertices[pid + 1] = (float)CGAL::to_double(p.y());
		m_work_vertices[pid + 2] = (float)CGAL::to_double(p.z());
		i += 3;
	}
	//m_tree->rebuild(m_mesh->faces_begin(),m_mesh->faces_end());
	PMP::build_AABB_tree(*m_mesh, *m_tree);
}