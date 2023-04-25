#ifndef _PM25_H_
#define _PM25_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
//#include "sensor_config.h"

#define Delay_us Delay_usnop 

#define PM25_High() GPIO_SetBits(GPIOB,GPIO_Pin_5)
#define PM25_Low()  GPIO_ResetBits(GPIOB,GPIO_Pin_5)


extern float pm25_val;

void PM25_Gpio_Init(void);
float PM25_Get_Value(void);
float PM25_Show_Usart(void);


#endif
