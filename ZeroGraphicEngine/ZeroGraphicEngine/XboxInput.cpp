
#include "stdafx.h"
#include "XboxInput.h"

int XboxInput::NUM_CONTROLLERS = 0;

XboxInput::XboxInput() noexcept
	:id(NUM_CONTROLLERS++)
	, isConnect(false)
	, currState{}, prevState{}, vibration{}
	, leftDeadZone{ XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE }
	, rightDeadZone{ XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE }
	, triggerDeadZone{ XINPUT_GAMEPAD_TRIGGER_THRESHOLD }

	, LeftStickX(0.0f)
	, LeftStickY(0.0f)
	, RightStickX(0.0f)
	, RightStickY(0.0f)

	, LeftTrigger(0)
	, RightTrigger(0)
	, prevLeftTrigger(0)
	, prevRightTrigger(0)

	, prevLeftStickX(0.0f)
	, prevLeftStickY(0.0f)
	, prevRightStickX(0.0f)
	, prevRightStickY(0.0f)
{}

XboxInput::~XboxInput()
{
	Vibrate(0, 0);
}

XINPUT_GAMEPAD* XboxInput::GetGamepad()
{
	return &currState.Gamepad;
}

XINPUT_STATE* XboxInput::GetState() 
{
	return &currState;
}

bool XboxInput::IsConnected() const
{
	return isConnect;
}

void XboxInput::Vibrate(WORD leftVal, WORD rightVal, WORD limit){
	if (!isConnect) return;
	vibration.wLeftMotorSpeed  = leftVal  > limit ? 65535 : leftVal;
	vibration.wRightMotorSpeed = rightVal > limit ? 65535 : rightVal;

	XInputSetState(id, &vibration);

	//vibration.wLeftMotorSpeed = 0;
	//vibration.wRightMotorSpeed = 0;
	//XInputSetState(id, &vibration);
}

void XboxInput::Update() 
{
	//update prev state first
	prevState		 = currState   ;
	prevLeftStickX   = LeftStickX  ;
	prevLeftStickY   = LeftStickY  ;
	prevRightStickX  = RightStickX ;
	prevRightStickY  = RightStickY ;
	prevLeftTrigger  = LeftTrigger ;
	prevRightTrigger = RightTrigger;

	//update current state
	isConnect = XInputGetState(id, &currState) == ERROR_SUCCESS ? true : false;
	if (!isConnect) return;

	if (currState.Gamepad.sThumbLX < 0) {
		LeftStickX = fmaxf(-1.0f, (currState.Gamepad.sThumbLX < -leftDeadZone ? currState.Gamepad.sThumbLX : 0.0f) / 0x00007FFF);
	}
	else {
		LeftStickX = fmaxf(-1.0f, (currState.Gamepad.sThumbLX > leftDeadZone ? currState.Gamepad.sThumbLX : 0.0f) / 0x00007FFF);
	}

	if (currState.Gamepad.sThumbLY < 0) {
		LeftStickY = fmaxf(-1.0f, (currState.Gamepad.sThumbLY < -leftDeadZone ? currState.Gamepad.sThumbLY : 0.0f) / 0x00007FFF);
	}
	else {
		LeftStickY = fmaxf(-1.0f, (currState.Gamepad.sThumbLY > leftDeadZone ? currState.Gamepad.sThumbLY : 0.0f) / 0x00007FFF);
	}

	if (currState.Gamepad.sThumbRX < 0) {
		RightStickX = fmaxf(-1.0f, (currState.Gamepad.sThumbRX < -rightDeadZone ? currState.Gamepad.sThumbRX : 0.0f) / 0x00007FFF);
	}
	else {
		RightStickX = fmaxf(-1.0f, (currState.Gamepad.sThumbRX > rightDeadZone ? currState.Gamepad.sThumbRX : 0.0f) / 0x00007FFF);
	}

	if (currState.Gamepad.sThumbRY < 0) {
		RightStickY = fmaxf(-1.0f, (currState.Gamepad.sThumbRY < -rightDeadZone ? currState.Gamepad.sThumbRY : 0.0f) / 0x00007FFF);
	}
	else {
		RightStickY = fmaxf(-1.0f, (currState.Gamepad.sThumbRY > rightDeadZone ? currState.Gamepad.sThumbRY : 0.0f) / 0x00007FFF);
	}

	//float dX = 1.0f / (1.0f - deadzoneX);
	//float dY = 1.0f / (1.0f - deadzoneY);
	//float normLX = fmaxf(-1.0f, (float)currState.Gamepad.sThumbLX / 0x00007FFF);
	//float normLY = fmaxf(-1.0f, (float)currState.Gamepad.sThumbLY / 0x00007FFF);
	//
	//LeftStickX = (abs(normLX) < deadzoneX ? 0.0f : (abs(normLX) - deadzoneX) * (normLX / abs(normLX)));
	//LeftStickY = (abs(normLY) < deadzoneY ? 0.0f : (abs(normLY) - deadzoneY) * (normLY / abs(normLY)));
	//
	//float normRX = fmaxf(-1.0f, (float)currState.Gamepad.sThumbRX / 0x00007FFF);
	//float normRY = fmaxf(-1.0f, (float)currState.Gamepad.sThumbRY / 0x00007FFF);
	//
	//RightStickX = (abs(normRX) < deadzoneX ? 0 : (abs(normRX) - deadzoneX) * (normRX / abs(normRX)));
	//RightStickY = (abs(normRY) < deadzoneY ? 0 : (abs(normRY) - deadzoneY) * (normRY / abs(normRY)));

	LeftTrigger    = currState.Gamepad.bLeftTrigger  <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? 0 : currState.Gamepad.bLeftTrigger;
	RightTrigger   = currState.Gamepad.bRightTrigger <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD ? 0 : currState.Gamepad.bRightTrigger;
}

