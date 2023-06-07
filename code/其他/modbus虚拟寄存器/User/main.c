#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "usart.h"
#include "string.h"
#include "stddef.h"


#define REG_MODBUS(addr)             (*(volatile uint16_t *)((uint32_t)(0x20000010) + addr * 2))

#define HUM                          REG_MODBUS(0x0000)
#define TEM                          REG_MODBUS(0x0001)
#define EC                           REG_MODBUS(0x0002)
#define PH                           REG_MODBUS(0x0003)	

//MODBUS ÐéÄâ¼Ä´æÆ÷
int main()
{

	uint8_t number = 0;
	
	systick_config();//Ê±ÖÓ³õÊ¼»¯
	
	usart0_init(115200U);
	printf("usart0 init successed..\r\n");
	
	HUM = 32;
	TEM = 64;
	EC  = 280;
	PH  = 648;
	
	printf("\r\n");
	
	printf("%d\r\n",sizeof(HUM));
	
	printf("%x\r\n%x\r\n%x\r\n%x\r\n",(uint32_t)&HUM,(uint32_t)&TEM,(uint32_t)&EC,(uint32_t)&PH);
	
	printf("\r\n");
	
	printf("HUM:%d\r\nTEM:%d\r\nEC :%d\r\nPH :%d\r\n",HUM * 2,TEM + 20,EC / 10,PH);
	
	REG_MODBUS(0x0000) = 1024;
	REG_MODBUS(0x0001) = 77;
	EC = 556;
	REG_MODBUS(0x0003) = 580;
	
	HUM = 98;
	
	REG_MODBUS(0x0002) = (7 << 8) | 23;
	
	printf("\r\n");
	
	printf("HUM:%d\r\nTEM:%d\r\nEC :%d\r\nPH :%d\r\n",HUM,TEM,EC,PH);
	
	printf("\r\n");
	
	
	while(1)
	{
		for(uint8_t i = 0;i < 4;i ++)
		{
			REG_MODBUS(i) = number++;
		}
		printf("\r\n");
	
		printf("HUM:%d\r\nTEM:%d\r\nEC :%d\r\nPH :%d\r\n",HUM,TEM,EC,PH);
		
		printf("\r\n");
		
		delay_1ms(500);
		
	}
}


