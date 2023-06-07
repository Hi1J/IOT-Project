#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "usart.h"
#include "string.h"
#include "stddef.h"

/*功能：取地址值 取值类型：char*  参数： x:地址 y :偏移  */
#define CharValue(x,y)  (*(char **)(x + y))

/*
功能：对指针偏移地址后对应地址的值进行操作 
参数：type:偏移后地址对应的数据类型 pinter:指针 offsetaddr:偏移地址量
备注：该操作为危险操作
偏移量必须符合偏移前地址对应数据在内存中存储的地址偏移量
(不同的硬件控制器或操作系统对应的偏移量可能不同),
且必须保证偏移后的地址是有意义的
*/
#define REG_OFFSET(type,pinter,offsetaddr)  (*(type *)((uint32_t)(pinter) + offsetaddr))



/*自定义地址变量*/
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

//USART收发
int main()
{
	
//	int a = 32;
//	uint8_t number = 0;
	
	systick_config();//时钟初始化
	
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

