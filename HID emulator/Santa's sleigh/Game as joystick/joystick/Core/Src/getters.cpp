/*
 * getters.cpp
 *
 *  Created on: Oct 4, 2024
 *      Author: user
 */

#include "getters.hpp"

volatile uint32_t systick_counter = 0;

uint32_t raw_X = 0;
uint32_t raw_Y = 0;

uint8_t adc_X = 0;
uint8_t adc_Y = 0;

int8_t X_value = 0;
int8_t Y_value = 0;

bool LimiterClicked = false;


double Get_X(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};
	sConfig.Channel = ADC_CHANNEL_1;  // PA1 -> ADC_IN1
	sConfig.Rank = 1;
	sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
	 HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	HAL_ADC_Start(&hadc1);

	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
		raw_X = HAL_ADC_GetValue(&hadc1);
		adc_X = (uint8_t)((raw_Y * 255) / 4096);
		X_value = (int8_t)(adc_Y - 128);
	}

	double x_db = static_cast<double>(X_value);
	double x_final = (x_db * 90) / 128;


	return x_final;
}

double Get_Y(void)
{
    ADC_ChannelConfTypeDef sConfig = {0};
    sConfig.Channel = ADC_CHANNEL_2;  // PA2 -> ADC_IN2
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_56CYCLES;
    HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	HAL_ADC_Start(&hadc1);

	if (HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
		raw_Y = HAL_ADC_GetValue(&hadc1);
		adc_Y = (uint8_t)((raw_X * 255) / 4096);
		Y_value = (int8_t)(adc_X - 128);
	}

	double y_db = static_cast<double>(Y_value);
	double y_final = -((y_db * 90) / 128);
	return y_final + 90;
}

bool Get_Limiter(void)
{
	if (LimiterClicked == true && systick_counter >= 50)
	{
		LimiterClicked = false;
	}
	return LimiterClicked;
}

