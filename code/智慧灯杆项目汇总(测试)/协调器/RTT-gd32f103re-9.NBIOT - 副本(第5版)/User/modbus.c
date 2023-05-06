/*                                                    
	                                                    ʹ��˵��
												����������������Ƭ�����ӻ�
								�ó����е�MODBUSЭ�鲢�����ƣ�Ҳ�����Ͻ�����ʵ�ֵĹ���ֻ�������ڱ�����
								�û�����ֱ�ӵ��ñ������е�MODBUSЭ��ȥִ���������̣�����ᵼ�¸��ִ���

*/
#include "modbus.h"

/*
�������ܣ��Ĵ�����ַת��Ϊ��ֵ
������addr���Ĵ�����ַ
����ֵ����Ӧ��ֵ
��ע��
*/
int number_count(int addr)
{
	return ((((addr & 61440) >> 12) * 1000) + (((addr & 3840 ) >> 8) * 100) + ((addr & 240) >> 4) * 10) + (addr & 15);
}

/*
�������ܣ�H03�����봦��
������data�����յ�������
����ֵ��void
��ע��03������������Ƕ�ȡ����Ĵ�����ֵ 
	 ��Ƭ����Ϊ�ӻ�����Ӧ�������Ĳ�ѯ����
*/
void MODBUS_H03(uint8_t *data)
{
	uint8_t send_buf[128] = {0};
	uint16_t i = 0;
	int number = 0;
	int crc = 0;
	send_buf[0] = Slave_Address;
	send_buf[1] = 0x03;
	send_buf[2] = 2 * data[5];//�ֽ���
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
�������ܣ�H06�����봦��
������data�����յ�������
����ֵ��void
��ע��03��������������޸ĵ����Ĵ�����ֵ
	 ��Ƭ����Ϊ�ӻ���ִ�з��������޸�Ҫ��
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
	rt_kprintf("�޸ļĴ���%d��ֵ��%d\n",number,REG(number));//Debug
	//rt_kprintf("��֤��%d\n",NODE1_LED_CONTROL);
}
/*
�������ܣ�MODBUS��������
������data�����յ�������
����ֵ��void
��ע���ô��������ܽ��� ֻ���ϱ�����Ĺ���
*/
int MODBUS_DATA_HANDLE(uint8_t *data)
{
	
	if(data[0] == Slave_Address)
	{
		
		switch(data[1])
		{
			case 0x03://03������(��ȡ����Ĵ�����ֵ)
				MODBUS_H03(data);
				break;
			case 0x06://06������(�޸ĵ����Ĵ�����ֵ)
				MODBUS_H06(data);
				break;
			default:
				break;
		}
	
	}
	return 0;
}




/*
�������ܣ��ӻ������ϱ� 
������data������������ len�����͵����ݳ��� R_addre���Ĵ�����ַ buf����buf ���ڴ�������֡
����ֵ�����ݳ���
��ע�������룺0x46
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
�������ܣ�CRCУ��
������buff������֡��ȥCRCУ���ʣ��Ĳ��� len��ʣ�ಿ�ֵĳ���
����ֵ��16λ��CRCУ��ֵ
��ע��
*/
int CRC_Verification(uint8_t *buff,int len)
{
	unsigned short tmp = 0xffff;
    unsigned short ret1 = 0;
   

    for (int n = 0; n < len; n++) {/*�˴���n -- ҪУ���λ��Ϊn��*/
        tmp = buff[n] ^ tmp;
        for (int i = 0; i < 8; i++) {  /*�˴���8 -- ָÿһ��char������8bit��ÿbit��Ҫ����*/
            if (tmp & 0x01) {
                tmp = tmp >> 1;
                tmp = tmp ^ 0xa001;
            }
            else {
                tmp = tmp >> 1;
            }
        }
    }
    /*CRCУ����ֵ*/
    //printf("%X\n", tmp);
    /*��CRCУ��ĸߵ�λ�Ի�λ��*/
    ret1 = tmp >> 8;
    ret1 = ret1 | (tmp << 8);
    //printf("ret: %X\n", ret1);
	return ret1;

}
