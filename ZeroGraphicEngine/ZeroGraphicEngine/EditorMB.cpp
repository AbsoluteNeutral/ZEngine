/*****************************************************************************/
/*!
\file			EditorMenuBar.cpp
*/
/*****************************************************************************/
#include "stdafx.h"
#include "EditorMB.h"
#include <map>

std::map<int, void(*)()> function_Runner;
//WINDOW_LAYOUT windowLayout = WL_ALL_RIGHT;
//bool layoutChosen[WL_COUNT] = { windowLayout == 0, windowLayout == 1, windowLayout == 2 };

bool toggle_WB[WB_END]{};
const char* toggle_name[WB_END]{
	"Quaternion_Calculator",

	"Properties",
	"Inspector"
};

const float editor_menubar_offsety = 20.0f;

void EditorMBInit() {
	for (unsigned i = 0; i < int(WB_END); ++i) {
		toggle_WB[i] = false;
	}
	toggle_WB[WB_PROPERTIES] = true;
	toggle_WB[WB_INSPECTOR] = true;

	function_Runner[WB_QUATERNION] = BuildQuaternionCalculator;

	function_Runner[WB_INSPECTOR]  = MB_Window_Inspector;
	function_Runner[WB_PROPERTIES] = MB_Window_Properties;
}


