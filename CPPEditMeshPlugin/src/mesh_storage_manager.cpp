#include "..//include/mesh_storage_manager.h"

//init
MeshStorageManager* MeshStorageManager::g_pInstance = new (std::nothrow) MeshStorageManager();


MeshStorageManager::MeshStorageManager()
{
	//if (MeshStorageManager::Instance == nullptr) {
	//	Instance = this;
	//}
	//else {
	//	delete this;
	//	return;
	//}

	this->idx_count = 0;
	this->meshStorageMap = std::map<int,MeshStorage*>();
}

MeshStorageManager::~MeshStorageManager()
{
	for (auto it : meshStorageMap) {
		delete it.second;
	}
}

void MeshStorageManager::SetCurrent(int idx) {
	current = meshStorageMap[idx];
}

void MeshStorageManager::SetCurrent(MeshStorage* pMeshStorage) {
	current = pMeshStorage;
}

int MeshStorageManager::GetIndex(MeshStorage* pMeshStorage) {

	for (std::map<int, MeshStorage*>::iterator it = meshStorageMap.begin();it != meshStorageMap.end();it++)
	{
		if (it->second == pMeshStorage)
			return it->first;
	}
	return -1;
}

int MeshStorageManager::AddMeshStorage(MeshStorage* pMeshStorage) {
	int idx = idx_count++;
	meshStorageMap.insert(std::make_pair(idx, pMeshStorage));
	return idx;
}