// Zrobione dla TIM4 o wartościach (docelowo elastycznie):
// Preskaler: 63
// Period: 999
// Pulse: 500 (50 %)
// HCLKFreq: 64 MHz


#include "motor_lib.h"
#include "leds_effects.h"
#include "stm32f1xx_hal.h"
#include <stdlib.h>

int32_t steps = 0;
const uint32_t epsilon = 30;
char direction = MOTOR_BACK;
const uint16_t speed = 150;

// Motor Init function:
void Motor_Init(Motor_HandleTypeDef *hmotor, TIM_HandleTypeDef *htim, uint32_t channel,
                 GPIO_TypeDef *enablePort, uint16_t enablePin,
                 GPIO_TypeDef *directionPort, uint16_t directionPin, GPIO_TypeDef *buttonPort, uint16_t buttonPin)
{
    hmotor->htim = htim;
    hmotor->channel = channel;

    hmotor->enablePort = enablePort;
    hmotor->enablePin = enablePin;

    hmotor->directionPort = directionPort;
    hmotor->directionPin = directionPin;

    hmotor->buttonPort = buttonPort;
    hmotor->buttonPin = buttonPin;

    hmotor->frequency = 0;
    hmotor->step_angle = 1.8f;
}

// Enable Microstep Driver:
void Motor_Enable(Motor_HandleTypeDef *hmotor, uint8_t enable)
{
    HAL_GPIO_WritePin(hmotor->enablePort, hmotor->enablePin, enable ? GPIO_PIN_RESET : GPIO_PIN_SET);
}

void Motor_Stop(Motor_HandleTypeDef *hmotor)
{
	HAL_TIM_PWM_Stop(hmotor->htim, hmotor->channel);
}

// Select motor direction:
void Motor_DirectionSelect(Motor_HandleTypeDef *hmotor, char dir)
{
	direction = dir;
	hmotor->direction = direction;

    if (dir == MOTOR_BACK) // Go Back
    {
    	Motor_Enable(hmotor, 1);
        HAL_GPIO_WritePin(hmotor->directionPort, hmotor->directionPin, GPIO_PIN_RESET);
        HAL_TIM_PWM_Start(hmotor->htim, hmotor->channel);
    }
    if (dir == MOTOR_AHEAD) // Go Ahead
    {
    	Motor_Enable(hmotor, 1);
        HAL_GPIO_WritePin(hmotor->directionPort, hmotor->directionPin, GPIO_PIN_SET);
        HAL_TIM_PWM_Start(hmotor->htim, hmotor->channel);
    }
    if (dir == 'S')// Stop
    {
    	HAL_TIM_PWM_Stop(hmotor->htim, hmotor->channel);
    	HAL_Delay(1000);
    }
}

// Motor set PWM frequency:
void Motor_SetFreq(Motor_HandleTypeDef *hmotor, uint32_t freq)
{
	uint32_t new_freq = freq/2;
    if (new_freq == 0)
    {
        HAL_TIM_PWM_Stop(hmotor->htim, hmotor->channel);
        return;
    }

    uint32_t clk_freq = HAL_RCC_GetPCLK1Freq(); // Assuming 64 MHz
    uint32_t prescaler = (clk_freq / (new_freq * 1000)) - 1;
    uint32_t period = ((clk_freq / (prescaler + 1)) / new_freq) - 1;

    // Ensure the prescaler and period are within the valid range
    if (prescaler > 0xFFFF)
    {
        prescaler = 0xFFFF;
    }
    if (period > 0xFFFF)
    {
        period = 0xFFFF;
    }

    // Update Timer with new prescaler and period
    __HAL_TIM_SET_PRESCALER(hmotor->htim, prescaler);
    __HAL_TIM_SET_AUTORELOAD(hmotor->htim, period);

    // Set compare value to achieve 50% duty cycle
    __HAL_TIM_SET_COMPARE(hmotor->htim, hmotor->channel, period / 2);

    // Start PWM
    HAL_TIM_PWM_Start(hmotor->htim, hmotor->channel);
}


// Set Speed in RPM
void Motor_SetSpeedRPM(Motor_HandleTypeDef *hmotor, uint32_t rpm)
{
	const uint32_t microstep = 32;
	const uint32_t steps_per_rotation = 200;
	uint32_t steps_per_revolution = steps_per_rotation * microstep;
	uint32_t freq = (rpm * steps_per_revolution)/60;
	Motor_SetFreq(hmotor, freq);
}


// Go to start position
void Motor_Restart(Motor_HandleTypeDef *hmotor)
{
	Set_WS2812_ResetAnimation();
	Motor_Enable(hmotor, 1);
	Motor_DirectionSelect(hmotor, MOTOR_BACK);
	Motor_SetSpeedRPM(hmotor, 50);
	while(HAL_GPIO_ReadPin(hmotor->buttonPort, hmotor->buttonPin) == GPIO_PIN_SET){

	}
	Motor_Stop(hmotor);
	steps = 0;
	Reset_LEDs();
}

// Go to target position:
uint8_t Motor_Go(Motor_HandleTypeDef *hmotor, int32_t target, uint32_t rpm) // Setposition -> zmień
{
	if (HAL_GPIO_ReadPin(hmotor->buttonPort, hmotor->buttonPin) == GPIO_PIN_RESET)
	{
		steps = 0;
	}

	Motor_Enable(hmotor, 1);


	int32_t error = (target - steps);

	if(error > 0)
	{
		Motor_DirectionSelect(hmotor, MOTOR_AHEAD);
	}
	if(error < 0)
	{
		Motor_DirectionSelect(hmotor, MOTOR_BACK);
	}
	if(abs(error) < epsilon)
	{
		HAL_TIM_PWM_Stop(hmotor->htim, hmotor->channel);
		return 1;
	}else{
		Motor_SetSpeedRPM(hmotor, rpm);
		return 0;
	}
}



