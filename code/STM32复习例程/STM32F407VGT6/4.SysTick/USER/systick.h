#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f4xx.h"
#include "led.h"

#define SysTick_ ( 168 * 1000000 ) / 8

void delay_ms(u32 ms);
void delay_Inter_ms(u32 ms);
void SysTick_Handler();


#endif