#include "dma.h"

#if USE_USART_DMA_RX 
 
void USARTx_DMA_Rx_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
 
	// 开启DMA时钟
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&USART1->DR;// 设置DMA源地址：串口数据寄存器地址*/
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)USART1_RX_BUF;// 内存地址(要传输的变量的指针)
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;// 方向：从外设到内存	
	DMA_InitStructure.DMA_BufferSize = USART1_REC_LEN;// DMA通道的DMA缓存的大小    
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;// 外设地址寄存器不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;	//内存地址寄存器递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;// 外设数据单位  8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;	 // 内存数据单位   8位
	// DMA模式，一次或者循环模式
	//DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;	
	DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh; // 优先级：高	
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;//DMA通道x没有设置为内存到内存传输
		   
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);// 配置DMA通道		串口1接收对应通道5	
	
	DMA_ClearFlag(DMA1_FLAG_TC5);// 清除DMA所有标志
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TE, ENABLE);//传输错误 中断屏蔽
	// 使能DMA
	DMA_Cmd (DMA1_Channel5,ENABLE);
}
#endif