#include "..//include/mesh_storage_manager.h"

//init
MeshStorageManager* MeshStorageManager::g_pInstance = new (std::nothrow) MeshStorageManager();


MeshStorageManager::MeshStorageManager()
{
	this->idx_count = 0;
	this->meshStorageMap = std::map<int,std::shared_ptr< MeshStorage>>();
}

MeshStorageManager::~MeshStorageManager()
{
	
}

void MeshStorageManager::SetCurrent(int idx) {
	current = meshStorageMap.at(idx).get();
}

void MeshStorageManager::SetCurrent(MeshStorage* pMeshStorage) {
	current = pMeshStorage;
}

int MeshStorageManager::GetIndex(MeshStorage* pMeshStorage) {

	for (auto it = meshStorageMap.begin();it != meshStorageMap.end();it++)
	{
		if (it->second.get() == pMeshStorage)
			return it->first;
	}
	return -1;
}

int MeshStorageManager::AddMeshStorage(MeshStorage* pMeshStorage) {
	int idx = idx_count++;
	meshStorageMap.insert(std::make_pair(idx, pMeshStorage));
	pMeshStorage->m_uid = idx;
	return idx;
}

Unity_Vector3 point2uvec(Point3 p) {
	Unity_Vector3 vec = Unity_Vector3();
	vec.x = (float)CGAL::to_double(p.x());
	vec.y = (float)CGAL::to_double(p.y());
	vec.z = (float)CGAL::to_double(p.z());
	return vec;
}

HitInfo MeshStorageManager::RaycastCurrent(Ray ray_query) {
	HitInfo res;
	res.meshIdx = current->m_uid;
	//res.normal = 0;
	if (current->m_tree->do_intersect(ray_query)) {
		Face_location ray_location = PMP::locate_with_AABB_tree(ray_query, *current->m_tree, *current->m_mesh);
		//auto fid =ray_location.first;
		//auto cod = ray_location.second;

		//auto fviter = current->m_mesh->vertices_around_face(current->m_mesh->halfedge( fid));
		//for (auto v : fviter) {
		//	Point3 p = current->m_mesh->point(v);
		//	//Vector3 n = current->m_mesh->
		//	cod.
		//}

		Point3 correspondPoint = PMP::construct_point(ray_location, *current->m_mesh);
		res.pos = point2uvec(correspondPoint);
	}

	return res;
}



HitInfo MeshStorageManager::RaycastAny(Ray ray_query) {
	HitInfo res;
	for (auto m : this->meshStorageMap) {
		if (m.second->m_tree->do_intersect(ray_query)) {
			Face_location ray_location = PMP::locate_with_AABB_tree(ray_query, *m.second->m_tree, *m.second->m_mesh);
			Point3 correspondPoint = PMP::construct_point(ray_location, *m.second->m_mesh);
			res.pos = point2uvec(correspondPoint);
			res.meshIdx = m.second->m_uid;
			break;
		}

	}

	return res;
}


#pragma region [export]


void MergeCloseVertices(int& vertexCount, int& indexCount) {
	MeshStorageManager::getInstance()->current->MergeCloseVertices(vertexCount, indexCount);

}
void GetMeshCountsInfo(int& vertexCount, int& indexCount) {
	MeshStorageManager::getInstance()->current->GetMeshCountsInfo(vertexCount, indexCount);
}

void UpdateWorkMesh() {
	MeshStorageManager::getInstance()->current->UpdateWorkMesh();

}


#pragma endregion
