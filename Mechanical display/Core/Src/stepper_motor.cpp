#include "stepper_motor.hpp"

StepperMotor::StepperMotor(TIM_HandleTypeDef* htim,
		GPIO_TypeDef* in1_port, uint16_t in1_pin,
        GPIO_TypeDef* in2_port, uint16_t in2_pin,
        GPIO_TypeDef* in3_port, uint16_t in3_pin,
        GPIO_TypeDef* in4_port, uint16_t in4_pin)
: htim (htim),
  IN1_PORT(in1_port), IN1_PIN(in1_pin),
  IN2_PORT(in2_port), IN2_PIN(in2_pin),
  IN3_PORT(in3_port), IN3_PIN(in3_pin),
  IN4_PORT(in4_port), IN4_PIN(in4_pin),
  dt(0),
  position(0),
  position_deg(0),
  position_deg_int(0),
  step(0)
{
	// Reset ALL Pins before using motor:
	HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);
}

bool StepperMotor::setPosition(uint32_t target_steps, Direction direction)
{
	// Check that motor is on target_position (steps unit):
	if (position == static_cast<int>(target_steps))
	{
		motorStop();
		return true;
	}
	else
	{
		motorGo(direction);
		return false;
	}

}

bool StepperMotor::setPositionDeg(uint32_t target_position_deg, Direction direction)
{
	int target_position = static_cast<int>(target_position_deg);

	// Check that motor is on target_position (deg unit):
	if (position_deg_int == target_position)
	{
		motorStop();
		return true;
	}
	else
	{
		motorGo(direction);
		return false;
	}
}

void StepperMotor::motorGo(Direction direction)
{
	// Start motor timer and set correct motor pin:
	HAL_TIM_Base_Start_IT(htim);
	setCorrectStep(direction);
}

void StepperMotor::motorStop(void)
{
	// Stop motor timer:
	HAL_TIM_Base_Stop_IT(htim);
}

uint8_t StepperMotor::setCorrectStep(Direction direction)
{
	// Pin from 0-4 selector:
	int selector = selectPin(direction);

	// Set correct pin:
	switch (selector)
	{
	case 0:
		HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);

		break;
	case 1:
		HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);

		break;
	case 2:
		HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_SET);
		HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_RESET);

		break;
	case 3:
		HAL_GPIO_WritePin(IN1_PORT, IN1_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN2_PORT, IN2_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN3_PORT, IN3_PIN, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(IN4_PORT, IN4_PIN, GPIO_PIN_SET);

		break;
	};

	return selector;
}

int StepperMotor::selectPin(Direction direction)
{
	// Increment or decrement steps:
	if(dt >= dt_limiter)
	{
		// Set dt flag to 0;
		dt = 0;

		// Direction check condition:
		if (direction == Direction_ClockWise)
		{
			position++;
			step++;
			if (step >= 4) step = 0;
			if(position >= 2048) position = 0;
		}
		else
		{
			position--;
			step--;
			if(step < 0) step = 3;
			if(position < 0) position = 2047;
		}
	}

	// Calculate position_deg:
	calculatePositionDeg();

	return step;
}

void StepperMotor::calculatePositionDeg(void)
{
	position_deg = (static_cast<double>(position)) / STEPS_PER_REVOLUTION * DEG_PER_REVOLUTION;
	position_deg_int = static_cast<int>(position_deg);
}

//#endif
