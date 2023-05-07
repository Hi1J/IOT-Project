/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-07     y zy       the first version
 */
#ifndef APPLICATIONS_APP_H_
#define APPLICATIONS_APP_H_

#include <rtthread.h>
#include "gd32f10x.h"
#include "drv_uart.h"
#include "zigbee_a72.h"
#include "Light_Sensor.h"
#include "drv_pwm.h"


#define th1_test_priority   20
#define th2_test_priority   24
#define th3_test_priority   21
#define th4_test_priority   20
#define th5_test_priority   22
#define th6_test_priority   19
#define th7_test_priority   18
#define th8_test_priority   26
//#define th8_test_priority   18

extern rt_uint8_t A72_Device_Connect;
extern rt_err_t A72_Cheak;
	
extern rt_thread_t th1_ptr;
extern struct rt_thread th2;
extern struct rt_thread th3;
extern struct rt_thread th4;
extern struct rt_thread th5;
extern struct rt_thread th6;
extern struct rt_thread th7;
extern struct rt_thread th8;
//extern struct rt_thread th8;


extern rt_uint8_t th2_stack[512];
extern rt_uint8_t th3_stack[512];
extern rt_uint8_t th4_stack[512];
extern rt_uint8_t th5_stack[512];
extern rt_uint8_t th6_stack[512];
extern rt_uint8_t th7_stack[512];
extern rt_uint8_t th8_stack[512];
//extern rt_uint8_t th8_stack[512];


extern rt_sem_t dynamic_sem;

extern rt_sem_t A72_Respond;
extern rt_sem_t A72_Connect;
extern rt_sem_t A72_Data_handle;

extern uint8_t Led_Status;
extern uint8_t Turn_next;

void th1_test(void* paremeter);
void th1_test_start(void *paremeter);
void WLAN_INIT(void* paremeter);
void Zigbee_Network_Status_Cheak_front(void* paremeter);
void Zigbee_Network_Status_Cheak_after(void *parameter);
void Zigbee_Send_Data(void *parameter);
void Zigbee_Data_handle(void *parameter);
void th7_test(void* paremeter);
void th8_test(void* paremeter);



#endif /* APPLICATIONS_APP_H_ */
