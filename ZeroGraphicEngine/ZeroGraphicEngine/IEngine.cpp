/*****************************************************************************/
/*!
\file			IEngine.cpp
\brief			Front End of Engine.cpp
*/
/*****************************************************************************/
#include "stdafx.h"

#include "IEngine.h"
#include "Engine.h"
//#include "ResourceManager.h"

ZG_API Camera* MainCam = nullptr;

static GameObject	EngineGameObjectCamera{ "DefaultCamera" };	//internal only
static Camera		EngineCam{};								//internal only
static std::string	LoadTitle{};								//internal only
static std::string	SaveFilePath{};								//internal only
//static int levelunlock = 1;

//void luatest();

static int level1State = -2;
static int level2State = -2;
static int level3State = -2;
static int difficulty = 2;

#define LOAD_FROMSAVEFILE 0
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined(_EDITOR_ON)
void EngineInitialize(HINSTANCE hInstanceH, WndProc function_, const char* windowname_)
#else
void EngineInitialize(HINSTANCE hInstanceH, const char* windowname_ )
#endif
{
	//Load file path
	LoadSaveFilePath(windowname_);

#if LOAD_FROMSAVEFILE 
	#if defined(_EDITOR_ON)
		//SaveFilePath = std::string{ SAVE_FILE_PATH } + "\\configeditor.data";
		SaveFilePath = std::string{ SAVE_FILE_PATH } + "\\configeditor.xml";
	#else
		//SaveFilePath = std::string{ SAVE_FILE_PATH } + "\\config.data";
		SaveFilePath = std::string{ SAVE_FILE_PATH } + "\\config.xml";
	#endif
#endif
	FXMLDocument doc;
	FXMLNode *pRoot			= XMLLoadPath(doc, SaveFilePath) ? doc.FirstChild() : nullptr;
	FXMLElement* pElement	= pRoot ? pRoot->FirstChildElement() : nullptr;

	//load config file
#ifdef _EDITOR_ON
	int  LoadWidth			= pElement ? pElement->IntAttribute("width", zg::GOBAL_RESOLUTION_WIDTH) : zg::GOBAL_RESOLUTION_WIDTH;
	int  LoadHeight			= pElement ? pElement->IntAttribute("height", zg::GOBAL_RESOLUTION_HEIGHT) : zg::GOBAL_RESOLUTION_HEIGHT;
	bool LoadShowConsole	= pElement ? pElement->BoolAttribute("showconsole", true) : true;
	bool LoadFullScreen		= pElement ? pElement->BoolAttribute("fullscreen", false) : false;
	//int  LoadWidth		= 1280;
	//int  LoadHeight		= 720;
	//bool LoadShowConsole	= true;
	//bool LoadFullScreen	= false;
	//int tmp = pElement ? pElement->IntAttribute("levelunlock") : 1;
	//levelunlock = tmp ? tmp : 3;
#else
	int  LoadWidth			= pElement ? pElement->IntAttribute("width", zg::GOBAL_RESOLUTION_WIDTH) : zg::GOBAL_RESOLUTION_WIDTH;
	int  LoadHeight			= pElement ? pElement->IntAttribute("height", zg::GOBAL_RESOLUTION_HEIGHT) : zg::GOBAL_RESOLUTION_HEIGHT;
	//int  LoadWidth		 = 1920;
	//int  LoadHeight		 = 1080;
	//int tmp = pElement ? pElement->IntAttribute("levelunlock") : 1;
	//levelunlock = tmp ? tmp : 1;
#if NDEBUG
	bool LoadShowConsole	= pElement ? pElement->BoolAttribute("showconsole", false) : false;
	bool LoadFullScreen		= pElement ? pElement->BoolAttribute("fullscreen", true) : true;
#else
	bool LoadShowConsole	= pElement ? pElement->BoolAttribute("showconsole", true) : true;
	bool LoadFullScreen		= pElement ? pElement->BoolAttribute("fullscreen", false) : false;
#endif //NDEBUG

#endif //_EDITOR_ON
	LoadTitle   = pElement ? pElement->Attribute("name") : windowname_; 

	//Engine init
#if defined(_EDITOR_ON)
	if (!pEngine.EngineInitialize(hInstanceH, LoadWidth, LoadHeight, LoadShowConsole, LoadTitle.c_str(), function_)) {
		ErrorMsg("IEngine with Editor failed");
	}
#else
	if (!pEngine.EngineInitialize(hInstanceH, LoadWidth, LoadHeight, LoadShowConsole, LoadTitle.c_str())) {
		ErrorMsg("IEngine failed");
	}
#endif
	//load essential 
	EngineCam.Attach(&EngineGameObjectCamera, 0);
	//RM::audioListener->Attach(&EngineGameObjectCamera);
	MainCam = &EngineCam;
	MainCam->SetSize(120);
	FullScreen(LoadFullScreen);

	//graphics
	//Engine::sys_OpenGLGraphic.gamma				= pElement ? pElement->FloatAttribute("gamma", 1.0f) : 1.0f;

	//input
	Engine::sys_XInputSystem.ControllerSensitivity	= pElement ? pElement->FloatAttribute("ControllerSensitivity", Engine::sys_XInputSystem.ControllerSensitivity) : Engine::sys_XInputSystem.ControllerSensitivity;
	Engine::sys_Input.MouseSensitivity				= pElement ? pElement->FloatAttribute("MouseSensitivity", Engine::sys_Input.MouseSensitivity) : Engine::sys_Input.MouseSensitivity;

	//audio
	float* b = AudioListener::channelgroup_volume;
	float* e = AudioListener::channelgroup_volume + NUMBEROF_CHANNELGROUP;
	while (b != e) {
		*b = pElement ? pElement->FloatAttribute( std::string{ "volume" + (e - b) }.c_str(), *b) : *b;
		++b;
	}
}

