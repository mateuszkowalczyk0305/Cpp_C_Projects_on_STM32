/*
 * my_tims_irq.hpp
 *
 *  Created on: Sep 23, 2024
 *      Author: user
 */

#ifndef INC_MY_TIMS_IRQ_HPP_
#define INC_MY_TIMS_IRQ_HPP_

#include "main.h"
#include "stepper_motor.hpp"
#include "display_module.hpp"
#include "tim.h"
#include <map>

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);



#endif /* INC_MY_TIMS_IRQ_HPP_ */
