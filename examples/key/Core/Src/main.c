/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
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
#include <stdint.h>
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define KEY0 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4) 	//KEY0 按键 PE4
#define KEY1 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3) 	//KEY1 按键 PE3
#define KEY2 HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2) 	//KEY2 按键 PE2
#define WK_UP HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0) 	//WKUP 按键 PA0

#define KEY0_PRES (1)
#define KEY1_PRES (1<<1)
#define KEY2_PRES (1<<2)
#define WKUP_PRES (1<<3)
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
enum KEY_PRESS_MODE {
	KEY_PRESS_SHORT_MODE, KEY_PRESS_LONG_MODE,
};
uint8_t Key_Scan(enum KEY_PRESS_MODE mode) {
	static uint8_t key_up = 1;
	if (mode == KEY_PRESS_SHORT_MODE) {
		key_up = 1;
	}

	if (key_up && (KEY0 == 0 || KEY1 == 0 || KEY2 == 0 || WK_UP == 1)) {
		HAL_Delay(10);
		key_up = 0;
		if (KEY0 == 0) {
			return KEY0_PRES;
		} else if (KEY1 == 0) {
			return KEY1_PRES;
		} else if (KEY2 == 0) {
			return KEY2_PRES;
		} else if (WK_UP == 1) {
			return WKUP_PRES;
		}
	} else if (KEY0 == 1 && KEY1 == 1 && KEY2 == 1 && WK_UP == 0) {
		key_up = 1;
	}
	return 0;

}
/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {
	/* USER CODE BEGIN 1 */
	//add by lfs
	GPIO_InitTypeDef GPIO_Initure;
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
	/* USER CODE BEGIN 2 */
	//add by lfs
	GPIO_Initure.Pin = GPIO_PIN_5; 						//PB5 LED0
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;  			//推挽输出
	GPIO_Initure.Pull = GPIO_PULLUP;          			//上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;  		//高速
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);

	GPIO_Initure.Pin = GPIO_PIN_5; 						//PE5 LED1
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);

	GPIO_Initure.Pin = GPIO_PIN_8; 						//PB8 BEEP
	HAL_GPIO_Init(GPIOB, &GPIO_Initure);

	GPIO_Initure.Pin = GPIO_PIN_0; 						//PA0 WK_UP
	GPIO_Initure.Mode = GPIO_MODE_INPUT; 				//输入
	GPIO_Initure.Pull = GPIO_PULLDOWN; 					//下拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;			//高速
	HAL_GPIO_Init(GPIOA, &GPIO_Initure);

	GPIO_Initure.Pin = GPIO_PIN_2; 						//PE2 KEY2
	GPIO_Initure.Mode = GPIO_MODE_INPUT; 				//输入
	GPIO_Initure.Pull = GPIO_PULLUP; 					//上拉
	GPIO_Initure.Speed = GPIO_SPEED_FREQ_HIGH;			//高速

	GPIO_Initure.Pin = GPIO_PIN_3; 						//PE3 KEY1
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);

	GPIO_Initure.Pin = GPIO_PIN_4; 						//PE4 KEY0
	HAL_GPIO_Init(GPIOE, &GPIO_Initure);

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1) {
		/* USER CODE END WHILE */

		if (Key_Scan(KEY_PRESS_LONG_MODE) == WKUP_PRES) {
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_SET);	//PB5置1 LED0(ON)
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_SET);	//PE5置1 LED1(ON)
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);	//PB8置1 BEEP(ON)

			HAL_Delay(100);

			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET); //PB8置1 BEEP(OFF)
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, GPIO_PIN_RESET); //PB5置0 LED0(OFF)
			HAL_GPIO_WritePin(GPIOE, GPIO_PIN_5, GPIO_PIN_RESET); //PE5置0 LED1(OFF)
		}
		//add by lfs
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
	RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
	RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
			| RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK) {
		Error_Handler();
	}
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	//add by lfs
	__HAL_RCC_GPIOE_CLK_ENABLE();
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1) {
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
