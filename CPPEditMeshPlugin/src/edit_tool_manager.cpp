#include "..//include/edit_tool_manager.h"

#include "..//include/grab_brush_tool.h"

#include "..//include/deform_brush_tool.h"



//init
EditToolManager* EditToolManager::g_pInstance = new (std::nothrow) EditToolManager();


EditToolManager::EditToolManager()
{
	this->keysMap = std::map<INTERACTION_KEYS, bool>();
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



void EditToolManager::SetKey(INTERACTION_KEYS key, bool state) {
	if (keysMap.find(key) == keysMap.end()) {
		keysMap.insert(std::make_pair(key, state));
	}
	else {
		keysMap.at(key) = state;
	}
}

bool EditToolManager::GetKey(INTERACTION_KEYS key) {
	if (keysMap.find(key) == keysMap.end()) {
		return false;
	}
	return	keysMap.at(key);

}


void EditToolManager::SetSelectionBuffer(int* selectedBuffer) {
	this->m_selected_vertices = selectedBuffer;
	this->SetKey(DISPLAY_SELECTION, true);
}







#pragma region export

void SetSelectionBuffer(int* selectedBuffer) {
	EditToolManager::getInstance()->SetSelectionBuffer(selectedBuffer);

}
#pragma endregion
