#include "modbus.h"


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
