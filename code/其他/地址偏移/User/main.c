#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "usart.h"
#include "string.h"
#include "stddef.h"

/*���ܣ�ȡ��ֵַ ȡֵ���ͣ�char*  ������ x:��ַ y :ƫ��  */
#define CharValue(x,y)  (*(char **)(x + y))

/*
���ܣ���ָ��ƫ�Ƶ�ַ���Ӧ��ַ��ֵ���в��� 
������type:ƫ�ƺ��ַ��Ӧ���������� pinter:ָ�� offsetaddr:ƫ�Ƶ�ַ��
��ע���ò���ΪΣ�ղ���
ƫ�����������ƫ��ǰ��ַ��Ӧ�������ڴ��д洢�ĵ�ַƫ����
(��ͬ��Ӳ�������������ϵͳ��Ӧ��ƫ�������ܲ�ͬ),
�ұ��뱣֤ƫ�ƺ�ĵ�ַ���������
*/
#define REG_OFFSET(type,pinter,offsetaddr)  (*(type *)((uint32_t)(pinter) + offsetaddr))



/*�Զ����ַ����*/
#define BEST                              (*(volatile uint16_t *)((uint32_t)(0x20000010)))
#define TEM                               (*(volatile uint16_t *)((uint32_t)(0x20000012)))
#define HUM                               (*(volatile uint16_t *)((uint32_t)(0x20000014)))
#define EC                                (*(volatile uint16_t *)((uint32_t)(0x20000016)))
#define PH                                (*(volatile uint16_t *)((uint32_t)(0x20000018)))
	
#define MODBUS_REG(addr)                  (*(volatile uint16_t *)((uint32_t)(&BEST) + addr * 2))

#define Uint8Value(x,y) (*(uint8_t *)(uint32_t)(x) + y)


struct TEST1
{
	uint8_t aa;
	uint8_t bb;
	uint8_t cc;
	uint8_t dd;
	uint8_t ee;
	uint8_t ff;
	uint8_t gg;

};


struct TEST2
{
	uint16_t A;
	uint32_t B;
	uint8_t  C;
	uint8_t  D;

};

struct TEST1 test1 = {0,0,0,0,0,0,0};
struct TEST2 test2 = {0,0,0,0};		 

//USART�շ�
int main()
{
	
//	int a = 32;
//	uint8_t number = 0;
	
	systick_config();//ʱ�ӳ�ʼ��
	
	usart0_init(115200U);
	printf("usart0 init successed..\r\n");
	
	printf("0x123+0x456=0x%x\r\n",0x125+0x456);
	
	TEM = 36;
	HUM = 24;
	EC = 58;
	PH = 74;
	
	
	MODBUS_REG(0x0001) = 78;
	MODBUS_REG(0x0002) = 66;
	
	
	
	printf("\r\n");
	
	printf("TEM:%d\r\nHUM:%d\r\nEC :%d\r\nPH:%d\r\n",TEM,HUM,EC + 30,PH * 2);
	
	printf("\r\n");
	
	TEM = 7;
	HUM = 8;
	
	printf("TEM:%d\r\nHUM:%d\r\nEC :%d\r\nPH:%d\r\n",TEM,HUM,EC,PH);
	
	printf("\r\n");
	
	printf("%x\r\n%x\r\n%x\r\n%x\r\n",(uint32_t)&test2.A,(uint32_t)&test2.B,(uint32_t)&test2.C,(uint32_t)&test2.D);
	
	printf("\r\n");
	
	
	printf("%d %d %d %d\r\n",offsetof(struct TEST2,A),offsetof(struct TEST2,B),offsetof(struct TEST2,C),offsetof(struct TEST2,D));
	
	REG_OFFSET(uint16_t,&test2,offsetof(struct TEST2,A)) = 77;
	REG_OFFSET(uint32_t,&test2,offsetof(struct TEST2,B)) = 36485;
	REG_OFFSET(uint8_t,&test2,offsetof(struct TEST2,C)) = 23;
	REG_OFFSET(uint8_t,&test2,offsetof(struct TEST2,D)) = 'u';
	
	printf("A:%d\r\nB:%d\r\nC:%d\r\nD:%c\r\n",test2.A,test2.B,test2.C,test2.D);
	
	while(1)
	{
//		for(uint8_t i = 0;i < 7;i ++)
//		{
//			REG_OFFSET(uint8_t,&test1,i * sizeof(uint8_t)) = number++;
//		}
//		printf("\r\n");
//		printf("aa:%d\r\n",test1.aa);
//		printf("bb:%d\r\n",test1.bb);
//		printf("cc:%d\r\n",test1.cc);
//		printf("dd:%d\r\n",test1.dd);
//		printf("ee:%d\r\n",test1.ee);
//		printf("ff:%d\r\n",test1.ff);
//		printf("gg:%d\r\n",test1.gg);
//		printf("\r\n");
//		
//		delay_1ms(2000);
		
	}
}

