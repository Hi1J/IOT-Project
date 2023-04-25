#ifndef __MQTT_H_
#define __MQTT_H_

#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "esp8266.h"


#define OneNet "183.230.40.33"
#define Prot 80
//#define DeviceID "1067904715"
#define DeviceID "1065278300"
//#define Api_key "XRwrs6kXGAZ5NFSmbDqCJCPTfZI="
#define Api_key "01GwvANAtrA4U5FBMjDbgqlodqg="

extern char OneNet_tmp[512];
extern char OneNet_data_buf[123];

uint8_t OneNet_Init(void);

#endif