#include "NVIC.h"
u8 k;
void KEYD_INT_INIT(void) {
	NVIC_InitTypeDef  NVIC_InitStruct;	
	EXTI_InitTypeDef  EXTI_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);     
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource3);

	EXTI_InitStruct.EXTI_Line = EXTI_Line3;  
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;             
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;     
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;  



	EXTI_Init(& EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;     
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2; 
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;     
	NVIC_Init(& NVIC_InitStruct);

	
}
void  EXTI3_IRQHandler(void) {
		if (EXTI_GetITStatus(EXTI_Line3) != RESET) {
				k=1;
			EXTI_ClearITPendingBit(EXTI_Line3);   //清除 LINE 上的中断标志位		
		}
	}
