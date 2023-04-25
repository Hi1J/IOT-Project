#ifndef __ZIGBEE_H_
#define __ZIGBEE_H_

#include "stm32f10x.h"
#include "wlan_user.h"
#include "string.h"
#include "new.h"


#define Zigbee_Address  0xff



extern u8 Zigbee_Rx_Buf[128];
extern u8 Zigbee_Rx_OK;

void Uart4_Init(u32 brr);
void Zigbee_Init(void);
void Zigbee_Send_Data(u8 *data,u16 len);
void Zigbee_Data_Analysis(void);



#endif