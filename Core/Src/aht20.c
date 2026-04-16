/*
 * aht20.c
 *
 *  Created on: Feb 25, 2026
 *      Author: AugiJL
 */
#define transmit(pdata, size)  HAL_I2C_Master_Transmit(&hi2c2, 0x38<<1, pdata, size, HAL_MAX_DELAY)
#define receive(pdata, size)  HAL_I2C_Master_Receive(&hi2c2, 0x38<<1, pdata, size, HAL_MAX_DELAY)

#include "aht20.h"
#include <math.h>

void AHT20_Init(void) {
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, SET);
//	HAL_Delay(10);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, RESET);
//	HAL_Delay(5);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, RESET);
	uint8_t readbuffer;

	readbuffer = receive(&readbuffer, 1);
	if ((readbuffer & 0x08) == 0x00) {
		uint8_t sendbuffer[3] = { 0xBE, 0x08, 0x00 };
		transmit(sendbuffer, 3);
	}
}

float AHT20_Temp(void) {
	float t;
	uint8_t sendbuffer[3] = { 0xAC, 0x33, 0x00 };
	uint8_t readbuffer[6];
	int st = 0;
	transmit(sendbuffer, 3);
	HAL_Delay(80);
	readbuffer[6] = receive(readbuffer, 6);
	if ((readbuffer[0] & 0x80) == 0x00) {
		st = (((uint32_t) readbuffer[3] & 0x0F) << 16)
				+ ((uint32_t) readbuffer[4] << 8) + (uint32_t) readbuffer[5];
		t = (st / pow(2, 20)) * 200 - 50;
	}

	return t;
}

float AHT20_Humi(void) {
	float Rh;
	uint8_t sendbuffer[3] = { 0xAC, 0x33, 0x00 };
	uint8_t readbuffer[6];
	int srh = 0;
	transmit(sendbuffer, 3);
	HAL_Delay(80);
	readbuffer[6] = receive(readbuffer, 6);
	if ((readbuffer[0] & 0x80) == 0x00) {
		srh = ((uint32_t)readbuffer[1] << 12) + ((uint32_t)readbuffer[2] << 4) + ((uint32_t)readbuffer[3] & 0xF0);
		Rh = (srh / pow(2,20)) * 100;
	}
	return Rh;
}
