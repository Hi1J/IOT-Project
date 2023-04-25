#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "usart.h"

//USART收发
int main()
{
	systick_config();//时钟初始化
	usart0_init(115200U);
	printf("串口0初始化完成！\r\n");
	usart1_init(115200U);
	USART1_Send_Data_buf("串口1初始化完成！\r\n",sizeof("串口2初始化完成！\r\n"));
	while(1)
	{
		
	}
}

