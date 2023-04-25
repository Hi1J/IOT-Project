#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "stdio.h"
#include "string.h"


//DMA+串口接收不定长数据
int main()
{
	USART_Config();//串口初始化
	printf("串口1初始化完成！\r\n");
	
	while(1)
	{
		if(USART1_RX_STA & 0xC000)//判断接收是否完成
		{
			printf("接收到的数据为：\r\n%s\r\n",USART1_RX_BUF);
			memset(USART1_RX_BUF,0,USART1_REC_LEN);//清空接收数组数据
			USART1_RX_STA = 0;//接收标志位清零
		}
		
	
	}
	
	
}