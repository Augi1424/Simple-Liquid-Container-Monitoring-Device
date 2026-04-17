/*
 * hx711.h
 *
 *  Created on: Jan 29, 2026
 *      Author: Lenovo1
 */

#ifndef INC_HX711_H_
#define INC_HX711_H_

#include "stm32f1xx_hal.h"

typedef struct _hx711
{
	GPIO_TypeDef* gpioSck;
	GPIO_TypeDef* gpioData;
	uint16_t pinSck;
	uint16_t pinData;
	int offset;
	int gain;
	// 1: channel A, gain factor 128
	// 2: channel B, gain factor 32
    // 3: channel A, gain factor 64
} HX711;


void HX711_Init(HX711 data);
HX711 HX711_Tare(HX711 data, uint8_t times);
int HX711_Value(HX711 data);
int HX711_Average_Value(HX711 data, uint8_t times);
float HX711_getweight(HX711 data, uint8_t times, float m, int weight_adc_m);


//void HX711_Init1(HX711 data);
//HX711 HX711_Tare1(HX711 data, uint8_t times);
//int HX711_Value1(HX711 data);
//int HX711_Average_Value1(HX711 data, uint8_t times);
//float HX711_getweight1(HX711 data, uint8_t times, float m, int weight_adc_m);
//m为校准用已知质量，weight_adc_m为该质量下的adc转换值。

#endif /* INC_HX711_H_ */