bool XboxInput::IsStickMoving(int left_or_right_sitck) const
{
	switch (left_or_right_sitck) {
	case GAME_LSTICK:
		return LeftStickX != 0.0f || LeftStickY != 0.0f;
	case GAME_RSTICK:
		return RightStickX != 0.0f || RightStickY != 0.0f;
	}
	return false;
}

float XboxInput::StickValue(int left_or_right_sitck, int X_0_or_Y_1) const
{
	switch (left_or_right_sitck) {
	case GAME_LSTICK:
		switch (X_0_or_Y_1) {
		case 0:
			return LeftStickX;
		case 1:
			return LeftStickY;
		}
		break;
	case GAME_RSTICK:
		switch (X_0_or_Y_1) {
		case 0:
			return RightStickX;
		case 1:
			return RightStickY;
		}
		break;
	}
	return 0.0f;
}

float XboxInput::StickABSValue(int left_or_right_sitck, int X_0_or_Y_1) const 
{
	switch (left_or_right_sitck) {
	case GAME_LSTICK:
		switch (X_0_or_Y_1) {
		case 0:
			return LeftStickX < 0.0f ? floorf(LeftStickX) : ceilf(LeftStickX);
		case 1:
			return LeftStickY < 0.0f ? floorf(LeftStickY) : ceilf(LeftStickY);
		}
		break;
	case GAME_RSTICK:
		switch (X_0_or_Y_1) {
		case 0:
			return RightStickX < 0.0f ? floorf(RightStickX) : ceilf(RightStickX);
		case 1:
			return RightStickY < 0.0f ? floorf(RightStickY) : ceilf(RightStickY);
		}
		break;
	}
	return 0.0f;
}

