#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "adc.h"

//烟雾传感器MQ2
int main(void)
{
	u16 value = 0;//设置一个值来存放MQ2
	Usart1_Init(115200);//串口初始化
	ADC1_CH9_Init();//ADC1通道9初始化
	
	while(1)
	{
		value = MQ_2_GetValue();//获取数据
		printf("烟雾传感器：%d\r\n",value);//打印
		delay_s(1);
			
		
		
	}
	
	

}
