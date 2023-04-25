#ifndef __KEY_H_
#define __KEY_H_

#include "stm32f10x.h"
#include "delay.h"

#define KEY1 GPIO_Pin_0
#define KEY2 GPIO_Pin_1
#define KEY3 GPIO_Pin_2
#define KEY4 GPIO_Pin_3
#define KEY5 GPIO_Pin_4
#define KEY6 GPIO_Pin_5
#define KEY7 GPIO_Pin_6
#define KEY8 GPIO_Pin_7

void KEY_Indenpendent_Init(void);
u8 Key_GetValue(void);

#endif