/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define MOT1_1_Pin GPIO_PIN_12
#define MOT1_1_GPIO_Port GPIOB
#define MOT1_2_Pin GPIO_PIN_13
#define MOT1_2_GPIO_Port GPIOB
#define MOT1_3_Pin GPIO_PIN_14
#define MOT1_3_GPIO_Port GPIOB
#define MOT1_4_Pin GPIO_PIN_15
#define MOT1_4_GPIO_Port GPIOB
#define HALL1_Pin GPIO_PIN_8
#define HALL1_GPIO_Port GPIOA
#define MOT2_1_Pin GPIO_PIN_9
#define MOT2_1_GPIO_Port GPIOA
#define MOT2_2_Pin GPIO_PIN_10
#define MOT2_2_GPIO_Port GPIOA
#define MOT2_3_Pin GPIO_PIN_11
#define MOT2_3_GPIO_Port GPIOA
#define MOT2_4_Pin GPIO_PIN_12
#define MOT2_4_GPIO_Port GPIOA
#define HALL2_Pin GPIO_PIN_15
#define HALL2_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
