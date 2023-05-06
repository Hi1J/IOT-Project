/*                                                    
	                                                    使用说明
												服务器作主机，单片机作从机
								该程序中的MODBUS协议并不完善，也并不严谨，其实现的功能只能适用于本程序
								用户不可直接调用本程序中的MODBUS协议去执行其他工程，否则会导致各种错误

*/
#include "modbus.h"

/*
函数功能：寄存器地址转换为数值
参数：addr：寄存器地址
返回值：对应数值
备注：
*/
int number_count(int addr)
{
	return ((((addr & 61440) >> 12) * 1000) + (((addr & 3840 ) >> 8) * 100) + ((addr & 240) >> 4) * 10) + (addr & 15);
}

/*
函数功能：H03功能码处理
参数：data：接收到的数据
返回值：void
备注：03功能码的作用是读取保存寄存器的值 
	 单片机作为从机需响应服务器的查询需求
*/
void MODBUS_H03(uint8_t *data)
{
	uint8_t send_buf[128] = {0};
	uint16_t i = 0;
	int number = 0;
	int crc = 0;
	send_buf[0] = Slave_Address;
	send_buf[1] = 0x03;
	send_buf[2] = 2 * data[5];//字节数
	number = number_count((data[2] << 8) | data[3]) + 1;
	for(i=0;i<data[5];i++)
	{
		send_buf[3 + (i * 2)] = REG(number + i) >> 8;
		send_buf[4 + (i * 2)] = REG(number + i) & 255;
	}
	crc = CRC_Verification(send_buf,3 + i * 2);
	send_buf[3 + i * 2] = crc >> 8;
	send_buf[4 + i * 2] = crc & 255;
	
	for(number=0;number<5+i*2;number++)
	{
		rt_thread_mdelay(1);
		NB73_Send_A_Data(send_buf[number]);
	}
}
/*
函数功能：H06功能码处理
参数：data：接收到的数据
返回值：void
备注：03功能码的作用是修改单个寄存器的值
	 单片机作为从机需执行服务器的修改要求
*/
void MODBUS_H06(uint8_t *data)
{
	int number = 0;
	int i = 0;
	for(i=0;i<8;i++)
	{
		rt_thread_mdelay(1);
		NB73_Send_A_Data(data[i]);
	}
	number = number_count((data[2] << 8) | data[3]) + 1;
	REG(number) = (data[4] <<8) | data[5];
	switch(number)
	{
		case 7:
			LED1_Control_do = 1;
			rt_sem_release(LED_Control);
			break;
		case 8:
			LED2_Control_do = 1;
			rt_sem_release(LED_Control);
			break;
		default:
			break;
	}
	rt_kprintf("修改寄存器%d的值：%d\n",number,REG(number));//Debug
	//rt_kprintf("验证：%d\n",NODE1_LED_CONTROL);
}
/*
函数功能：MODBUS处理数据
参数：data：接收到的数据
返回值：void
备注：该处理函数功能较少 只符合本程序的功能
*/
int MODBUS_DATA_HANDLE(uint8_t *data)
{
	
	if(data[0] == Slave_Address)
	{
		
		switch(data[1])
		{
			case 0x03://03功能码(读取保存寄存器的值)
				MODBUS_H03(data);
				break;
			case 0x06://06功能码(修改单个寄存器的值)
				MODBUS_H06(data);
				break;
			default:
				break;
		}
	
	}
	return 0;
}




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
