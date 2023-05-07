#ifndef _DRE_IIC_H_
#define _DRE_IIC_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "drv_pin.h"
#include "rtthread.h"


/******************IIC函数相关宏定义**********************/

#define I2C_SCL 				 (GET_PIN(A, 5))
#define I2C_SDA					 (GET_PIN(A, 6))


#define I2C_GPIO    			 GPIOA
#define I2C_SCL_PIN 			 GPIO_PIN_5
#define I2C_SDA_PIN 			 GPIO_PIN_6


#define I2C_TIM_Delay(ms) 		 rt_thread_mdelay(ms) 



/**********************光敏传感器相关宏定义***********************/

#define Light_W_Adress 0x46
#define Light_R_Adress 0x47

#define Cont_H_1_Mode  0x10
#define Cont_H_2_Mode  0x11
#define Cont_L_Mode    0x13


#define Once_H_1_Mode  0x20
#define Once_H_2_Mode  0x21
#define Once_L_Mode  	 0x23


/***************************IIC函数声明**************************/

void rt_iic_init(void);

void IIC_W_SCL(uint8_t BitValue);

void IIC_W_SDA(uint8_t BitValue);

uint8_t IIC_R_SDA(void);

void IIC_Start(void);

void IIC_Stop(void);

void IIC_SendByte(uint8_t Byte);

void IIC_SendAddress(uint8_t Byte);

uint8_t IIC_ReceiveByte(void);

void IIC_SendAsk(uint8_t AskByte);

uint8_t IIC_ReceiveAsk(void);

/************************光照传感器函数声明**********************/


void Send_W_Adress(void);

void Send_R_Adress(void);

void Mode_Read_Wait_Time(uint8_t Mode_Selet);

void Light_Serior(uint8_t Mode_Selet);

uint16_t Light_Read_Date(uint8_t Mode_Selet);
/**************************************************************/
#endif

