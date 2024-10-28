/*
 * game.c
 *
 *  Created on: Jul 10, 2024
 *      Author: user
 */
#include "game.h"
#include "stm32f1xx_it.h"
#include <stdlib.h>
#include <time.h>
#include "motor_lib.h"
#include "leds_effects.h"


State_t currentState = STATE_RESTART;
int32_t target_position = 0; // Init 0 - start position
uint8_t wylosowane;
uint8_t answer;
uint8_t clicked;
uint8_t f_move = 1;
extern int32_t steps;
const uint16_t jump = 1500;
extern volatile uint16_t counterIDLE;

extern Motor_HandleTypeDef hmotorX;
extern Motor_HandleTypeDef hmotorY;


void Player_Init(Player_HandleTypeDef *hplayer, Motor_HandleTypeDef *hmotor, GPIO_TypeDef *button1_Port,
		uint16_t button1_Pin, GPIO_TypeDef *button2_Port, uint16_t button2_Pin, int led_start_num_btn1, int led_start_num_btn2, uint16_t animation_iteration)
{
	hplayer->hmotor = hmotor;
	hplayer->button1_Port = button1_Port;
	hplayer->button1_Pin = button1_Pin;
	hplayer->button2_Port = button2_Port;
	hplayer->button2_Pin = button2_Pin;
	//hplayer->led_start_num_btn1 = led_start_num_btn1;
	//hplayer->led_start_num_btn2 = led_start_num_btn2;
	//hplayer->animation_iteration = animation_iteration;
}

void Led_Start(Player_HandleTypeDef *hplayer)
{
	int toggle_count = 0;

	while(toggle_count < 6)
	{
		HAL_GPIO_TogglePin(hleds->button_led1Port, hleds->button_led1Pin);
		HAL_GPIO_TogglePin(hleds->button_led2Port, hleds->button_led2Pin);
		HAL_Delay(500);

		toggle_count++;
	}
}


int Random_0_or_1(Player_HandleTypeDef *hplayer)
{
    static int initialized = 0;
    if (!initialized)
    {
        srand(time(NULL));
        initialized = 1;
    }
	return rand()%2;
}

void Game_Update(Motor_HandleTypeDef *hMotor, Player_HandleTypeDef *hplayer, int32_t *target_position, const uint16_t jump, uint16_t *speed)
{
	//counter = 0;
    switch(currentState)
    {
    case STATE_RESTART:
    	Reset_LEDs();
        Motor_Restart(hMotor);
        *target_position = 0;
        //counter = 0;
        currentState = STATE_IDLE;
        break;
    case STATE_IDLE:

    	Set_WS2812_IDLEAnimation();

        if ((HAL_GPIO_ReadPin(hplayer->button1_Port, hplayer->button1_Pin) == GPIO_PIN_RESET) ||
            (HAL_GPIO_ReadPin(hplayer->button2_Port, hplayer->button2_Pin) == GPIO_PIN_RESET))
        {
        	counterIDLE = 0;
        	currentState = STATE_START;
        }
        break;
    case STATE_START:
        HAL_Delay(500);
        Set_WS2812_StartAnimation();

        wylosowane = Random_0_or_1(hplayer);
        HAL_Delay(100);
        currentState = STATE_GAME;
    	break;
    case STATE_GAME:
    	if (counterIDLE > 10000)
    	{
    		counterIDLE = 0;
    		currentState = STATE_RESTART; // Go to Restart State if nobody's playing
    	}
        if (f_move == 1 && (Motor_Go(hMotor, *target_position, *speed)) == 1)
        {
            HAL_Delay(100);
            wylosowane = Random_0_or_1(hplayer);
            f_move = 0;
            Reset_LEDs();

            if (wylosowane)
            {
            	Set_WS2812_Button(2, 0, 0, 255);
            }
            else
            {
            	Set_WS2812_Button(1, 0, 0, 255);
            }
        }
        else
        {
           Motor_Enable(hMotor, 1);
        }
        if (HAL_GPIO_ReadPin(hplayer->button1_Port, hplayer->button1_Pin) == GPIO_PIN_RESET && f_move == 0)
        {
        	counterIDLE = 0;
            if (wylosowane == 0)
            {
            	answer = 1;
            	Set_WS2812_Button(1, 0, 255, 0);
            	HAL_Delay(350);
            	Set_Brightness(0);
            	Reset_LEDs();

            }
            else
            {
            	answer = 0;
            	Set_WS2812_Button(1, 255, 0, 0);
            	Set_WS2812_Button(2, 255, 0, 0);
            	HAL_Delay(350);
            	Set_Brightness(0);
            	Reset_LEDs();
            }
            clicked = 1;
        }

        if (HAL_GPIO_ReadPin(hplayer->button2_Port, hplayer->button2_Pin) == GPIO_PIN_RESET && f_move == 0)
        {
        	counterIDLE = 0;
            if (wylosowane == 1)
            {
            	answer = 1;
            	Set_WS2812_Button(2, 0, 255, 0);
            	HAL_Delay(350);
            	Set_Brightness(0);
            	Reset_LEDs();
            }
            else
            {
            	answer = 0;
            	Set_WS2812_Button(1, 255, 0, 0);
            	Set_WS2812_Button(2, 255, 0, 0);
            	HAL_Delay(350);
            	Set_Brightness(0);
            	Reset_LEDs();
            }
            clicked = 1;
        }
        if (clicked)
        {
        	//Reset_LEDs();
            if (answer)
            {
                *target_position += jump;
                f_move = 1;
            }
            else
            {
                *target_position -= 2 * jump;
                f_move = 1;
                if (*target_position <= 0)
                {
                    *target_position = 0;
                }
            }
            clicked = 0;
            currentState = STATE_GAME;
        }

        if (steps >= 149000)
        {
            Motor_Stop(hMotor);
            currentState = STATE_RESTART;
        }
        break;
    case STATE_STOP:
        break;
    }
}