void EngineSetFrameRate(bool lock_frameRate_, float frameRate_) {
#if defined(_DEBUG) || defined(_EDITOR_ON)
	if (frameRate_ <= 0) {
		ErrorMsg("Framerate is set to <= 0");
	}
#endif
	Engine::sys_FrameRateController.LockFrameRate(lock_frameRate_, frameRate_);
}

bool  EngineIsRunning() {
	return pEngine.IsRunning();
}

void  EngineUpdate() {
	EngineGameObjectCamera.transform.Update();
	pEngine.EngineUpdate();
}

void  EngineLateUpdate() {
	pEngine.EngineLateUpdate();
}

void EngineUnForcedClose() {
	Engine::sys_WinDisplay.mForceClose = false;
}

bool  IsEngineForcedClose() {
	return Engine::sys_WinDisplay.mForceClose;
}
void EngineQuit() {
	pEngine.StopRunning();
}

void EngineSaveSettings() {
	FXMLDocument doc;
	FXMLNode * pRoot = doc.NewElement("configuration");   //root
	doc.InsertFirstChild(pRoot);					  //create first child
	FXMLElement* pElement = doc.NewElement("settings"); // insert new element called Dimensions
	pElement->SetAttribute("width", zg::GOBAL_RESOLUTION_WIDTH);
	pElement->SetAttribute("height", zg::GOBAL_RESOLUTION_HEIGHT);
	pElement->SetAttribute("showconsole", Engine::sys_WinDisplay.mShowConsole);
	pElement->SetAttribute("fullscreen", Engine::sys_WinDisplay.mFullScreen);
	pElement->SetAttribute("name", LoadTitle.c_str());
	//pElement->SetAttribute("levelunlock", levelunlock);
	//graphics
	//pElement->SetAttribute("gamma", Engine::sys_OpenGLGraphic.gamma);

	//input
	pElement->SetAttribute("ControllerSensitivity", Engine::sys_XInputSystem.ControllerSensitivity);
	pElement->SetAttribute("MouseSensitivity", Engine::sys_Input.MouseSensitivity);

	//audio
	float* b = AudioListener::channelgroup_volume;
	float* e = AudioListener::channelgroup_volume + NUMBEROF_CHANNELGROUP;
	while (b != e) {
		pElement->SetAttribute(std::string{ "volume" + std::to_string((e-b)) }.c_str() , *b);
		++b;
	}

	pRoot->InsertEndChild(pElement); // end of Dimensions
	XMLSavePath(doc, SaveFilePath);
}

