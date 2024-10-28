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
#include "adc.h"
#include "i2c.h"
#include "tim.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "cmath"
#include "getters.hpp"
#include "usbd_custom_hid_if.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// Ranges enums:
enum X_Ranges
{
	X_Range_Q,
	X_Range_W,
	X_Range_E,
	X_Range_R,
	X_Range_S,
	X_Range_T,
	X_Range_Y,
	X_Range_U,
	X_Range_I,
	X_Range_O,
	X_Range_None
};

enum Y_Ranges
{
	Y_Range_A,
	Y_Range_S,
	Y_Range_D,
	Y_Range_F,
	Y_Range_None
};


struct KeyboardState
{
	X_Ranges previousRangeX; // Previous range for X Axis
	Y_Ranges previousRangeY; // Previous range for Y Axis
};

double X_val = 0;
double Y_val = 0;
bool Limiter = false;


KeyboardState keyboard_state = {X_Range_None, Y_Range_None};

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
X_Ranges getRange_X(double value);
Y_Ranges getRange_Y(double value);
void Send_KeyboardButton(double X_val, double Y_val, bool Limiter, KeyboardState *state);

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
  MX_ADC1_Init();
  MX_USB_DEVICE_Init();
  MX_I2C1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
//  HAL_ADC_Start(&hadc1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  X_val = Get_X();
	  Y_val = Get_Y();
	  Limiter = Get_Limiter();

//	  send_keyboard(0, 0x17, 0, 0, 0, 0, 0);
//	  HAL_Delay(100);
//	  send_keyboard(0, 0, 0, 0, 0, 0, 0);
//	  HAL_Delay(5000);
	  Send_KeyboardButton(X_val, Y_val, Limiter, &keyboard_state);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
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
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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

X_Ranges getRange_X(double value)
{
    if (value >= 70) {
        return X_Range_O;
    } else if (value >= 50 && value < 70) {
        return X_Range_I;
    } else if (value >= 30 && value < 50) {
        return X_Range_U;
    } else if (value >= 10 && value < 30) {
        return X_Range_Y;
    } else if (value >= -10 && value < 10) {
        return X_Range_T;
    } else if (value >= -30 && value < -10) {
        return X_Range_R;
    } else if (value >= -50 && value < -30) {
        return X_Range_E;
    } else if (value >= -70 && value < -50) {
        return X_Range_W;
    } else if (value < -70) {
        return X_Range_Q;
    }
    return X_Range_None;
}

Y_Ranges getRange_Y(double value)
{
	if (value >= 135) {
		return Y_Range_F;
	} else if (value >= 90 && value < 135) {
		return Y_Range_D;
	} else if (value >= 45 && value < 90) {
		return Y_Range_S;
	} else if (value > 0 && value < 45) {
		return Y_Range_A;
	}
	return Y_Range_None;
}

void Send_KeyboardButton(double X_val,double Y_val, bool Limiter, KeyboardState *state)
{
	uint8_t keycodeX = 0;
	uint8_t keycodeY = 0;
	bool keySent = false;

	// Check X Range:
	X_Ranges currentXRange = getRange_X(X_val);
	if (currentXRange != state->previousRangeX)
	{
		switch (currentXRange)
		{
		case X_Range_Q: keycodeX = 0x14; break;
		case X_Range_W: keycodeX = 0x1A; break;
		case X_Range_E: keycodeX = 0x08; break;
		case X_Range_R: keycodeX = 0x15; break;
		case X_Range_S: keycodeX = 0x16; break;
		case X_Range_T: keycodeX = 0x17; break;
		case X_Range_Y: keycodeX = 0x1C; break;
		case X_Range_U: keycodeX = 0x18; break;
		case X_Range_I: keycodeX = 0x0C; break;
		case X_Range_O: keycodeX = 0x12; break;
		case X_Range_None: keycodeX = 0x00; break;
		default: break;
		}

		if (keycodeX != 0)
		{
			send_keyboard(0, keycodeX, 0, 0, 0, 0, 0);
			HAL_Delay(100);
			send_keyboard(0, 0, 0, 0, 0, 0, 0);

			keySent = true;

			state->previousRangeX = currentXRange;
		}
	}

	// Check Y Range:
	Y_Ranges currentYRange = getRange_Y(Y_val);
	if(currentYRange != state->previousRangeY)
	{
		switch(currentYRange)
		{
		case Y_Range_F: keycodeY = 0x09; break;
		case Y_Range_D: keycodeY = 0x07; break;
		case Y_Range_S: keycodeY = 0x16; break;
		case Y_Range_A: keycodeY = 0x04; break;
		case Y_Range_None: keycodeY = 0x00; break;
		default: break;
		}

		if (keycodeY != 0)
		{
			send_keyboard(0, keycodeY, 0, 0, 0, 0, 0);
			HAL_Delay(100);
			send_keyboard(0, 0, 0, 0, 0, 0, 0);

			keySent = true;

			state->previousRangeY = currentYRange;
		}
	}

	// Check Limiter:
	if (Limiter == true)
	{
		send_keyboard(0, 0x0A, 0, 0, 0, 0, 0);
		HAL_Delay(100);
		send_keyboard(0, 0, 0, 0, 0, 0, 0);
		keySent =  true;
	}
//	else
//	{
//		send_keyboard(0, 0, 0, 0, 0, 0, 0);
//	}

    if (keySent)
    {
        HAL_Delay(100);
    }

}


