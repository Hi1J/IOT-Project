#include "modbus.h"


/*
函数功能：从机主动上报 
参数：data：待发送数据 len：发送的数据长度 R_addre：寄存器地址 buf：空buf 用于储存数据帧
返回值：数据长度
备注：功能码：0x46
*/
int MODBUS_Data_Reporting(int *data,uint8_t len,uint16_t R_addre,uint8_t *buf)
{
	//uint8_t buf[9 + (len * 2)];
	uint8_t i = 0;
	int count = 0;
	uint16_t crc;
	
	//if(sizeof(buf) < (9 + (len * 2)))return ERROR;
	
	buf[0] = 0x01;
	buf[1] = 0x46;
	buf[2] = R_addre >> 8;
	buf[3] = R_addre & 255;
	buf[4] = 0x00;
	buf[5] = len;
	buf[6] = len * 2;
	for(i=0;i<len;i++)
	{
		buf[7 + (i * 2)] = *data >> 8;
		buf[8 + (i * 2)] = *data & 255;
		data ++ ;
	}
	count = 7 + (i * 2);
	crc = CRC_Verification(buf,count);
	buf[count] = crc >> 8;
	buf[count + 1] = crc & 255;
	
//	for(i = 0 ; i < count + 2 ; i ++)
//		rt_kprintf("%x ",buf[i]);
//	rt_kprintf("\n");
	
	return count + 2;
	
}
/*
函数功能：CRC校验
参数：buff：数据帧除去CRC校验后剩余的部分 len：剩余部分的长度
返回值：16位的CRC校验值
备注：
*/
int CRC_Verification(uint8_t *buff,int len)
{
	unsigned short tmp = 0xffff;
    unsigned short ret1 = 0;
   

    for (int n = 0; n < len; n++) {/*此处的n -- 要校验的位数为n个*/
        tmp = buff[n] ^ tmp;
        for (int i = 0; i < 8; i++) {  /*此处的8 -- 指每一个char类型又8bit，每bit都要处理*/
            if (tmp & 0x01) {
                tmp = tmp >> 1;
                tmp = tmp ^ 0xa001;
            }
            else {
                tmp = tmp >> 1;
            }
        }
    }
    /*CRC校验后的值*/
    //printf("%X\n", tmp);
    /*将CRC校验的高低位对换位置*/
    ret1 = tmp >> 8;
    ret1 = ret1 | (tmp << 8);
    //printf("ret: %X\n", ret1);
	return ret1;

}
