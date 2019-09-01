/*****************************************************************************/
/*!
\file	Logging.cpp
\brief	Logging using printf and Error checking, delete safe and free safe
*/
/*****************************************************************************/
#include "stdafx.h"

#include <assert.h>
#include <Windows.h>
#include "Logging.h"
#pragma comment(lib, "user32.lib")


void SignalBox(const char* msg, const char* functionName_, bool iserror){
	char finalMsg[512];
	sprintf_s(finalMsg, "%s\n IN Function: \n\t %s, Line: %d \n%s", __FILE__, functionName_, __LINE__, msg);

	if (iserror) {
#if defined(_USE__MESSAGE_BOX)
		MessageBoxA(NULL, finalMsg, "Error", 0);
#endif
	}
	else {
#if defined(_USE__MESSAGE_BOX)
		MessageBoxA(NULL, finalMsg, "Message", 0);
#endif
	}

#ifdef _GENERATE_ERROR_LOG
	//FileMgr file;
	//std::string tmp{ __TIMESTAMP__ };
	//size_t pos = tmp.find_first_of(":");
	//do {
	//	tmp[pos] = ' ';
	//	pos = tmp.find_first_of(":");
	//} while (pos != std::string::npos);
	//if (file.Open(PATH_ETC + "ErrorLog " + tmp + ".txt", FILE_WRITE)) {
	//	file.Write(__TIMESTAMP__, 0);
	//	file.Write(finalMsg, 0);
	//	file.Close();
	//}
#endif

	if (iserror) {
#if defined(DEBUG_MODE_ON) || defined(_DEBUG)
	#if defined(_USE__ASSERT)
			assert(0);
	#endif
#endif
	}
}

void PrintColor(bool Expression_, const char* functionname, const char* format, ...)
{
	// Change to the error color
	HANDLE console = GetStdHandle(STD_ERROR_HANDLE);
	
	if (Expression_) {
		printf(functionname); printf(": ");
		SetConsoleTextAttribute(console, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		printf("PASSED ");
	}
	else {
		SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);
		printf(functionname); printf(": ");
		printf("FAILED ");
	}

	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	va_list vl;
	va_start(vl, format);
	
	vfprintf(stderr, format, vl);
	
	// Change back to the normal color
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	
	// Only popup the console upon the first error
	static bool firstError = true;
	if (firstError == true)
	{
		// Bring the console window to the front
		//HWND hwnd = GetConsoleWindow();
		//SetForegroundWindow(hwnd);
	
		// We won't popup the console anymore (popups with every error gets annoying!)
		firstError = false;
	}
	
	va_end(vl);
}
void PrintError(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	// Change to the error color
	HANDLE console = GetStdHandle(STD_ERROR_HANDLE);
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_INTENSITY);

	vfprintf(stderr, format, vl);

	// Change back to the normal color
	SetConsoleTextAttribute(console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

	// Only popup the console upon the first error
	static bool firstError = true;
	if (firstError == true)
	{
		// Bring the console window to the front
		//HWND hwnd = GetConsoleWindow();
		//SetForegroundWindow(hwnd);

		// We won't popup the console anymore (popups with every error gets annoying!)
		firstError = false;
	}

	va_end(vl);
}

void JustPrint(const char* format, ...)
{
	va_list vl;
	va_start(vl, format);

	vfprintf(stderr, format, vl);

	// Only popup the console upon the first error
	static bool firstError = true;
	if (firstError == true)
	{
		// Bring the console window to the front
		//HWND hwnd = GetConsoleWindow();
		//SetForegroundWindow(hwnd);

		// We won't popup the console anymore (popups with every error gets annoying!)
		firstError = false;
	}

	va_end(vl);
}
