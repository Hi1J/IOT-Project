#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"


int main(void)
{
	u8 i;
	u16 value = 0;
	Usart1_Init(115200);
	printf("串口1初始化完成！\r\n");
	Motor_Gpio_Init();
	delay_ms(200);
	while(1)
	{
		if(i<100)
		{
			Motorcw(1);//正转
			delay_ms(10);//转一圈需要的时间
			i++;
		}
		else
		{
			Motorcw(0);//反转
			delay_ms(10);
			i++;
			if(i>=200)i=0;
		
		}
		
	}

}