bool XboxInput::IsDown(const WORD& key_) const 
{
	switch (key_) {
	case GAME_LEFT_TRIGGER:
		return (prevLeftTrigger != 0 && LeftTrigger != 0);
	case GAME_RIGHT_TRIGGER:
		return (prevRightTrigger != 0 && RightTrigger != 0);

	//case GAME_LSTICK_PRESS:
	//	return ((prevState.dwPacketNumber & key_) != 0 && (currState.Gamepad.wButtons  & key_) != 0);
		
	case GAME_LSTICK_UP:
		return LeftStickY > 0.0f;
	case GAME_LSTICK_DOWN:
		return LeftStickY < 0.0f;
	case GAME_LSTICK_LEFT:
		return LeftStickX < 0.0f;
	case GAME_LSTICK_RIGHT:
		return LeftStickX > 0.0f;

	case GAME_RSTICK_UP:
		return RightStickY > 0.0f;
	case GAME_RSTICK_DOWN:
		return RightStickY < 0.0f;
	case GAME_RSTICK_LEFT:
		return RightStickX < 0.0f;
	case GAME_RSTICK_RIGHT:
		return RightStickX > 0.0f;

	default:
		return (currState.Gamepad.wButtons & key_) != 0;
	}
}

bool XboxInput::IsUp(const WORD& key_) const 
{
	switch (key_) {
	case GAME_LEFT_TRIGGER:
		return (prevLeftTrigger == 0 && LeftTrigger == 0);
	case GAME_RIGHT_TRIGGER:
		return (prevRightTrigger == 0 && RightTrigger == 0);

	case GAME_LSTICK_UP:
		return LeftStickY == 0.0f;
	case GAME_LSTICK_DOWN:
		return LeftStickY == 0.0f;
	case GAME_LSTICK_LEFT:
		return LeftStickX == 0.0f;
	case GAME_LSTICK_RIGHT:
		return LeftStickX == 0.0f;

	case GAME_RSTICK_UP:
		return RightStickY == 0.0f;
	case GAME_RSTICK_DOWN:
		return RightStickY == 0.0f;
	case GAME_RSTICK_LEFT:
		return RightStickX == 0.0f;
	case GAME_RSTICK_RIGHT:
		return RightStickX == 0.0f;
	default:
		return (currState.Gamepad.wButtons & key_) == 0;
	}
}

bool XboxInput::IsPressed(const WORD& key_) const 
{
	switch (key_) {
	case GAME_LEFT_TRIGGER:
		return (prevLeftTrigger == 0 && LeftTrigger != 0);
	case GAME_RIGHT_TRIGGER:
		return (prevRightTrigger == 0 && RightTrigger != 0);

	case GAME_LSTICK_UP:
		return (prevLeftStickY == 0.0f && LeftStickY > 0.0f);
	case GAME_LSTICK_DOWN:
		return (prevLeftStickY == 0.0f && LeftStickY < 0.0f);
	case GAME_LSTICK_LEFT:
		return (prevLeftStickX == 0.0f && LeftStickX < 0.0f);
	case GAME_LSTICK_RIGHT:
		return (prevLeftStickX == 0.0f && LeftStickX > 0.0f);

	case GAME_RSTICK_UP:
		return (prevRightStickY == 0.0f && RightStickY > 0.0f);
	case GAME_RSTICK_DOWN:
		return (prevRightStickY == 0.0f && RightStickY < 0.0f);
	case GAME_RSTICK_LEFT:
		return (prevRightStickX == 0.0f && RightStickX < 0.0f);
	case GAME_RSTICK_RIGHT:
		return (prevRightStickX == 0.0f && RightStickX > 0.0f);
	default:
		return ((prevState.Gamepad.wButtons & key_) == 0 && (currState.Gamepad.wButtons & key_) != 0);
	}
}
bool XboxInput::IsAnyKeyPressed() const
{
	return (prevState.Gamepad.wButtons == 0 && currState.Gamepad.wButtons != 0)
		|| (prevLeftTrigger == 0 && LeftTrigger != 0)
		|| (prevRightTrigger == 0 && RightTrigger != 0)
		;
}
bool XboxInput::IsAnyKeyRelease() const 
{
	return (prevState.Gamepad.wButtons != 0 && currState.Gamepad.wButtons == 0)
		|| (prevLeftTrigger != 0 && LeftTrigger == 0)
		|| (prevRightTrigger != 0 && RightTrigger == 0)
		;
}
bool XboxInput::IsAnyKeyDown() const 
{
	return (prevState.Gamepad.wButtons != 0 && currState.Gamepad.wButtons != 0)
		|| (prevLeftTrigger != 0 && LeftTrigger != 0)
		|| (prevRightTrigger != 0 && RightTrigger != 0)
		;
}
bool XboxInput::IsAnyKeyUp() const 
{
	return (prevState.Gamepad.wButtons == 0 && currState.Gamepad.wButtons == 0)
		|| (prevLeftTrigger == 0 && LeftTrigger == 0)
		|| (prevRightTrigger == 0 && RightTrigger == 0)
		;
}

