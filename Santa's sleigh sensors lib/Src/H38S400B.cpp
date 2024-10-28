// Peripheral:
#include <main.h>
#include "i2c.h"
#include "usart.h"
#include "tim.h"
#include "gpio.h"

// Header file:
#include "H38S400B.hpp"
#include <cmath>

H38S400B::H38S400B(TIM_HandleTypeDef* htim, H38S400B_Mode mode, uint32_t period) : htim(htim), mode(mode), period(period)
{
	if(mode == LINEAR) htim->Instance->ARR = pow(2,32) - 1;
	else if(mode == ROTARY) htim->Instance->ARR = period;

	htim->Instance->CNT = 0;

	HAL_TIM_Encoder_Start(htim, TIM_CHANNEL_ALL);
}

void H38S400B::resetEncoder()
{
	htim->Instance->CNT = 0;
}

// Linear position:
double H38S400B::getLinearPosition(DistanceUnits d_unit)
{
	if(mode == LINEAR)
	{
	// Getting current timer parameters:
	uint32_t encoder_imp = htim->Instance->CNT;
	int encoder_imp_int = static_cast<int>(encoder_imp);

	double pi = 3.14159;
	double shaft_diameter = 12;

	// Calculate linear position:
	double district = pi * shaft_diameter;
	double rotations = static_cast<double>(encoder_imp_int)/ static_cast<double>(period);
	double linear_position = district * rotations;

	if (d_unit == MILIMETERS) return linear_position;
	if (d_unit == CENTIMETERS) return linear_position / 10.0;
	} else return 0;
}

// Rotary position:
double H38S400B::getRotaryPosition(RotaryUnits r_unit)
{
	double deg_per_rotate = 360;
	double rad_per_rotate = 2.0 * M_PI;

	if(mode == ROTARY)
	{
	uint32_t encoder_imp = htim->Instance->CNT;
	uint32_t imp_per_rotate = htim->Instance-> ARR;


	int encoder_imp_int = static_cast<int>(encoder_imp);
	int imp_per_rotate_int = static_cast<int>(imp_per_rotate);

	double rotations = static_cast<double>(encoder_imp_int) / imp_per_rotate_int;

	switch ( r_unit )
	{
	case ROTATIONS:
		return rotations;
		break;
	case DEGREES:
		return rotations * deg_per_rotate;
		break;
	case RADIANS:
		return rotations * rad_per_rotate;
		break;
	}}else return 0;
}














