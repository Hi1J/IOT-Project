#include "hal_wireless.h"
//#include "task.h"

WIRELESS_RX_TX wireless_data = {.rx_over = 0,.rx_count = 0,.tx_count = 0};
/*
UART4_TX -- PC10
UART4_RX -- PC11
*/
void UART4_Config(u32 boad)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	//1.打开c端口时钟和UART2端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);
	//2.Pc10配置为复用推挽输出模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//通用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//速度50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_10;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	//3.Pc11配置为浮空输入模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	//4.配置USART4:RS232协议 = 1开始位+8个数据位+0个奇偶校验位 + 1个停止位
	
	USART_InitStruct.USART_BaudRate = boad;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//全双工模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//1+8 字长
	USART_Init(UART4, &USART_InitStruct);
	//串口中断的配置
	UART4_NVICConfig();
	//5.使能串口4
	USART_Cmd(UART4,ENABLE);
}
void UART4_NVICConfig(void)
{
	//配置为串口接收中断
	USART_ITConfig(UART4, USART_IT_RXNE,ENABLE);
	//配置优先级以及串口使能
	NVIC_SetPriority(UART4_IRQn,0);
	NVIC_EnableIRQ(UART4_IRQn);
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
	TIM_Cmd(TIM4,DISABLE);
}

/****处理数据的接收以及判断是否接收完成代码*****/
//接收中断
void UART4_IRQHandler(void)
{
	if(USART_GetITStatus(UART4, USART_IT_RXNE))
	{
		//请接收中断标志位
		USART_ClearITPendingBit(UART4, USART_IT_RXNE);
		//保存数据
//		if(wireless_data.rx_over == 0)		//注意 zigbee时使用了，Lora没有加这句话
		if(wireless_data.rx_over == 0){
			
			if(wireless_data.rx_count == 0)//当一个帧数据的第一个数据到来时 开启TIM4
			{
				TIM_SetCounter(TIM4,0);
				TIM_Cmd(TIM4,ENABLE);
			}
			wireless_data.rx_buff[wireless_data.rx_count++] = UART4->DR;
		}
			
		//启动定时器并清零计数器
		
		
	}
}

//定时器4的溢出中断
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update)==SET)
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除中断标志位
		TIM_Cmd(TIM4,DISABLE);//失能定时器4
//		wireless_data.rx_over = 1;//接收完成标志置1
		
		if(wireless_data.rx_count > 1)
		{
			wireless_data.rx_over = 1;//接收完成标志置1
		}
		else //干扰数据，滤除掉
		{
			wireless_data.rx_count = 0;
		}
	}
}


