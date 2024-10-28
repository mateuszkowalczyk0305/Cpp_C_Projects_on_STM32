#ifndef __LEDS_EFFECTS_H__
#define __LEDS_EFFECTS_H__

#include "stm32f1xx_hal.h"

// Definitions:
#define MAX_LED 32
#define USE_BRIGHTNESS 0
#define PI 3.14159265

// Variables definition:
extern int flag_data_sent;
extern uint16_t LED_Data[MAX_LED][4];
extern uint16_t LED_Mod[MAX_LED][4];  // tylko jeśli używasz jasności
extern uint16_t animation_iteration;
extern uint16_t pwmData[(24 * MAX_LED) + 50];

// Functions prototypes:
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim);
void Set_LED(int LEDnum, int Red, int Green, int Blue);
void Set_Brightness(int brightness);  // 0-45
void WS2812_Send(void);
void Reset_LEDs(void);
void Set_WS2812_Button(uint8_t button_num, int Red, int Green, int Blue);
void Set_WS2812_StartAnimation(void);
void Set_WS2812_IDLEAnimation(void);
void Set_WS2812_ResetAnimation(void);
#endif /* __LEDS_EFFECTS_H__ */
