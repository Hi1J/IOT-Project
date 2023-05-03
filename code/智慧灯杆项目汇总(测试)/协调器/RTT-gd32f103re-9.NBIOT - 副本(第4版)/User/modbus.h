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
			  : (x) == 4 ? &NODE2_CONNECT_STATUS\
			  : (x) == 5 ? &NODE2_LIGHT\
			  : (x) == 6 ? &NODE2_PEOPLE\
			  : 0)


int MODBUS_Data_Reporting(int *data,uint8_t len,uint16_t R_addre,uint8_t *buf);
int CRC_Verification(uint8_t *buff,int len);
int MODBUS_DATA_HANDLE(uint8_t *data);

#endif 

