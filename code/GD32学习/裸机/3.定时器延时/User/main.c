#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include "usart.h"
#include "timer.h"

//USART收发
int main()
{
	systick_config();//时钟初始化
	usart0_init(115200U);
	printf("串口0初始化完成！\r\n");
	TIM6_Delay_Init();
	
	printf("标记1\r\n");
	TIM6_Delay_ms(500);
	printf("标记2\r\n");
	
	
	while(1)
	{
//		TIM6_Delay_ms(500);
//		printf("yes\r\n");
		
	}
}

