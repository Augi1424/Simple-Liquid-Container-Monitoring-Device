/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2026 STMicroelectronics.
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
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "aht20.h"
#include "font.h"
#include "oled.h"
//#include "hx711.h"
#include "Ultrasound.h"
#include "ds18b20.h"
#include "motor.h"
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
HX711 data = { .gpioSck = GPIOC, .gpioData = GPIOC, .pinSck = GPIO_PIN_14,
		.pinData = GPIO_PIN_13, .offset = 0, .gain = 1 };
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void delay_us(uint32_t us) {
	uint32_t delay = us * 72 / 5;
	while (delay--) {

	}
}                                       //微秒级延时函数

float distance;
int flag;
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
  MX_I2C2_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
	HAL_Delay(20);
	OLED_Init();

	HAL_TIM_Base_Start(&htim1);
	HAL_TIM_IC_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_IC_Start_IT(&htim1, TIM_CHANNEL_4);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&htim3, TIM_CHANNEL_2);
	HAL_ADCEx_Calibration_Start(&hadc1);

	HAL_Delay(20);
	AHT20_Init();
	HX711_Init(data);
	Ds18_Init(GPIOB, GPIO_PIN_14);

	float aim = 0;
	uint32_t value;
	float temp;
	float weight;
	float aim_temp;
	char message_weight[24] = "";
	char message_kind[24] = "";
	char message_aim_height[24] = "";
	char message_real_height[24] = "";
	char message_temp[24] = "";
	char message_aim_temp[24] = "";

	typedef enum {
		water, milk, saline, vinegar
	} Liqkind;
	Liqkind liqkind;
	const char *liquid_name[] = { "water", "milk", "saline", "vinegar" };

	data = HX711_Tare(data, 10);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1) {
		if (a == 0) {
			HAL_ADC_Start(&hadc1);
			HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
			value = HAL_ADC_GetValue(&hadc1);
			HAL_ADC_Stop(&hadc1);
			if (value < 1500) {
				liqkind = saline;
			} else if (value < 1700) {
				liqkind = vinegar;
			} else if (value < 1970) {
				liqkind = milk;
			} else if (value > 2400) {
				liqkind = water;
			} else {
				liqkind = water;
			}

			aim = __HAL_TIM_GET_COUNTER(&htim3) / 10.0;
			temp = Ds18_MeasureTemp(GPIOB, GPIO_PIN_14);
			weight = HX711_getweight(data, 5, 353, 8300000);

			while (temp >= aim_temp) {                                    //过热保护
				Moto_Start(GPIOA, GPIO_PIN_3, GPIOA, GPIO_PIN_4);
				temp = Ds18_MeasureTemp(GPIOB, GPIO_PIN_14);
				OLED_NewFrame();
				OLED_PrintASCIIString(0, 0, "overheat protect", &afont16x8,
						OLED_COLOR_NORMAL);
				OLED_ShowFrame();
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
				HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
				if (temp < aim_temp) {
					Moto_Stop(GPIOA, GPIO_PIN_3, GPIOA, GPIO_PIN_4);
				}
			}

			distance = 12.4;
			if (flag == 1) {
				Measure();
				if (distance <= aim - 0.1) {
					Moto_Start(GPIOA, GPIO_PIN_3, GPIOA, GPIO_PIN_4);
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
				} else if (distance >= aim - 0.1 && distance <= aim) {
					Moto_Stop(GPIOA, GPIO_PIN_3, GPIOA, GPIO_PIN_4);
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_SET);
					HAL_Delay(3000);
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, GPIO_PIN_RESET);
					flag = 0;
				} else {
					Moto_Stop(GPIOA, GPIO_PIN_3, GPIOA, GPIO_PIN_4);
				}
			}

			sprintf(message_kind, "kind: %s", liquid_name[liqkind]);
			sprintf(message_weight, "weight: %.1fg", weight);
			sprintf(message_aim_height, "aim: %.1fcm", aim);
			sprintf(message_real_height, "height: %.1fcm", distance);
			sprintf(message_temp, "temperature: %.1f %d", temp, value);
			OLED_NewFrame();
			OLED_PrintASCIIString(0, 0, message_kind, &afont12x6,
					OLED_COLOR_NORMAL);
			OLED_PrintASCIIString(0, 12, message_weight, &afont12x6,
					OLED_COLOR_NORMAL);
			OLED_PrintASCIIString(0, 24, message_aim_height, &afont12x6,
					OLED_COLOR_NORMAL);
			OLED_PrintASCIIString(0, 36, message_real_height, &afont12x6,
					OLED_COLOR_NORMAL);
			OLED_PrintASCIIString(0, 48, message_temp, &afont12x6,
					OLED_COLOR_NORMAL);
			OLED_ShowFrame();

		} else if (a == 1) {
			aim_temp = __HAL_TIM_GET_COUNTER(&htim3);
			sprintf(message_aim_temp, "aim temp:%.1f", aim_temp);
			OLED_NewFrame();
			OLED_PrintASCIIString(0, 0, message_aim_temp, &afont12x6,
					OLED_COLOR_NORMAL);
			OLED_ShowFrame();

		}

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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
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
	while (1) {
	}
  /* USER CODE END Error_Handler_Debug */
}
#ifdef USE_FULL_ASSERT
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
