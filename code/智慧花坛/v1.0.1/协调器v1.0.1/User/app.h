#ifndef __APP_H_
#define __APP_H_

#include "gd32f10x.h"
#include <rtthread.h>
#include <rtdbg.h>
#include "stdio.h"
#include "string.h"
#include "stdbool.h"
#include "drv_usart.h"
#include "zigbee_a72.h"
#include "fs00905b.h"
#include "4g_bc28.h"
#include "drv_bc28.h"


#define th1_test_priority   20
#define th2_test_priority   19
#define th3_test_priority   21
#define th4_test_priority   20
#define th5_test_priority   18
#define th6_test_priority   19

//��������
extern struct rt_thread th1;
extern rt_uint8_t th1_stack[512];

extern struct rt_thread th2;
extern rt_uint8_t th2_stack[512];

extern struct rt_thread th3;
extern rt_uint8_t th3_stack[512];

extern struct rt_thread th4;
extern rt_uint8_t th4_stack[1024];

extern struct rt_thread th5;
extern rt_uint8_t th5_stack[512];

extern struct rt_thread th6;
extern rt_uint8_t th6_stack[1024];

extern rt_sem_t A72_Respond;
extern rt_sem_t A72_Connect;
extern rt_sem_t A72_Data_handle;
extern rt_sem_t FS_Respond;
extern rt_sem_t BC28_Respond;
extern rt_uint8_t A72_Device_Connect;
extern rt_err_t A72_Cheak;
extern rt_uint8_t BC28_Connect;
extern rt_uint8_t MQTT_Connect;


//��������
void rt_hw_us_delay(rt_uint32_t us);
void WLAN_INIT(void *parameter);
void Zigbee_Data_handle(void *parameter);
void Zigbee_Heartbeat(void *parameter);
void Air_quality_takeValue(void *parameter);
void MQTT_INIT(void *parameter);
void MQTT_PUB_DATA(void *parameter);

#endif
