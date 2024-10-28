/*
 * motor_lib.h
 *
 *  Created on: Jul 3, 2024
 *      Author: user
 */

#ifndef INC_MOTOR_LIB_H_
#define INC_MOTOR_LIB_H_

#include <stdint.h>
#include "stm32f1xx_hal.h"

#define MOTOR_AHEAD 0
#define MOTOR_BACK 1

// Variables definition:
extern int32_t steps;
extern const uint32_t epsilon;
extern const uint16_t speed;

// Motor Structure definition:
typedef struct {
    TIM_HandleTypeDef *htim;
    uint32_t channel;

    GPIO_TypeDef *enablePort;
    uint16_t enablePin;

    GPIO_TypeDef *directionPort;
    uint16_t directionPin;

    GPIO_TypeDef *buttonPort;
    uint16_t buttonPin;

    uint32_t frequency;
    float step_angle;
    char direction;

    int steps;
} Motor_HandleTypeDef;

// Functions prototypes:
void Motor_Init(Motor_HandleTypeDef *hmotor, TIM_HandleTypeDef *htim, uint32_t channel,
                GPIO_TypeDef *enablePort, uint16_t enablePin,
                GPIO_TypeDef *directionPort, uint16_t directionPin,
                GPIO_TypeDef *buttonPort, uint16_t buttonPin);
void Motor_Enable(Motor_HandleTypeDef *hmotor, uint8_t enable);
void Motor_Stop(Motor_HandleTypeDef *hmotor);
void Motor_DirectionSelect(Motor_HandleTypeDef *hmotor, char dir);
void Motor_SetFreq(Motor_HandleTypeDef *hmotor, uint32_t freq);
void Motor_SetSpeedRPM(Motor_HandleTypeDef *hmotor, uint32_t rpm);
void Motor_Restart(Motor_HandleTypeDef *hmotor);
uint8_t Motor_Go(Motor_HandleTypeDef *hmotor, int32_t target, uint32_t rpm);
#endif /* INC_MOTOR_LIB_H_ */
