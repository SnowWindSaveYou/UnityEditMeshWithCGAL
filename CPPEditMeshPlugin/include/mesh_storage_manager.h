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

	static MeshStorageManager *Instance;


	MeshStorage *current = nullptr;

	std::map<int,MeshStorage*> meshStorageMap;

	void SetCurrent(int idx);
	void SetCurrent(MeshStorage* pMeshStorage);

	int GetIndex(MeshStorage* pMeshStorage);

	int AddMeshStorage(MeshStorage* pMeshStorage);

private:
	int idx_count = 0;
};



#endif // !MESH_STORAGE_MANAGER_H
