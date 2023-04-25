#ifndef __TIM6_H
#define __TIM6_H

// 
#include "stm32f4xx.h"


void TIM6_Delay_Init(void);

void TIM6_Delay_Ms(uint16_t ms);

void TIM6_Delay_Us(uint32_t us);

void TIM6_Inter_Init(u16 psc,u32 arr);

void TIM6_DAC_IRQHandler(void);


//void TIM6_Init(void);
//void TIM6_Delay_ms(u16 ms);
//void TIM6_Delay_us(u16 us);





#endif