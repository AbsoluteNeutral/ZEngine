
#ifndef _ZG_IENGINE_H
#define _ZG_IENGINE_H

#include <Windows.h>
#include "System.h"
#include "Camera.h"
#include "XboxInput.h"

ZG_API extern Camera* MainCam;
//
//ZG_API extern float gameTime;
//
//
//Engine
#if defined(_EDITOR_ON)
ZG_API void EngineInitialize(HINSTANCE hInstanceH, WndProc function_, const char* windowname_ = "ZeroGraphicEngine");
#else
ZG_API void EngineInitialize(HINSTANCE hInstanceH, const char* windowname_ = "ZeroGraphicEngine");
#endif
ZG_API void EngineSetFrameRate(bool lock_frameRate_ = true, float frameRate_ = 60.0f);
ZG_API bool EngineIsRunning();
ZG_API void EngineUpdate();
ZG_API void EngineLateUpdate();
ZG_API void EngineUnForcedClose();
ZG_API bool IsEngineForcedClose();
ZG_API void EngineQuit();
ZG_API void EngineSaveSettings();
ZG_API void EngineExit();

ZG_API float GetFrameRate();
ZG_API void BindColliderAndRigidBody();
ZG_API void BindDrawComponent();

ZG_API void SetEngineActive(bool active_);
ZG_API bool IsEngineActive();

//getters
ZG_API float GetControllerSensitivity();
ZG_API float GetMouseSensitivity();
//setters
ZG_API void SetControllerSensitivity(float value_);
ZG_API void SetMouseSensitivity(float value_);

//Display
ZG_API bool IsFullScreen();
ZG_API void FullScreen(bool fullScreen_);
ZG_API int  ScreenWidth();
ZG_API int  ScreenHeight();
ZG_API int  ScreenResolutionWidth();
ZG_API int  ScreenResolutionHeight();
ZG_API void SetResolution(RESOLUTION_SIZE size_);

//graphic;
ZG_API void WindowBackGroundColor(const zg::Color& color_);

//camera
ZG_API Camera* GetDefaultCamera();
ZG_API void SetDefaultCamera();
ZG_API bool IsDefaultCamera();
ZG_API void SetCAMERA(Camera* current_camera);

//input
//ZG_API XboxInput* SetNumberOfXboxControllers(unsigned num);
ZG_API XboxInput* GetController(unsigned num);

ZG_API zg::Vector2 GetMouseDelta();
ZG_API zg::Vector2 GetPrevMouseScreenPos();
ZG_API zg::Vector2 GetMouseScreenPos();
ZG_API long  GetMouseScreenPosX();
ZG_API long  GetMouseScreenPosY();
ZG_API float GetMouseWheelDelta();
ZG_API zg::Vector2 GetRawAxis();
ZG_API void SetCursorPosition(float x, float y);
ZG_API void LockCursor(bool toggle_);

#ifdef USING_SOL2
ZG_API sol::state_view GetLuaState();
#endif

ZG_API zg::Vector3 ConvertToWorldCoor(const zg::Vector2& mouse_);
ZG_API zg::Vector3 MouseToWorld();

ZG_API void ShowMouseCursor	(bool toggle_);
ZG_API bool IsMouseMoving	();
ZG_API bool KeyDown			(unsigned key);
ZG_API bool KeyUp			(unsigned key);
ZG_API bool KeyPressed		(unsigned key);
ZG_API bool KeyRelease		(unsigned key);
ZG_API bool KeyAliasDown		(const std::string& keyaliasname_);
ZG_API bool KeyAliasUp		(const std::string& keyaliasname_);
ZG_API bool KeyAliasPressed	(const std::string& keyaliasname_);
ZG_API bool KeyAliasRelease	(const std::string& keyaliasname_);

ZG_API bool PressAnyKey		 ();
ZG_API bool PressAnyKeyRelease();
ZG_API bool PressAnyKeyDown	 ();
ZG_API bool PressAnyKeyUp	 ();

ZG_API void  ConVibrate			(WORD leftVal, WORD rightVal);
ZG_API bool  ConIsConnected		();
ZG_API bool  ConIsStickMoving	(int left_or_right_sitck);
ZG_API float ConStickValue		(int left_or_right_sitck, int X_0_or_Y_1);
ZG_API float ConStickABSValue	(int left_or_right_sitck, int X_0_or_Y_1);
ZG_API bool  ConIsDown			(const WORD& key_);
ZG_API bool  ConIsUp				(const WORD& key_);
ZG_API bool  ConIsPressed		(const WORD& key_);
ZG_API bool  ConIsRelease		(const WORD& key_);
ZG_API int   ConTriggerValue		(const WORD& key_);

//FrameRate
ZG_API void SetTimeScale(float timescale_);
ZG_API float GetTimeScale();
ZG_API float GetDt();
ZG_API float GetRealDt();
ZG_API float GetGameTime();
ZG_API float GetSceneTime();
ZG_API float GetFPS();

//AudioListener
ZG_API float GetChannelGroupVolume(unsigned channelnumber);
ZG_API void SetChannelGroupVolume(unsigned channelnumber, float value);
ZG_API void SetMute(unsigned channelnumber, bool DoYouWantToMuteMeThisAudioListenerYo);
ZG_API void IsMute(unsigned channelnumber);
//
//
//ZG_API int GetLevelUnlock();
//ZG_API void SetLevelUnlock(int level_);
//ZG_API void DisablePhysicsSystem(bool disable_);
//
//
//ZG_API void SetLevel1State(int state);
//ZG_API void SetLevel2State(int state);
//ZG_API void SetLevel3State(int state);
//ZG_API void SetDifficulty(int state);
//
//ZG_API int GetLevel1State();
//ZG_API int GetLevel2State();
//ZG_API int GetLevel3State();
//ZG_API int GetDifficulty();

#endif // _I_ENGINE_H
