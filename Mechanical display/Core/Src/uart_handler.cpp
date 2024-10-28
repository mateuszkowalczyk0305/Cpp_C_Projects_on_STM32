// Includes:
#include "uart_handler.hpp"

// Global buffer:
char data_package[20];
char single_byte;
uint8_t data_index = 0;
bool is_parsing = false;
DataPackage data_pack;
extern std::map<std::string, DisplayModule*> displays_map;

// UART data handling function:
void uart_CmdHandler()
{
	std::string command = data_pack.command_info;
	std::string display_data = data_pack.display_info;

	// All displays handling commands:
	if (display_data[1] == 'A' || display_data[1] == 'a')
	{
		// State Reset:
		if (command == "RESET" || command == "reset")
		{
			for (auto& display : displays_map)
			{
				if (display.second->state == State_IDLE) display.second->state = State_Reset;
			}
			data_pack.command_info = "";
		}
		// State Move:
		if (command == "MOVE" || command == "move")
		{
			std::string first_parameter;
			std::string second_parameter;

			size_t comma_position = data_pack.parameter_info.find(',');

			first_parameter = data_pack.parameter_info.substr(comma_position - 1, 1);
			second_parameter = data_pack.parameter_info.substr(comma_position + 1);

			for (auto& display : displays_map)
			{
				if (display.second->state == State_IDLE)
				{
					display.second->segment = static_cast<uint8_t>(std::stoi(first_parameter));

					if (second_parameter[0] == '+') display.second->direction = Direction_ClockWise;
					if (second_parameter[0] == '-') display.second->direction = Direction_CounterClockWise;

					display.second->state = State_Move;
				}
			}
			data_pack.command_info = "";
		}
		// State Jump:
		if (command == "JUMP" || command == "jump")
		{
			for (auto& display : displays_map)
			{
				if (display.second->state == State_IDLE)
				{
					if (data_pack.parameter_info[0] == '+') display.second->direction = Direction_ClockWise;
					if (data_pack.parameter_info[0] == '-') display.second->direction = Direction_CounterClockWise;
					display.second->state = State_Jump;
				}
			}
			data_pack.command_info = "";
		}
	}
	else // For single display handling command:
	{
		// State Reset:
		if (command == "RESET" || command == "reset")
		{
			for(auto& display : displays_map)
			{
				if (display.first == display_data && display.second->state == State_IDLE)
				{
					display.second->state = State_Reset;
				}
			}
			data_pack.command_info = "";
		}
		// State Move:
		if (command == "MOVE" || command == "move")
		{
			std::string first_parameter;
			std::string second_parameter;

			size_t comma_position = data_pack.parameter_info.find(',');

			first_parameter = data_pack.parameter_info.substr(comma_position - 1, 1);
			second_parameter = data_pack.parameter_info.substr(comma_position + 1);

			for (auto& display : displays_map)
			{
				if (display.first == display_data && display.second->state == State_IDLE)
				{
					display.second->segment = static_cast<uint8_t>(std::stoi(first_parameter));
					if (second_parameter[0] == '+') display.second->direction = Direction_ClockWise;
					if (second_parameter[0] == '-') display.second->direction = Direction_CounterClockWise;
					display.second->state = State_Move;
				}
			}
			data_pack.command_info = "";
		}
		// State Jump:
		if (command == "JUMP" || command == "jump")
		{
			if (data_pack.parameter_info[0] == '+')
			{
				for (auto& display : displays_map)
				{
					if (display.first == display_data && display.second->state == State_IDLE)
					{
						display.second->direction = Direction_ClockWise;
					}
				}
			}
			if (data_pack.parameter_info[0] == '-')
			{
				for (auto& display : displays_map)
				{
					if (display.first == display_data && display.second->state == State_IDLE)
					{
						display.second->direction = Direction_CounterClockWise;
					}
				}
			}

			for (auto& display : displays_map)
			{
				if (display.first == display_data && display.second->state == State_IDLE)
				{
					display.second->state = State_Jump;
				}
			}

			data_pack.command_info = "";
		}
	}
}


// UART Callback handling:
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1)
    {
    	// Increment data_index in every single byte receiving callback:
    	data_package[data_index++] = single_byte;

    	// Receive single bytes until single_byte = '\n':
    	if (single_byte == '\n' || data_index >= sizeof(data_package))
    	{
    		// Start data parsing:
    		is_parsing = true;

    		std::string data(data_package);

    		// Delete "\r\n":
    		data.erase(std::remove(data.begin(), data.end(), '\r'), data.end());
    		data.erase(std::remove(data.begin(), data.end(), '\n'), data.end());

    		// Find colons:
    		size_t colon1_position = data.find(':');
			size_t colon2_position = data.find(':' , colon1_position + 1);
			size_t position_R = data.find('R');
			size_t position_r = data.find('r');

			// Add information to DataPackage structure:
			if (colon2_position == std::string::npos && (position_R != std::string::npos || position_r != std::string::npos))
			{
				data_pack.parameter_info = "";
				data_pack.command_info = "RESET";
				data_pack.display_info = data.substr(0, colon1_position);
			}
			else
			{

				data_pack.display_info = data.substr(0, colon1_position);
				data_pack.command_info = data.substr(colon1_position + 1, colon2_position - colon1_position - 1);
				data_pack.parameter_info = data.substr(colon2_position + 1);
			}

    		// Finish data parsing:
    		is_parsing = false;

    		data_index = 0;
    		data_package[20] = {0};
    	}

        // Init next data receiving if parsing finished:
        if (!is_parsing) HAL_UART_Receive_IT(&huart1, (uint8_t*)&single_byte, sizeof(single_byte));
    }
}




