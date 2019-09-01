/*****************************************************************************/
/*!
\file			System.h
\brief			Engine's system base class
*/
/*****************************************************************************/
#ifndef _SYSTEM_H
#define _SYSTEM_H

#include <Windows.h>
#include "Logging.h"

#define _EDITOR_ON 1
typedef LRESULT(CALLBACK *WndProc)(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

class ZG_API System
{
	System(const System&);
	void operator=(const System&);
public:
	System() noexcept {}
	virtual ~System() {};
	virtual void SystemUpdate() {};
	virtual bool SystemExit() { return true; };
};

#endif // !_SYSTEM_H
