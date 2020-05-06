#ifndef _EDITOR_SIMULATIONS_H
#define _EDITOR_SIMULATIONS_H

enum _EDITOR_SIMULATION_NUM {
	_SIMULATION0 = 0,
	_SIMULATION1,
	_SIMULATION2,
	_SIMULATION3,

	_NUMOF_SIMULATION
};

extern void (*InitSimulation[_NUMOF_SIMULATION])();
extern void (*UpdateSimulation[_NUMOF_SIMULATION])();
extern void (*UpdateImguiSimulation[_NUMOF_SIMULATION])();

//zero run nothing
void Init_Simulation0();
void Update_Simulation0();
void UpdateImgui_Simulation0();


//Simulation1.cpp , so on so forth
void Init_Simulation1();
void Update_Simulation1();
void UpdateImgui_Simulation1();

void Init_Simulation2();
void Update_Simulation2();
void UpdateImgui_Simulation2();

void Init_Simulation3();
void Update_Simulation3();
void UpdateImgui_Simulation3();

#endif
