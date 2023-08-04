#include "..//include/edit_tool_manager.h"

#include "..//include/grab_brush_tool.h"

#include "..//include/deform_brush_tool.h"



//init
EditToolManager* EditToolManager::g_pInstance = new (std::nothrow) EditToolManager();


EditToolManager::EditToolManager()
{
	this->editToolList = std::map<int, IEditTool*>();
	this->editToolList.insert(std::make_pair(1, new GrabBrushTool()));
	this->editToolList.insert(std::make_pair(2, new DeformBrushTool()));
	this->SetCurrentTool(0);
}

EditToolManager::~EditToolManager()
{
}

void EditToolManager::SetCurrentTool(int idx) {
	if (idx == 0) {

	}
	else {
		currentEditTool = editToolList.at(idx);
	}
}


void EditToolManager::ProcessToolBegin() {
	currentEditTool->OnEditBegin();
}
void EditToolManager::ProcessToolEnd() {
	currentEditTool->OnEditEnd();
}

// TODO put into independent thread
void EditToolManager::ProcessToolUpdate() {
	currentEditTool->OnEditProcess();
}
