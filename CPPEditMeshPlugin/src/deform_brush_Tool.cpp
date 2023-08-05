
#include "..//include/deform_brush_tool.h"
#include "..//include/mesh_storage_manager.h"
#include "..//include/edit_tool_manager.h"
#include "..//include/cgal_defs.h"
#include <math.h>



DeformBrushTool::DeformBrushTool()
{
	v_select_roi = std::vector<Vertex_index>();
	v_select_control = std::vector<Vertex_index>();
	isEditBegin = false;
}

DeformBrushTool::~DeformBrushTool()
{
}

void DeformBrushTool::OnSelected() {
	MeshStorage* ms = MeshStorageManager::getInstance()->current;
	EditToolManager* etm = EditToolManager::getInstance();
	//ms->m_deformation->reset();
	m_deformation.reset(new SM_Deformation(*ms->m_mesh));

	v_select_roi.clear();
	v_select_control.clear();

	if (etm->GetKey(DISPLAY_SELECTION)) {
		memset(etm->m_selected_vertices, 0, sizeof(int) * ms->m_vertexCount);
	}

}
void DeformBrushTool::OnRemoved() {

}

void DeformBrushTool::OnEditBegin() {
	MeshStorage* ms = MeshStorageManager::getInstance()->current;
	EditToolManager* etm = EditToolManager::getInstance();
	Mesh& mesh = *ms->m_mesh;
	Tree& tree = *ms->m_tree;

	bool displaySelection = etm->GetKey(DISPLAY_SELECTION);


	Point3 hitPoint;
	Ray ray = etm->m_ray;
	if (tree.do_intersect(etm->m_ray)) {

		Face_location ray_location = PMP::locate_with_AABB_tree(etm->m_ray, tree, mesh);
		hitPoint = PMP::construct_point(ray_location, mesh);
		isEditBegin = true;
	}
	else {
		isEditBegin = false;
		return;
	}
	//hitPoint = ray.source();

	float radius = 0.5f;
	float squared_radius = radius * radius;

	auto closestPoint = ms->m_tree->closest_point_and_primitive(hitPoint);
	Face_index face_idx = closestPoint.second;
	Point3 startPoint = closestPoint.first;

	Halfedge_index he_idex = mesh.halfedge(face_idx);
	Vertex_index v_idex = mesh.target(he_idex);
	std::stack<Vertex_index> v_stack = std::stack<Vertex_index>();
	std::unordered_set<Vertex_index> visited_set = std::unordered_set<Vertex_index>();
	v_stack.push(v_idex);

	// control
	m_deformation->insert_control_vertex(v_idex);
	v_current_control = v_idex;
	v_select_roi.push_back(v_idex);

	// roi
	while (!v_stack.empty()) {
		Vertex_index src_v = v_stack.top();
		v_stack.pop();
		CGAL::Vertex_around_target_circulator<Mesh> vbegin(mesh.halfedge(src_v), mesh), vend(vbegin);
		do {
			auto v = *vbegin++;
			//int vid = v.idx;
			if (visited_set.find(v) == visited_set.end()) {
				visited_set.insert(v);
				Point3 p = mesh.point(v);
				float squared_dist = (hitPoint - p).squared_length();
				if (squared_dist < squared_radius) {
					v_stack.push(v);
					v_select_roi.push_back(v);
					if (displaySelection) {
						etm->m_selected_vertices[ms->m_vmap.at(v)] = 1;
					}
				}
			}
		} while (vbegin != vend);
	}
	lastPos = hitPoint;

	// 
	//ms->m_deformation->insert_control_vertices(v_select.begin(),v_select.end());
	m_deformation->insert_roi_vertices(v_select_roi.begin(), v_select_roi.end());
	m_deformation->preprocess();


	// display state
	//if (etm->GetKey(DISPLAY_SELECTION)) {
	//	for (auto v : v_select_roi) {

	//	}
	//}

}

void DeformBrushTool::OnEditEnd() {
	v_select_roi.clear();
	MeshStorageManager::getInstance()->current->UpdateTree();
}

void DeformBrushTool::OnEditProcess() {
	if (!isEditBegin)return;
	auto ms = MeshStorageManager::getInstance()->current;
	Mesh& mesh = *ms->m_mesh;
	Point3 pos = EditToolManager::getInstance()->m_ray.source();
	Vector3 vec = pos - lastPos;
	//Vector3 vec = Vector3(0.1, 0, 0);
	auto p = mesh.point(v_current_control) + vec;
	m_deformation->set_target_position(v_current_control, p);
	m_deformation->deform(10,5);


	lastPos = pos;
}