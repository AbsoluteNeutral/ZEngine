/*****************************************************************************/
/*!
\file			WinDisplay.h
\brief			WIN32 API display window
*/
/*****************************************************************************/
#ifndef _WINDISPLAY_H_
#define _WINDISPLAY_H_

#include "System.h"

class ZG_API WinDisplay : public System
{
public:
	WinDisplay() noexcept;
	virtual ~WinDisplay();

	bool Create(WndProc function_, HINSTANCE hInstanceH, int width, int height, bool showConsole,
				const char* name = "GAMETITLE");

	void ForcedClose(const char* message = "Forced Close?", const char* title = "Quit");
	void FullScreen(bool fullscreen_);		//set
	HWND GetHWND()const			;			//get
	HDC	 GetHDC()const			;			//get
	int  GetHalfWidth() const	;			//get
	int  GetHalfHeight() const	;			//get
	int  GetHeight() const		;			//get
	int  GetWidth() const		;			//get
	bool IsFullScreen() const	;			//get

	WPARAM ProcessMessages();
	void StopRunning();

	void SwapBuffers();
	virtual void SystemUpdate();
	virtual bool SystemExit();
	void UpdateScreenSize();				//set

	int mWidth;
	int mHeight;

	bool mFullScreen;
	bool mActive;
	bool mShowConsole;
	bool mForceClose;
	bool mRunning;

	DEVMODE    dmScreenSettings_;
private:
	//WINDOWPLACEMENT Placement;

	HWND m_hWND;			//Handle to A Window
	HDC m_windowDC;			//global Window-device context
	HGLRC m_wglDC;			//OpenGL-device context
	HINSTANCE m_hInst;

	RECT mRect;
	const char windowsClassName[16];

	ATOM MyRegisterClass(HINSTANCE hInstance, WndProc function_);
	bool CreateConsole();
};

#endif //_WINDISPLAY_H_
