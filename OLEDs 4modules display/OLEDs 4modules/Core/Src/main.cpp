/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "map"
#include "ssd1306.h"
#include "ssd1306_fonts.h"
#include "bitmaps.h"
#include "oled.hpp"
#include "uart_handler.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
OLED*  display1 = nullptr;
OLED*  display2 = nullptr;
OLED*  display3 = nullptr;
OLED*  display4 = nullptr;

std::map<std::string, OLED*> oleds_map;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_UART_Receive_IT(&huart2, (uint8_t*)&single_byte, sizeof(single_byte));


  display1 = new OLED(&hspi1, CS_1_GPIO_Port, CS_1_Pin, DC_1_GPIO_Port, DC_1_Pin, RES_1_GPIO_Port, RES_1_Pin);
  display2 = new OLED(&hspi1, CS_2_GPIO_Port, CS_2_Pin, DC_2_GPIO_Port, DC_2_Pin, RES_2_GPIO_Port, RES_2_Pin);
  display3 = new OLED(&hspi1, CS_3_GPIO_Port, CS_3_Pin, DC_3_GPIO_Port, DC_3_Pin, RES_3_GPIO_Port, RES_3_Pin);
  display4 = new OLED(&hspi1, CS_4_GPIO_Port, CS_4_Pin, DC_4_GPIO_Port, DC_4_Pin, RES_4_GPIO_Port, RES_4_Pin);


  oleds_map["D1"] = display1;
  oleds_map["D2"] = display2;
  oleds_map["D3"] = display3;
  oleds_map["D4"] = display4;


  display1->init();
  display2->init();
  display3->init();
  display4->init();

  display1->addSegment(go_bitmap, 14, 2, x_default, y_default);
  display1->addSegment(one_bitmap, 14, 2, x_default, y_default);
  display1->addSegment(two_bitmap, 14, 2, x_default, y_default);
  display1->addSegment(three_bitmap, 14, 2, x_default, y_default);
  display1->addSegment(four_bitmap, 14, 2, x_default, y_default);
  display1->addSegment(five_bitmap, 14, 2, x_default, y_default);
  display1->addSegment(ready_bitmap, 14, 2, x_default, y_default);

  display2->addSegment(go_bitmap, 14, 2, x_default, y_default);
  display2->addSegment(one_bitmap, 14, 2, x_default, y_default);
  display2->addSegment(two_bitmap, 14, 2, x_default, y_default);
  display2->addSegment(three_bitmap, 14, 2, x_default, y_default);
  display2->addSegment(four_bitmap, 14, 2, x_default, y_default);
  display2->addSegment(five_bitmap, 14, 2, x_default, y_default);
  display2->addSegment(ready_bitmap, 14, 2, x_default, y_default);

  display3->addSegment(go_bitmap, 14, 2, x_default, y_default);
  display3->addSegment(one_bitmap, 14, 2, x_default, y_default);
  display3->addSegment(two_bitmap, 14, 2, x_default, y_default);
  display3->addSegment(three_bitmap, 14, 2, x_default, y_default);
  display3->addSegment(four_bitmap, 14, 2, x_default, y_default);
  display3->addSegment(five_bitmap, 14, 2, x_default, y_default);
  display3->addSegment(ready_bitmap, 14, 2, x_default, y_default);

  display4->addSegment(go_bitmap, 14, 2, x_default, y_default);
  display4->addSegment(one_bitmap, 14, 2, x_default, y_default);
  display4->addSegment(two_bitmap, 14, 2, x_default, y_default);
  display4->addSegment(three_bitmap, 14, 2, x_default, y_default);
  display4->addSegment(four_bitmap, 14, 2, x_default, y_default);
  display4->addSegment(five_bitmap, 14, 2, x_default, y_default);
  display4->addSegment(ready_bitmap, 14, 2, x_default, y_default);

//  display2->addSegment(ready_bitmap, 14, 2, x_default, y_default);
//  display2->addSegment(go_bitmap, 14, 2, x_default, y_default);
//  display2->addSegment(one_bitmap, 14, 2, x_default, y_default);
//  display2->addSegment(two_bitmap, 14, 2, x_default, y_default);
//  display2->addSegment(three_bitmap, 14, 2, x_default, y_default);
//  display2->addSegment(four_bitmap, 14, 2, x_default, y_default);
//  display2->addSegment(five_bitmap, 14, 2, x_default, y_default);

display1->setSegment(1);
display2->setSegment(2);
display3->setSegment(3);
display4->setSegment(4);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	  uart_CmdHandler();
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 84;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
