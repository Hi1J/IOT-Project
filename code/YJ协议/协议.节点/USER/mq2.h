#ifndef _MQ2_H_
#define _MQ2_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
//#include "sensor_config.h"

//宏定义端口和管脚（底层修改这里就行了）
//#if(MQ2_POS == MIDDLE_POS)
#define MQ2_CLK   RCC_APB2Periph_GPIOB //宏定义端口时钟
#define MQ2_PORT  GPIOB                //宏定义端口
#define MQ2_PIN   GPIO_Pin_1           //宏定义管脚
#define MQ2_ADC_CHANNEL ADC_Channel_9  //宏定义ADC通道
//#elif(MQ2_POS == RIGHT_POS)
//#define MQ2_CLK   RCC_APB2Periph_GPIOA //宏定义端口时钟
//#define MQ2_PORT  GPIOA                //宏定义端口
//#define MQ2_PIN   GPIO_Pin_0           //宏定义管脚
//#define MQ2_ADC_CHANNEL ADC_Channel_0  //宏定义ADC通道
//#endif

extern u16 mq2_val;

void MQ2_Gpio_Init(void);
int MQ2_GetValue(u16 *adc_val);
u16 MQ2_Show_Usart(void);

#endif

