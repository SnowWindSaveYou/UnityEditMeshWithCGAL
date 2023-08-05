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


	VertexNormalMap m_vnormals;
	std::map<vertex_descriptor, int> m_vmap;

	int* m_work_triangles;//info for unity to display
	float* m_work_vertices;
	int m_vertexCount;
	int m_indexCount;


	bool notifiedMeshUpdated = false;

	MeshStorage();
	~MeshStorage() {

	}
	// remove default copy func to avoid some mistakes
	MeshStorage(const MeshStorage&) = delete;
	MeshStorage& operator=(const MeshStorage&) = delete;

	void SetMesh(float* vertices, int vertexCount, int* indices, int indexCount);

	void UpdateWorkMeshVertices();

	void UpdateWorkMesh(float* vertices, int* indices);// need get counts and make new buffers first

	void MergeCloseVertices(int& vertexCount, int& indexCount);//the result counts are lower than original

	void GetMeshCountsInfo(int& vertexCount, int& indexCount);

	void UpdateTree();



	
};


#endif //  MESH_STORAGE_H
