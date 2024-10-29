
#ifndef INC_UART_HANDLER_HPP_
#define INC_UART_HANDLER_HPP_

// Includes:
#include "display_module.hpp"
#include "main.h"
#include <map>
#include <string>
#include <algorithm>
#include <cstdlib>

// Raw data information structure:
struct DataPackage
{
	std::string display_info;
	std::string command_info;
	std::string parameter_info;
};

// UART 1 (115200 BaudRate):
extern UART_HandleTypeDef huart1;

// Buffer variables:
extern char data_package[20];
extern char single_byte;
extern bool is_parsing;
extern bool ready_for_cmd;


// UART Callback function definition:
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart);

// UART data handling function:
void uart_CmdHandler(void);

#endif /* INC_UART_HANDLER_HPP_ */
