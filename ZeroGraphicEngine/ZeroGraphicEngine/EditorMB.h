
#ifndef _EDITOR_MB_H
#define _EDITOR_MB_H

#include "EditorMain.h"
#include "Selection.h"

//software building
enum TOGGLE_WINDOW_BOOL {
	WB_QUATERNION = 0,

	WB_PROPERTIES,
	WB_INSPECTOR,
	WB_END
};

extern bool toggle_WB[WB_END];
extern const char* toggle_name[WB_END];



//______________________________________________________________________ editor's menubar
extern const float editor_menubar_offsety;			//EditorMB.cpp

void EditorMBInit();								//EditorMB.cpp
void EditorMainMenuBarUpdate();						//EditorMB.cpp

//______________________________________________________________________ Create Tab
void EditorMainMenuBar_Create();					//MB_Create.cpp
PickingCircle* CreateGameObject(int type_, const std::string& name_ = "GameObject");

//______________________________________________________________________ Software Tab
void BuildQuaternionCalculator();					//MB_Software_QuaternionCalculator.cpp

//______________________________________________________________________ Window Tab
void EditorMainMenuBar_Windows();										  //MB_Window.cpp

extern Selection selectedObj;		//MB_Window_Inspector.cpp										
void MB_Window_Inspector();			//MB_Window_Inspector.cpp									  


void MB_Window_Properties();												//MB_Window_Properties.cpp
void SelectionUpdate_UponGameObjectAddAndDeleted(PickingCircle* picker_);	//MB_Window_Inspector.cpp
void SelectObj(GameObject* obj_);											//MB_Window_Inspector.cpp			
//void ShowConfirmationWindow();							
//
//void SelectSingle(PickingCircle* obj_);
//void SelectSingle(GameObject* obj_);
//void UnSelectObjectAndSaveInfo();
//Selection& GetSelectedObj();
//std::vector<Selection>& GetAllSelected();
//
//PickingCircle* GetPickFromID(int ID);
//void UpdateAllObjectsOnScreen();
//
//void LoadSceneFile();
//ImVec4 ConvertColorToImVec4(const Color& c);
//bool CompareGameObject(GameObject* lhs, GameObject* rhs);
//
//void ShowNonErrorWindow(const char* msg, const char* windowTitle);
//void ShowErrorWindow(const char* msg);
//
///////// === ShortCutbarcpp === ///////
//extern const float	shortcut_bar_offsety;
//extern ImGuizmo::OPERATION mCurrentGizmoOperation;
//extern ImGuizmo::MODE mCurrentGizmoMode;
//extern bool toggle_shortcut_snap;
//extern Vector3 snap_value;
//extern Vector3 boundsSnap;
//extern bool boundSizing;
//extern bool boundSizingSnap;
//
////together
//extern bool toggle_shortcut_2Dview;
//extern bool toggle_shortcut_draw_grid;
//extern bool toggle_shortcut_draw_lights;
//extern bool toggle_shortcut_draw_physics;
//extern bool toggle_shortcut_draw_physics_all;
//extern bool toggle_shortcut_draw_pickingcircle;
//extern bool toggle_shortcut_draw_skybox;
//extern bool toggle_shortcut_draw_wireframe;
//extern bool toggle_shortcut_draw_skeleton;
//
//extern bool toggle_shortcut_play;
//
//
//void Show_ShortCutBar();
//void Stop_Function();
//void Free_Function();
//
///////// === ImguiFunction_KeyBoardMouseInput.cpp === ///////
//extern std::vector<PickingCircle*> pickingChoice;
//void Editor_Input();
//void Editor_DuringPlay_Input();
//
///////// === ImguiShowHelp.cpp === ///////
//void ShowToggleHelp();

#endif	//_IMGUIFUNCTION_H
