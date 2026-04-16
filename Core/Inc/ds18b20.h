/*
 * ds18b20.h
 *
 *  Created on: Apr 4, 2026
 *      Author: AugiJL
 */

#ifndef INC_DS18B20_H_
#define INC_DS18B20_H_

/*配置方法：选择一个GPIO引脚，配置为开漏输出并且拉高*/
#define Duration_Time 65
#define Recovery_Time 5
#define Write1_Lim 10
//#define Init_Reco_ReadTime_Lim 5       //  初始化读时隙/恢复时间

#include "main.h"

void Write1(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN);
void Write0(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN);
void Ds18_Transmit(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN, uint8_t message);              //发送信息函数
void Ds18_Init(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN);                     //初始化函数
uint8_t Ds18_ReadBit(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN);                  //bit读取
uint8_t Ds18_ReadByte(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN);                 //字节读取
float Ds18_MeasureTemp(GPIO_TypeDef* GPIOX, uint16_t GPIO_PIN);             //温度测量函数


#endif /* INC_DS18B20_H_ */
