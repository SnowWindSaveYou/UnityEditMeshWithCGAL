#ifndef  MESH_STORAGE_H
#define MESH_STORAGE_H

#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <stack>
#include <unordered_set>
#include "cgal_defs.h"

class MeshStorage {
public:
	int m_uid;
	std::shared_ptr <Mesh> m_mesh;
	std::shared_ptr <Tree> m_tree;
	std::shared_ptr < Deformation> m_deformation;
	VertexNormalMap m_vnormals;


	int* m_work_triangles;//info for unity to display
	float* m_work_vertices;
	int m_vertexCount;
	int m_indexCount;

	bool notifiedMeshUpdated = false;

	MeshStorage() {
		m_mesh = std::shared_ptr <Mesh >(new Mesh());
		m_tree = std::shared_ptr <Tree >(new Tree());
		//m_deformation = new Deformation(m_mesh);
	}
	~MeshStorage() {

	}
	// remove default copy func to avoid some mistakes
	MeshStorage(const MeshStorage&) = delete;
	MeshStorage& operator=(const MeshStorage&) = delete;

	void SetMesh(float* vertices, int vertexCount, int* indices, int indexCount);

	void UpdateWorkMeshVertices();

	//void UpdateWorkMesh();
};


#endif //  MESH_STORAGE_H
