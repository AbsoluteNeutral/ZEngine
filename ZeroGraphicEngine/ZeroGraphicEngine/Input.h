

#ifndef INPUT_H_
#define INPUT_H_

#include "inputkeydef.h"
#include "System.h"

class ZG_API Input : public System
{
public:
	Input() noexcept;
	virtual ~Input();

	void Init(HWND mainHWND, int width_, int height_);

	POINT GetMouseDelta();
	POINT GetPrevMouseScreenPos() const;
	POINT GetMouseScreenPos() const;
	long GetMouseScreenPosX() const;
	long GetMouseScreenPosY() const;

	bool IsLockMouse	() const;
	bool IsMouseShown	() const;
	bool IsMouseMoving	() const;

	bool IsKeyDown		(unsigned int key) const;
	bool IsKeyUp		(unsigned int key) const;
	bool IsKeyPressed	(unsigned int key) const;
	bool IsKeyRelease	(unsigned int key) const;

	bool IsAnyKeyPressed() const;
	bool IsAnyKeyRelease() const;
	bool IsAnyKeyDown	() const;
	bool IsAnyKeyUp		() const;

	int* GetRawAxis();
	void SetCursorPosition(POINT pos);

	void LockCursor(bool toggle_);
	void ShowMouseCursor(bool toggle_);

	void UpdateScreenSize(int width_, int height_);
	virtual void SystemUpdate();
	virtual bool SystemExit();

	static float MouseSensitivity;
	static std::map<size_t, int> KEYBOARD_NAME_MAP;
	static std::map<size_t, int> CONTROLLER_NAME_MAP;
	static std::map<size_t, std::vector<int>> INPUT_KEYCODE;
	static std::map<size_t, std::vector<int>> INPUT_CONTROLLERCODE;

	float mouseWheelDelta;
private:
	bool keyBoardFlicker;
	bool lockMouse;
	bool showingMouse;
	HWND winHWND;					//get handle from window
	BYTE* currKeyboardState;		//get current state of keyboard
	BYTE* prevKeyboardState;		//get previous state of keyboard
	BYTE keyBoardSwitch_0[256];		//set current state of keyboard
	BYTE keyBoardSwitch_1[256];		//set current state of keyboard
	POINT currMouseScreenPos;
	POINT prevMouseScreenPos;
	POINT deltaMouse;
	int rawAxis[2];
	POINT halfscreen;
};

//Internal use only
ZG_API void AddInputKeyAlias				(const std::string& inputaliasName_);
ZG_API void UpdateInputKeyCode			(const std::string& inputaliasName_, int position, int keycode_);
ZG_API void UpdateInputControllerCode	(const std::string& inputaliasName_, int position, int controllercode);
ZG_API void AddInputKeyCode				(const std::string& inputaliasName_, int keycode_);
ZG_API void AddInputControllerCode		(const std::string& inputaliasName_, int controllercode);
ZG_API void RemoveInputKeyCode			(const std::string& inputaliasName_, int position);
ZG_API void RemoveInputControllerCode	(const std::string& inputaliasName_, int position);

ZG_API const std::map<size_t, std::vector<int>>& GetListOfKeyCode();
ZG_API const std::map<size_t, std::vector<int>>& GetListOfControllerCode();

ZG_API void RemoveInputKeyAlias(const std::string& inputaliasName_);
ZG_API void SaveInputAlias();

//Game
ZG_API int GetKeyboardAlias(const std::string& inputaliasName_, int position);
ZG_API int GetControllerAlias(const std::string& inputaliasName_, int position);

#endif //INPUT_
