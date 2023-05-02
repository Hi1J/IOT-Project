#ifndef __APP_H_
#define __APP_H_

#include "gd32f10x.h"
#include <rtthread.h>
#include "drv_usart.h"
#include "drv_a72.h"
#include "drv_timer.h"
#include "drv_nb73.h"
#include "modbus.h"

#define GET_PIN(PORTx,PIN) (rt_base_t)((16 * ( ((rt_base_t)__GD32_PORT(PORTx) - (rt_base_t)GPIO_BASE)/(0x0400UL) )) + PIN)
#define PIN_PORT(pin) ((uint8_t)(((pin) >> 4) & 0xFu))
#define PIN_NO(pin) ((uint8_t)((pin) & 0xFu))
#define PIN_GDPORT(pin) (GPIO_BASE + (0x400u * PIN_PORT(pin)))
#define PIN_GDPIN(pin) ((uint16_t)(1u << PIN_NO(pin)))

#define BSP_USING_UART1
#define BSP_UART1_TX_PIN "PA2"
#define BSP_UART1_RX_PIN "PA3"


//变量声明


extern struct rt_thread th4;
extern rt_uint8_t th4_stack[512];

extern struct rt_thread th5;
extern rt_uint8_t th5_stack[512];

extern struct rt_thread th6;
extern rt_uint8_t th6_stack[512];

extern struct rt_thread th7;
extern rt_uint8_t th7_stack[512];

extern struct rt_thread th8;
extern rt_uint8_t th8_stack[512];

extern struct rt_thread th9;
extern rt_uint8_t th9_stack[512];


extern rt_sem_t A72_Respond;

extern rt_sem_t A72_Connect;

extern rt_sem_t A72_Data_handle;

extern rt_err_t A72_Cheak;

extern uint8_t A72_flag;

extern uint8_t A72_Device_Connect;

extern uint8_t NB73_Device_Connect;

extern rt_sem_t NB73_Data_handle;

extern rt_sem_t NODE_Appear_person;

extern uint8_t NODE_Appear_dirction;

extern uint8_t NODE1_CONNECT_STATUS;
extern uint8_t NODE1_LIGHT_STATUS;
extern uint16_t NODE1_LIGHT;
extern int NODE1_PEOPLE;
extern uint8_t RX_NODE1;

extern uint8_t NODE2_CONNECT_STATUS;
extern uint8_t NODE2_LIGHT_STATUS;
extern uint16_t NODE2_LIGHT;
extern int NODE2_PEOPLE;
extern uint8_t RX_NODE2;

//函数声明
void Zigbee_Data_handle(void *parameter);

void NB73_Send_Data(void *parameter);
void IOT_Data_handle(void *parameter);
void CHEK_NODE(void *parameter);
void Turn_on_next(void *parameter);

void WALN_Init(void *parameter);

#endif
