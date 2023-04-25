#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
//#include "tim4.h"
#include "pwm.h"
#include "steering_control.h"


u8 key = NULL;
//智能车

//左转最大 1700
//中间值 1515
//右转最大 1320

int main(void)
{
	int c = 0xFFFF;
	Usart1_Init(115200);//串口初始化
	printf("串口1初始化完成！\r\n");
	TIM3_PWM_Init(2500,72);//2.5ms
//	TIM4_CH3_Init(72,2500);//2.5ms
//	
//	TIM4->CCR3 = 	1100;
//	TIM_SetCompare3(TIM3,1515); 
	TIM3->CCR3 = 1515;
	while(1)
	{
//		if(RX_OK == ON)
//		{
//			for(u8 i=0;i<6;i++)
//			{
//				printf("%x ",RX_BUF[i]);
//			}
//			printf("\r\n");
//			RX_OK = OFF;
//		}
//		Car_Steering(key);
//		while(c--);
//		c = 0xFFFF;
		
	}
	
	

}