void  EngineExit() {

	EngineSaveSettings();
	//FileMgr file;
	//if (file.Open(SaveFilePath, FILE_WRITE)) {
	//	file.Write(">settings:", 0);
	//	file.Write("width:", Engine::sys_WinDisplay.ResolutionWidth, 1);
	//	file.Write("height:", Engine::sys_WinDisplay.ResolutionHeight, 1);
	//	file.Write("showconsole:", Engine::sys_WinDisplay.mShowConsole, 1);
	//	file.Write("fullscreen:", Engine::sys_WinDisplay.mFullScreen, 1);
	//	file.Write("name:", LoadTitle, 1);
    //file.Write("gamma:", Engine::sys_OpenGLGraphic.gamma, 1);
	//
	//	file.Write(">physics:", 0);
	//	file.Write("Gravity", Physics::Gravity, 1);
	//	file.Write("MaxVelocity", Physics::MaxVelocity, 1);
	//	file.Write("PenetrationSlop", PENETRATION_SLOP, 1);
	//	file.Write("PenetrationEpsilon", PENETRATION_EPSILON, 1);
	//	file.Write("PenetrationResolvePercentage", PENETRATION_RESOLVE_PERCENTAGE, 1);
	//	file.Write("AccelrationAdjustment", ACCELRATION_ADJUSTMENT, 1);
	//	file.Close();
	//}
	RM::ClearResourceVaribles();
	DestroyAllAssets();	//Detroy all assets loaded from DIR
	pEngine.EngineExit();
}

float GetFrameRate() {
	return Engine::sys_FrameRateController.GetFPS();
}

void BindColliderAndRigidBody() {
	//Engine::sys_Physics.BindColliderAndRigidBody();
}
void BindDrawComponent() {
	//Engine::sys_OpenGLGraphic.BindDrawComponent();
}

void SetEngineActive(bool active_) {
	pEngine.SetActive(active_);
}

bool IsEngineActive() {
	return Engine::sys_WinDisplay.mActive;
}

