#ifndef __APP_H_
#define __APP_H_

#include "gd32f10x.h"
#include <rtthread.h>
#include "drv_usart.h"
#include "drv_a72.h"
#include "drv_timer.h"

#define GET_PIN(PORTx,PIN) (rt_base_t)((16 * ( ((rt_base_t)__GD32_PORT(PORTx) - (rt_base_t)GPIO_BASE)/(0x0400UL) )) + PIN)
#define PIN_PORT(pin) ((uint8_t)(((pin) >> 4) & 0xFu))
#define PIN_NO(pin) ((uint8_t)((pin) & 0xFu))
#define PIN_GDPORT(pin) (GPIO_BASE + (0x400u * PIN_PORT(pin)))
#define PIN_GDPIN(pin) ((uint16_t)(1u << PIN_NO(pin)))

#define BSP_USING_UART1
#define BSP_UART1_TX_PIN "PA2"
#define BSP_UART1_RX_PIN "PA3"


//变量声明
extern struct rt_thread th1;
extern rt_uint8_t th1_stack[512];

extern struct rt_thread th2;
extern rt_uint8_t th2_stack[512];

extern struct rt_thread th3;
extern rt_uint8_t th3_stack[512];

extern struct rt_thread th4;
extern rt_uint8_t th4_stack[512];


extern rt_sem_t A72_Respond;

extern rt_sem_t A72_Connect;

extern rt_sem_t A72_Data_handle;

extern rt_err_t A72_Cheak;

extern uint8_t A72_flag;

extern uint8_t A72_Device_Connect;


//函数声明
void Zigbee_Send_Data(void *parameter);
void Zigbee_Network_Status_Cheak_front(void *parameter);
void Zigbee_Network_Status_Cheak_after(void *parameter);
void Zigbee_Data_handle(void *parameter);

#endif
