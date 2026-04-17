/*
 * aht20.h
 *
 *  Created on: Feb 25, 2026
 *      Author: AugiJL
 */

#include "i2c.h"

#ifndef INC_AHT20_H_
#define INC_AHT20_H_

void AHT20_Init(void);
float AHT20_Temp(void);
float AHT20_Humi(void);

#endif /* INC_AHT20_H_ */
