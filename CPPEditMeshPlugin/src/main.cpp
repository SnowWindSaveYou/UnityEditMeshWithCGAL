// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "windows.h"
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <stack>
#include <unordered_set>


#include <Eigen/Dense>
#include <Eigen/Sparse>


#include "..//include/unity_includes.h"
#include "..//include/cgal_defs.h"
#include "..//include/mesh_storage.h"
#include "..//include/I_edit_tool.h"
#include "..//include/mesh_storage_manager.h"



#include "grab_brush_tool.cpp"

typedef Eigen::Matrix4d EMatrix4;
typedef MeshStorageManager MSM;

// API must be "strict" C
// It is STRONGLY encourage that these functions should NEVER
// crash, throw, abort, or terminate.
#ifdef __cplusplus
extern "C" {
#endif

#pragma region TestExport

	__declspec(dllexport) int simple_func();
	int simple_func() {
		return 42;
	}

#pragma endregion


	__declspec(dllexport) void InitPlugin();
	__declspec(dllexport) void DisposePlugin();

	__declspec(dllexport) void SetCurrentMesh(int idx);
	__declspec(dllexport) void SetCurrentTool(int idx);

	__declspec(dllexport) void ProcessToolBegin();
	__declspec(dllexport) void ProcessToolEnd();
	__declspec(dllexport) void ProcessToolUpdate();

	__declspec(dllexport) void  initMesh(float* vertices, int vertexCount, int* indices, int indexCount);

	__declspec(dllexport) Unity_Vector3 RayInteract(Unity_Vector3 localPos, Unity_Vector3 localDir);


	__declspec(dllexport) void RaiseVertices(Unity_Vector3 localPos, float radius);

	__declspec(dllexport) void UpdateVertexPosition();


	__declspec(dllexport) int addMesh(float* vertices, int vertexCount, int* indices, int indexCount);


#ifdef __cplusplus
}
#endif




static MeshStorage* currMeshStorage;

static IEditTool* currentEditTool;
static std::map<int,IEditTool*> editToolList;


void InitPlugin() {
	auto msm = new MeshStorageManager();

	editToolList = std::map<int,IEditTool*>();
	editToolList.insert(std::make_pair(0, new GrabBrushTool()));


}

void DisposePlugin() {
	delete MSM::Instance;
}

void SetCurrentMesh(int idx) {
	MSM::Instance->SetCurrent(idx);
}

void SetCurrentTool(int idx) {
	currentEditTool = editToolList[idx];
}


void SetRayInfo(Unity_Vector3 localPos, Unity_Vector3 localDir) {
	Point3 localPosPoint = Point3(localPos.x, localPos.y, localPos.z);
	Vector3 localDirPoint = Vector3(localDir.x, localDir.y, localDir.z);
	Ray ray_query(localPosPoint, localDirPoint);
	IEditTool::s_ray = ray_query;
}

void ProcessToolBegin() {
	currentEditTool->OnEditBegin();
}
void ProcessToolEnd() {
	currentEditTool->OnEditEnd();
}

// TODO put into independent thread
void ProcessToolUpdate() {
	currentEditTool->OnEditProcess();
}


int addMesh(float* vertices, int vertexCount, int* indices, int indexCount) {
	auto sm = new MeshStorage();
	sm->SetMesh(vertices, vertexCount, indices, indexCount);

	int idx = MSM::Instance->AddMeshStorage(sm);
	MSM::Instance->SetCurrent(sm);
	return idx;
}


void initMesh(float* vertices, int vertexCount, int* indices, int indexCount) {

	currMeshStorage =  new MeshStorage();
	currMeshStorage->SetMesh( vertices,  vertexCount,indices,  indexCount);
}

void UpdateVertexPosition() {
	currMeshStorage->UpdateWorkMeshVertices();
}


