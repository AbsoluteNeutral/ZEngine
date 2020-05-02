

#include "stdafx.h"
#include "Input.h"
#include "Engine.h"
//#include "GlobalFileSystem.h"

//hased string, arraynum
float Input::MouseSensitivity = 1.0f;
std::map<size_t, int> Input::KEYBOARD_NAME_MAP;
std::map<size_t, int> Input::CONTROLLER_NAME_MAP;
std::map<size_t, std::vector<int>> Input::INPUT_KEYCODE;
std::map<size_t, std::vector<int>> Input::INPUT_CONTROLLERCODE;

Input::Input() noexcept
	:mouseWheelDelta(0.0f)
	, lockMouse(false)
	, showingMouse(true)
	, keyBoardFlicker(true)
	, winHWND(0)
	, currKeyboardState(nullptr)
	, prevKeyboardState(nullptr)
	, keyBoardSwitch_0{ 0 }
	, keyBoardSwitch_1{ 0 }
	, currMouseScreenPos{}
	, prevMouseScreenPos{}
	, deltaMouse{}
	, rawAxis{}
	, halfscreen{}
{}

Input::~Input(){}

void Input::Init(HWND mainHWND, int width_, int height_){
	winHWND = mainHWND;
	halfscreen.x = width_  >> 1;
	halfscreen.y = height_ >> 1;
#if defined(_DEBUG)
	if (!GetKeyboardState(keyBoardSwitch_0) && !GetKeyboardState(keyBoardSwitch_1)) {
		Warn("KeyBoard not Found");
	}
#else
	GetKeyboardState(keyBoardSwitch_0);
	GetKeyboardState(keyBoardSwitch_1);
#endif
	prevKeyboardState = keyBoardSwitch_0;	//set prev to 0
	currKeyboardState = keyBoardSwitch_1;	//set curr to 1

	static std::hash<std::string> hasher;
	size_t hashedString = 0;
	for (int i = 0; i < NUMBER_OF_KEYBOARD_KEY; ++i) {
		hashedString = hasher(LIST_KEYBOARD_NAME_[i]);
		KEYBOARD_NAME_MAP[hashedString] = i; // LIST_KEYBOARD_CODE_[i];
	}
	for (int i = 0; i < NUMBER_OF_CONTROLLER_KEY; ++i) {
		hashedString = hasher(LIST_CONTROLLER_NAME_[i]);
		CONTROLLER_NAME_MAP[hashedString] = i;// LIST_CONTROLLER_CODE_[i];
	}
	
}

