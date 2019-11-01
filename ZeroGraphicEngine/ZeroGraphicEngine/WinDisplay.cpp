
#include "stdafx.h"
#include "WinDisplay.h"
#include "Engine.h"

WinDisplay::WinDisplay() noexcept
	:System{}, mWidth(0), mHeight(0),
	mFullScreen(false), mActive(true), mShowConsole(true), mForceClose(false), mRunning{ false },
	m_hWND{ NULL }, m_windowDC{ 0 }, m_wglDC{ 0 }, 
	m_hInst{0}, mRect {}, windowsClassName{ "ENGINEWINDOWFRA" }
{}

WinDisplay::~WinDisplay()
{}

//Register class
ATOM WinDisplay::MyRegisterClass(HINSTANCE hInstance, WndProc function_){

	WNDCLASSEX wc{
	sizeof(WNDCLASSEX),						//wc.cbSize
	CS_HREDRAW | CS_VREDRAW,				//wc.style			
	function_,								//wc.lpfnWndProc	
	NULL,									//wc.cbClsExtra		
	NULL,									//wc.cbWndExtra		
	hInstance,								//wc.hInstance		
	LoadIcon(NULL, IDI_APPLICATION),		//wc.hIcon			
	LoadCursor(NULL, IDC_ARROW),			//wc.hCursor		
	NULL,									//wc.hbrBackground	
	NULL,									//wc.lpszMenuName	
	windowsClassName,						//wc.lpszClassName
	NULL };									//wc.hIconSm

	return RegisterClassEx(&wc);	//register window
}

