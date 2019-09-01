#ifndef _EDITOR_MAIN_H
#define _EDITOR_MAIN_H

#include <vector>
#include <stack>
#include <map>

//define to build software

//#include "EFF_Engine.h"
//#include "ImguiWrapper.h"
//
//#include "ImguiFunctionView.h"
//#include "PickingCircle.h"
//#include "CommandManager.h"
//#include "Multiselect.h"
//
//#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))
//
///////// === ImguiFunction.cpp === //////
//extern const float				COMPONENTS_REMOVE_OFFSETS;
//extern const float				COMPONENTS_GAP_OFFSETS;
//extern const float				menu_bar_offsety;
//extern const float				total_offsetY;
//
//extern Vector3					imgui_previous_cam_position;
//extern const Vector2			create_arch_pos;
//extern bool						toogle_menubar_help;
//extern bool						placing_archetype;
//extern void*					previousVar;
//extern bool						showConfirmWindow;
//extern std::string				confirmWindowName;
//extern std::string				confirmMsg;
//
//extern std::map<std::string, std::string> sceneFile_map;
//extern std::string selectedArchPath;
//
//extern float graphicsTime;
//extern float physicsTime;
//extern float scriptsTime;
//extern float audioTime;

void EditorInit();
void EditorUpdate();		//main Editor Update
void EditorInputUpdate();	//Editor's Input Update


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
