#ifndef __DRV_TIMER_H_
#define __DRV_TIMER_H_

#include "gd32f10x.h"
#include "stdio.h"
#include <rtthread.h>
#include "app.h"

void TIM6_Delay_Init(void);
void TIM6_Delay_us(uint16_t us);
void TIM6_Delay_ms(uint16_t ms);

#endif

