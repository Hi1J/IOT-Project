#include "stm32f4xx.h"
#include "tim9.h"


//闪灯程序
int main(void)
{
	
	TIM9_CH1_Init(16800,10000,5000);//pwm:1/2  500ms高电平 500ms低电平
	

	while(1)
	{
		
	}
}

