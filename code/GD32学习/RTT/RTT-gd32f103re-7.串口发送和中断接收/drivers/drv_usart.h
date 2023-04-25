#ifndef __DRV_USART_H_
#define __DRV_USART_H_


#include "gd32f10x.h"
#include "stdio.h"
#include <rtthread.h>
#include <rtdbg.h>
#include "app.h"

extern rt_device_t u1_dev;


void usart0_init(uint32_t brr);
void usart1_init(uint32_t brr);

#define DEBUG_USART	USART0

#endif
