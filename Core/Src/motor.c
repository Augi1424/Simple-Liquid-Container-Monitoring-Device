/*
 * motor.c
 *
 *  Created on: Apr 5, 2026
 *      Author: AugiJL
 */
#include "motor.h"

void Moto_Start(GPIO_TypeDef* GPIOX, uint16_t GPIO_PINX, GPIO_TypeDef* GPIOY, uint16_t GPIO_PINY){
	HAL_GPIO_WritePin(GPIOX, GPIO_PINX, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOY, GPIO_PINY, GPIO_PIN_RESET);
}

void Moto_Stop(GPIO_TypeDef* GPIOX, uint16_t GPIO_PINX, GPIO_TypeDef* GPIOY, uint16_t GPIO_PINY){
	HAL_GPIO_WritePin(GPIOX, GPIO_PINX, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOY, GPIO_PINY, GPIO_PIN_RESET);
}
