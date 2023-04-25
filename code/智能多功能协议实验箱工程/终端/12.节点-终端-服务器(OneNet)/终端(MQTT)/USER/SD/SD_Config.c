#include "SD_Config.h"

/*
SPI2接口
SD_CS -- PD12
*/
void SD_GPIOConfig(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//SD_CS管脚 -- PD12
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	//CS管脚拉高
	GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET);
	//SPI1初始化
	SPI2_Init();
}





