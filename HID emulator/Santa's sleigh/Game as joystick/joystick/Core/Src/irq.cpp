#include "irq.hpp"
#include "main.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == Limit_SW_Pin)
		{
			LimiterClicked = true;
			systick_counter = 0;
		}
}
