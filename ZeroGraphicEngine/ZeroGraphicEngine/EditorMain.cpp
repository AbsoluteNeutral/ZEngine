#include "stdafx.h"

#include "EditorMain.h"
#include "EditorMB.h"
#include "Engine.h"
#include "Simulation.h"


_EDITOR_SIMULATION_NUM simulationNum = _SIMULATION0;

enum _FFFPS_ENUM {
	_30fps = 0,
	_60fps,
	_fpsfree
};
_FFFPS_ENUM set_fpsenum = _60fps;

#if  defined(_DEBUG)
bool show_test_window = true;
//bool show_test_window = false2;
#else
bool show_test_window = false;
#endif


void EditorInit() 
{
	InitSimulation[simulationNum]();

	EditorMBInit();
}

void EditorUpdate()
{
	//GRIDMANAGER_3D.Draw();
	//Engine::sys_FrameRateController.Start();
	//Engine::sys_WinDisplay.SystemUpdate();

	//if (!IsEngineActive())
	//{
	//	Engine::EditorGoingToPause();
	//}

	//if (!toggle_shortcut_play && IsEngineActive())
	//	Editor_Input();
	//else if (toggle_shortcut_play && IsEngineActive())
	//	Editor_DuringPlay_Input();

	GRIDMANAGER_2D.Draw();

	EditorInputUpdate();

	UpdateSimulation[simulationNum]();
	EngineUpdate();

	//RM::DrawAllPickingCircle();

	glUseProgram(0);

	ImGui::NewFrame();
	EditorMainMenuBarUpdate();
	//______________________________________________________________________ Imgui
	
	{
		ImGui::Begin("Controller", NULL);

		ImGui::Checkbox("Toggle Bone", &Engine::sys_OpenGLGraphic.toggle_bone);
		ImGui::Checkbox("Toggle WireFrame", &Engine::sys_OpenGLGraphic.toggle_wireframe);
		ImGui::ColorEdit4("WireFrameColor", &Engine::sys_OpenGLGraphic.wireframe_color.r);
		ImGui::Checkbox("Toggle DrawMesh", &Engine::sys_OpenGLGraphic.toggle_drawmesh);

		// framerate
		ImGui::Text("Camera Position: %f %f %f", MainCam->owner->transform.position.x, MainCam->owner->transform.position.y, MainCam->owner->transform.position.z);
		zg::Vector3 euler = MainCam->owner->transform.rotation.ToEularDegree();
		ImGui::Text("Camera Rotation: %f %f %f", euler.x, euler.y, euler.z);
		ImGui::Text("FrameRate: %3.f %f", GetFrameRate(), GetFrameRate());
		ImGui::Text("dt: %f", FrameRateController::dt);
		if (ImGui::RadioButton("30Fps", set_fpsenum == _30fps)) {
			set_fpsenum = _30fps;
			EngineSetFrameRate(true, 30);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("60Fps", set_fpsenum == _60fps)) {
			set_fpsenum = _60fps;
			EngineSetFrameRate(true, 60);
		}
		ImGui::SameLine();
		if (ImGui::RadioButton("Free", set_fpsenum == _fpsfree)) {
			set_fpsenum = _fpsfree;
			EngineSetFrameRate(false);
		}

		UpdateImguiSimulation[simulationNum]();

		ImGui::End();
	}


	if (show_test_window)
	{
		ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_Once);
		ImGui::ShowDemoWindow(&show_test_window);
	}

	ImGui::Render();

}//End DrawEditor

//void ShowConfirmationWindow()
//{
//	ImGui::OpenPopup(confirmWindowName.c_str());
//
//	if (ImGui::BeginPopupModal(confirmWindowName.c_str(), NULL, ImGuiWindowFlags_NoResize))
//	{
//		ImGui::Text(confirmMsg.c_str());
//		ImGui::Separator();
//
//		if (ImGui::Button("Ok"))
//		{
//			showConfirmWindow = false;
//			ImGui::CloseCurrentPopup();
//		}
//
//		ImGui::EndPopup();
//	}
//}

