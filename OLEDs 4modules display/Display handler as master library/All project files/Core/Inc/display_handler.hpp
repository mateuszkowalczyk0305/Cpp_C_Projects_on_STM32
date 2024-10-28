/*
Library to communicate via UART with Mechanical Display.

DisplayHandler methods:
	1. Set target segment [target module/ all modules],
	2. Jump to next/previous segment [target module/ all modules],
	3. Reset target segment [target module/ all modules].
 */
#ifdef __USART_H__

#ifndef INC_DISPLAY_HANDLER_HPP_
#define INC_DISPLAY_HANDLER_HPP_

#include "main.h"
#include <string>

extern UART_HandleTypeDef huart1;

enum Direction{
	Direction_Incrementally,
	Direction_Decrementally
};

class DisplayHandler{
public:
	DisplayHandler(UART_HandleTypeDef *_uart, uint32_t _timeout);

	// Reset target display module:
	bool reset_DisplayModule(uint8_t target_module);

	// Reset all display modules:
	bool reset_AllDisplayModules(void);

	// Set target segment on target display module:
	bool set_DisplayModuleSegment(uint8_t target_module, uint8_t target_segment, Direction direction);

	// Set target segment on all display modules:
	bool set_AllDisplaysModulesSegment(uint8_t target_segment, Direction direction);

	// Jump target module to next/previous display segment:
	bool jump_DisplayModule(uint8_t target_module, Direction direction);

	// Jump all modules to next/previous display segment:
	bool jump_AllDisplayModule(Direction direction);

private:

	// Your Command sending function with new line characters:
	bool send_Command(std::string command);

	UART_HandleTypeDef* uart;
	uint32_t timeout;
};

#endif /* INC_DISPLAY_HANDLER_HPP_ */
#endif
