/*****************************************************************************/
/*!
\file			ImguiProc.cpp
\brief			Imgui Window Proc
*/
/*****************************************************************************/
#include "stdafx.h"
//#include "../Engine/Engine.h"

#include "Engine.h"
#include "ImguiWrapper.h"
#include <ShellAPI.h>
#include <Windows.h>
#include <stdio.h>

#define SIZE 1024
#define MAX_MB 20971520 

std::string dropErrorTitle = "Drop Error";
std::string dropErrorMsg = "Only .fbx/.png/.dds/.ogg/.wav files less than 20MB are accepted.";

//void CheckForUpdatedFiles(std::time_t);
#if defined(_EDITOR_ON)

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	HDROP hDropInfo = NULL;
	UINT buffsize = SIZE;
	char buf[SIZE];
	ImGuiIO& io = ImGui::GetIO();

	static std::time_t prevTime;

	switch (msg)                                   // Check For Windows Messages
	{
	case WM_ACTIVATE:                           // Watch For Window Activate Message
	{
		if (EngineIsRunning()) {
			switch (wParam) {
			case WA_INACTIVE:
				prevTime = std::time(nullptr);
				SetEngineActive(false);
				break;
			default:
				SetEngineActive(true);
				//CheckForUpdatedFiles(prevTime);
				break;
			}
		}
		//if (EngineIsRunning()) {
		//	if (HIWORD(wParam) || wParam == WA_INACTIVE) {   // Check Minimization State
		//													 //Engine::sys_WinDisplay.mActive = FALSE;    // Program Is not Active
		//													 //pEngine.GetUpdate = &Engine::UpdateNonActive;
		//		SetEngineActive(false);
		//	}
		//	else {
		//		//Engine::sys_WinDisplay.mActive = TRUE;    // Program Is Active
		//		//pEngine.GetUpdate = &Engine::UpdateActive;
		//		SetEngineActive(true);
		//	}
		//}

		return 0;                               // Return To The Message Loop
	}
	case WM_SYSCOMMAND:                         // Intercept System Commands
	{
		switch (wParam)                         // Check System Calls
		{
		case SC_SCREENSAVE:                 // Screensaver Trying To Start?
		case SC_MONITORPOWER:               // Monitor Trying To Enter Powersave?
			return 0;                           // Prevent From Happening
		}
		break;                                  // Exit
	}
	case WM_CLOSE:                              // Did We Receive A Close Message?
	{
		PostQuitMessage(0);                     // Send A Quit Message
		return 0;                               // Jump Back
	}
	case WM_SIZE:                               // Resize The OpenGL Window
	{
		//if (!Engine::sys_WinDisplay.mFullScreen && EngineIsRunning())
		//Engine::sys_WinDisplay.mWidth  = LOWORD(lParam);
		//Engine::sys_WinDisplay.mHeight = HIWORD(lParam);
		Engine::sys_WinDisplay.UpdateScreenSize();

		IMGU.w = LOWORD(lParam);
		IMGU.h = HIWORD(lParam);

		Engine::sys_OpenGLGraphic.ReSizeGLScene(Engine::sys_WinDisplay.mWidth, Engine::sys_WinDisplay.mHeight);
		Engine::sys_Input.UpdateScreenSize(Engine::sys_WinDisplay.mWidth, Engine::sys_WinDisplay.mHeight);
		io.DisplaySize = ImVec2(LOWORD(lParam), HIWORD(lParam));   // Display size, in pixels. For clamping windows positions.

		return 0;                               // Jump Back
	}
	case WM_LBUTTONDOWN:
		io.MouseDown[0] = true;
		return true;
	case WM_LBUTTONUP:
		io.MouseDown[0] = false;
		return true;
	case WM_RBUTTONDOWN:
		io.MouseDown[1] = true;
		return true;
	case WM_RBUTTONUP:
		io.MouseDown[1] = false;
		return true;
	case WM_MOUSEWHEEL:

		//if (!toggle_shortcut_play) 
		{
			io.MouseWheel += GET_WHEEL_DELTA_WPARAM(wParam) * 0.01f;// > 0 ? +0.1f : -0.1f;
			if (!ImGui::IsMouseHoveringAnyWindow()) {
				switch (MainCam->GetType()) {
				case CAM_PERSPECTIVE:
				{
					MainCam->owner->transform.Translate(2.0f * io.MouseWheel * MainCam->GetForward());
					break;
				}
				default:
				{
					int size_camee = MainCam->GetSize();
					size_camee -= static_cast<int>(io.MouseWheel);
					size_camee = size_camee <= 0 ? 1 : size_camee;
					MainCam->SetSize(size_camee);
					break;
				}
				}
			}
		}

		return true;
	case WM_MOUSEMOVE:
		// Mouse position, in pixels (set to -1,-1 if no mouse / on another screen, etc.)
		io.MousePos.x = (signed short)(lParam);
		io.MousePos.y = (signed short)(lParam >> 16);
		io.MousePos.x /= IMGU.highDpiScale;
		io.MousePos.y /= IMGU.highDpiScale;
		return true;
	case WM_CHAR:
		// You can also use ToAscii()+GetKeyboardState() to retrieve characters.
		if (wParam > 0 && wParam < 0x10000)
			io.AddInputCharacter((unsigned short)wParam);
		return true;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_CREATE:
		DragAcceptFiles(hwnd, TRUE);
		return 0;
	case WM_DROPFILES:
		hDropInfo = (HDROP)wParam;

		DragQueryFile(hDropInfo, 0, buf, buffsize);

		//FILE *pFile;
		char* pch_fbx = strstr(buf, ".fbx");

		char* pch_png = strstr(buf, ".png");
		char* pch_dds = strstr(buf, ".dds");

		char * pch_ogg = strstr(buf, ".ogg");
		char * pch_wav = strstr(buf, ".wav");

		if (pch_fbx != NULL)
		{
			if (GetFileSize(buf) < MAX_MB)
			{
				CopyFileToDirectory(buf, PATH_MODELS);
			}
			else
				MessageBox(hwnd, dropErrorMsg.c_str(), dropErrorTitle.c_str(), MB_OK);
		}
		else if (pch_png != NULL || pch_dds != NULL)
		{
			if (GetFileSize(buf) < MAX_MB) 
			{
				CopyFileToDirectory(buf,  PATH_TEXTURES);
				//LoadTexture(buf);
			}				
			else
				MessageBox(hwnd, dropErrorMsg.c_str(), dropErrorTitle.c_str(), MB_OK);
		}
		else if(pch_ogg != NULL || pch_wav != NULL)
		{
			if (GetFileSize(buf) < MAX_MB) 
			{
				CopyFileToDirectory(buf, PATH_SOUNDS);
				//LoadSoundTracks(buf);
			}
			else
				MessageBox(hwnd, dropErrorMsg.c_str(), dropErrorTitle.c_str(), MB_OK);
		}
		else
			MessageBox(hwnd, dropErrorMsg.c_str(), dropErrorTitle.c_str(), MB_OK);
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
#endif

//void CheckForUpdatedFiles(std::time_t prev)
//{
//	std::string path = getPath(_PATH_SCRIPTS);
//
//	for (const auto& file : fs::directory_iterator(path))
//	{
//		auto fTime = fs::last_write_time(file.path());
//
//		if (std::difftime(decltype(fTime)::clock::to_time_t(fTime), prev) > 0)
//		{
//			std::cout << "Updating "<< file.path().filename() << "..." << std::endl;
//			for (auto& elem : RM::R_Script) 
//			{
//				try
//				{
//					if (elem->filename == file.path().filename())
//					{
//						elem->blacklist.clear();
//						elem->Init(false);
//					}
//				}
//				catch (const std::string& e) {
//					std::cout << "Compile error!\n";
//					LuaConsoleManager.Push(e);
//					toggle_menubar_window_Assets = true;
//					currAssetTab = TAB_LUACONSOLE;
//				}
//			}
//
//			std::cout << "Scripts updated\n";
//		}
//	}
//}
