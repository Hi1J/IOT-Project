#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "new.h"

//YJ通信协议
int main(void)
{
	Usart1_Init(115200);//串口初始化
	printf("串口1初始化完成\r\n");
	YJ_Init();//通信协议初始化
	while(1)
	{
		if(YJ_RX_Aframe_OK == 1)
		{
			YJ_Data_Analysis();
		}
		
		
	}
	
	

}
