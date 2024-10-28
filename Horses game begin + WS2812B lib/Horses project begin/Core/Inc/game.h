/*
 * game.h
 *
 *  Created on: Jul 10, 2024
 *      Author: user
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_


#include <stdint.h>
#include "motor_lib.h"
#include "stm32f1xx_hal.h"

typedef struct {
    Motor_HandleTypeDef *hmotor;

	GPIO_TypeDef *button1_Port;
	uint16_t button1_Pin;

	GPIO_TypeDef *button2_Port;
	uint16_t button2_Pin;

	//int led_start_num_btn1;

	//int led_start_num_btn2;

	//uint16_t animation_iteration;

} Player_HandleTypeDef;


// State machine enum:
typedef enum State{
	  STATE_RESTART ,
	  STATE_IDLE ,
	  STATE_START ,
	  STATE_GAME ,
	  STATE_STOP
} State_t;

extern State_t currentState;
extern int32_t target_position;
extern uint8_t wylosowane;
extern uint8_t answer;
extern uint8_t clicked;
extern uint8_t f_move;
extern volatile uint16_t counterIDLE;



//void PushButtons_Init(Player_HandleTypeDef *hplayer, GPIO_TypeDef *button_led1Port, uint16_t button_led1Pin, GPIO_TypeDef *button_led2Port, uint16_t button_led2Pin,
//		GPIO_TypeDef *push_button1Port, uint16_t push_button1Pin, GPIO_TypeDef *push_button2Port, uint16_t push_button2Pin);
//void Led_Start(Player_HandleTypeDef *hplayer);
//int Random_0_or_1(Player_HandleTypeDef *hplayer);
//void Game_Update(Motor_HandleTypeDef *hMotor, Player_HandleTypeDef *hplayer, int32_t *target_position, const uint16_t jump, uint16_t *speed);
//#endif /* INC_GAME_H_ */
