/*
 * motor.h
 *
 *  Created on: Apr 5, 2026
 *      Author: AugiJL
 */

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "main.h"

void Moto_Start(GPIO_TypeDef* GPIOX, uint16_t GPIO_PINX, GPIO_TypeDef* GPIOY, uint16_t GPIO_PINY);
void Moto_Stop(GPIO_TypeDef* GPIOX, uint16_t GPIO_PINX, GPIO_TypeDef* GPIOY, uint16_t GPIO_PINY);

#endif /* INC_MOTOR_H_ */