void Input::UpdateScreenSize(int width_, int height_) {
	halfscreen.x = width_  >> 1;
	halfscreen.y = height_ >> 1;
}
void Input::SystemUpdate(){

	if (keyBoardFlicker)
	{
#ifdef _64BITMACHINE
		for (int i = 0; i < 256; i += 8)
		{
			keyBoardSwitch_1[i]		= GetAsyncKeyState(i	) ? 0xF0 : 0x0;
			keyBoardSwitch_1[i + 1] = GetAsyncKeyState(i + 1) ? 0xF0 : 0x0;
			keyBoardSwitch_1[i + 2] = GetAsyncKeyState(i + 2) ? 0xF0 : 0x0;
			keyBoardSwitch_1[i + 3] = GetAsyncKeyState(i + 3) ? 0xF0 : 0x0;
			keyBoardSwitch_1[i + 4] = GetAsyncKeyState(i + 4) ? 0xF0 : 0x0;
			keyBoardSwitch_1[i + 5] = GetAsyncKeyState(i + 5) ? 0xF0 : 0x0;
			keyBoardSwitch_1[i + 6] = GetAsyncKeyState(i + 6) ? 0xF0 : 0x0;
			keyBoardSwitch_1[i + 7] = GetAsyncKeyState(i + 7) ? 0xF0 : 0x0;
		}
#else
		//BYTE* b = keyBoardSwitch_1;
		//BYTE* e = keyBoardSwitch_1 + 256;
		//while (b != e)
		//{
		//	*b = GetAsyncKeyState(b - keyBoardSwitch_1) ? 0xF0 : 0x0;
		//	++b;
		//}

		//Unroll x4
		BYTE* b = keyBoardSwitch_1;
		BYTE* e = keyBoardSwitch_1 + 256;
		while (b != e)
		{
			int diff = b - keyBoardSwitch_1;
			*b		 = GetAsyncKeyState(diff) ? 0xF0 : 0x0;
			*(b + 1) = GetAsyncKeyState(diff + 1) ? 0xF0 : 0x0;
			*(b + 2) = GetAsyncKeyState(diff + 2) ? 0xF0 : 0x0;
			*(b + 3) = GetAsyncKeyState(diff + 3) ? 0xF0 : 0x0;
			b += 4;
		}
#endif
		//GetKeyboardState(keyBoardSwitch_1);		//get curent state to keyboard1
		prevKeyboardState = keyBoardSwitch_0;	//set prev to 0
		currKeyboardState = keyBoardSwitch_1;	//set curr to 1
	}
	else
	{
#ifdef _64BITMACHINE
		for (int i = 0; i < 256; i += 8)
		{
			keyBoardSwitch_0[i]		= GetAsyncKeyState(i) ? 0xF0 : 0x0;
			keyBoardSwitch_0[i + 1] = GetAsyncKeyState(i + 1) ? 0xF0 : 0x0;
			keyBoardSwitch_0[i + 2] = GetAsyncKeyState(i + 2) ? 0xF0 : 0x0;
			keyBoardSwitch_0[i + 3] = GetAsyncKeyState(i + 3) ? 0xF0 : 0x0;
			keyBoardSwitch_0[i + 4] = GetAsyncKeyState(i + 4) ? 0xF0 : 0x0;
			keyBoardSwitch_0[i + 5] = GetAsyncKeyState(i + 5) ? 0xF0 : 0x0;
			keyBoardSwitch_0[i + 6] = GetAsyncKeyState(i + 6) ? 0xF0 : 0x0;
			keyBoardSwitch_0[i + 7] = GetAsyncKeyState(i + 7) ? 0xF0 : 0x0;
		}
#else
		//BYTE* b = keyBoardSwitch_0;
		//BYTE* e = keyBoardSwitch_0 + 256;
		//while (b != e)
		//{
		//	*b = GetAsyncKeyState(b - keyBoardSwitch_0) ? 0xF0 : 0x0;
		//	++b;
		//}

		//Unroll x4
		BYTE* b = keyBoardSwitch_0;
		BYTE* e = keyBoardSwitch_0 + 256;
		while (b != e)
		{
			int diff = b - keyBoardSwitch_0;
			*b		 = GetAsyncKeyState(diff) ? 0xF0 : 0x0;
			*(b + 1) = GetAsyncKeyState(diff + 1) ? 0xF0 : 0x0;
			*(b + 2) = GetAsyncKeyState(diff + 2) ? 0xF0 : 0x0;
			*(b + 3) = GetAsyncKeyState(diff + 3) ? 0xF0 : 0x0;
			b += 4;
		}
#endif
		//GetKeyboardState(keyBoardSwitch_0);	//get curent state to keyboard0
		prevKeyboardState = keyBoardSwitch_1;	//set prev to 1
		currKeyboardState = keyBoardSwitch_0;	//set curr to 0
	}

	keyBoardFlicker = !keyBoardFlicker;			//toggling

	prevMouseScreenPos = currMouseScreenPos;

	if (GetCursorPos(&currMouseScreenPos))
		ScreenToClient(winHWND, &currMouseScreenPos);

	if (lockMouse) 
	{
		deltaMouse.x = (halfscreen.x - currMouseScreenPos.x );
		deltaMouse.y = (halfscreen.y - currMouseScreenPos.y );
		POINT tmp = halfscreen;
		ClientToScreen(Engine::sys_WinDisplay.GetHWND(), &tmp);
		SetCursorPos(tmp.x, tmp.y);
	}
	else 
	{
		deltaMouse.x = (prevMouseScreenPos.x - currMouseScreenPos.x);
		deltaMouse.y = (prevMouseScreenPos.y - currMouseScreenPos.y);
	}

	//prevMouseScreenPos = currMouseScreenPos;

	//deltaMouse.x = deltaMouse.x > 5 ? 5   : deltaMouse.x;
	//deltaMouse.x = deltaMouse.x < -5 ? -5 : deltaMouse.x;
	//deltaMouse.y = deltaMouse.x > 5 ? 5   : deltaMouse.y;
	//deltaMouse.y = deltaMouse.x < -5 ? -5 : deltaMouse.y;
	//POINT p{ Engine::sys_Input.GetMouseScreenPos() };
	//
	//bool change = false;
	//if (p.x < 0) {
	//	p.x += 5; change = true;
	//}
	//else if (p.x > Engine::sys_WinDisplay.mWidth) {
	//	p.x -= 5; change = true;
	//}
	//if (p.y < 0) {
	//	p.y += 5; change = true;
	//}
	//else if (p.y > Engine::sys_WinDisplay.mHeight) {
	//	p.y -= 5; change = true;
	//}
}

