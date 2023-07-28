
#include "..//include/I_edit_tool.h"

class GrabBrushTool: public IEditTool
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


};

GrabBrushTool::GrabBrushTool()
{
}

GrabBrushTool::~GrabBrushTool()
{
}


void GrabBrushTool::OnEditBegin() {

}