//void SelectSingle(PickingCircle* obj_) {
//
//	if (!imgui_click_select_script && !imgui_click_create_script)
//	{
//		Multiselection.Clear();
//		Multiselection.InsertOrUnselect(obj_);
//	}	
//}
//
//void SelectSingle(GameObject* obj_) {
//
//	if (!imgui_click_select_script && !imgui_click_create_script)
//	{	
//		Multiselection.Clear();
//		Multiselection.InsertOrUnselect(obj_);
//	}
//}
//
//void UnSelectObjectAndSaveInfo()
//{
//	if (!imgui_click_select_script && !imgui_click_create_script)
//	{
//		Multiselection.Clear();
//	}
//}
//
//Selection& GetSelectedObj()
//{
//	return Multiselection.GetSingleSelection();
//}
//
//std::vector<Selection>& GetAllSelected()
//{
//	return Multiselection.GetAllSelected();
//}
//
//PickingCircle* GetPickFromID(int ID)
//{
//	for (auto& pick : RE::R_PICKING)
//	{
//		if (pick->owner->GetID() == ID)
//			return pick;
//	}
//
//	ShowErrorWindow("PickingCircle to find does not exist.");
//
//	return nullptr;
//}
//
//void LoadSceneFile() {
//	DIR_itr&& directory = GetDirectory(PATH_SCENE);
//	for (auto & p : directory) {
//		std::string path{ p.path().string() };
//		std::string file;
//		GetFileNameWithExtension(path, file);
//		sceneFile_map[file] = p.path().string();
//	}
//}
//
////only call at main function
//void UpdateAllObjectsOnScreen() {
//	//ClickPLay Button
//  graphicsTime = 0.f;
//  physicsTime = 0.f;
//  scriptsTime = 0.f;
//  audioTime = 0.f;
//
//  if (toggle_shortcut_play) 
//  {  
//	  if (IsEngineActive())
//	  {
//		  BeginTimer();
//		  Engine::sys_Logic.SystemScriptUpdate();
//		  scriptsTime += EndTimer();
//		  BeginTimer();
//		  Engine::sys_Physics.SystemUpdate();
//		  physicsTime += EndTimer();
//	  }
//
//    BeginTimer();
//    Engine::sys_OpenGLGraphic.SystemUpdate();
//
//	  if (IsEngineActive())
//	  {
//		  Engine::sys_CutsceneManager.Update();
//		  MainCam->Update();
//		  graphicsTime += EndTimer();
//		  BeginTimer();
//		  Engine::sys_Logic.SystemTransformUpdate();
//		  physicsTime += EndTimer();
//	  }
//
//    BeginTimer();
//
//    Engine::sys_OpenGLGraphic.ShadowPass();
//		
//		Engine::sys_OpenGLGraphic.StartBuffer(MainCam->id);
//		glCullFace(GL_FRONT);
//		Engine::sys_OpenGLGraphic.DrawSkybox(MainCam);
//
//		if (toggle_shortcut_draw_physics)	Engine::sys_Physics.DrawDebug();
//
//		DebugGizmo.Draw(MainCam);
//
//		Engine::sys_OpenGLGraphic.RenderPass(MainCam);
//
//		Engine::sys_OpenGLGraphic.EndBuffer(MainCam->id);
//		Engine::sys_OpenGLGraphic.RenderBuffer(MainCam);
//
//    Engine::sys_OpenGLGraphic.RenderPost(MainCam);
//	Engine::sys_OpenGLGraphic.RenderBloom(MainCam);
//    Engine::sys_OpenGLGraphic.RenderBuffer(MainCam, true);
//
//    Engine::sys_OpenGLGraphic.RenderUI(MainCam);
//
//		Engine::sys_OpenGLGraphic.DrawSkybox(MainCam, false);
//
//    graphicsTime += EndTimer();
//    BeginTimer();
//		RM::audioListener->Update();	//Update audio
//    audioTime += EndTimer();
//		Engine::sys_FrameRateController.WasteTime();
//	}
//	///////////////////////////////////////////////////////////////////////////////////////////////////FALSE 
//	else {
//    Engine::sys_OpenGLGraphic.SystemUpdate();
//    Engine::sys_CutsceneManager.Update();
//    MainCam->Update();
//		for (auto& elem : RM::R_GameObject) 
//    {
//			elem->transform.UpdateNoReset();
//		}
//		Engine::sys_Physics.EditorUpdate();
//
//		Engine::sys_OpenGLGraphic.ShadowPass();
//
//		Engine::sys_OpenGLGraphic.StartBuffer(MainCam->id);
//    if (toggle_shortcut_draw_skybox)
//    {
//      glCullFace(GL_FRONT);
//      Engine::sys_OpenGLGraphic.DrawSkybox(MainCam, true);
//    }
//
//	  if (toggle_shortcut_draw_physics && CheckSelectionType() != SEL_NONE) {
//	  	//Selection& elem = GetSelectedObj();
//	  	for (auto& elem : GetAllSelected()) {
//	  		if (elem.aabb)						elem.aabb->DebugDraw();
//	  		else if (elem.bc)					elem.bc->DebugDraw();
//	  		else if (elem.capsuleCollider)		elem.capsuleCollider->DebugDraw();
//	  		else if (elem.obb)					elem.obb->DebugDraw();
//	  	}
//	  }
//	  if (toggle_shortcut_draw_physics_all)	Engine::sys_Physics.DrawDebug();
//	  if (toggle_shortcut_draw_grid)			GRIDMANAGER_2D.Draw();
//
//	  DebugGizmo.Draw(MainCam);
//
//	  if (toggle_shortcut_draw_pickingcircle) {
//	  	for (auto& elem : RM::R_Camera)		elem->DebugDraw();
//	  	for (auto& sel : GetAllSelected())	sel.pickingCircle->Draw(CC_ORANGERED);
//	  	for (auto& elem : RE::R_PICKING)	elem->Draw();
//	  }
//	  DebugGizmo.Draw(MainCam);
//
//	  Engine::sys_OpenGLGraphic.RenderPass(MainCam);
//
//	  DebugGizmo.Draw(MainCam);
//
//		Engine::sys_OpenGLGraphic.EndBuffer(MainCam->id);
//
//		Engine::sys_OpenGLGraphic.RenderBuffer(MainCam);
//
//    Engine::sys_OpenGLGraphic.RenderPost(MainCam);
//	Engine::sys_OpenGLGraphic.RenderBloom(MainCam);
//    Engine::sys_OpenGLGraphic.RenderBuffer(MainCam, true);
//
//    Engine::sys_OpenGLGraphic.RenderUI(MainCam);
//
//		Engine::sys_OpenGLGraphic.DrawSkybox(MainCam, false);
//
//		RM::audioListener->Update();	//Update audio
//		Engine::sys_FrameRateController.Stop();
//		for (auto& elem : RM::R_GameObject) {
//			elem->transform.Reset();
//		}
//
//		Engine::sys_FrameRateController.WasteTime();
//	}
//}
//
//ImVec4 ConvertColorToImVec4(const Color& c)
//{
//	return ImVec4{ c.r, c.g, c.b, c.a };
//}
//
//bool CompareGameObject(GameObject* lhs, GameObject* rhs)
//{
//	return lhs->name < rhs->name;
//}
//
//void ShowNonErrorWindow(const char* msg, const char* windowTitle)
//{
//	std::string tmp = "NON-ERROR: ";
//	tmp += msg;
//
//	HistoryMgr.PushLogFileOnly(tmp);
//	::MessageBoxA(nullptr, msg, windowTitle, MB_OK);	
//}
//
//void ShowErrorWindow(const char* msg)
//{
//	// Save backup file
//	RE::SaveScene(PATH_SCENE + "Crashed_Backup.xml");
//
//	std::string tmp = "ERROR: ";
//	tmp += msg;
//
//	HistoryMgr.PushLogFileOnly(tmp);
//
//	tmp += "\n\nA backup file was saved as 'Crashed_Backup.xml'.";
//
//	ErrorMsg(tmp.c_str());
//}

