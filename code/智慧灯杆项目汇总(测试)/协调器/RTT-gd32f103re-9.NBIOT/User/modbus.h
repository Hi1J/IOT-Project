#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>
#include "app.h"

#define Slave_Address 0x01


/*           虚拟寄存器创建
说明：      寄存器        值
            40001      灯1状态
			40002      灯1光强
			40003      灯1人流
			40004      灯2状态
			40005      灯2光强
			40006      灯2人流
*/
#define REG     *reg
//作用与int* reg(int x)一致
#define reg(x) ((x) == 1 ? &NODE1_CONNECT_STATUS\
			  : (x) == 2 ? &NODE1_LIGHT\
			  : (x) == 3 ? &NODE1_PEOPLE\
			  : (x) == 4 ? &NODE2_CONNECT_STATUS\
			  : (x) == 5 ? &NODE2_LIGHT\
			  : (x) == 6 ? &NODE2_PEOPLE\
			  : 0)


int MODBUS_Data_Reporting(int *data,uint8_t len,uint16_t R_addre,uint8_t *buf);
int CRC_Verification(uint8_t *buff,int len);
int MODBUS_DATA_HANDLE(uint8_t *data);

#endif 

