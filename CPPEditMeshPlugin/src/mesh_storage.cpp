#include "..//include/mesh_storage.h"

MeshStorage::MeshStorage() {
	m_mesh = std::shared_ptr <Mesh >(new Mesh());
	m_tree = std::shared_ptr <Tree >(new Tree());

	m_vnormals = m_mesh->template add_property_map<vertex_descriptor, Vector3>("v:normals", CGAL::NULL_VECTOR).first;

	m_vmap = std::map<vertex_descriptor, int>();

}

void MeshStorage::SetMesh(
	float* vertices, int vertexCount, 
	int* indices, int indexCount
) {

	m_work_triangles = indices;
	m_indexCount = indexCount;
	m_work_vertices = vertices;
	m_vertexCount = vertexCount;

	m_vmap.clear();

	// 添加顶点
	std::vector<Mesh::Vertex_index> vertexIndices;
	vertexIndices.reserve(m_vertexCount / 3);
	for (int i = 0; i < m_vertexCount; i += 3)
	{
		Point3 p(m_work_vertices[i], m_work_vertices[i + 1], m_work_vertices[i + 2]);
		auto vid = m_mesh->add_vertex(p);
		vertexIndices.push_back(vid);
		m_vmap.insert(std::make_pair(vid, i));
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


	PMP::compute_vertex_normals(*m_mesh, m_vnormals);
	PMP::build_AABB_tree(*m_mesh, *m_tree);
	
}

void MeshStorage::UpdateWorkMeshVertices() { // not update tropo
	int i = 0;
	for (auto v : m_mesh->vertices()) {
		auto p = m_mesh->point(v);
		int pid = i*3;
		m_work_vertices[pid] = (float)CGAL::to_double(p.x());
		m_work_vertices[pid + 1] = (float)CGAL::to_double(p.y());
		m_work_vertices[pid + 2] = (float)CGAL::to_double(p.z());
		i ++;
	}
}

void MeshStorage::UpdateWorkMesh(float* vertices, int* indices) { // not update tropo
	m_work_vertices = vertices;
	m_work_triangles = indices;

	int i = 0;
	m_vmap.clear();
	for (auto v : m_mesh->vertices()) {
		auto p = m_mesh->point(v);
		int pid = i * 3;
		m_work_vertices[pid] = (float)CGAL::to_double(p.x());
		m_work_vertices[pid + 1] = (float)CGAL::to_double(p.y());
		m_work_vertices[pid + 2] = (float)CGAL::to_double(p.z());
		m_vmap.insert(std::make_pair(v, i));

		i++;
	}

	i = 0;
	for (auto f : m_mesh->faces()) {
		auto iter = m_mesh->vertices_around_face(m_mesh->halfedge(f)).begin();
		auto v1 = *iter++;
		auto v2 = *iter++;
		auto v3 = *iter;
		int fid = i * 3;
		m_work_triangles[fid] = m_vmap.at(v1);
		m_work_triangles[fid + 1] = m_vmap.at(v2);
		m_work_triangles[fid + 2] = m_vmap.at(v3);
		i ++;
	}
}

void MeshStorage::MergeCloseVertices(int& vertexCount, int& indexCount) {

	PMP::stitch_borders(*m_mesh);

	m_vertexCount = m_mesh->vertices().size();
	m_indexCount = m_mesh->faces().size() * 3;

	vertexCount = m_vertexCount;
	indexCount = m_indexCount;
}

void MeshStorage::GetMeshCountsInfo(int& vertexCount, int& indexCount) {
	m_vertexCount = m_mesh->vertices().size();
	m_indexCount = m_mesh->faces().size() *3;

	vertexCount = m_vertexCount;
	indexCount = m_indexCount;
}

void MeshStorage::UpdateTree() {
	PMP::build_AABB_tree(*m_mesh, *m_tree);
}