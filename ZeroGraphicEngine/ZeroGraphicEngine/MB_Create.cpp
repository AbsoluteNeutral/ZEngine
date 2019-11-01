
#include "stdafx.h"
#include "EditorMB.h"
#include <map>

enum CREATE_TYPE_ID {
	_Empty = 0,
	_Cube,
	_Pyramid,
	_Model_Heavy,
	_Model_Asmodeus,
	_Cube27,
	//_Camera
	_endingcountnumber
};

const char* _CreatedModelName[]{
	"Empty",
	"Cube",
	"Pyramid",
	"Model_Heavy",
	"Model_Asmodeus",
	"Cube27"
};

PickingCircle* CreateGameObject(int type_, const std::string& name_) {
	GameObject* obj = nullptr;
	PickingCircle* picker = RM::CreateGameObjectWithPickingCircle(obj);

	switch (type_) {
	case _Cube: //1
		RM::CreateMeshRenderer(obj, "Cube.models", "black.dds");
		obj->name = name_;
		break;
	case _Pyramid:
		RM::CreateMeshRenderer(obj, "pyramid.models", "checkerbox_2048x2048.dds");
		obj->name = name_;
		break;
	case _Model_Heavy:
		RM::CreateMeshRenderer(obj, "Heavy.models", "Gabriel_D.dds");
		obj->name = name_;
		break;
	case _Model_Asmodeus:
		RM::CreateMeshRenderer(obj, "Asmodeus.models", "Lucio_D.dds");
		obj->name = name_;
		break;
	case _Cube27:
		RM::CreateMeshRenderer(obj, "cube27.models", "black.dds");
		obj->name = name_;
		break;
	default:
		break;
	}
	return picker;
}

void EditorMainMenuBar_Create() {
	if (ImGui::BeginMenu("Create")) 
	{
		for (int i = 0; i < _endingcountnumber; ++i)
		{
			if (ImGui::MenuItem(_CreatedModelName[i]))
			{
				PickingCircle* picker = nullptr;
				picker = CreateGameObject(i, _CreatedModelName[i] + std::to_string(RM::R_GameObject.size()));
				SelectionUpdate_UponGameObjectAddAndDeleted(picker);
				break;
			}
		}

		ImGui::EndMenu();
	}
}
