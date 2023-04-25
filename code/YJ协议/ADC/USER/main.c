#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "string.h"



int main()
{
	


	USART1_Config();
	USART2_Config();
	printf("usart1初始化完成!\r\n");

	
	while(1)
	{
		if(rx_ok == 1)
		{
			for(u8 i=0;i<8;i++)
			{
				printf("%x ",USART2_RX_BUF[i]);
			}
			printf("\r\n");
			rx_ok = 0;
			memset(USART2_RX_BUF,0,sizeof(USART2_RX_BUF));
		}
		
	
	}
	
	
}