#ifndef __MODBUS_H_
#define __MODBUS_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>
#include "app.h"

#define Slave_Address 0x01


/*           ����Ĵ�������
˵����      �Ĵ���        ֵ
            40001      ��1״̬
			40002      ��1��ǿ
			40003      ��1����
			40004      ��2״̬
			40005      ��2��ǿ
			40006      ��2����
*/
#define REG     *reg
//������int* reg(int x)һ��
#define reg(x) ((x) == 1 ? &NODE1_CONNECT_STATUS\
			  : (x) == 2 ? &NODE1_LIGHT\
			  : (x) == 3 ? &NODE1_PEOPLE\
			  : (x) == 4 ? &NODE1_LED_CONTROL\
			  : (x) == 5 ? &NODE2_CONNECT_STATUS\
			  : (x) == 6 ? &NODE2_LIGHT\
			  : (x) == 7 ? &NODE2_PEOPLE\
			  : (x) == 8 ? &NODE2_LED_CONTROL\
			  : (x) == 9 ? &NODE3_CONNECT_STATUS\
			  : (x) == 10 ? &NODE3_LIGHT\
			  : (x) == 11 ? &NODE3_PEOPLE\
			  : (x) == 12 ? &NODE3_LED_CONTROL\
			  : 0)


int MODBUS_Data_Reporting(int *data,uint8_t len,uint16_t R_addre,uint8_t *buf);
int CRC_Verification(uint8_t *buff,int len);
int MODBUS_DATA_HANDLE(uint8_t *data);

#endif 

