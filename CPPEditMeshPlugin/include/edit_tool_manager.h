#ifndef EDIT_TOOL_MANAGER_H
#define EDIT_TOOL_MANAGER_H

#include "I_edit_tool.h"
#include "cgal_defs.h"


struct BrushInfo {
	Point3 pos;
	Vector3 dir;
	float radius;
	float intensity;
};

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
	BrushInfo m_brushInfo;

	IEditTool* currentEditTool;
	std::map<int, IEditTool*> editToolList;

	std::map<INTERACTION_KEYS, bool> keysMap;

	int* m_selected_vertices;


	void SetCurrentTool(int idx);

	void ProcessToolBegin();
	void ProcessToolEnd();
	// TODO put into independent thread
	void ProcessToolUpdate();

	void SetKey(INTERACTION_KEYS key, bool state);

	bool GetKey(INTERACTION_KEYS key);

	void SetSelectionBuffer(int* selectedBuffer);


private:
	static EditToolManager* g_pInstance;
};


#ifdef __cplusplus
extern "C" {
#endif


	__declspec(dllexport) void SetSelectionBuffer(int* selectedBuffer);


#ifdef __cplusplus
}
#endif



#endif // !EDIT_TOOL_MANAGER_H
