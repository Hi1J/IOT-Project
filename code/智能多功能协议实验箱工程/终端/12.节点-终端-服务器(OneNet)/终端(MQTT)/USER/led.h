#ifndef __LED_H_
#define __LED_H_

#include "stm32f10x.h"

void LED_Init();

/**********************************
LED1~LED8
PE0 PE1 PE2 PE3 PE4 PE5 PE6 PC13
**********************************/
#define LED1_7_Port GPIOE
#define LED8_Port GPIOC
#define LED1_Pin GPIO_Pin_0
#define LED2_Pin GPIO_Pin_1
#define LED3_Pin GPIO_Pin_2
#define LED4_Pin GPIO_Pin_3
#define LED5_Pin GPIO_Pin_4
#define LED6_Pin GPIO_Pin_5

#define LED7_Pin GPIO_Pin_6
#define LED8_Pin GPIO_Pin_13
#define LED1_7_CLK RCC_APB2Periph_GPIOE

#define LED8_CLK RCC_APB2Periph_GPIOC
#define LED1_TOGGLE (LED1_7_Port->ODR ^= LED1_Pin)
#define LED2_TOGGLE (LED1_7_Port->ODR ^= LED2_Pin)
#define LED3_TOGGLE (LED1_7_Port->ODR ^= LED3_Pin)
#define LED4_TOGGLE (LED1_7_Port->ODR ^= LED4_Pin)
#define LED5_TOGGLE (LED1_7_Port->ODR ^= LED5_Pin)
#define LED6_TOGGLE (LED1_7_Port->ODR ^= LED6_Pin)
#define LED7_TOGGLE (LED1_7_Port->ODR ^= LED7_Pin)
#define LED8_TOGGLE (LED8_Port->ODR ^= LED8_Pin)

#define LED1_ON GPIOE->ODR &= ~(1 << 0);
#define LED1_OFF GPIOE->ODR |= 1 << 0;



#endif