// get ray interact position to mesh
Unity_Vector3 RayInteract(Unity_Vector3 localPos, Unity_Vector3 localDir) {
	Unity_Vector3 res = Unity_Vector3{0,0,0};
    Point3 localPosPoint = Point3(localPos.x, localPos.y, localPos.z);
	Vector3 localDirPoint= Vector3(localDir.x, localDir.y, localDir.z);
    Ray ray_query(localPosPoint, localDirPoint);

	try {
		Tree* tree = (currMeshStorage->m_tree).get();
		Mesh mesh = *(currMeshStorage->m_mesh).get();
		if (tree->do_intersect(ray_query)) {
			Face_location ray_location = PMP::locate_with_AABB_tree(ray_query, *tree, mesh);
			Point3 correspondPoint = PMP::construct_point(ray_location, mesh);
			res.x = (float) CGAL::to_double(correspondPoint.x());
			res.y = (float) CGAL::to_double(correspondPoint.y());
			res.z = (float) CGAL::to_double(correspondPoint.z());
		}

	}
	catch (...) {
		// 
	}

    return res;
}

void SelectVertices(Unity_Vector3 localPos, float radius) {
	Point3 localPosPoint = Point3(localPos.x, localPos.y, localPos.z);
	Mesh& mesh = *currMeshStorage->m_mesh;

	float squared_radius = radius * radius;
	auto closestPoint = currMeshStorage->m_tree->closest_point_and_primitive(localPosPoint);
	Face_index face_idx = closestPoint.second;
	Point3 startPoint = closestPoint.first;
	Halfedge_index he_idex = currMeshStorage->m_mesh->halfedge(face_idx);
	Vertex_index v_idex = currMeshStorage->m_mesh->target(he_idex);
	std::stack<Vertex_index> v_stack = std::stack<Vertex_index>();
	std::unordered_set<Vertex_index> visited_set = std::unordered_set<Vertex_index>();
	v_stack.push(v_idex);

	while (!v_stack.empty()) {
		Vertex_index src_v = v_stack.top();
		CGAL::Vertex_around_target_circulator<Mesh> vbegin(mesh.halfedge(src_v), mesh), vend(vbegin);
		do {
			auto v = *vbegin++;
			//int vid = v.idx;
			if (visited_set.find(v) != visited_set.end()) {
				visited_set.insert(v);
				Point3 p = currMeshStorage->m_mesh->point(v);
				float squared_dist = (localPosPoint - p).squared_length();
				if (squared_dist < squared_radius) {
					v_stack.push(v);
					//meshStorage->m_deformation->insert_roi_vertex(v);
				}
			}
		} while (vbegin != vend);
	}



	
}


void RaiseVertices(Unity_Vector3 localPos, float radius) {
	Point3 localPosPoint = Point3(localPos.x, localPos.y, localPos.z);

	Mesh& mesh = *currMeshStorage->m_mesh;

	float squared_radius = radius * radius;
	auto closestPoint = currMeshStorage->m_tree->closest_point_and_primitive(localPosPoint);
	Face_index face_idx = closestPoint.second;
	Point3 startPoint = closestPoint.first;
	Halfedge_index he_idex = currMeshStorage->m_mesh->halfedge(face_idx);
	Vertex_index v_idex = currMeshStorage->m_mesh->target(he_idex);
	std::stack<Vertex_index> v_stack = std::stack<Vertex_index>();
	std::unordered_set<Vertex_index> visited_set = std::unordered_set<Vertex_index>();
	v_stack.push(v_idex);

	std::stack< Vertex_index> v_select = std::stack<Vertex_index>();

	while (!v_stack.empty()) {
		Vertex_index src_v = v_stack.top();
		v_stack.pop();
		CGAL::Vertex_around_target_circulator<Mesh> vbegin(mesh.halfedge(src_v), mesh), vend(vbegin);
		do {
			auto v = *vbegin++;
			//int vid = v.idx;
			if (visited_set.find(v) == visited_set.end()) {
				visited_set.insert(v);
				Point3 p = currMeshStorage->m_mesh->point(v);
				float squared_dist = (localPosPoint - p).squared_length();
				if (squared_dist < squared_radius) {
					v_stack.push(v);
					v_select.push(v);
					//meshStorage->m_deformation->insert_roi_vertex(v);
				}
			}
		} while (vbegin != vend);
	}


	while (!v_select.empty())
	{
		Vertex_index src_v = v_select.top();
		v_select.pop();
		Vector3 test = Vector3(10,0,0);
		Point3 p = mesh.point(src_v);
		mesh.point(src_v) = p+test;
	}
	

}
