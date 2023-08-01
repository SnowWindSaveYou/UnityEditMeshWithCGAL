
#include "..//include/grab_brush_tool.h"
#include "..//include/mesh_storage_manager.h"
#include "..//include/edit_tool_manager.h"
#include "..//include/cgal_defs.h"
#include <math.h>
GrabBrushTool::GrabBrushTool()
{
	v_select = std::vector<std::pair<Vertex_index,float>>();
	isEditBegin = false;
}

GrabBrushTool::~GrabBrushTool()
{
}

void GrabBrushTool::OnSelected() {

}
void GrabBrushTool::OnRemoved() {

}

void GrabBrushTool::OnEditBegin() {
	MeshStorage* ms = MeshStorageManager::getInstance()->current;
	EditToolManager* etm = EditToolManager::getInstance();
	Mesh &mesh = *ms->m_mesh;
	Tree &tree = *ms->m_tree;


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

	float radius = 0.3f;
	float squared_radius = radius * radius;

	auto closestPoint = ms->m_tree->closest_point_and_primitive(hitPoint);
	Face_index face_idx = closestPoint.second;
	Point3 startPoint = closestPoint.first;

	Halfedge_index he_idex = mesh.halfedge(face_idx);
	Vertex_index v_idex = mesh.target(he_idex);
	std::stack<Vertex_index> v_stack = std::stack<Vertex_index>();
	std::unordered_set<Vertex_index> visited_set = std::unordered_set<Vertex_index>();
	v_stack.push(v_idex);

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
					float w =  squared_dist / squared_radius;
					w = 1 - w;
					v_select.push_back(std::make_pair( v,w));
					//meshStorage->m_deformation->insert_roi_vertex(v);
				}
			}
		} while (vbegin != vend);
	}
	lastPos = hitPoint;
}

void GrabBrushTool::OnEditEnd() {
	v_select.clear();
}

void GrabBrushTool::OnEditProcess() {
	if (!isEditBegin)return;
	auto ms = MeshStorageManager::getInstance()->current;
	Mesh& mesh = *ms->m_mesh;
	Point3 pos = EditToolManager::getInstance()->m_ray.source();
	Vector3 vec = pos - lastPos;
	for (auto v : v_select) {
		mesh.point(v.first) += vec*v.second;
	}
	lastPos = pos;
}