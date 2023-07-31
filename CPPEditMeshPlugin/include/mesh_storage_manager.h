#ifndef MESH_STORAGE_MANAGER_H
#define MESH_STORAGE_MANAGER_H


#include <vector>
#include <map>
#include "mesh_storage.h"

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

	std::map<int,MeshStorage*> meshStorageMap;

	void SetCurrent(int idx);
	void SetCurrent(MeshStorage* pMeshStorage);

	int GetIndex(MeshStorage* pMeshStorage);

	int AddMeshStorage(MeshStorage* pMeshStorage);

private:
	static MeshStorageManager* g_pInstance;
	int idx_count = 0;
};


#endif // !MESH_STORAGE_MANAGER_H