bool XboxInput::IsRelease(const WORD& key_) const 
{
	switch (key_) {
	case GAME_LEFT_TRIGGER:
		return (prevLeftTrigger != 0 && LeftTrigger == 0);
	case GAME_RIGHT_TRIGGER:
		return (prevRightTrigger != 0 && RightTrigger == 0);

	case GAME_LSTICK_UP:
		return (prevLeftStickY != 0.0f && LeftStickY == 0.0f);
	case GAME_LSTICK_DOWN:
		return (prevLeftStickY != 0.0f && LeftStickY == 0.0f);
	case GAME_LSTICK_LEFT:
		return (prevLeftStickX != 0.0f && LeftStickX == 0.0f);
	case GAME_LSTICK_RIGHT:
		return (prevLeftStickX != 0.0f && LeftStickX == 0.0f);

	case GAME_RSTICK_UP:
		return (prevRightStickY != 0.0f && RightStickY == 0.0f);
	case GAME_RSTICK_DOWN:
		return (prevRightStickY != 0.0f && RightStickY == 0.0f);
	case GAME_RSTICK_LEFT:
		return (prevRightStickX != 0.0f && RightStickX == 0.0f);
	case GAME_RSTICK_RIGHT:
		return (prevRightStickX != 0.0f && RightStickX == 0.0f);
	default:
		return ((prevState.Gamepad.wButtons & key_) != 0 && (currState.Gamepad.wButtons & key_) == 0);
	}
}

int XboxInput::TriggerValue(const WORD& key_) const 
{
	switch (key_) {
	case GAME_LEFT_TRIGGER:
		return LeftTrigger;
	case GAME_RIGHT_TRIGGER:
		return RightTrigger;
	default:
		return 0;
	}
}

#ifdef USING_SOL2 
void XboxInput::BindLua(sol::state& lua) {
	lua.new_usertype<XboxInput>("XboxInput",
		sol::constructors<XboxInput()>(),

		"Vibrate", &XboxInput::Vibrate,
		"IsConnected", &XboxInput::IsConnected,
		"IsStickMoving", &XboxInput::IsStickMoving,
		"StickValue", &XboxInput::StickValue,
		"StickABSValue", &XboxInput::StickABSValue,
		"IsDown", &XboxInput::IsDown,
		"IsUp", &XboxInput::IsUp,
		"IsPressed", &XboxInput::IsPressed,
		"IsRelease", &XboxInput::IsRelease,
		"IsAnyKeyPressed", &XboxInput::IsAnyKeyPressed,
		"IsAnyKeyRelease", &XboxInput::IsAnyKeyRelease,
		"IsAnyKeyDown", &XboxInput::IsAnyKeyDown,
		"IsAnyKeyUp", &XboxInput::IsAnyKeyUp,

		"TriggerValue", &XboxInput::TriggerValue
		);
}

#endif //USING_SOL2
