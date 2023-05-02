#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>
#include "app.h"

#define Slave_Address 0x01

//#define slave_reg(x) 


//extern int *reg_0001;
//extern int *reg_0002;
//extern int *reg_0003;
//extern int *reg_0004;
//extern int *reg_0005;
//extern int *reg_0006;

int MODBUS_Data_Reporting(int *data,uint8_t len,uint16_t R_addre,uint8_t *buf);
int CRC_Verification(uint8_t *buff,int len);
int MODBUS_DATA_HANDLE(uint8_t *data);

#endif 

