#include "stdafx.h"
#include "Simulation.h"

void(*InitSimulation[_NUMOF_SIMULATION])() {
	Init_Simulation1
	, Init_Simulation2
	, Init_Simulation3
};

void(*UpdateSimulation[_NUMOF_SIMULATION])() {
	Update_Simulation1
	, Update_Simulation2
	, Update_Simulation3
};

void(*UpdateImguiSimulation[_NUMOF_SIMULATION])() {
	UpdateImgui_Simulation1
	, UpdateImgui_Simulation2
	, UpdateImgui_Simulation3
};
