/* H38S400B .ioc file conifg:
 *
 * 1. Timer in encoder mode,
 * 2. Encoder mode -> encoder mode T1 and T2,
 * 3. Declare your encoder resolution as period,
 * 4. Other functions - default
 *
*/

#ifndef H38S400B_H
#define H38S400B_H

#include <main.h>

enum H38S400B_Mode{
	ROTARY,
	LINEAR
};

enum DistanceUnits{
	CENTIMETERS,
	MILIMETERS
};

enum RotaryUnits{
	ROTATIONS,
	DEGREES,
	RADIANS
};

class H38S400B {
public:
	H38S400B(TIM_HandleTypeDef* htim, H38S400B_Mode mode, uint32_t period);

	// Start/Restart functions:
	void resetEncoder();

	// Linear mode:
	double getLinearPosition(DistanceUnits d_unit);

	// Rotary mode:
	double getRotaryPosition(RotaryUnits r_unit);


private:
    TIM_HandleTypeDef* htim;
    H38S400B_Mode mode;
	uint32_t period;
};

#endif