//getters
float GetControllerSensitivity() {
	return Engine::sys_XInputSystem.ControllerSensitivity;
}
float GetMouseSensitivity() {
	return Engine::sys_Input.MouseSensitivity;
}
//setters
void SetControllerSensitivity(float value_) {
	Engine::sys_XInputSystem.ControllerSensitivity = value_;
}
void SetMouseSensitivity(float value_) {
	Engine::sys_Input.MouseSensitivity = value_;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool IsFullScreen() { return Engine::sys_WinDisplay.IsFullScreen(); }
void FullScreen(bool fullScreen_) {
	Engine::sys_WinDisplay.FullScreen(fullScreen_);
}

int ScreenWidth()				{ return Engine::sys_WinDisplay.mWidth; }
int ScreenHeight()				{ return Engine::sys_WinDisplay.mHeight; }
int ScreenResolutionWidth()		{ return zg::GOBAL_RESOLUTION_WIDTH; }
int ScreenResolutionHeight()	{ return zg::GOBAL_RESOLUTION_HEIGHT; }
void  SetResolution(RESOLUTION_SIZE size_) {
	Engine::sys_OpenGLGraphic.SetResoultion(size_);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void WindowBackGroundColor(const zg::Color& color_) {
	Engine::sys_OpenGLGraphic.SetBackgroundColor(color_);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Camera * GetDefaultCamera()
{
  return &EngineCam;
}
void SetDefaultCamera() {
	MainCam = &EngineCam;
}
bool IsDefaultCamera() {
	return MainCam == &EngineCam;
}
void SetCAMERA(Camera* current_camera) {
  if (current_camera)
    MainCam = current_camera;
  else
    PopUp_Warn("SetCAMERA(), current_camera is a nullptr pass in\n");
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//XboxInput* SetNumberOfXboxControllers(unsigned num) {
//	if (Engine::sys_XInputSystem.Create(num)) {
//		return Engine::sys_XInputSystem.GetXboxConHandle();
//	}
//	else return nullptr;
//}

XboxInput* GetController(unsigned num) {
	return Engine::sys_XInputSystem.GetXboxCon(num);
}

zg::Vector2  GetMouseDelta() {
	POINT p{ Engine::sys_Input.GetMouseDelta() };
	return zg::Vector2{ static_cast<float>(p.x),
					static_cast<float>(p.y) };
}
zg::Vector2  GetPrevMouseScreenPos() {
	POINT p{ Engine::sys_Input.GetPrevMouseScreenPos() };
	return zg::Vector2{ static_cast<float>(p.x),
					static_cast<float>(p.y) };
}
zg::Vector2  GetMouseScreenPos() {
	POINT p{ Engine::sys_Input.GetMouseScreenPos() };
	return zg::Vector2{ static_cast<float>(p.x),
					static_cast<float>(p.y) };
}

long  GetMouseScreenPosX() {
	return Engine::sys_Input.GetMouseScreenPosX();
}
long  GetMouseScreenPosY() {
	return Engine::sys_Input.GetMouseScreenPosY();
}
float  GetMouseWheelDelta() {
	return Engine::sys_Input.mouseWheelDelta;
}
void  ShowMouseCursor(bool toggle_) {
	Engine::sys_Input.ShowMouseCursor(toggle_);
}
bool IsMouseMoving() {
	return Engine::sys_Input.IsMouseMoving();
}

zg::Vector2 GetRawAxis() {
	return zg::Vector2{ static_cast<float>(Engine::sys_Input.GetRawAxis()[0]),
					static_cast<float>(-Engine::sys_Input.GetRawAxis()[1]) };
}

void SetCursorPosition(float x, float y){
	Engine::sys_Input.SetCursorPosition(POINT{ static_cast<long>(x), static_cast<long>(y) });
}

void LockCursor(bool toggle_) {
	Engine::sys_Input.LockCursor(toggle_);
}
#ifdef USING_SOL2
sol::state_view GetLuaState(){
	return Engine::sys_Logic.GetLuaState();
}
#endif

zg::Vector3 ConvertToWorldCoor(const zg::Vector2& mouse_)
{
	zg::Vector2 ms{ mouse_ };
	float x = (2.0f * ms.x) / ScreenWidth() - 1.0f;
	float y = 1.0f - (2.0f * ms.y) / ScreenHeight();
	zg::Vector4 ray_clip{ x,y,-1.0f, 1.0f };
	
	if (MainCam->GetType() == CAM_ORTHO)
	{
		ray_clip = zg::GaussJordanInverse(MainCam->ProjViewMatrix()) * ray_clip;
		return zg::Vector3{ ray_clip.x / ray_clip.w, ray_clip.y / ray_clip.w, 0.0f };
	}
	else 
	{
		zg::Vector4 ray_eye = zg::GaussJordanInverse(MainCam->ProjectionMatrix()) * ray_clip;
		ray_eye.z = -1.0f;
		ray_eye.w = 0.0f;
		zg::Vector3 ray_wor = zg::Vector3{ zg::GaussJordanInverse(MainCam->ViewMatrix()) * ray_eye };
		//ray_wor.Normalize();
		return ray_wor;
	}
}
zg::Vector3 MouseToWorld() {
	return ConvertToWorldCoor(GetMouseScreenPos());
}

bool KeyDown(unsigned key)		{ return Engine::sys_Input.IsKeyDown(key); }
bool KeyUp(unsigned key)		{ return Engine::sys_Input.IsKeyUp(key); }
bool KeyPressed(unsigned key)	{ return Engine::sys_Input.IsKeyPressed(key); }
bool KeyRelease(unsigned key)	{ return Engine::sys_Input.IsKeyRelease(key); }

bool KeyAliasDown(const std::string& keyaliasname_) {
	size_t hashnum = GetHashFromString(keyaliasname_);
	bool check = false;
	for (auto& elem : Input::INPUT_KEYCODE[hashnum]) {
		check = Engine::sys_Input.IsKeyDown(LIST_KEYBOARD_CODE_[elem]) ? true : false;
		if (!check) return false;
	}
	return check;
}
bool KeyAliasUp(const std::string& keyaliasname_) {
	size_t hashnum = GetHashFromString(keyaliasname_);
	bool check = false;
	for (auto& elem : Input::INPUT_KEYCODE[hashnum]) {
		check = Engine::sys_Input.IsKeyUp(LIST_KEYBOARD_CODE_[elem]) ? true : false;
		if (!check) return false;
	}
	return check;
}
bool KeyAliasPressed(const std::string& keyaliasname_) {
	size_t hashnum = GetHashFromString(keyaliasname_);
	bool check = false;
	for (auto& elem : Input::INPUT_KEYCODE[hashnum]) {
		check = Engine::sys_Input.IsKeyPressed(LIST_KEYBOARD_CODE_[elem]) ? true : false;
		if (!check) return false;
	}
	return check;
}
bool KeyAliasRelease(const std::string& keyaliasname_) {
	size_t hashnum = GetHashFromString(keyaliasname_);
	bool check = false;
	for (auto& elem : Input::INPUT_KEYCODE[hashnum]) {
		check = Engine::sys_Input.IsKeyRelease(LIST_KEYBOARD_CODE_[elem]) ? true : false;
		if (!check) return false;
	}
	return check;
}
bool PressAnyKey() {
	return Engine::sys_Input.IsAnyKeyPressed()
		|| Engine::sys_XInputSystem.controllers[0].IsAnyKeyPressed();
}
bool PressAnyKeyRelease() {
	return Engine::sys_Input.IsAnyKeyRelease()
		|| Engine::sys_XInputSystem.controllers[0].IsAnyKeyRelease();
}
bool PressAnyKeyDown() {
	//std::cout << "BBB" << Engine::sys_Input.IsAnyKeyDown() << "CCC" << Engine::sys_XInputSystem.controllers[0].IsAnyKeyDown() << std::endl;

	return Engine::sys_Input.IsAnyKeyDown()
		|| Engine::sys_XInputSystem.controllers[0].IsAnyKeyDown();
}
bool PressAnyKeyUp() {
	return Engine::sys_Input.IsAnyKeyUp()
		|| Engine::sys_XInputSystem.controllers[0].IsAnyKeyUp();
}

void  ConVibrate(WORD leftVal, WORD rightVal) {
	return Engine::sys_XInputSystem.controllers[0].Vibrate(leftVal, rightVal);
}
bool  ConIsConnected() {
	return Engine::sys_XInputSystem.controllers[0].IsConnected();
}
bool  ConIsStickMoving(int left_or_right_sitck) {
	return Engine::sys_XInputSystem.controllers[0].IsStickMoving(left_or_right_sitck);
}
float ConStickValue(int left_or_right_sitck, int X_0_or_Y_1) {
	return Engine::sys_XInputSystem.controllers[0].StickValue(left_or_right_sitck, X_0_or_Y_1);
}
float ConStickABSValue(int left_or_right_sitck, int X_0_or_Y_1) {
	return Engine::sys_XInputSystem.controllers[0].StickABSValue(left_or_right_sitck, X_0_or_Y_1);
}
bool  ConIsDown(const WORD& key_) {
	return Engine::sys_XInputSystem.controllers[0].IsDown(key_);
}
bool  ConIsUp(const WORD& key_) {
	return Engine::sys_XInputSystem.controllers[0].IsUp(key_);
}
bool  ConIsPressed(const WORD& key_) {
	return Engine::sys_XInputSystem.controllers[0].IsPressed(key_);
}
bool  ConIsRelease(const WORD& key_) {
	return Engine::sys_XInputSystem.controllers[0].IsRelease(key_);
}
int   ConTriggerValue(const WORD& key_) {
	return Engine::sys_XInputSystem.controllers[0].TriggerValue(key_);
}

void SetTimeScale(float timescale_) {
	FrameRateController::timescale = MAX2(timescale_, 0.0f);
}
float GetTimeScale() {
	return FrameRateController::timescale;
}
float GetDt() {
	return FrameRateController::dttimescale;
}
float GetRealDt() {
	return FrameRateController::dt;
}
//float GetGameTime() {
//	return GameStateManager::GameTime;
//}
//float GetSceneTime() {
//	return GameStateManager::SceneTime;
//}

float GetFPS() {
	return Engine::sys_FrameRateController.GetFPS();
}


//AudioListener
//float GetChannelGroupVolume(unsigned channelnumber){
//	return RM::audioListener->GetChannelGroupVolume(channelnumber);
//}
//void SetChannelGroupVolume(unsigned channelnumber, float value) {
//	RM::audioListener->SetChannelGroupVolume(channelnumber, value);
//}
//void SetMute(unsigned channelnumber, bool DoYouWantToMuteMeThisAudioListenerYo) {
//	RM::audioListener->SetMute(channelnumber, DoYouWantToMuteMeThisAudioListenerYo);
//}
//void IsMute(unsigned channelnumber) {
//	RM::audioListener->IsMute(channelnumber);
//}

void DisablePhysicsSystem(bool disable_) {
	UNREFERENCED_PARAMETER(disable_);
	//Engine::sys_Physics.DisableSystem(disable_);
}

void SetLevel1State(int state)
{
  level1State = state;
}

void SetLevel2State(int state)
{
  level2State = state;
}

void SetLevel3State(int state)
{
  level3State = state;
}

void SetDifficulty(int state)
{
  difficulty = state;
}

int GetLevel1State()
{
  return level1State;
}

int GetLevel2State()
{
  return level2State;
}

int GetLevel3State()
{
  return level3State;
}

int GetDifficulty()
{
  return difficulty;
}

//struct DummyObject
//{
//	std::string name = "Dummy";
//	int objNum = 24;
//
//	void shout()
//	{
//		std::cout << "A dummy can't shout!\n";
//	}
//};
//
//void luatest()
//{
//	DummyObject dummy;
//	DummyObject* test = new DummyObject;
//
//	std::cout << "=== opening a state ===" << std::endl;
//
//	sol::state lua;
//	// open some common libraries
//	lua.open_libraries();
//
//	//Executing from string
//	lua.script("print('Hello World!')");
//
//	//Creating your own table
//	//First arg is the name of the table, followed by var name -> value
//	lua.create_named_table("gameobject", "name", "Player");
//	std::string name = lua["gameobject"]["name"];
//
//	//Binding struct/class into script 
//	lua.new_usertype<DummyObject>("DummyObject", "name", &DummyObject::name, "objNum", &DummyObject::objNum, "shout", &DummyObject::shout);	
//	//Passing a ref of a c++ struct/class
//	lua["dummy"] = &dummy;
//	lua["test"] = test;
//	std::cout << "Hi I am " << dummy.name << std::endl;
//	std::cout << "My number is " << dummy.objNum << std::endl;
//
//	std::cout << "Gameobject Name: " << name << std::endl;
//
//	//Executing from file
//	lua.script_file("../all_resources/scripts/variables.lua");
//	
//	//Getting table from file
//	sol::table table = lua["config"];
//	bool fullscreen = table["fullscreen"];
//
//	sol::table resolution = lua["config"]["resolution"];
//	int x = resolution["x"];
//	int y = resolution["y"];
//
//	std::cout << "Fullscreen: " << std::boolalpha << fullscreen << std::endl;
//	std::cout << "Resolution: " << x << "x" << y << std::endl;
//
//	std::string dName = lua["dummy"]["name"];
//	std::string dNum = lua["dummy"]["objNum"];
//	std::cout << "Hi I am " << dName << std::endl;
//	std::cout << "My number is " << dNum << std::endl;
//
//	name = lua["gameobject"]["name"];
//	std::cout << "Gameobject Name: " << name << std::endl;
//
//	std::cout << std::endl;
//}
