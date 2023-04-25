#ifndef _MOTOR_H_
#define _MOTOR_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "delay.h"
//#include "sensor_config.h"

enum CURTAIN_STATE
{
	CURTAIN_CLOSE,CURTAIN_OPEN
};
//宏定义端口和管脚（底层修改这里就行了）

//左边
/****************************
A+:PB13
A-:PA8
B+:PB12
B-:PB1
*****************************/
//宏定义端口时钟
#define A1_CLK RCC_APB2Periph_GPIOB		//A+
#define A0_CLK RCC_APB2Periph_GPIOA		//A-
#define B1_CLK RCC_APB2Periph_GPIOB		//B+
#define B0_CLK RCC_APB2Periph_GPIOB		//B-
//宏定义端口
#define A1_PORT GPIOB		//A+
#define A0_PORT GPIOA		//A-
#define B1_PORT GPIOB		//B+
#define B0_PORT GPIOB		//B-
//宏定义管脚
#define A1_PIN GPIO_Pin_13	//A+
#define A0_PIN GPIO_Pin_8	//A-
#define B1_PIN GPIO_Pin_12	//B+
#define B0_PIN GPIO_Pin_1	//B-


//右边
///****************************
//A+:PB7
//A-:PB5
//B+:PB6
//B-:PA0
//*****************************/
////宏定义端口时钟
//#define A1_CLK RCC_APB2Periph_GPIOB		//A+
//#define A0_CLK RCC_APB2Periph_GPIOB		//A-
//#define B1_CLK RCC_APB2Periph_GPIOB		//B+
//#define B0_CLK RCC_APB2Periph_GPIOA		//B-
////宏定义端口
//#define A1_PORT GPIOB		//A+
//#define A0_PORT GPIOB		//A-
//#define B1_PORT GPIOB		//B+
//#define B0_PORT GPIOA		//B-
////宏定义管脚
//#define A1_PIN GPIO_Pin_7	//A+
//#define A0_PIN GPIO_Pin_5	//A-
//#define B1_PIN GPIO_Pin_6	//B+
//#define B0_PIN GPIO_Pin_0	//B-
//#endif

#define A1_OPT(x)	GPIO_WriteBit(A1_PORT,A1_PIN,(BitAction)x)
#define A0_OPT(x) GPIO_WriteBit(A0_PORT,A0_PIN,(BitAction)x)
#define B1_OPT(x) GPIO_WriteBit(B1_PORT,B1_PIN,(BitAction)x)
#define B0_OPT(x) GPIO_WriteBit(B0_PORT,B0_PIN,(BitAction)x) 


extern unsigned char curtain_state;
void Motor_Gpio_Init(void);
u8 Motorcw(unsigned char anti);

#endif

