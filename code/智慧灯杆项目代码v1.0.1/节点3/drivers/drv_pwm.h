#ifndef _DRV_PWM_
#define _DRV_PWM_

#include "gd32f10x.h"
#include "stdio.h"
#include "rtthread.h"


/********************������غ궨��**********************/
#define TIMER_2  TIMER2 


/***********************��������************************/



/***********************��������************************/



void pwm1_siet_init(void);


uint16_t PWM1_CH1_Crol(uint16_t out_value);

/*******************************************************/
#endif
