
#include "stdafx.h"
#include "FrameRateController.h"

float FrameRateController::dt = 0.0f;
float FrameRateController::dttimescale = 0.0f;
float FrameRateController::timescale = 1.0f;
float FrameRateController::invFrame = 0.0f;

FrameRateController::FrameRateController() noexcept
	: deltaTimef{ 0.0f }, frameRatef{ 60.0f }, 
	//deltaTime{ 0.0 }, frameRate{ 0.0 },
	start{}, end{}, frameCap{ true }
{
	invFrame = (1.0f / frameRatef);
}

FrameRateController::~FrameRateController() 
{}

void FrameRateController::LockFrameRate(bool lock_framerate_, float frameRate_)
{
	frameCap = lock_framerate_;
	//frameRate = static_cast<double>(frameRate_);
	//deltaTime = 1.0 / frameRate_;
	frameRatef = frameRate_;
	invFrame = 1.0f / frameRatef;
}

void FrameRateController::Start(){
    start = std::chrono::high_resolution_clock::now();
}

void FrameRateController::Stop() {
	std::chrono::duration<float> tmp = std::chrono::high_resolution_clock::now() - start;
	deltaTimef = tmp.count();
	dt = deltaTimef;
    dttimescale = dt * timescale;
}

void FrameRateController::WasteTime(){
	std::chrono::duration<float> tmp = std::chrono::high_resolution_clock::now() - start;
	deltaTimef = tmp.count();

    if (frameCap)
	{
        while (deltaTimef < invFrame)
        {
			tmp = std::chrono::high_resolution_clock::now() - start;
			deltaTimef = tmp.count();
        }
    }
	dt = deltaTimef;
	dttimescale = dt * timescale;
}

bool FrameRateController::DoSomething() {
	std::chrono::duration<float> tmp = std::chrono::high_resolution_clock::now() - start;
	deltaTimef = tmp.count();

	dt = deltaTimef;
	dttimescale = dt * timescale;
	return deltaTimef < invFrame;
}

bool FrameRateController::IsEnd() {
	std::chrono::duration<float> tmp = std::chrono::high_resolution_clock::now() - start;
	return tmp.count() >= invFrame;
}

//void FrameRateController::PrintOnWindow(){
//	std::string toReturn{ "The Legend     FPS: " };
//	toReturn += std::to_string(1.f);
//	toReturn += "   FrameTime: ";
//	toReturn += std::to_string(deltaTimef);
//
//	//typedef std::basic_string<TCHAR> tstring;
//	//tstring z = TEXT("Engine Title: %f", 1.f / elapsed.count());
//	//LPTSTR toRet = new TCHAR[z.size() + 1];
//	//_tcscpy(toRet, z.c_str());
//	//return toRet;
//
//	//std::string op{ "Engine Title: XXXX " + std::to_string(1.f / elapsed.count()) };
//	//std::tstring test { TEXT(std::to_string(1.f / elapsed.count())) };
//	//test += std::to_string(1.f / elapsed.count());
//	//return toReturn;
//	//SetWindowTextA(winHWND, const_cast<char*>(toReturn.c_str()));
//}

void FrameRateController::SetFrameRate(int fr){
	frameRatef = static_cast<float>(fr);
	invFrame = 1.0f / frameRatef;
}

double FrameRateController::getDeltaTime(){
    return deltaTimef;
}

float FrameRateController::GetFPS() const{
    return 1.0f / deltaTimef;
}