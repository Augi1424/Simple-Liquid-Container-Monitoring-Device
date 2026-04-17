/*
 * da18b20.h
 *
 *  Created on: Apr 4, 2026
 *      Author: AugiJL
 */

#include "ds18b20.h"


void Ds18_Init(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN){
	int flag1 = 0;                        //这个警告不用管，虽然标志位没被调用，但是不影响功能
	uint32_t timeout = 100000;
	HAL_GPIO_WritePin(GPIOX, GPIO_PIN, GPIO_PIN_RESET);
	delay_us(500);
	HAL_GPIO_WritePin(GPIOX, GPIO_PIN, GPIO_PIN_SET);
	delay_us(10);
	do{
		if(HAL_GPIO_ReadPin(GPIOX, GPIO_PIN) == GPIO_PIN_RESET){
			flag1 = 1;
			break;
		}
	}while(timeout--);
	delay_us(500);
}

void Write1(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN){
	HAL_GPIO_WritePin(GPIOX, GPIO_PIN, GPIO_PIN_RESET);
	delay_us(Write1_Lim);
	HAL_GPIO_WritePin(GPIOX, GPIO_PIN, GPIO_PIN_SET);
	delay_us(Duration_Time + Recovery_Time);
}

void Write0(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN){
	HAL_GPIO_WritePin(GPIOX, GPIO_PIN, GPIO_PIN_RESET);
	delay_us(Duration_Time);
	HAL_GPIO_WritePin(GPIOX, GPIO_PIN, GPIO_PIN_SET);
	delay_us(Recovery_Time);
}

void Ds18_Transmit(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN, uint8_t message){
	for(int i = 0; i < 8; i++){
		if((message & 0x01) == 0x01){
			Write1(GPIOX, GPIO_PIN);
		}else{
			Write0(GPIOX, GPIO_PIN);
		}
		message >>= 1;
	}
}

uint8_t Ds18_ReadBit(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN){
	uint8_t bit;
	HAL_GPIO_WritePin(GPIOX, GPIO_PIN, GPIO_PIN_RESET);
	delay_us(2);
	HAL_GPIO_WritePin(GPIOX, GPIO_PIN, GPIO_PIN_SET);
	delay_us(10);
	bit = HAL_GPIO_ReadPin(GPIOX, GPIO_PIN);
	delay_us(48);                                //整个时隙>60us
	return bit;
}

uint8_t Ds18_ReadByte(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN){
	uint8_t bit;
	uint8_t byte = 0x00;
	for(int i = 0; i < 8; i++){
		bit = Ds18_ReadBit(GPIOX, GPIO_PIN);
		byte = byte + (bit << i);
	}
	return byte;
}

float Ds18_MeasureTemp(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN){
	Ds18_Init(GPIOX, GPIO_PIN);
	Ds18_Transmit(GPIOX, GPIO_PIN, 0xCC);
	Ds18_Transmit(GPIOX, GPIO_PIN, 0x44);
	HAL_Delay(750);
	Ds18_Init(GPIOX, GPIO_PIN);
	Ds18_Transmit(GPIOX, GPIO_PIN, 0xCC);
	Ds18_Transmit(GPIOX, GPIO_PIN, 0xBE);
	uint8_t byte1 = Ds18_ReadByte(GPIOX, GPIO_PIN);
	uint8_t byte2 = Ds18_ReadByte(GPIOX, GPIO_PIN);
	Ds18_Init(GPIOX, GPIO_PIN);
	uint16_t TEMP = byte1 + (byte2 << 8);
	float Temp = 0;
	int i ;
	if((TEMP & 0x8000) == 0x0000){
		Temp = (TEMP & 0x07FF) / 16.0;
	}else if((TEMP & 0x8000) == 0x8000){
		for(i = 0; i < 16; i++){
			if((TEMP & (0x0001 << i)) == 1){
				break;
			}
		}
		Temp = -( (TEMP ^ (0xFFFF << (i+1))) / 16.0 );
	}
	return Temp;
}
