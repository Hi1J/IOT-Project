#include "led.h"


void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_LED,ENABLE);
  GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin=LED1|LED2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	
	GPIO_Init(LED,&GPIO_InitStructure);
	

}

//void LED_TOGGLE(uint16_t LEDx) //LED·­×ªº¯Êý
//{
//	if(LEDx==LED1){
//		GPIO_WriteBit(LED,LED1,(BitAction)(1-GPIO_ReadInputDataBit(LED,LED1)));
//	}
//	if(LEDx==LED2){
//		GPIO_WriteBit(LED,LED2,(BitAction)(1-GPIO_ReadInputDataBit(LED,LED2)));
//	}
//}