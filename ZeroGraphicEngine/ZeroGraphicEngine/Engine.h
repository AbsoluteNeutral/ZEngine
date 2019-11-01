
#ifndef _ENGINE_H
#define _ENGINE_H

#include "WinDisplay.h"
#include "OpenGLGraphic.h"
#include "Input.h"
#include "XInputSystem.h"
#include "FrameRateController.h"
//#include "Physics.h"
//#include "Logic.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ZG_API Engine
{
public:
	Engine();
	~Engine();

	bool EngineInitialize(HINSTANCE hInstanceH, int width, int height, bool showConsole, 
						 const char* name = "GAMETITLE", WndProc function_ = nullptr);
	void EngineUpdate();
	void EngineLateUpdate();
	bool EngineExit();
	bool IsRunning() const;
	void StopRunning();
	void SetActive(bool active_);
	static void ExitPause();
	static bool GoingToPause();
	static void EditorGoingToPause();

	static WinDisplay			sys_WinDisplay;
	static Input				sys_Input;
	static XInputSystem			sys_XInputSystem;
	static FrameRateController  sys_FrameRateController;
	static OpenGLGraphic		sys_OpenGLGraphic;
    //static Physics              sys_Physics;
    //static Logic                sys_Logic;
	//static CutsceneManager sys_CutsceneManager;

private:
	void(Engine::*GetUpdate)();
	void(Engine::*PrevUpdate)();
	void UpdateActive();
	void UpdateNonActive();

	Engine(const Engine&);				//cannot copy
	Engine& operator=(const Engine&);   //cannot assign
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
};

extern Engine pEngine;

#endif // !_ENGINE_H
