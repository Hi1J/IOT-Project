#ifndef _ILLU_H_
#define _ILLU_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
//#include "sensor_config.h"


//器件地址
#define	  SlaveAddress   0x46 


//左板
//宏定义端口时钟
#define RELAY_CLK   RCC_APB2Periph_GPIOB
//宏定义端口
#define RELAY_PORT  GPIOB
//宏定义管脚
#define RELAY_PIN   GPIO_Pin_12

//右板
////宏定义端口时钟
//#define RELAY_CLK   RCC_APB2Periph_GPIOB
////宏定义端口
//#define RELAY_PORT  GPIOB
////宏定义管脚
//#define RELAY_PIN   GPIO_Pin_6


//左板
  #define SDA_CLK   RCC_APB2Periph_GPIOB
	#define SDA_PORT  GPIOB
	#define SDA_PIN   GPIO_Pin_13

  #define SCL_CLK   RCC_APB2Periph_GPIOB
	#define SCL_PORT  GPIOB
	#define SCL_PIN   GPIO_Pin_12

//右板
//  #define SDA_CLK   RCC_APB2Periph_GPIOB
//	#define SDA_PORT  GPIOB
//	#define SDA_PIN   GPIO_Pin_7

//  #define SCL_CLK   RCC_APB2Periph_GPIOB
//	#define SCL_PORT  GPIOB
//	#define SCL_PIN   GPIO_Pin_6

 

#define LIGHT_SDA(X) 	(GPIO_WriteBit(SDA_PORT,SDA_PIN,(BitAction)X))
#define LIGHT_SCL(X) 	(GPIO_WriteBit(SCL_PORT,SCL_PIN,(BitAction)X))
#define IN_SDA         GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN)


extern u16 illu_val;

void ILLU_Gpio_Init(void);
void SDA_Gpio_init(int flag);
void IIC_SendACK(u8 ack);
u8 IIC_RecvACK(void);
u8 IIC_SendByte(u8 dat);
u8 IIC_RecvByte(void);
void Single_Write_BH1750(u8 REG_Address);
void Multiple_Read_BH1750(void);
u16 ILLU_ReadData(void);
u16 ILLU_Show_Usart(void);

#endif
