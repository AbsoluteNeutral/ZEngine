
#include "stdafx.h"
#include "XInputSystem.h"

float XInputSystem::ControllerSensitivity = 180.0f;

XInputSystem::XInputSystem() noexcept
	:numOfControllers(0), controllers(nullptr)
{}

XInputSystem::~XInputSystem() noexcept
{}

bool XInputSystem::Init() 
{
	numOfControllers = NUM_OF_CONTROLLERS;
	//allocate number of controllers
	try {
		controllers = new XboxInput[numOfControllers];
	}
	catch (...) {
		PopUp_Warn("Not enought mem for xboxcontrollers");
		return false;
	}
	return true;
}

unsigned XInputSystem::GetNumOfConllers() const 
{
	return numOfControllers;
}

XboxInput* XInputSystem::GetXboxConHandle()
{
	return controllers;
}

XboxInput* XInputSystem::GetXboxCon(unsigned arrayIndex_) 
{
	return controllers + arrayIndex_;
}

void XInputSystem::StopAllVirbration() 
{
	XboxInput* b = controllers;
	XboxInput* e = b + numOfControllers;
	while (b != e) {
		b->Vibrate(0, 0);
		++b;
	}
}

void XInputSystem::SystemUpdate()
{
	XboxInput* b = controllers;
	XboxInput* e = b + numOfControllers;
	while (b != e) {
		b->Update();
		++b;
	}
}

bool XInputSystem::SystemExit() 
{
	DeleteA(controllers);	//free memory
	numOfControllers = 0;
	return true;
}
