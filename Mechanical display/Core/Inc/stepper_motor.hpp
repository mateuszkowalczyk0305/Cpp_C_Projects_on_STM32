#ifndef INC_STEPPER_MOTOR_HPP_
#define INC_STEPPER_MOTOR_HPP_

// Includes:
#include "main.h"
#include "stm32f4xx_it.h"

// Full steps define
#define STEPS_PER_REVOLUTION 2048
#define DEG_PER_REVOLUTION 360

// Rotate direction:
enum Direction
{
	Direction_ClockWise,
	Direction_CounterClockWise
};

// Single stepper motor class:
class StepperMotor
{
public:
	StepperMotor (TIM_HandleTypeDef* htim,
			GPIO_TypeDef* in1_port, uint16_t in1_pin,
            GPIO_TypeDef* in2_port, uint16_t in2_pin,
            GPIO_TypeDef* in3_port, uint16_t in3_pin,
            GPIO_TypeDef* in4_port, uint16_t in4_pin);

	// Set position [degrees/steps]:
	bool setPosition(uint32_t target_steps, Direction direction);
	bool setPositionDeg(uint32_t target_position_deg, Direction direction);

	// Start/Stop motor rotation (Counter Clockwise/Clockwise):
	void motorGo(Direction direction);
	void motorStop(void);

	// Motor stepping timer variable:
	int dt;

	// Position describing variables:
	int position;
	double position_deg;
	int position_deg_int;

	// step counter:
	int step;

private:
	// Set correct Pin in step sequence:
	uint8_t setCorrectStep(Direction direction);

	// Select which pin you should turn ON in step sequence:
	int selectPin(Direction direction);

	// Calculate current position (deg unit):
	void calculatePositionDeg(void);

	// Max speed limiter:
	int32_t dt_limiter = 230;
	TIM_HandleTypeDef* htim;

	GPIO_TypeDef* IN1_PORT;
	uint16_t IN1_PIN;

	GPIO_TypeDef* IN2_PORT;
	uint16_t IN2_PIN;

	GPIO_TypeDef* IN3_PORT;
	uint16_t IN3_PIN;

	GPIO_TypeDef* IN4_PORT;
	uint16_t IN4_PIN;
};

#endif /* INC_STEPPER_DISPLAY_HPP_ */


