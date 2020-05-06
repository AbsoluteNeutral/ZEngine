
#ifndef XINPUT_SYSTEM_H_
#define XINPUT_SYSTEM_H_

#include "System.h"
#include "XboxInput.h"

#define NUM_OF_CONTROLLERS 1

//this class controls the number of xbox controllers exist
class ZG_API XInputSystem : public System
{
public:
	XInputSystem			() noexcept;
	virtual ~XInputSystem	() noexcept;

	bool Init					();
	unsigned GetNumOfConllers	() const;
	XboxInput* GetXboxCon		(unsigned arrayIndex_);
	XboxInput* GetXboxConHandle	();
	
	void StopAllVirbration		();
	virtual void SystemUpdate	();
	virtual bool SystemExit		();

	unsigned numOfControllers;
	XboxInput* controllers;
	static float ControllerSensitivity;
private:
	
};

#endif //XINPUT_SYSTEM_H_
