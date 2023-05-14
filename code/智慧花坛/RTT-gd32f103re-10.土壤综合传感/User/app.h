#ifndef __APP_H_
#define __APP_H_

#include "gd32f10x.h"
#include <rtthread.h>
#include <rtdbg.h>
#include "string.h"
#include "drv_usart.h"
#include "drv_sensor.h"
#include "sensor.h"
#include "modbus.h"


//变量声明
extern struct rt_thread th1;
extern rt_uint8_t th1_stack[512];

extern struct rt_thread th2;
extern rt_uint8_t th2_stack[512];




//函数声明
void th1_entry(void *parameter);
void th2_entry(void *parameter);


#endif
