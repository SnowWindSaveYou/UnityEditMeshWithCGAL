#ifndef EDIT_TOOL_MANAGER_H
#define EDIT_TOOL_MANAGER_H

#include "I_edit_tool.h"
#include "cgal_defs.h"

class EditToolManager
{
public:
	EditToolManager();
	~EditToolManager();
	EditToolManager(const EditToolManager&) = delete;
	EditToolManager(const EditToolManager&&) = delete;
	EditToolManager& operator=(const EditToolManager&) = delete;

	static EditToolManager* getInstance() {
		return g_pInstance;
	};
	static void deleteInstance() {
		if (g_pInstance) {
			delete g_pInstance;
			g_pInstance = nullptr;
		}
	}

	Ray m_ray;
	IEditTool* currentEditTool;
	std::map<int, IEditTool*> editToolList;

	void SetCurrentTool(int idx);



	void ProcessToolBegin();
	void ProcessToolEnd();

	// TODO put into independent thread
	void ProcessToolUpdate();


private:
	static EditToolManager* g_pInstance;
};


#endif // !EDIT_TOOL_MANAGER_H
