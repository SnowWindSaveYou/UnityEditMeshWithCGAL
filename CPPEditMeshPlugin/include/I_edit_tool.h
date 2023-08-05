#ifndef I_EDIT_TOOL_H
#define I_EDIT_TOOL_H

#include "keys_enums.h"
#include "cgal_defs.h"
class IEditTool {
public:

	//static Ray s_ray;


	//IEditTool();
	//~IEditTool();
	////
	virtual void OnSelected() {};
	virtual void OnRemoved() {};

	virtual void OnEditBegin() {};
	virtual void OnEditEnd() {};
	virtual void OnEditProcess() {};

};

#endif // !I_EDIT_TOOL