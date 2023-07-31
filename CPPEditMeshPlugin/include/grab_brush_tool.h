#ifndef GRAB_BRUSH_TOOL_H
#define GRAB_BRUSH_TOOL_H

#include "I_edit_tool.h"

class GrabBrushTool : public IEditTool
{
public:
	GrabBrushTool();
	~GrabBrushTool();

	void OnSelected();
	void OnRemoved();
	void OnEditBegin();
	void OnEditEnd();
	void OnEditProcess();

private:

	bool isEditBegin;
	Point3 lastPos;
	std::vector< Vertex_index> v_select;
};


#endif // !GRAB_BRUSH_TOOL_H
