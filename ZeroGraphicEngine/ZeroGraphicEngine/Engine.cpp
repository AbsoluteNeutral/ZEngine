/*****************************************************************************/
/*!
\file			Engine.cpp
\brief			The Back end of encapsulate system components and managers
*/
/*****************************************************************************/
#include "stdafx.h"
#include "Engine.h"

Engine pEngine{};
WinDisplay			  Engine::sys_WinDisplay{};
Input			      Engine::sys_Input{};
XInputSystem		  Engine::sys_XInputSystem{};
FrameRateController   Engine::sys_FrameRateController{};

OpenGLGraphic		  Engine::sys_OpenGLGraphic{};
//Physics			  Engine::sys_Physics{};
//Logic			      Engine::sys_Logic{};

//CutsceneManager       Engine::sys_CutsceneManager{};

Engine::Engine()
	:GetUpdate(&Engine::UpdateActive), PrevUpdate(GetUpdate)
{}

Engine::~Engine(){}

bool Engine::EngineInitialize(HINSTANCE hInstanceH, int width, int height, bool showConsole, const char* name, WndProc function_)
{
	if (function_ == nullptr) {
		if (!sys_WinDisplay.Create(WindowProc, hInstanceH, width, height, showConsole, name)) {
			EngineExit();
			ErrorMsg("Failed to create Display");
			return false;
		}
	}
	else {
		if (!sys_WinDisplay.Create(function_, hInstanceH, width, height, showConsole, name)) {
			EngineExit();
			ErrorMsg("Failed to create Display");
			return false;
		}
	}

	sys_Input.Init(sys_WinDisplay.GetHWND(), sys_WinDisplay.mWidth, sys_WinDisplay.mHeight);
	sys_XInputSystem.Init();

	if (!sys_OpenGLGraphic.InitGLEW()) {
		EngineExit();
		ErrorMsg("Failed to init GLEW");
		return false;
	}

	//RM::audioListener = Allocate<AudioListener>(1);
	LoadAllAssets();

	if (!sys_OpenGLGraphic.InitSystem(sys_WinDisplay.mWidth, sys_WinDisplay.mHeight)) {
		EngineExit();
		ErrorMsg("Failed to init sys_OpenGLGraphic.InitSystem");
		return false;
	}

#if !defined(NDEBUG) || defined(_EDITOR_ON)
	DebugGizmo.Init();
#endif
	return true;
}

bool Engine::IsRunning() const { 
	return sys_WinDisplay.mRunning;
}

void Engine::StopRunning() {
	sys_WinDisplay.StopRunning();
}

void Engine::EngineUpdate(){
	sys_FrameRateController.Start();
	(this->*GetUpdate)();
}

static bool goingSetPause = false;
void Engine::ExitPause() {
	goingSetPause = false;
}
bool Engine::GoingToPause() {
	return goingSetPause;
}
void Engine::EditorGoingToPause() {
	goingSetPause = true;
}

void Engine::SetActive(bool active_) {
	sys_WinDisplay.mActive = active_;

	//Update system
	pEngine.GetUpdate = active_ ? &Engine::UpdateActive : &Engine::UpdateNonActive;

	//Update lock mouse if there is
	//Update cursor
	static bool get_input_toogle_lock_mouse;
	static bool get_input_toogle_show_mouse_cursor;
	static float get_frameratecontroller_timescale = 1.0f;

	if (!active_) {
		goingSetPause = true;
		get_frameratecontroller_timescale = FrameRateController::timescale;
		get_input_toogle_lock_mouse = sys_Input.IsLockMouse();
		get_input_toogle_show_mouse_cursor = sys_Input.IsMouseShown();
	}
	
	active_ ? sys_Input.LockCursor(get_input_toogle_lock_mouse) : sys_Input.LockCursor(false);
	active_ ? sys_Input.ShowMouseCursor(get_input_toogle_show_mouse_cursor) : sys_Input.ShowMouseCursor(false);
	//active_ ? RM::audioListener->Pause(false) : RM::audioListener->Pause(true);
	FrameRateController::timescale = active_ ? get_frameratecontroller_timescale : 0.0f;
}

void Engine::UpdateActive() {
	sys_WinDisplay.SystemUpdate();		//window display update
	sys_Input.SystemUpdate();			//input update
	sys_XInputSystem.SystemUpdate();	//controllers update
	//sys_Logic.SystemScriptUpdate();		//script update
	//sys_Physics.SystemUpdate();			//physics update
	//sys_CutsceneManager.Update();
	sys_OpenGLGraphic.SystemUpdate();	//draw
	//sys_Logic.SystemTransformUpdate();	//transform matrix update
	MainCam->Update();
#if !defined(NDEBUG) || defined(_EDITOR_ON)
	//sys_Physics.DrawDebug();
#endif
	//sys_OpenGLGraphic.Draw();
}

void Engine::UpdateNonActive() {
	sys_WinDisplay.SystemUpdate();
	//sys_Input.SystemUpdate();
	//sys_XInputSystem.SystemUpdate();
	MainCam->Update();
#if !defined(NDEBUG) || defined(_EDITOR_ON)
	//sys_Physics.DrawDebug();
#endif
	sys_OpenGLGraphic.SystemUpdate();
}

void Engine::EngineLateUpdate(){
	//sys_Logic.SystemLateUpdate();	//reset transform flag bit
	//RM::audioListener->Update();	//Update audio
	//while (sys_FrameRateController.DoSomething()) {

	//}
	sys_FrameRateController.WasteTime();
	sys_WinDisplay.SwapBuffers();	//swap buffers
}

bool Engine::EngineExit(){
	//DeallocateOne(RM::audioListener);
	//sys_Logic.SystemExit();			//exit logic/etc first
	//sys_Physics.SystemExit();		//exit phyics first
	sys_Input.SystemExit();			//exit Input
	sys_XInputSystem.SystemExit();	//ext controllers
	sys_OpenGLGraphic.SystemExit(); //exit graphics
	sys_WinDisplay.SystemExit();	//exit window

	return true;
}

#if !defined(_EDITOR_ON)
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg)                                   // Check For Windows Messages
	{
	case WM_ACTIVATE:                           // Watch For Window Activate Message
	{
		if (EngineIsRunning()) {
			switch (wParam) {
			case WA_INACTIVE:
				SetEngineActive(false);
				break;
			default:
				SetEngineActive(true);
				break;
			}
		}
		return 0;                               // Return To The Message Loop
	}
	case WM_CLOSE:                              // Did We Receive A Close Message?
	{
		PostQuitMessage(0);                     // Send A Quit Message
		return 0;                               // Jump Back
	}
	case WM_SIZE:                               // Resize The OpenGL Window
	{											// LoWord=Width, HiWord=Height
		Engine::sys_WinDisplay.mWidth  = LOWORD(lParam);
		Engine::sys_WinDisplay.mHeight = HIWORD(lParam);
		Engine::sys_OpenGLGraphic.ReSizeGLScene(Engine::sys_WinDisplay.mWidth, Engine::sys_WinDisplay.mHeight);
		Engine::sys_Input.UpdateScreenSize(Engine::sys_WinDisplay.mWidth, Engine::sys_WinDisplay.mHeight);
		return 0;
	}
	case WM_MOUSEWHEEL:
		Engine::sys_Input.mouseWheelDelta += GET_WHEEL_DELTA_WPARAM(wParam) / 100.0f > 0 ? +0.1f : -0.1f;
		return true;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif
//std::string GetSaveFilePath() {
//	return SAVE_FILE_PATH;
//}

