#include "usart.h"

u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.

u16 USART1_RX_STA=0;       //接收状态标记	




//串口初始化
void USART_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
 
	// 打开串口GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	// 打开串口外设的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
 
	// 将USART Tx的GPIO配置为推挽复用模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  // 将USART Rx的GPIO配置为浮空输入模式
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// 配置串口的工作参数
	// 配置波特率
	USART_InitStructure.USART_BaudRate = General_BaudRate;
	// 配置 针数据字长
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// 配置停止位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// 配置校验位
	USART_InitStructure.USART_Parity = USART_Parity_No ;
	// 配置硬件流控制
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// 配置工作模式，收发一起
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// 完成串口的初始化配置
	USART_Init(USART1, &USART_InitStructure);
	// 串口中断优先级配置
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
	
#if USE_USART_DMA_RX 
	// 开启 串口空闲IDEL 中断
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  
  // 开启串口DMA接收
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE); 
	/* 使能串口DMA */
	USARTx_DMA_Rx_Config();
#else
	// 使能串口接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	
#endif
 
#if USE_USART_DMA_TX 
	// 开启串口DMA发送
//	USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE); 
	USARTx_DMA_Tx_Config();
#endif
 
	// 使能串口
	USART_Cmd(USART1, ENABLE);	    
}




//重定向printf函数
int fputc(int ch,FILE *fp)
{
	while((USART1 -> SR & ( 1 << 7 )) == 0);//如果SR寄存器中的第六位TC为0，表示数据未发送完成，则会一直在此等待
	USART1 -> DR = ch;//如果发送完成，则把数据放入DR寄存器中
	return ch;
}




#if USE_USART_DMA_RX
void Receive_DataPack(void)
{
	/* 接收的数据长度 */
	uint32_t buff_length;
	
	/* 关闭DMA ，防止干扰 */
	DMA_Cmd(DMA1_Channel5, DISABLE);  /* 暂时关闭dma，数据尚未处理 */ 
	
	/* 清DMA标志位 */
	DMA_ClearFlag( DMA1_FLAG_TC5 );  
	
	/* 获取接收到的数据长度 单位为字节*/
	buff_length = USART1_REC_LEN - DMA_GetCurrDataCounter(DMA1_Channel5);
  
    /* 获取数据长度 */
    USART1_RX_STA = buff_length;
 
	printf("buff_length = %d\n ",buff_length);
	
	/* 重新赋值计数值，必须大于等于最大可能接收到的数据帧数目 */
	DMA1_Channel5->CNDTR = USART1_REC_LEN;    
  
	/* 此处应该在处理完数据再打开，如在 DataPack_Process() 打开*/
	DMA_Cmd(DMA1_Channel5, ENABLE);      
	
	/* (OS)给出信号 ，发送接收到新数据标志，供前台程序查询 */
	
    /* 标记接收完成，在 DataPack_Handle 处理*/
    USART1_RX_STA |= 0xC000;
  
    /* 
    DMA 开启，等待数据。注意，如果中断发送数据帧的速率很快，MCU来不及处理此次接收到的数据，
    中断又发来数据的话，这里不能开启，否则数据会被覆盖。有2种方式解决：
 
    1. 在重新开启接收DMA通道之前，将Rx_Buf缓冲区里面的数据复制到另外一个数组中，
    然后再开启DMA，然后马上处理复制出来的数据。
 
    2. 建立双缓冲，重新配置DMA_MemoryBaseAddr的缓冲区地址，那么下次接收到的数据就会
    保存到新的缓冲区中，不至于被覆盖。
	*/
}
#endif



void USART1_IRQHandler(void)
{
#if USE_USART_DMA_RX
	/* 使用串口DMA */
	if(USART_GetITStatus(USART1,USART_IT_IDLE)!=RESET)
	{		
		/* 接收数据 */
		Receive_DataPack();
		// 清除空闲中断标志位
		USART_ReceiveData(USART1);
	}	
#else
  /* 接收中断 */
	if(USART_GetITStatus(DEBUG_USARTx,USART_IT_RXNE)!=RESET)
	{		
    Receive_DataPack();
	}
#endif
}






