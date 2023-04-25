#include "spi.h"

void SPI1_Init(void)
{
	/*GPIOA配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_5;//MOSI  SCK
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;//MISO
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*SPI1配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	SPI_InitTypeDef SPI_InitStructure;
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线双向
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;//主SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; // 8位帧结构
	SPI_InitStructure.SPI_CPOL = I2S_CPOL_Low;//总线空闲时为低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//数据捕获第一个时钟沿
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//NSS使用软件管理
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4; //四分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;//高位在前
	SPI_Init(SPI1,&SPI_InitStructure);
	
	SPI_Cmd(SPI1,ENABLE);//使能SPI1
	
}

