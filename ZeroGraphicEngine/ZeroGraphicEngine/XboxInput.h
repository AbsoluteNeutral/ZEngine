
#ifndef _XBOXINPUT_H_
#define _XBOXINPUT_H_

#include <Xinput.h>
#pragma comment(lib, "XInput.lib")

//this class controls the functionlity of 1 xbox controller
class ZG_API XboxInput
{
public:
	XboxInput() noexcept;
	~XboxInput();

	XINPUT_GAMEPAD* GetGamepad();
	XINPUT_STATE* GetState();

	void Update			();
	void Vibrate		(WORD leftVal = 0, WORD rightVal = 0, WORD limit = 65535);
						
	bool IsConnected	() const;
	bool IsStickMoving	(int left_or_right_sitck) const;
	float StickValue	(int left_or_right_sitck, int X_0_or_Y_1) const;
	float StickABSValue	(int left_or_right_sitck, int X_0_or_Y_1) const;	//get abs value of stick instead

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
	int id;						//id of xbox
	bool isConnect;				//check if is connected to PC

	XINPUT_STATE currState;		//track current state of key
	XINPUT_STATE prevState;		//track prev state of key
	XINPUT_VIBRATION vibration;	//vibration staff

	SHORT leftDeadZone;			//the value/radius of LEFT THUMB stick to start moving
	SHORT rightDeadZone;		//the value/radius of RIGHT THUMB stick to start moving
	BYTE triggerDeadZone;		//the value		   of TRIGGER to start moving

	//able to track how far the stick have been tilt
	float LeftStickX;
	float LeftStickY;
	float RightStickX;
	float RightStickY;

	//trigger itself is like a thumb stick, able to track how hard or soft the trigger it is been pressed
	BYTE  LeftTrigger;
	BYTE  RightTrigger;
	BYTE  prevLeftTrigger;		//prevState of trigger, this varible is used to track state like a button, instead of like thumb stick
	BYTE  prevRightTrigger;		//prevState of trigger, this varible is used to track state like a button, instead of like thumb stick

	float prevLeftStickX;		//prevstate, ditto of trigger
	float prevLeftStickY;
	float prevRightStickX;
	float prevRightStickY;
	
};

#endif //_XBOXINPUT_H_
