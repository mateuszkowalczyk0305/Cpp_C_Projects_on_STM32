/*
 Możesz napisać coś o tym jak ustawiasz GPIO HALLa czy coś
 */


#ifndef INC_DISPLAY_MODULE_HPP_
#define INC_DISPLAY_MODULE_HPP_

//Includes:
#include "main.h"
#include "stepper_motor.hpp"
#include <cmath>
#include <string>

//extern uint8_t is_zero_position;
extern bool ready_for_cmd;

enum CurrentState
{
	State_Reset,
	State_IDLE,
	State_Jump,
	State_Move
};

class DisplayModule
{
public:
	DisplayModule(TIM_HandleTypeDef* htim, uint8_t segments,
			GPIO_TypeDef* _hall_port, uint16_t _hall_pin,
			 GPIO_TypeDef* in1_port, uint16_t in1_pin,
		     GPIO_TypeDef* in2_port, uint16_t in2_pin,
			 GPIO_TypeDef* in3_port, uint16_t in3_pin,
			 GPIO_TypeDef* in4_port, uint16_t in4_pin);

	// State machine:
	void StateMachine();

	// Set/Reset Position:
	bool reset_DisplayModule(Direction direction);
	bool reset_DisplayModule(void);

	// Set target segment:
	bool set_DisplaySegment(uint8_t target_segment);
	bool set_DisplaySegment(uint8_t target_segment, Direction direction);

	// Jump to next/previous segment:
	bool set_NextSegment(void);
	bool set_PreviousSegment(void);

	// Hall detection function:
	void is_hallDetected(void);

	int is_zero_position;

	CurrentState state;
	Direction direction;
	uint8_t segment;
//	char dir;

	uint16_t single_segment_angle;

	StepperMotor motor;

private:

	GPIO_TypeDef* HALL_SENSOR_PORT;
	uint16_t HALL_SENSOR_PIN;
	uint8_t segments;
	bool next_segment_selected;
	double current_segment;
	uint8_t target_segment;
};

#endif
