#ifndef MESH_STORAGE_MANAGER_H
#define MESH_STORAGE_MANAGER_H


#include <vector>
#include <map>
#include "mesh_storage.h"
#include "cgal_defs.h"
#include "unity_includes.h"


struct HitInfo {
	Unity_Vector3 pos;
	Unity_Vector3 normal;
	int meshIdx;
};



class MeshStorageManager
{
public:
	MeshStorageManager();
	~MeshStorageManager();
	MeshStorageManager(const MeshStorageManager&) = delete;
	MeshStorageManager(const MeshStorageManager&&) = delete;
	MeshStorageManager& operator=(const MeshStorageManager&) = delete;

	static MeshStorageManager* getInstance() {
		return g_pInstance;
	}
	static void deleteInstance() {
		if (g_pInstance) {
			delete g_pInstance;
			g_pInstance = nullptr;
		}
	}

	MeshStorage *current = nullptr;

	std::map<int, std::shared_ptr< MeshStorage>>meshStorageMap;

	void SetCurrent(int idx);
	void SetCurrent(MeshStorage* pMeshStorage);

	int GetIndex(MeshStorage* pMeshStorage);

	int AddMeshStorage(MeshStorage* pMeshStorage);


	HitInfo RaycastCurrent(Ray ray);

	HitInfo RaycastAny(Ray ray);

private:
	static MeshStorageManager* g_pInstance;
	int idx_count = 0;
};


#endif // !MESH_STORAGE_MANAGER_H




#ifdef __cplusplus
extern "C" {
#endif

	__declspec(dllexport) void MergeCloseVertices(int& vertexCount, int& indexCount);

	__declspec(dllexport) void GetMeshCountsInfo(int& vertexCount, int& indexCount);

	__declspec(dllexport) void UpdateWorkMesh(float* vertices, int* indices);


#ifdef __cplusplus
}
#endif
