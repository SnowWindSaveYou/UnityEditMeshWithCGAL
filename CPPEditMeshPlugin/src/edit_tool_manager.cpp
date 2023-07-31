#include "..//include/edit_tool_manager.h"

#include "..//include/grab_brush_tool.h"


//init
EditToolManager* EditToolManager::g_pInstance = new (std::nothrow) EditToolManager();


EditToolManager::EditToolManager()
{
	this->editToolList = std::map<int, IEditTool*>();
	this->editToolList.insert(std::make_pair(0, new GrabBrushTool()));
	this->SetCurrentTool(0);
}

EditToolManager::~EditToolManager()
{
}

void EditToolManager::SetCurrentTool(int idx) {
	currentEditTool = editToolList.at(idx);
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
