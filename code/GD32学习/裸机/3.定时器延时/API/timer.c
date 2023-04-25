#include "timer.h"

void TIM6_Delay_Init(void)
{
	
	rcu_periph_clock_enable(RCU_TIMER6);
	
	TIMER_CTL0(TIMER6) |= 1 << 7;
	
	TIMER_CTL0(TIMER6) |= 1 << 3;
	
	TIMER_CTL0(TIMER6) &= ~(1 << 2);
	
	TIMER_CTL0(TIMER6) &= ~(1 << 1);
	
	
	
}

void TIM6_Delay_us(uint16_t us)//108000000MHZ/108  --- 1us
{
	TIMER_PSC(TIMER6) = 108 - 1;
	
	TIMER_CAR(TIMER6) = us;
	
	TIMER_SWEVG(TIMER6) |= 1 << 0;
	
	TIMER_INTF(TIMER6) &= ~(1 << 0);
	
	TIMER_CTL0(TIMER6) |= 1 << 0;
	
	while(!(TIMER_INTF(TIMER6) & (1 << 0)));
	
	TIMER_INTF(TIMER6) &= ~(1 << 0);
	
	TIMER_CTL0(TIMER6) &= ~(1 << 0);
	
}

void TIM6_Delay_ms(uint16_t ms)//108000000MHZ/10800  --- 0.1ms
{
	TIMER_PSC(TIMER6) = 10800 - 1;
	
	TIMER_CAR(TIMER6) = ms * 10;
	
	TIMER_SWEVG(TIMER6) |= 1 << 0;
	
	TIMER_INTF(TIMER6) &= ~(1 << 0);
	
	TIMER_CTL0(TIMER6) |= 1 << 0;
	
	while(!(TIMER_INTF(TIMER6) & (1 << 0)));
	
	TIMER_INTF(TIMER6) &= ~(1 << 0);
	
	TIMER_CTL0(TIMER6) &= ~(1 << 0);
	
}
