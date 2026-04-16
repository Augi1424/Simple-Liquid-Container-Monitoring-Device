/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "hx711.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern float distance;
extern int flag;
extern HX711 data;
extern int a;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void delay_us(uint32_t us);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define hx711_data_Pin GPIO_PIN_13
#define hx711_data_GPIO_Port GPIOC
#define hx711_sck_Pin GPIO_PIN_14
#define hx711_sck_GPIO_Port GPIOC
#define motor_p_Pin GPIO_PIN_3
#define motor_p_GPIO_Port GPIOA
#define motor_n_Pin GPIO_PIN_4
#define motor_n_GPIO_Port GPIOA
#define encoder1_Pin GPIO_PIN_6
#define encoder1_GPIO_Port GPIOA
#define encoder2_Pin GPIO_PIN_7
#define encoder2_GPIO_Port GPIOA
#define start_Pin GPIO_PIN_12
#define start_GPIO_Port GPIOB
#define start_EXTI_IRQn EXTI15_10_IRQn
#define ds18_Pin GPIO_PIN_14
#define ds18_GPIO_Port GPIOB
#define setting_Pin GPIO_PIN_15
#define setting_GPIO_Port GPIOB
#define setting_EXTI_IRQn EXTI15_10_IRQn
#define trig_Pin GPIO_PIN_11
#define trig_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