void EditorMainMenuBarUpdate() {
	if (ImGui::BeginMainMenuBar()) {

		//Create tab
		EditorMainMenuBar_Create();
		EditorMainMenuBar_Windows();
		//SoftWareTab
		//if (ImGui::BeginMenu("Software")) {
		//	ImGui::MenuItem("Quaternion Calculator", NULL, &toggle_WB[WB_QUATERNION]);
		//	//if (ImGui::MenuItem("Quaternion Calculator", NULL, &toogle_SWB[SWB_QUATERNION])) {
		//	//	function_Runner[SWB_QUATERNION] = BuildQuaternionCalculator;
		//	//}
		//
		//
		//	ImGui::EndMenu();
		//}


		ImGui::EndMainMenuBar();
	}

	for (unsigned i = 0; i < int(WB_END); ++i) {
		if (toggle_WB[i])
			function_Runner[i]();
	}

}
//void MenuBar(){
//	if (ImGui::BeginMainMenuBar()){
////////////////////////////////////////////////////////////////////////////////////////////////////// === FILE === //////////
//#pragma region FILE
//		if (ImGui::BeginMenu("File")) {
//
//			if (ImGui::MenuItem("Load Scene (CTRL + O)"))
//			{
//				LoadSceneFile();
//				imgui_click_menubar_load = true;
//			}
//
//			if (ImGui::MenuItem("Save Scene (CTRL + S)"))
//				imgui_click_menubar_save = true;
//
//			if (ImGui::MenuItem("Clear Scene"))
//				imgui_click_menubar_clear = true;
//
//			if (ImGui::MenuItem("Import Image"))
//			{
//				logLine("Opening...");
//				char filename[MAX_PATH];
//
//				OPENFILENAMEA ofn;
//				ZeroMemory(&filename, sizeof(filename));
//				ZeroMemory(&ofn, sizeof(ofn));
//				ofn.lStructSize = sizeof(ofn);
//				ofn.hwndOwner = NULL;  // If you have a window to center over, put its HANDLE here
//				ofn.lpstrFilter = "Image Files (*.png)";
//				ofn.lpstrFile = filename;
//				ofn.nMaxFile = MAX_PATH;
//				ofn.lpstrTitle = "Choose image file";
//				ofn.Flags = OFN_DONTADDTORECENT | OFN_FILEMUSTEXIST;
//				ofn.lpstrInitialDir = "..\\all_resources\\art\\";
//				// Do stuff here when file has been selected.
//				if (GetOpenFileNameA(&ofn)) {
//					CopyFileToDirectory(filename, PATH_TEXTURES);
//				}
//				else
//				{
//					// All this stuff below is to tell you exactly how you messed up above. 
//					// Once you've got that fixed, you can often (not always!) reduce it to a 'user cancelled' assumption.
//					switch (CommDlgExtendedError()) {
//					case CDERR_DIALOGFAILURE: std::cout << "CDERR_DIALOGFAILURE\n";   break;
//					case CDERR_FINDRESFAILURE: std::cout << "CDERR_FINDRESFAILURE\n";  break;
//					case CDERR_INITIALIZATION: std::cout << "CDERR_INITIALIZATION\n";  break;
//					case CDERR_LOADRESFAILURE: std::cout << "CDERR_LOADRESFAILURE\n";  break;
//					case CDERR_LOADSTRFAILURE: std::cout << "CDERR_LOADSTRFAILURE\n";  break;
//					case CDERR_LOCKRESFAILURE: std::cout << "CDERR_LOCKRESFAILURE\n";  break;
//					case CDERR_MEMALLOCFAILURE: std::cout << "CDERR_MEMALLOCFAILURE\n"; break;
//					case CDERR_MEMLOCKFAILURE: std::cout << "CDERR_MEMLOCKFAILURE\n";  break;
//					case CDERR_NOHINSTANCE: std::cout << "CDERR_NOHINSTANCE\n";     break;
//					case CDERR_NOHOOK: std::cout << "CDERR_NOHOOK\n";          break;
//					case CDERR_NOTEMPLATE: std::cout << "CDERR_NOTEMPLATE\n";      break;
//					case CDERR_STRUCTSIZE: std::cout << "CDERR_STRUCTSIZE\n";      break;
//					case FNERR_BUFFERTOOSMALL: std::cout << "FNERR_BUFFERTOOSMALL\n";  break;
//					case FNERR_INVALIDFILENAME: std::cout << "FNERR_INVALIDFILENAME\n"; break;
//					case FNERR_SUBCLASSFAILURE: std::cout << "FNERR_SUBCLASSFAILURE\n"; break;
//					default: std::cout << "You cancelled.\n";
//					}
//				}
//			}
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem("Exit"))
//				imgui_click_menubar_exit = true;
//
//			ImGui::EndMenu();
//		}
//#pragma endregion
//
////////////////////////////////////////////////////////////////////////////////////////////////////// === EDIT === //////////
//#pragma region  EDIT
//		if (ImGui::BeginMenu("Edit")) {
//			if (ImGui::MenuItem("(CTRL + Z) Undo"))
//			{
//				CmdMgr.Undo();
//			}
//			if (ImGui::MenuItem("(CTRL + Y) Redo"))
//			{
//				CmdMgr.Redo();
//			}
//
//			ImGui::Separator();
//			if (ImGui::MenuItem("(CTRL + C) Copy"))
//			{
//				Copy();
//			}
//			if (ImGui::MenuItem("(CTRL + V) Paste"))
//			{
//				Paste();
//			}
//			if (ImGui::MenuItem("(CTRL + D) Duplicate"))
//			{
//				Duplicate();
//			}
//			if (ImGui::MenuItem("(Delete)   Delete"))
//			{
//				Delete();
//			}
//
//			ImGui::Separator();
//			if (ImGui::MenuItem("Set Parent..."))
//			{
//				imgui_click_menubar_attach = true;
//			}
//			if (ImGui::MenuItem("(CTRL + G) Group Selection"))
//			{
//				Group();
//			}
//			if (ImGui::MenuItem("(ALT + G)  Unparent"))
//			{
//				RemoveFromGroup();
//			}
//
//			ImGui::Separator();
//			if (ImGui::MenuItem("(CTRL + R) Save Archetype"))
//			{
//				imgui_click_menubar_saveArch = true;
//			}
//			
//
//			ImGui::Separator();
//			if (ImGui::BeginMenu("Settings", &toggle_menubar_edit_settings))
//			{
//				if (ImGui::MenuItem("Game"))
//				{
//					toggle_menubar_edit_settings = true;
//					menubar_edit_settings_type = _SETTINGS_GAME;
//				}
//				if (ImGui::MenuItem("Input"))
//				{
//					toggle_menubar_edit_settings = true;
//					Update_Settings_Input();
//					menubar_edit_settings_type = _SETTINGS_INPUT;
//				}
//				if (ImGui::MenuItem("Graphics"))
//				{
//					toggle_menubar_edit_settings = true;
//					menubar_edit_settings_type = _SETTINGS_GRAPHICS;
//				}
//				if (ImGui::MenuItem("Physics"))
//				{
//					toggle_menubar_edit_settings		  = true;
//					menubar_edit_settings_type = _SETTINGS_PHYSICS;
//				}
//				ImGui::EndMenu();
//			}
//
//
//			ImGui::EndMenu();
//		}
//#pragma endregion
//
////////////////////////////////////////////////////////////////////////////////////////////////////// === SELECTION === //////////
//#pragma region  EDIT
//		if (ImGui::BeginMenu("Selection")) {
//			if (ImGui::MenuItem("Clear Selection"))
//			{
//				Multiselection.Clear();
//			}
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem("(CTRL + A) Select All"))
//			{
//				for (auto& pick : RE::R_PICKING)
//					Multiselection.Insert(pick);
//			}
//
//			if (ImGui::MenuItem("(CTRL + I) Select Inverse"))
//			{
//				SelectInverse();
//			}
//
//			ImGui::EndMenu();
//		}
//#pragma endregion
//
////////////////////////////////////////////////////////////////////////////////////////////////////// === CREATE === //////////
//#pragma region CREATE
//		if (ImGui::BeginMenu("Create"))
//		{
//			if (ImGui::MenuItem("Empty Game Object"))
//				CreateGameObject(_EMPTY, "GameObject" + std::to_string(RM::R_GameObject.size()));
//
//			else if (ImGui::MenuItem("Child Game Object"))
//			{
//				if (CheckSelectionType() == SEL_NONE)
//				{
//					ImGui::EndMenu();
//					return;
//				}
//
//				GameObject* parent = GetSelectedObj().gameObject;
//				GameObject* created = CreateGameObject(_EMPTY, "GameObject" + std::to_string(RM::R_GameObject.size()));
//
//				GroupCommand* cmd = Allocate<GroupCommand>(1);
//				cmd->SetParentObj(parent);
//				CmdMgr.RunCommand(cmd);
//
//				created->transform.SetLocalPosition(0.0f, 0.0f, 0.0f);
//
//				Inspector.SetCollapseAll(false);
//			}
//
//			else if (ImGui::MenuItem("Cube"))
//				CreateGameObject(_CUBE, "Cube" + std::to_string(RM::R_GameObject.size()) );
//
//			//else if (ImGui::MenuItem("2D Sprite"))
//			//	CreateGameObject(_SPRITE2D, "Sprite2D" + std::to_string(RM::R_GameObject.size()));
//
//			else if (ImGui::MenuItem("Camera"))
//				CreateGameObject(_CAMERA, "Camera" + std::to_string(RM::R_Camera.size()));
//
//			else if (ImGui::MenuItem("Light"))
//				CreateGameObject(_LIGHT, "Light" + std::to_string(RM::R_Light.size()));
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem("UISprite"))
//				CreateGameObject(_UISPRITE, "UISprite" + std::to_string(RM::R_UISprite.size()));
//
//			else if (ImGui::MenuItem("UIText"))
//				CreateGameObject(_TEXT, "Text" + std::to_string(RM::R_GameObject.size()));
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem("Script"))
//			{
//				imgui_click_create_script = true;
//			}
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem("Add/Remove GameTag/CollisionGroup", NULL, &toggle_menubar_create_addremovegametag))
//			{
//				if (toggle_menubar_create_addremovegametag)
//					UpdateCollisionGroup();
//			}
//
//			ImGui::Separator();
//			//if (ImGui::BeginMenu("Set Game Camera"))
//			//{
//			//	for (unsigned i = 0; i < RM::R_Camera.size(); ++i) {
//			//		if (ImGui::MenuItem(RM::R_Camera[i]->owner->name.c_str(), NULL))
//			//		{
//			//			GAMECamera_NUM = i;
//			//		}
//			//	}
//			//	ImGui::EndMenu();
//			//}
//
//			if (ImGui::MenuItem("Set Object Pool"))
//			{
//				imgui_addToPool_clicked = true;
//			}
//
//			ImGui::EndMenu();
//		}
//#pragma endregion
//
////////////////////////////////////////////////////////////////////////////////////////////////////// === VIEW === //////////
//#pragma region VIEW
//		if (ImGui::BeginMenu("View"))
//		{
//			if (ImGui::MenuItem("(F11) FullScreen"))
//				FullScreen(!IsFullScreen());
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem("(F3) Toggle Persp/Ortho view"))
//				toggle_shortcut_2Dview = !toggle_shortcut_2Dview;
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem("(C)  Look at selection"))
//				LookAtSelection();
//
//			ImGui::EndMenu();
//		}
//#pragma endregion
//
////////////////////////////////////////////////////////////////////////////////////////////////////// === WINDOW === //////////
//#pragma region WINDOW
//
//		if (ImGui::BeginMenu("Window"))
//		{
//			if (ImGui::BeginMenu("Layout"))
//			{
//				if (ImGui::MenuItem("Inspector LEFT, Properties RIGHT", NULL, &layoutChosen[WL_PROPERTIES_RIGHT]))
//				{
//					windowLayout = WL_PROPERTIES_RIGHT;
//
//					for (int i = 0; i < WL_COUNT; ++i)
//						if (i != WL_PROPERTIES_RIGHT)
//							layoutChosen[i] = false;
//				}
//
//				if (ImGui::MenuItem("Properties LEFT, Inspector RIGHT", NULL, &layoutChosen[WL_PROPERTIES_LEFT]))
//				{
//					windowLayout = WL_PROPERTIES_LEFT;
//
//					for (int i = 0; i < WL_COUNT; ++i)
//						if (i != WL_PROPERTIES_LEFT)
//							layoutChosen[i] = false;
//				}
//
//				if (ImGui::MenuItem("All At Right", NULL, &layoutChosen[WL_ALL_RIGHT]))
//				{
//					windowLayout = WL_ALL_RIGHT;
//
//					for (int i = 0; i < WL_COUNT; ++i)
//						if (i != WL_ALL_RIGHT)
//							layoutChosen[i] = false;
//				}
//
//				ImGui::EndMenu();
//			}
//
//			ImGui::Separator();
//
//			if (ImGui::MenuItem(char_Assets, NULL, &toggle_menubar_window_Assets))
//			{
//				currAssetTab = TAB_ASSETS;
//				WindowName = char_Assets;
//			}
//			if (ImGui::MenuItem(char_History, NULL, &toggle_menubar_window_Assets))
//			{
//				currAssetTab = TAB_HISTORY;
//				WindowName = char_History;
//			}
//			if (ImGui::MenuItem(char_LuaConsole, NULL, &toggle_menubar_window_Assets))
//			{
//				currAssetTab = TAB_LUACONSOLE;
//				WindowName = char_LuaConsole;
//			}
//
//			ImGui::MenuItem(char_Inspector, NULL, &toggle_menubar_window_Inspector);
//			ImGui::MenuItem(char_Properties, NULL, &toggle_menubar_window_Properties);
//			ImGui::MenuItem("CutsceneManager", NULL, &toggle_cutscene_timeline);
//			ImGui::MenuItem("GlobalValues", NULL, &toggle_globalValues_display);
//
//			ImGui::Separator();
//			ImGui::MenuItem(char_debuginfo, NULL, &toggle_menubar_window_debuginfo);
//
//			ImGui::EndMenu();
//		}
//
//		//Update
//		if (toggle_menubar_window_Assets)		Show_Assets();
//		if (toggle_menubar_window_Properties)	ShowProperties();
//		if (toggle_menubar_window_Inspector)	Inspector.Show_Inspector();
//		//if (toggle_menubar_window_debuginfo) Show_Window_DebugInfo();
//		if (toggle_animation_timeline) ShowAnimationTimeline();
//		if (toggle_cutscene_timeline) ShowCutsceneTimeline();
//		if (toggle_globalValues_display) ShowGlobalValues();
//#pragma endregion
//
//	//////////////////////////////////////////////////////////////////////////////////////////////////// === HELP === //////////
//		//ImGui::Checkbox("(F1) Help ", &toogle_menubar_help);
//		ImGui::EndMainMenuBar();
//	}//end menu bar
//
//	 ////////// === UPDATE === //////////
//#pragma region UPDATE
//	 
//	//File
//	if (imgui_click_menubar_load)		ClickLoad();
//	if (imgui_click_menubar_save)		ClickSave();
//	if (imgui_click_menubar_clear)		ClickClearScene();
//	if (imgui_click_menubar_exit)		ClickExit();
//	if (imgui_click_menubar_attach)		AttachCustomParent();
//	if (imgui_click_menubar_saveArch)	SaveArchetype();
//	if (imgui_addToPool_clicked)		ShowAddToPool();
//	if (imgui_click_create_script)		CreateScript();
//	if (imgui_click_select_script)		SelectScripts();
//	if (showConfirmWindow)				ShowConfirmationWindow();
//
//	//Edit
//	if (toggle_menubar_edit_settings)  Show_Edit_Settings();
//
//
//	//Create
//	if (toggle_menubar_create_addremovegametag) Show_AddRemoveGameTagWindow();
//
//	//Window
//
//
//	//Help
//	//if (toogle_menubar_help)			ShowToggleHelp();
//#pragma endregion
//
//
//}