//void Send_KeyboardButton(double X_val,double Y_val, bool Limiter, KeyboardState *state)
//{
//	uint8_t keycode = 0;
//	bool keysent = false;
//
//	// X Axis key code handler (send only if different from previous X_val):
//	if (X_val != state->previousX)
//	{
//		if (X_val >= 70)
//		{
//			keycode = 0x17;  // 't'
//		}
//		else if (X_val >= 50 && X_val < 70)
//		{
//			keycode = 0x15;  // 'r'
//		}
//		else if (X_val >= 30 && X_val < 50)
//		{
//			keycode = 0x08;  // 'e'
//		}
//		else if (X_val >= 10 && X_val < 30)
//		{
//			keycode = 0x1A;  // 'w'
//		}
//		else if (X_val >= -10 && X_val < 10)
//		{
//			keycode = 0x14;  // 'q'
//		}
//		else if (X_val >= -30 && X_val < -10)
//		{
//			keycode = 0x04;  // 'a'
//		}
//		else if (X_val >= -50 && X_val < -30)
//		{
//			keycode = 0x16;  // 's'
//		}
//		else if (X_val >= -70 && X_val < -50)
//		{
//			keycode = 0x07;  // 'd'
//		}
//		else if (X_val < -70)
//		{
//			keycode = 0x09;  // 'f'
//		}
//
//		// Update previous X val:
//		state -> previousX = X_val;
//
//		// Send the keycode:
//		send_keyboard(0, keycode, 0, 0, 0, 0, 0);
//		// Release the key:
//		send_keyboard(0, 0, 0, 0, 0, 0, 0);
//
//		keysent = true;
//	}
//
//	// Y Axis key code handler (send only if different from previous Y_val):
//	if (Y_val != state->previousY) {
//		if (Y_val >= 135) {
//			keycode = 0x0F;  // 'l'
//		} else if (Y_val >= 90 && Y_val < 135) {
//			keycode = 0x0E;  // 'k'
//		} else if (Y_val >= 45 && Y_val < 90) {
//			keycode = 0x0D;  // 'j'
//		} else if (Y_val > 0 && Y_val < 45) {
//			keycode = 0x0B;  // 'h'
//		}
//
//		// Update the previous Y_val:
//		state->previousY = Y_val;
//
//		// Send the keycode:
//		send_keyboard(0, keycode, 0, 0, 0, 0, 0);
//		send_keyboard(0, 0, 0, 0, 0, 0, 0);  // Release the key
//		keysent = true;
//	}
//
//	if (Limiter != state->previousLimiter)
//	{
//		if (Limiter){
//			keycode = 0x0A; // 'g'
//		}
//		else{
//			keycode = 0x00;
//		}
//
//		state->previousLimiter = Limiter;
//
//		// Send the keycode:
//		send_keyboard(0, keycode, 0, 0, 0, 0, 0);
//		send_keyboard(0, 0, 0, 0, 0, 0, 0);  // Release the key
//	}
//	if (keysent){
//		HAL_Delay(10);
//	}
//}


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
