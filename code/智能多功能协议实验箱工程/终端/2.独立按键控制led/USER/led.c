#include "led.h"



void LED_Init()
{
	 GPIO_InitTypeDef GPIO_InitStructure;
	  //时钟
	 RCC_APB2PeriphClockCmd(LED1_7_CLK|LED8_CLK,ENABLE);
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//推挽
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//50MHz
	 GPIO_InitStructure.GPIO_Pin = LED1_Pin | LED2_Pin | LED3_Pin | LED4_Pin | LED5_Pin | LED6_Pin | LED7_Pin;
	 GPIO_Init(LED1_7_Port, &GPIO_InitStructure);

	 
	 GPIO_InitStructure.GPIO_Pin = LED8_Pin;
	 GPIO_Init(LED8_Port, &GPIO_InitStructure); 
	 
	 
	 //让所有灯先为灭状态
	 LED1_7_Port->ODR = 0x7F;
	 LED8_Port->ODR |= 1 << 13;



}