/*
 * irq.hpp
 *
 *  Created on: Oct 4, 2024
 *      Author: user
 */

#ifndef INC_IRQ_HPP_
#define INC_IRQ_HPP_

#include "stm32f4xx_hal.h"
#include "main.h"

extern bool LimiterClicked;
extern volatile uint32_t systick_counter;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin);


#endif /* INC_IRQ_HPP_ */
