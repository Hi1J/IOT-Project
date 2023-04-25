#include "stm32f10x.h"
#include "delay.h"
#include "tim6.h"
#include "usart.h"
#include "string.h"

//TIM6协助串口接收不等长数据 

int main()
{
	//根据波特率115200 可知1s可传输11520个字节 故10ms可传输115个字节 针对本程序 完全够用
	TIM6_Inter_Init(7200,200);//开启20ms中断
	USART1_Config();//串口初始化
	printf("串口1初始化成功！\r\n");
	USART1_RX_STA=0;	//串口1数据接收标志位清零
	

	while(1)
	{
		if(USART1_RX_STA & 0x8000)//接收到数据
		{
			printf("接收到的数据为(除去最后一行)：\r\n%s\r\n",USART1_RX_BUF);
			memset(USART1_RX_BUF,0,USART1_REC_LEN);//清空接收数组数据
			USART1_RX_STA=0;	//串口1数据接收标志位清零
		
		}
		
	
	}
	
	
}