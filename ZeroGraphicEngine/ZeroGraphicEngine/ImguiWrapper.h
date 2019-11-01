
#ifndef _IMGUIWRAPPER_H
#define _IMGUIWRAPPER_H

#include <iostream>
#include <windows.h>
#include <commdlg.h>

#include "imgui.h"

//#include "Selection.h"
#include "GridManager.h"
//#include "ResourceEditor.h"
//#include "ImguiObjectPool.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

class ImguiWrapper
{
public:
	ImguiWrapper() noexcept;
	~ImguiWrapper();
	friend LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	void CreateOneCameraOnScene();

	bool Init();
	void SystemUpdate(const HANDLE* dwChangeHandles, DWORD& dwWaitStatus);
	bool SystemExit();

	float h;
	float w;
	float highDpiScale;
	
private:
	HWND mainHWND;

	void UpdateImGui();
	
	void LoadFontTexture();
	//void LoadAllArchetypes();
	friend void ImImpl_RenderDrawLists(ImDrawData* draw_data);
};

extern ImguiWrapper		IMGU;
extern GridManager2D	GRIDMANAGER_2D;
//extern GridManager3D	GRIDMANAGER_3D;
extern unsigned			GAMECamera_NUM;
extern unsigned			GAMEAudioListener_CameraNUM;

void ToolTip(const char* text);

#endif //_IMGUIWRAPPER_H