//Create console
bool WinDisplay::CreateConsole(){
	if (AllocConsole()) {
		FILE* file;
		freopen_s(&file, "CONOUT$", "wt", stdout);
		freopen_s(&file, "CONOUT$", "wt", stderr);
		freopen_s(&file, "CONOUT$", "wt", stdin);
		SetConsoleTitle("CONSOLE ONLY VISIBLE FOR DEBUGGING YO!");

		//disable Close Button for console window
		HWND hwnd = ::GetConsoleWindow();
		if (hwnd != NULL) 
		{
			//Set Console Position at top left hand corner
			SetWindowPos(hwnd, 0, 58, 38, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

			HMENU hMenu = ::GetSystemMenu(hwnd, FALSE);
			if (hMenu != NULL){
				DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
				return true;
			}
		}
	}

	return false;
}

bool WinDisplay::Create(	WndProc function_, 
							HINSTANCE hInstanceH, int width, int height,bool showConsole, const char* name){

	ErrorIf(width <= 0 || height <= 0, "ScreenSize is terriable");

	if (!MyRegisterClass(hInstanceH, function_)) {
		ErrorMsg("Failed MyRegisterClass()");
		return false;
	}

	if (showConsole)	//Code to open console for in/out debug info
	{
		if (!CreateConsole()) {
			ErrorMsg("Failed CreateConsole()");
			return false;
		}
	}

	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	DWORD dwStyle = WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_OVERLAPPEDWINDOW;
	dwStyle &= ~WS_SIZEBOX;
	dwStyle &= ~WS_MAXIMIZEBOX;

	mRect = { 0, 0, (LONG)(width - 1), (LONG)(height - 1) };
//The AdjustWindowRect computes the exact client area without the title bar and all the extra pixels
	AdjustWindowRectEx(&mRect, dwStyle, FALSE, WS_EX_APPWINDOW);	

	m_hWND = CreateWindow(windowsClassName, 			 //lpClassName
							name, 						 //lpWindowName
						    WS_OVERLAPPEDWINDOW,		 //dwStyle
						    ((cx - width) / 2 ),		 //x
						    ((cy- height) / 2),		 //y
						    (LONG)width, (LONG)height,	 //width, height	
						    nullptr, 					 //hWndParent
						    nullptr, 					 //hMenu
							hInstanceH, 				 //hInstance
							nullptr);					 //lpParam
	if (!m_hWND) {
		ErrorMsg("Failed CreateWindow, m_hWND = null ");
		return false;
	}

	ShowWindow(m_hWND, SW_SHOWNORMAL);

	m_hInst			= hInstanceH;

	RECT testrect;
	GetClientRect(m_hWND, &testrect);
	mWidth			= testrect.right - testrect.left;
	mHeight			= testrect.bottom - testrect.top;

	mShowConsole	= showConsole;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////Init double screen
	m_windowDC = GetDC(m_hWND);	///get DC

	DEVMODE devMode = { 0 };
	devMode.dmSize = sizeof(DEVMODE);
	BOOL boo = EnumDisplaySettings(0, ENUM_CURRENT_SETTINGS, &devMode);
	if (boo == 0)
		return false;

	//drawing surface format
	PIXELFORMATDESCRIPTOR pfdesc;
	memset(&pfdesc, 0, sizeof(PIXELFORMATDESCRIPTOR));

	pfdesc.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfdesc.nVersion = 1;
	pfdesc.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED | PFD_DOUBLEBUFFER;
	pfdesc.iPixelType = PFD_TYPE_RGBA;
	pfdesc.cColorBits = (BYTE)devMode.dmBitsPerPel;//32; //24 bit color for front and back buffer
	pfdesc.cDepthBits = 24;							   //24 bit depth buffer - not used in this demo
	pfdesc.cStencilBits = 8;							   //8 bit stencil buffer - not used in this demo

	int pf = ChoosePixelFormat(m_windowDC, &pfdesc);	//checks if the graphics card can support the pixel format requested
	if (pf == 0) {
		ReleaseDC(m_hWND, m_windowDC);
		ErrorMsg("ChoosePixelFormat not supported");
		return false;
	}

	BOOL ok = SetPixelFormat(m_windowDC, pf, &pfdesc);
	if (!ok) {
		ReleaseDC(m_hWND, m_windowDC);
		ErrorMsg("Failed SetPixelFormat not supported");
		return false;
	}

	///Set OpenGL context
	m_wglDC = wglCreateContext(m_windowDC);
	if (!m_wglDC) {
		ReleaseDC(m_hWND, m_windowDC);
		ErrorMsg("Failed wglCreateContext");
		return false;
	}

	///Set OpenGL wglMakeCurrent
	ok = wglMakeCurrent(m_windowDC, m_wglDC);
	if (!ok) {
		wglDeleteContext(m_wglDC);
		ReleaseDC(m_hWND, m_windowDC);
		ErrorMsg("Failed wglMakeCurrent");
		return false;
	}

	mRunning		= true;
	Logg("WinDisplay.cpp WINDOW created");
	return true;
}

void WinDisplay::ForcedClose(const char* message, const char* title) {
	UNREFERENCED_PARAMETER(message);
	UNREFERENCED_PARAMETER(title);
	const int result = MessageBox(NULL, message, title, MB_YESNO);
	switch (result) {
	case IDYES:
		mForceClose = true;
		mRunning    = false;
		break;
	case IDNO:
		mForceClose = false;
		mRunning	= true;
		break;
	}
}

void WinDisplay::FullScreen(bool fullscreen_){
	if (mFullScreen == fullscreen_)
		return;

	mFullScreen = fullscreen_;
	
	int cx = GetSystemMetrics(SM_CXSCREEN);
	int cy = GetSystemMetrics(SM_CYSCREEN);

	//fullscreen
	if (fullscreen_){
		//GetWindowPlacement(m_hWND, &Placement);

		SetWindowLong(m_hWND, GWL_STYLE, WS_VISIBLE);
		///HWND_TOP        at the front of every window once
		///HWND_BOTTOM     at the behind of every window once
		///HWND_TOPMOST    Always on top
		///HWND_NOTOPMOST  Always on bottom
		SetWindowPos(m_hWND, NULL, 0, 0, ::GetDeviceCaps(m_windowDC, HORZRES), ::GetDeviceCaps(m_windowDC, VERTRES), SWP_FRAMECHANGED);
	}
	//not fullscreen
	else {
		SetWindowLong(m_hWND, GWL_STYLE, WS_OVERLAPPEDWINDOW | WS_VISIBLE);
		SetWindowPos(m_hWND, HWND_TOP,
					((cx - zg::GOBAL_RESOLUTION_WIDTH) >> 1), ((cy - zg::GOBAL_RESOLUTION_HEIGHT) >> 1),
					zg::GOBAL_RESOLUTION_WIDTH, zg::GOBAL_RESOLUTION_HEIGHT, SWP_SHOWWINDOW);
		//SetWindowPlacement(m_hWND, &Placement);
		//RECT realRect;
		//GetClientRect(m_hWND, &realRect);
		//mWidth      = realRect.right - realRect.left;
		//mHeight     = realRect.bottom - realRect.top;
	}
	UpdateWindow(m_hWND);
	UpdateScreenSize();
}

HWND WinDisplay::GetHWND()			const { return m_hWND; }
HDC  WinDisplay::GetHDC()			const { return m_windowDC; }
int  WinDisplay::GetHalfWidth()		const { return mWidth >> 1; }
int  WinDisplay::GetHalfHeight()	const { return mHeight >> 1; }
int  WinDisplay::GetHeight()		const { return mHeight; }
int  WinDisplay::GetWidth()			const { return mWidth; }
bool WinDisplay::IsFullScreen()		const { return mFullScreen; }

WPARAM WinDisplay::ProcessMessages(){
	MSG msg;                                // Windows Message Structure

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) // Is There A Message Waiting?
	{
		if (msg.message == WM_QUIT)         // Have We Received A Quit Message?
			mForceClose = true;
			//ForcedClose();
		else {								// If Not, Deal With Window Messages	
			TranslateMessage(&msg);         // Translate The Message
			DispatchMessage(&msg);          // Dispatch The Message
		}
	}
	return (msg.wParam);    // Exit The Program
}

void WinDisplay::StopRunning() { mRunning = false; }

///Double Buffering
void WinDisplay::SwapBuffers() {
	::SwapBuffers(m_windowDC);
}

void WinDisplay::SystemUpdate(){
    ProcessMessages();
}

bool WinDisplay::SystemExit(){

	mRunning = false;

	if (m_wglDC)
		if (!wglMakeCurrent(NULL, NULL))
			ErrorMsg("wglMakeCurrent() Failed");
	if (!wglDeleteContext(m_wglDC))
		ErrorMsg("wglDeleteContext() Failed");
	m_wglDC = NULL;
	if (m_windowDC && !ReleaseDC(m_hWND, m_windowDC))
		m_windowDC = NULL;


	if (m_hWND && !DestroyWindow(m_hWND)){
		m_hWND = NULL;
		ErrorMsg("Failed DestroyWindow");
	}
	if (!UnregisterClass(windowsClassName, m_hInst)) {
		m_hInst = NULL;
		ErrorMsg("Failed UnregisterClass");
	}


	FreeConsole();
	
	return true;
}

void WinDisplay::UpdateScreenSize() {
	RECT realRect;
	GetClientRect(m_hWND, &realRect);
	mWidth		= realRect.right - realRect.left;
	mHeight		= realRect.bottom - realRect.top;
}
