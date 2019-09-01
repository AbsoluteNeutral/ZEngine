#ifndef _EDITOR_SIMULATIONS_H
#define _EDITOR_SIMULATIONS_H

enum _EDITOR_SIMULATION_NUM {
	_SIMULATION1 = 0,
	_SIMULATION2,
	_SIMULATION3,

	_NUMOF_SIMULATION
};

extern void (*InitSimulation[_NUMOF_SIMULATION])();
extern void (*UpdateSimulation[_NUMOF_SIMULATION])();
extern void (*UpdateImguiSimulation[_NUMOF_SIMULATION])();

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
