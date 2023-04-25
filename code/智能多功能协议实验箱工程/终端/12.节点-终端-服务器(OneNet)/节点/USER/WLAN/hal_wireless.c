#include "hal_wireless.h"

#if Lora_Open

uint8_t Slave_ID = 0;
WIRELESS_RX_TX wireless_data = {.rx_over = 0,.rx_count = 0,.tx_count = 0};
//给设备分配ID号
//uint8_t Slave_GetID(void)
//{
//  //产品唯一身份标识寄存器 基地址： 0x1FFF F7E8
//  uint8_t *ID96 = (uint8_t *)0x1FFFF7E8;
//	Slave_ID = CRC_8_Tab(ID96,12);//96位ID变为8位ID。
//	printf("******从机本身ID：%d******\r\n",Slave_ID);
//  return Slave_ID;
//}
/*
USART3_TX -- PB10
USART3_RX -- PB11
*/
void USART3_Config(u32 boad)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	//1.打开A端口时钟和UART2端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	//2.PB10配置为复用推挽输出模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//通用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//3.PB11配置为浮空输入模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	//4.配置USART2:RS232协议 = 1开始位+8个数据位+0个奇偶校验位 + 1个停止位
	
	USART_InitStruct.USART_BaudRate = boad;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//全双工模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//1+8 字长
	USART_Init(USART3,&USART_InitStruct);
	//串口中断的配置
	USART3_NVICConfig();
	//5.使能串口3
	USART_Cmd(USART3,ENABLE);
}
void USART3_NVICConfig(void)
{
	//配置为串口接收中断
	USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
	//配置优先级以及串口使能
	NVIC_SetPriority(USART3_IRQn,0);
	NVIC_EnableIRQ(USART3_IRQn);
}
/*
串口发送函数
puart -- USART1 2 3 UART4 5
data -- 数据首地址
len  -- 要发送数据的长度
*/
void UART_SendData(USART_TypeDef *puart,u8 *data,u16 len)
{
	u16 i=0;
	for(i=0;i<len;i++)
	{
		while(USART_GetFlagStatus(puart,USART_FLAG_TC) != SET);
		puart->DR=data[i];
	}
	while(USART_GetFlagStatus(puart,USART_FLAG_TC) != SET);
}
void Time4_Config(u16 psc,u16 arr)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct={0};
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStruct.TIM_Period=arr-1;
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc-1;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);//更新中断
	NVIC_SetPriority(TIM4_IRQn,3);
	NVIC_EnableIRQ(TIM4_IRQn);
	TIM4->EGR |= (1<<0);
	//失能定时器4
	TIM_Cmd(TIM4,ENABLE);
}

/****处理数据的接收以及判断是否接收完成代码*****/
//接收中断
void USART3_IRQHandler(void)
{
	if(USART_GetITStatus(USART3,USART_IT_RXNE))
	{
		//请接收中断标志位
		USART_ClearITPendingBit(USART3,USART_IT_RXNE);
		//保存数据
//		if(wireless_data.rx_over == 0)		//注意 zigbee时使用了，Lora没有加这句话
			wireless_data.rx_buff[wireless_data.rx_count++] = USART3->DR;
		//启动定时器并清零计数器
		TIM_SetCounter(TIM4,0);
		TIM_Cmd(TIM4,ENABLE);
	}
}

//定时器4的溢出中断
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除中断标志位
		TIM_Cmd(TIM4,DISABLE);//失能定时器4
		if(wireless_data.rx_count > 1)
		{
			wireless_data.rx_over = 1;//接收完成标志置1
		}
	}
}

#endif

