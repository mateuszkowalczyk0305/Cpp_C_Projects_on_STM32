#ifdef __USART_H__
#include "display_handler.hpp"

DisplayHandler::DisplayHandler(UART_HandleTypeDef *_uart, uint32_t _timeout)
: uart(_uart), timeout(_timeout)
{}

bool DisplayHandler::send_Command(std::string command)
{
	// Create full command:
	std::string new_line_char = "\r\n";
	std::string target_command = command + new_line_char;

	// Data transmision:
	HAL_StatusTypeDef transmit = HAL_UART_Transmit(uart, (uint8_t*)target_command.c_str(), target_command.length(), timeout);

	// Check the correctness of UART transmission:
	if (transmit == HAL_ERROR || transmit == HAL_TIMEOUT || transmit == HAL_BUSY){
		return false;
	}
	else{
		return true;
	}
}

bool DisplayHandler::set_DisplayModuleSegment(uint8_t target_module, uint8_t target_segment, Direction direction)
{
	// Number to string conversion:
	std::string display_num = std::to_string(target_module);
	std::string segment_num = std::to_string(target_segment);

	// Creating full command:
	std::string display_part = "D";
	std::string command_part = ":MOVE:";

	std::string command;

	// Direction check:
	if (direction == Direction_Incrementally){
		command = display_part + display_num + command_part + segment_num + ",+";
	}
	else{
		command = display_part + display_num + command_part + segment_num + ",-";
	}

	return send_Command(command);
}

bool DisplayHandler::set_AllDisplaysModulesSegment(uint8_t target_segment, Direction direction)
{
	// Number to string conversion:
	std::string segment_num = std::to_string(target_segment);

	// Creating full command:
	std::string display_part = "DA";
	std::string command_part = ":MOVE:";

	std::string command;

	// Direction check:
	if (direction == Direction_Incrementally){
		command = display_part + command_part + segment_num + ",+";
	}
	else{
		command = display_part + command_part + segment_num + ",-";
	}
	return send_Command(command);
}

bool DisplayHandler::jump_DisplayModule(uint8_t target_module, Direction direction)
{
	// Number to string conversion:
	std::string display_num = std::to_string(target_module);

	// Creating full command:
	std::string display_part = "D";
	std::string command_part = ":JUMP:";

	std::string command;

	// Direction check:
	if (direction == Direction_Incrementally){
		command = display_part + display_num + command_part + "+";
	}
	else{
		command = display_part + display_num + command_part + "-";
	}
	return send_Command(command);
}

bool DisplayHandler::jump_AllDisplayModule(Direction direction)
{
	// Creating full command:
	std::string display_part = "DA";
	std::string command_part = ":JUMP:";

	std::string command;

	// Direction check:
	if (direction == Direction_Incrementally){
		command = display_part + command_part + "+";
	}
	else{
		command = display_part + command_part + "-";
	}
	return send_Command(command);
}

bool DisplayHandler::reset_DisplayModule(uint8_t target_module)
{
	// Number to string conversion:
	std::string display_num = std::to_string(target_module);

	// Creating full command:
	std::string display_part = "D";
	std::string command_part = ":RESET";

	std::string command = display_part + display_num + command_part;

	return send_Command(command);
}

bool DisplayHandler::reset_AllDisplayModules(void)
{
	std::string command = "DA:RESET";
	return send_Command(command);
}
#endif
