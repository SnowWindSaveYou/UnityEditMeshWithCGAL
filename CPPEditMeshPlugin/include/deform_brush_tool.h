#ifndef DEFORM_BRUSH_TOOL_H
#define DEFORM_BRUSH_TOOL_H


#include "I_edit_tool.h"
#include "cgal_defs.h"

class DeformBrushTool : public IEditTool
{
public:
	DeformBrushTool();
	~DeformBrushTool();

	void OnSelected();
	void OnRemoved();
	void OnEditBegin();
	void OnEditEnd();
	void OnEditProcess();

private:

	bool isEditBegin;
	Point3 lastPos;
	std::vector<Vertex_index> v_select_roi;
	std::vector<Vertex_index> v_select_control;
	Vertex_index v_current_control;

	std::shared_ptr < SM_Deformation> m_deformation;//TODO

};


#endif // !DEFORM_BRUSH_TOOL_H
