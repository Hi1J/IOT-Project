#include "spi.h"

/*
FLASH_CS   -- PD11
FLASH_SCK  -- SPI2_SCK  - PB13 -- 映射功能
FLASH_MISO -- SPI2_MISO - PB14 -- 映射功能
FLASH_MOSI -- SPI2_MOSI - PB15 -- 映射功能
//下降沿发送，上升沿接收
*/

void SPI2_Init(void)
{
	//时钟设置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	//GPIOB配置
	GPIO_InitTypeDef x;
	x.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	x.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_15;//SCK MOSI
	x.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&x);
	x.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	x.GPIO_Pin = GPIO_Pin_14;//MISO
	GPIO_Init(GPIOB,&x);
	
	//SPI2配置
	SPI_InitTypeDef y;
	y.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//四分频
	y.SPI_CPHA = SPI_CPHA_1Edge;//第一个时钟沿采集
	y.SPI_CPOL = SPI_CPOL_Low;//悬空时为低电平
	y.SPI_CRCPolynomial = 7;//无CRC校验
	y.SPI_DataSize = SPI_DataSize_8b;//8位数据位
	y.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//双线双向全双工
	y.SPI_FirstBit = SPI_FirstBit_MSB;//高位在前
	y.SPI_Mode = SPI_Mode_Master;//主机模式
	y.SPI_NSS = SPI_NSS_Soft;//NSS由软件控制
	SPI_Init(SPI2,&y);
	
	//使能SPI2
	SPI_Cmd(SPI2,ENABLE);

}
//SPI2发送和接收
u8 SPI2_Send_Recv(u8 data)
{
	//等待发送完成（发送前防止DR中还有数据未发送）
	while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)));
	//发送
	SPI_I2S_SendData(SPI2,data);
	//等待接收完成
	while(!(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)));
	//返回接收到的数据
	return SPI_I2S_ReceiveData(SPI2);

}