POINT Input::GetMouseDelta() {
	return deltaMouse;
}

POINT Input::GetMouseScreenPos() const{
	return currMouseScreenPos;
}

long Input::GetMouseScreenPosX() const {
	return currMouseScreenPos.x;
}

long Input::GetMouseScreenPosY() const {
	return currMouseScreenPos.y;
}

POINT Input::GetPrevMouseScreenPos() const{
	return prevMouseScreenPos;
}

bool Input::IsLockMouse() const {
	return lockMouse;
}
bool Input::IsMouseShown() const {
	return showingMouse;
}
bool Input::IsMouseMoving() const {
	return deltaMouse.x > 0.0f || deltaMouse.y > 0.0f;
}		  
bool Input::IsAnyKeyPressed() const {
	BYTE* b1 = prevKeyboardState;
	BYTE* b = currKeyboardState;
	BYTE* e = currKeyboardState + 256;
	while (b != e) {
		if ((*b1 & 0xF0) == 0 &&(*b & 0xF0) != 0)
			return true;
		++b; ++b1;
	}
	return false;
}
bool Input::IsAnyKeyRelease() const {
	BYTE* b1 = prevKeyboardState;
	BYTE* b = currKeyboardState;
	BYTE* e = currKeyboardState + 256;
	while (b != e) {
		if ((*b1 & 0xF0) != 0 && (*b & 0xF0) == 0)
			return true;
		++b; ++b1;
	}
	return false;
}
bool Input::IsAnyKeyDown() const {
	//BYTE* b1 = prevKeyboardState;
	//BYTE* b = currKeyboardState;
	//BYTE* e = currKeyboardState + 256;
	//while (b != e) {
	//	if ((*b1 & 0xF0) != 0 && (*b & 0xF0) != 0)
	//		return true;
	//	++b; ++b1;
	//}
	for (int i = 0; i < 256; ++i) {
		if (GetAsyncKeyState(i))
			return true;
		//std::cout << i << std::endl;
	}
	return false;
}
bool Input::IsAnyKeyUp() const {
	BYTE* b1 = prevKeyboardState;
	BYTE* b = currKeyboardState;
	BYTE* e = currKeyboardState + 256;
	while (b != e) {
		if ((*b1 & 0xF0) == 0 && (*b & 0xF0) == 0)
			return true;
		++b; ++b1;
	}
	return false;
}
bool Input::IsKeyDown(unsigned int key) const{
	return ((prevKeyboardState[key] & 0xF0) != 0 && (currKeyboardState[key] & 0xF0) != 0);
	//return ( (GetAsyncKeyState(key) & 0x0001) != 0 && (GetAsyncKeyState(key) & 0x8000) != 0);
	//return GetAsyncKeyState(key);
}

bool Input::IsKeyUp(unsigned int key) const {
	return ((prevKeyboardState[key] & 0xF0) == 0 && (currKeyboardState[key] & 0xF0) == 0);
	//return ( (GetAsyncKeyState(key) & 0x0001) == 0 && (GetAsyncKeyState(key) & 0x8000) == 0);
	//return !GetAsyncKeyState(key);
}		

bool Input::IsKeyPressed(unsigned int key) const {
	return ((prevKeyboardState[key] & 0xF0) == 0 && (currKeyboardState[key] & 0xF0) != 0);
	//return ( (GetAsyncKeyState(key) & 0x0001) == 0 && (GetAsyncKeyState(key) & 0x8000) != 0);
}

