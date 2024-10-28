#include "leds_effects.h"
#include <stdlib.h>
#include "math.h"



/*
 *  Diody na cele sprawdzenia były zmienone. Aktualna konfiguracja TIM1 jest pod WS2811. Na WS2812 zmień period = 99, pwmData = 56,28.
 */
#define USE_BRIGHTNESS 0

int flag_data_sent = 0;
uint16_t LED_Data[MAX_LED][4];
uint16_t LED_Mod[MAX_LED][4];  // only if you use brightness
uint16_t animation_iteration;

extern TIM_HandleTypeDef htim1;
extern volatile uint16_t counterAnimation;
extern uint8_t f_move;

// Callback - after data sending:
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance == TIM1)
	{
		HAL_TIM_PWM_Stop_DMA(&htim1, TIM_CHANNEL_1);
		flag_data_sent=1;
	}
}

// Led colors settings:
void Set_LED (int LEDnum, int Red, int Green, int Blue)
{
	LED_Data[LEDnum][0] = LEDnum;
	LED_Data[LEDnum][1] = Green;
	LED_Data[LEDnum][2] = Red;
	LED_Data[LEDnum][3] = Blue;
}

// (optional) Set Brightness [0 - 0%, 45 - 100%]:
#define PI 3.14159265

void Set_Brightness (int brightness)  // 0-45
{
#if USE_BRIGHTNESS

	if (brightness > 45) brightness = 45;
	for (int i=0; i<MAX_LED; i++)
	{
		LED_Mod[i][0] = LED_Data[i][0];
		for (int j=1; j<4; j++)
		{
			float angle = 90-brightness;  // in degrees
			angle = angle*PI / 180;  // in rad
			LED_Mod[i][j] = (LED_Data[i][j])/(tan(angle));
		}
	}

#endif

}

uint16_t pwmData[(24*MAX_LED)+50];

// Data sending:
void WS2812_Send (void)
{
	uint32_t indx = 0;
	uint32_t color;


	for (int i= 0; i<MAX_LED; i++)
	{
#if USE_BRIGHTNESS
		color = ((LED_Mod[i][1]<<16) | (LED_Mod[i][2]<<8) | (LED_Mod[i][3]));
#else
		color = ((LED_Data[i][1]<<16) | (LED_Data[i][2]<<8) | (LED_Data[i][3]));
#endif

		for (int i=23; i>=0; i--)
		{
			if (color&(1<<i))
			{
				pwmData[indx] = 76;  //56% //2/3 of 90 (period) //56
			}

			else pwmData[indx] = 32;  //28% (period = 100) //1/3 of 90 (period) //28

			indx++;
		}

	}

	for (int i=0; i<50; i++)
	{
		pwmData[indx] = 0;
		indx++;
	}

	HAL_TIM_PWM_Start_DMA(&htim1, TIM_CHANNEL_1, (uint32_t *)pwmData, indx);
	while (!flag_data_sent){};
	flag_data_sent = 0;
}

// LED reseting:
void Reset_LEDs (void)
{
Set_WS2812_Button(1, 0, 0, 0);
Set_WS2812_Button(2, 0, 0, 0);
}

// Set WS2812:
void Set_WS2812_Button(uint8_t button_num, int Red, int Green, int Blue)
{
	for(int i = 0; i <16; i++)
	{
		Set_Brightness(15);
		int lednum = 16*(button_num - 1)+i;
		Set_LED(lednum, Red, Green, Blue);
	}
	WS2812_Send();

}

// Start Animation:
void Set_WS2812_StartAnimation(void)
{
	for(int i =0; i <= 32; i++)
    {
		Set_LED(i, 50, 50, 50);
		WS2812_Send();
		HAL_Delay(50);
	}
}

// IDLE Animation:
void Set_WS2812_IDLEAnimation(void)
{
	if (counterAnimation > 200)
	{
		counterAnimation = 0;

		//Player animation
		animation_iteration++;

		if(animation_iteration > 31)
		{
			animation_iteration = 0;
			Reset_LEDs();
		}

		Set_LED(animation_iteration, 252, 102, 3);
		WS2812_Send();
	}
}

void Set_WS2812_ResetAnimation(void)
{
	for(int i = 0; i<32; i++)
	{
		Set_LED(i, 255, 0, 0);
	}
	WS2812_Send();
}
