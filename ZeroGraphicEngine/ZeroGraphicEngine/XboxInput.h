
#ifndef _XBOXINPUT_H_
#define _XBOXINPUT_H_

#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

class ZG_API XboxInput
{
public:
	XboxInput() noexcept;
	~XboxInput();

	XINPUT_GAMEPAD* GetGamepad();
	XINPUT_STATE* GetState();

	void Update();
	void Vibrate(WORD leftVal = 0, WORD rightVal = 0, WORD limit = 65535);

	bool IsConnected() const;
	bool IsStickMoving(int left_or_right_sitck) const;
	float StickValue(int left_or_right_sitck, int X_0_or_Y_1) const;
	float StickABSValue(int left_or_right_sitck, int X_0_or_Y_1) const;

	bool IsDown			(const WORD& key_) const;
	bool IsUp			(const WORD& key_) const;
	bool IsPressed		(const WORD& key_) const;
	bool IsRelease		(const WORD& key_) const;
	bool IsAnyKeyPressed() const;
	bool IsAnyKeyRelease() const;
	bool IsAnyKeyDown	() const;
	bool IsAnyKeyUp		() const;
	int TriggerValue	(const WORD& key_) const;
#ifdef USING_SOL2 
	static void BindLua(sol::state& lua);
#endif

	static int NUM_CONTROLLERS;

private:
	int id;
	bool isConnect;

	XINPUT_STATE currState;
	XINPUT_STATE prevState;
	XINPUT_VIBRATION vibration;

	SHORT leftDeadZone;
	SHORT rightDeadZone;
	BYTE triggerDeadZone;

	float LeftStickX;
	float LeftStickY;
	float RightStickX;
	float RightStickY;
	BYTE  LeftTrigger;
	BYTE  RightTrigger;

	float prevLeftStickX;
	float prevLeftStickY;
	float prevRightStickX;
	float prevRightStickY;
	BYTE prevLeftTrigger;
	BYTE prevRightTrigger;
};

#endif //_XBOXINPUT_H_
