#ifndef __APP_H_
#define __APP_H_

#include "gd32f10x.h"
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define GET_PIN(PORTx,PIN) (rt_base_t)((16 * ( ((rt_base_t)__GD32_PORT(PORTx) - (rt_base_t)GPIO_BASE)/(0x0400UL) )) + PIN)
#define PIN_PORT(pin) ((uint8_t)(((pin) >> 4) & 0xFu))
#define PIN_NO(pin) ((uint8_t)((pin) & 0xFu))
#define PIN_GDPORT(pin) (GPIO_BASE + (0x400u * PIN_PORT(pin)))
#define PIN_GDPIN(pin) ((uint16_t)(1u << PIN_NO(pin)))

//定义事件
#define EVENT_FLAGS_1 (0X1 << 0)
#define EVENT_FLAGS_2 (0X1 << 1)
#define EVENT_FLAGS_3 (0X1 << 2)


//变量声明
extern struct rt_thread th1;
extern rt_uint8_t th1_stack[512];

extern struct rt_thread th2;
extern rt_uint8_t th2_stack[512];

extern struct rt_thread th3;
extern rt_uint8_t th3_stack[512];

extern rt_event_t event1;

extern struct rt_event event2;



//函数声明
void th1_entry(void *parameter);
void th2_entry(void *parameter);
void th3_entry(void *parameter);

#endif
