

#ifndef INPUT_H_
#define INPUT_H_

#include "inputkeydef.h"
#include "System.h"

class ZG_API Input : public System
{
public:
	Input		  () noexcept;	//ctor
	virtual ~Input();	

	void Init(HWND mainHWND, int width_, int height_);

	POINT GetMouseDelta			();		  //the difference between previous and next mouse position
	POINT GetPrevMouseScreenPos	() const; //previous mouse screen position
	POINT GetMouseScreenPos		() const; //current mouse screen position
	long GetMouseScreenPosX		() const; //current mouse screen position X
	long GetMouseScreenPosY		() const; //current mouse screen position Y

	bool IsLockMouse		() const; //is Mouse lock in the middle of screen
	bool IsMouseShown		() const; //is Mouse visible?
	bool IsMouseMoving		() const; //is Mouse moving?
							
	bool IsKeyDown			(unsigned int key) const; //is key being held down
	bool IsKeyUp			(unsigned int key) const; //is key up, update every frame
	bool IsKeyPressed		(unsigned int key) const; //is key being pressed down ONCE?
	bool IsKeyRelease		(unsigned int key) const; //is key being release ONCE?
							
	bool IsAnyKeyPressed	() const;	//is any key being pressed down ONCE?
	bool IsAnyKeyRelease	() const;	//is any key being release ONCE?
	bool IsAnyKeyDown		() const;	//is any key being held down
	bool IsAnyKeyUp			() const;	//is any key up, update every frame

	int* GetRawAxis			();
	void SetCursorPosition	(POINT pos);	//force set mouse position

	void LockCursor			(bool toggle_);
	void ShowMouseCursor	(bool toggle_);

	void UpdateScreenSize		(int width_, int height_); //internal usage Update screensize
	virtual void SystemUpdate	();
	virtual bool SystemExit		();

	static float MouseSensitivity;
	static std::map<size_t, int> KEYBOARD_NAME_MAP;
	static std::map<size_t, int> CONTROLLER_NAME_MAP;
	static std::map<size_t, std::vector<int>> INPUT_KEYCODE;
	static std::map<size_t, std::vector<int>> INPUT_CONTROLLERCODE;

	float mouseWheelDelta;
private:
	bool keyBoardFlicker;			//internal usage for updating keyboard
	bool lockMouse;					//internal usage for locking mouse
	bool showingMouse;				//internal usage for showing mouse
	HWND winHWND;					//get handle from window
	BYTE* currKeyboardState;		//get current state of keyboard
	BYTE* prevKeyboardState;		//get previous state of keyboard
	BYTE keyBoardSwitch_0[256];		//set current state of keyboard
	BYTE keyBoardSwitch_1[256];		//set current state of keyboard
	POINT currMouseScreenPos;		//current mouse position
	POINT prevMouseScreenPos;		//previous mouse position
	POINT deltaMouse;				//difference of prevMouseScreenPos - currMouseScreenPos
	int rawAxis[2];					//check if [0]=X and [1]=Y is positive or negative in absolute value
	POINT halfscreen;				//internal usage for locking mouse
};

//Internal use only
ZG_API void AddInputKeyAlias			(const std::string& inputaliasName_);
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
