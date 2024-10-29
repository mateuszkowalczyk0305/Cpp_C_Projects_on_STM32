/*
 * my_tims_irq.cpp
 *
 *  Created on: Sep 23, 2024
 *      Author: user
 */
#include "my_tims_irq.hpp"

extern DisplayModule* module1;
extern DisplayModule* module2;

// EXTERN YOUR DISPLAYMODULE POINTER SECTION:
extern DisplayModule* module3;

extern std::map<std::string, DisplayModule*> displays_map;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim -> Instance == TIM1)
	{
		module1->motor.dt++;
	}
	if (htim -> Instance == TIM2)
	{
		module2->motor.dt++;
	}
	// YOUR DISPLAY MODULE CALLBACK SECTION:

}
