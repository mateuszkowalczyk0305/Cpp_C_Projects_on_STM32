#include "display_module.hpp"

DisplayModule::DisplayModule(TIM_HandleTypeDef* htim, uint8_t segments,
		GPIO_TypeDef* _hall_port, uint16_t _hall_pin,
		GPIO_TypeDef* in1_port, uint16_t in1_pin,
        GPIO_TypeDef* in2_port, uint16_t in2_pin,
        GPIO_TypeDef* in3_port, uint16_t in3_pin,
        GPIO_TypeDef* in4_port, uint16_t in4_pin)
: motor(htim, in1_port, in1_pin, in2_port, in2_pin, in3_port, in3_pin, in4_port, in4_pin),
  segment(0),											// Target segment position,
  direction(Direction_ClockWise),						// Direction of rotation
  segments(segments),									// Max number of display segments
  single_segment_angle(DEG_PER_REVOLUTION / segments),	// Single segment angle size
  next_segment_selected(false),							// Jumping flag
  state(State_Reset),									// Default starting state
  is_zero_position(0),									// Zero position
  HALL_SENSOR_PIN(_hall_pin),							// Sensor pin
  HALL_SENSOR_PORT(_hall_port)							// Sensor port
{

}

// State Machine of Display:
void DisplayModule::StateMachine()
{
	switch (state)
	{
	case State_Reset:
		// Reset:
		reset_DisplayModule();
		// If the reset has completed, go to IDLE state:
		if (reset_DisplayModule()) state = State_IDLE;
		break;
	case State_Jump:
		if (direction == Direction_CounterClockWise)
		{
			// Set next segment in clockwise direction:
			set_PreviousSegment();
			// If operation has completed, go to IDLE state:
			if (set_PreviousSegment()) state = State_IDLE;
		}
		if (direction == Direction_ClockWise)
		{
			// Set previous segment in clockwise direction:
			set_NextSegment();
			// If operation has completed, go to IDLE state:
			if (set_NextSegment()) state = State_IDLE;
		}
		break;
	case State_Move:
		// Set selected segment:
		set_DisplaySegment(segment);
		// Set next segment in clockwise direction:
		if (set_DisplaySegment(segment)) state = State_IDLE;
		break;
	case State_IDLE:
		// Clear all flags there and wait for other commands:
		next_segment_selected = false;
		is_zero_position = 0;
		break;
	default:
		break;
	};
}

// Reset Display Module:
bool DisplayModule::reset_DisplayModule(void)
{
	// if hall_sensor ends detecting magnet:
	if (is_zero_position == 1)
	{
		// Reset all types of display's motor positions:
		motor.position = 0;
		motor.position_deg = 0;
		motor.position_deg_int = 0;

		// Stop motor after detection finish:
		motor.motorStop();
		return true;
	}
	else
	{
		// Move display's motor forward:
		motor.motorGo(Direction_ClockWise);
		return false;
	}
}

// Set target segment of Display module in direction that you want to move:
bool DisplayModule::set_DisplaySegment(uint8_t target_segment)
{
	// Check that target is in range:
	if (target_segment >= segments) target_segment = 0;
	// Calculate target position angle:
	uint32_t target_position_deg = target_segment * single_segment_angle;
	// Set calculated position:
	return motor.setPositionDeg(target_position_deg, direction);
}

// Set next segment of display module:
bool DisplayModule::set_NextSegment(void)
{
	// Increment target_segment only one time [when flag is false]
	if (!next_segment_selected)
	{
		// Calculate current segment:
		current_segment = std::floor(motor.position_deg / static_cast<double>(single_segment_angle));
		// Increment segment:
		++current_segment;
		// Check that target is in range:
		if (current_segment >= segments) current_segment = 0;

		// Set target segment:
		target_segment = static_cast<uint8_t>(current_segment);
		set_DisplaySegment(target_segment);

		// Set selection flag:
		next_segment_selected = true;
	}

	// Check bool condition:
	if (set_DisplaySegment(target_segment) == true)
	{
		return true;
		target_segment = 0;
		current_segment = 0;
	}
	else
	{
		return false;
	}
}

// Set previous segment of display module:
bool DisplayModule::set_PreviousSegment(void)
{
	// Decrement target_segment only one time [when flag is false]
	if (!next_segment_selected)
	{
		// Calculate current segment:
		current_segment = std::floor(motor.position_deg / static_cast<double>(single_segment_angle));
		// Decrement segment:
		--current_segment;
		// Check that target is in range:
		if (current_segment < 0) current_segment = static_cast<double>(segments) - 1;

		target_segment = static_cast<uint8_t>(current_segment);

		set_DisplaySegment(target_segment);

		// Set selection flag:
		next_segment_selected = true;
	}
	// Check bool condition:
	if (set_DisplaySegment(target_segment) == true)
	{
		return true;
		target_segment = 0;
		current_segment = 0;
	}
	else
	{
		return false;
	}
}

void DisplayModule::is_hallDetected(void)
{
	// If sensor detects magnet:
	if (HAL_GPIO_ReadPin(HALL_SENSOR_PORT, HALL_SENSOR_PIN) == GPIO_PIN_RESET)
	{
		is_zero_position = 1;
	}
	else
	{
		is_zero_position = 0;
	}
}





