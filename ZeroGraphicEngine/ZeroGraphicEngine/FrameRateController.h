 /*****************************************************************************/
/*!
\file			FrameRateController.h
\brief
*/
/*****************************************************************************/
///////////////////////////////////////////////////////////////////////////////
#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H
///////////////////////////////////////////////////////////////////////////////
#include <ctime>
#include <chrono>
#include <string>
#include <windows.h>

#pragma warning(disable : 4616 4251) //dllexport

class ZG_API FrameRateController
{
	bool frameCap;          // Frame cap on or off
	float deltaTimef;
	float frameRatef;
	//double deltaTime;		// length of each frame
	//double frameRate;		// targeted frame rate
    
	std::chrono::high_resolution_clock::time_point start;                // start and end time of each frame
	std::chrono::high_resolution_clock::time_point end;                // start and end time of each frame
    //bool fpsToggle = false;
public:
	FrameRateController() noexcept;
	~FrameRateController();

	void LockFrameRate(bool lock_framerate_, float frameRate_ = 60.0f);	// Initialization of FR Controller (should be in game loop)
    void Start();					// Start counting frame
    void Stop();					// Stop counting frame

    void WasteTime();				// Set endT and check for frametime
	bool DoSomething();
	bool IsEnd();

	//void PrintOnWindow();			// Print frame rate and delta time on Window Bar
    void SetFrameRate(int fr);		// Set Frame Rate (slider IMGUI)

    double getDeltaTime();			// Returns time taken for that frame (should be sent to Engine.Update(here)
    float GetFPS() const;

	static float dt;
	static float fixeddt;
	static float dttimescale;
	static float timescale;
	static float invFrame;
};



//class ZG_API Timer
//{
//    double deltaTime;
//    std::chrono::high_resolution_clock::time_point startSys, endSys;
//    std::chrono::duration<double> elapsed;  // endSys-startSys
//    std::string toReturn;
//public:
//    Timer(const char* systemName)
//        : deltaTime{ 0.0 }, startSys{}, endSys{}, elapsed{},
//          toReturn{systemName}
//    {
//        startSys = std::chrono::high_resolution_clock::now();
//    }
//
//    void StopTimer()
//    {
//        endSys = std::chrono::high_resolution_clock::now();
//    }
//
//    std::string getString()
//    {
//        endSys = std::chrono::high_resolution_clock::now();
//        elapsed = endSys - startSys;
//        deltaTime = elapsed.count();
//
//        toReturn += "'s Frametime: ";
//        toReturn += std::to_string(deltaTime);
//        toReturn += "     ";
//        toReturn += std::to_string((deltaTime) / 60.0 * 100.0);
//        toReturn += "% of 60FPS frametime";
//        return toReturn;
//    }
//
//    ~Timer()
//    {}
//};


#endif //FRAMERATECONTROLLER_H
