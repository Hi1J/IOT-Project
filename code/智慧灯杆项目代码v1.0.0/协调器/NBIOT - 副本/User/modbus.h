#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>

int MODBUS_Data_Reporting(int *data,uint8_t len,uint16_t R_addre,uint8_t *buf);
int CRC_Verification(uint8_t *buff,int len);

#endif 

