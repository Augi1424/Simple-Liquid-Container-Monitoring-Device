/*
 * Ultrasound.c
 *
 *  Created on: Apr 1, 2026
 *      Author: AugiJL
 */
#include "tim.h"
#include "Ultrasound.h"
#include "stm32f1xx_hal.h"
#include "font.h"
#include "oled.h"
#include <math.h>
#include <stdio.h>


void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	int risingedge = 0;
	int fallingedge = 0;
	char message[24] = "";

	if(htim == &htim1 && htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4){
		risingedge = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_3);
		fallingedge = HAL_TIM_ReadCapturedValue(&htim1, TIM_CHANNEL_4);
		distance = 32.5 - ((fallingedge - risingedge) * (1e-6) * (3.24e4) / 2) - (-4.0657e-3 * pow((32.5 - ((fallingedge - risingedge) * (1e-6) * (3.24e4) / 2)),2) - 0.0291 * (32.5 - ((fallingedge - risingedge) * (1e-6) * (3.24e4) / 2)) + 2.0018);

//		sprintf(message, "高 %.1f cm", distance);
//		OLED_NewFrame();
//		OLED_PrintString(0, 0, message, &font16x16, OLED_COLOR_NORMAL);
//		OLED_ShowFrame();
	}
}

float Measure(void){

	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_Delay(2);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_11, GPIO_PIN_RESET);

	__HAL_TIM_SET_COUNTER(&htim1, 0);
	HAL_Delay(10);

	return distance;
}