bool Input::IsKeyRelease(unsigned int key) const {
	return ((prevKeyboardState[key] & 0xF0) != 0 && (currKeyboardState[key] & 0xF0) == 0);
	//return ( (GetAsyncKeyState(key) & 0x0001) != 0 && (GetAsyncKeyState(key) & 0x8000) == 0);
}			

int* Input::GetRawAxis() {
	POINT tmp = GetMouseDelta();
	//x
	if (tmp.x > 0)
		rawAxis[0] = 1;
	else if (tmp.x < 0)
		rawAxis[0] = -1;
	else
		rawAxis[0] = 0;
	//y
	if (tmp.y > 0)
		rawAxis[1] = 1;
	else if (tmp.y < 0)
		rawAxis[1] = -1;
	else
		rawAxis[1] = 0;
	return rawAxis;
}

void Input::SetCursorPosition(POINT pos)
{
	currMouseScreenPos = pos;
}

void Input::LockCursor(bool toggle_) {
	lockMouse = toggle_;
}
void Input::ShowMouseCursor(bool toggle_) {
	//Show Cursor uses as a counter instead as a boolean
	//this is why this if statment is needed
	if (showingMouse == toggle_) return;

	showingMouse = toggle_;
	ShowCursor(toggle_); 
}

bool Input::SystemExit(){
	return true;
}

void AddInputKeyAlias(const std::string& inputaliasName_) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	Input::INPUT_KEYCODE[hashedString] = std::vector<int>{};
	Input::INPUT_CONTROLLERCODE[hashedString] = std::vector<int>{};
}

void UpdateInputKeyCode(const std::string& inputaliasName_, int position, int keycode_ ) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	Input::INPUT_KEYCODE[hashedString][position] = keycode_;
}
void UpdateInputControllerCode(const std::string& inputaliasName_, int position, int controllercode) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	Input::INPUT_CONTROLLERCODE[hashedString][position] = controllercode;
}
void AddInputKeyCode(const std::string& inputaliasName_, int keycode_) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	Input::INPUT_KEYCODE[hashedString].push_back(keycode_);
}
void AddInputControllerCode(const std::string& inputaliasName_, int controllercode) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	Input::INPUT_CONTROLLERCODE[hashedString].push_back(controllercode);
}
void RemoveInputKeyCode(const std::string& inputaliasName_, int position) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	Input::INPUT_KEYCODE[hashedString].erase(Input::INPUT_KEYCODE[hashedString].begin() + position);
}
void RemoveInputControllerCode(const std::string& inputaliasName_, int position) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	Input::INPUT_CONTROLLERCODE[hashedString].erase(Input::INPUT_CONTROLLERCODE[hashedString].begin() + position);
}

int GetKeyboardAlias(const std::string& inputaliasName_, int position) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	return Input::INPUT_KEYCODE[hashedString][position];
}
int GetControllerAlias(const std::string& inputaliasName_, int position) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	return Input::INPUT_CONTROLLERCODE[hashedString][position];
}

const  std::map<size_t, std::vector<int>>& GetListOfKeyCode() {
	return Input::INPUT_KEYCODE;
}
const  std::map<size_t, std::vector<int>>& GetListOfControllerCode() {
	return Input::INPUT_CONTROLLERCODE;
}

void RemoveInputKeyAlias(const std::string& inputaliasName_) {
	size_t hashedString = GetHashFromString(inputaliasName_);
	auto find = Input::INPUT_KEYCODE.find(hashedString);
	if (find != Input::INPUT_KEYCODE.end()) {
		Input::INPUT_KEYCODE.erase(hashedString);
		Input::INPUT_CONTROLLERCODE.erase(hashedString);
	}
}

void SaveInputAlias() {
	//FileMgr file;
	//if (file.Open(PATH_ETC + "inputalias.data", FILE_WRITE)) {
	//	
	//	for (auto& elem : Input::INPUT_KEYCODE) {
	//		file.Write("qwert:", 0);
	//		file.Write(GetStringFromHash(elem.first), 0);
	//
	//		for (auto& elem2 : elem.second)
	//			file.Write("k", elem2, 0);
	//
	//		for (auto& elem2 : Input::INPUT_CONTROLLERCODE[elem.first])
	//			file.Write("c", elem2, 0);
	//	}	
	//}
}
