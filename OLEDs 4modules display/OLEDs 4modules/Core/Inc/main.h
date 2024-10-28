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
#define CS_4_Pin GPIO_PIN_1
#define CS_4_GPIO_Port GPIOB
#define DC_4_Pin GPIO_PIN_2
#define DC_4_GPIO_Port GPIOB
#define RES_4_Pin GPIO_PIN_10
#define RES_4_GPIO_Port GPIOB
#define RES_2_Pin GPIO_PIN_12
#define RES_2_GPIO_Port GPIOB
#define DC_2_Pin GPIO_PIN_13
#define DC_2_GPIO_Port GPIOB
#define CS_2_Pin GPIO_PIN_14
#define CS_2_GPIO_Port GPIOB
#define CS_1_Pin GPIO_PIN_15
#define CS_1_GPIO_Port GPIOB
#define DC_1_Pin GPIO_PIN_8
#define DC_1_GPIO_Port GPIOA
#define RES_1_Pin GPIO_PIN_9
#define RES_1_GPIO_Port GPIOA
#define RES_3_Pin GPIO_PIN_10
#define RES_3_GPIO_Port GPIOA
#define DC_3_Pin GPIO_PIN_11
#define DC_3_GPIO_Port GPIOA
#define CS_3_Pin GPIO_PIN_12
#define CS_3_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
