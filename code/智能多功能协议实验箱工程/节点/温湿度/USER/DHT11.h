
#ifndef _DHT11_H_
#define _DHT11_H_

#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
//#include "sensor_config.h"


//×ó±ß
//#define DHT11_CLK  RCC_APB2Periph_GPIOB 
//#define DHT11_PORT GPIOB
//#define DHT11_PIN GPIO_Pin_12

//ÓÒ±ß
#define DHT11_CLK  RCC_APB2Periph_GPIOB
#define DHT11_PORT GPIOB
#define DHT11_PIN GPIO_Pin_6


#define DATA(x)    x?(GPIO_SetBits(DHT11_PORT,DHT11_PIN)):(GPIO_ResetBits(DHT11_PORT,DHT11_PIN))
#define DATA_IN()  GPIO_ReadInputDataBit(DHT11_PORT,DHT11_PIN)

extern u16 tem,hum;

void DHT11_Config(u8 flag);
int Get_DHT11_Data(u16 *t,u16* h);
int DHT11_Show_Usart(u16 *t,u16 *h);

#endif


