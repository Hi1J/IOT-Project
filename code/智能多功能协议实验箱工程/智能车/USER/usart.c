#include "usart.h"

u8 RX_BUF[RX_MAX];
u8 RX_OK = 0;

void Usart1_Init(u32 b)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1,ENABLE);//开时钟
	
	//配置GPIOA
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//发送
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//接收
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	
	
	
	
	//配置串口
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = b;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件控制流
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//发送和接收
	USART_InitStructure.USART_Parity = USART_Parity_No;//不使用奇偶检验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8个数据位
	
	USART_Init(USART1,&USART_InitStructure);//写入寄存器
	
	
	//配置NVIC
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能IRQ通道
	NVIC_Init(&NVIC_InitStructure);//写入寄存器
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);//开启接收中断
	
	USART_Cmd(USART1,ENABLE);//使能串口1
	

}
//重写printf
int fputc(int ch,FILE *fp)
{
	USART1->DR = ch;
	while(!(USART1-> SR & (1 << 7)));
	return ch;
}


/*
	
格式规范：

一帧数据：

  位：        1            2          3           4           5         6
功能：    起始位高位   起始位低位   舵机状态    电机状态    速度高位   速度低位


功能详解：

起始位：0xFAFC

舵机状态：  00：向前   01：左转    10：右转    11：停止

电机状态：  00：加速   01：减速

速度：用两个字节 16位数据来存储
					

*/



//USART1串口中断函数
void USART1_IRQHandler(void)
{
	u8 res;//临时储存接收值
//	static u8 RX_Count = 2;//BUF计数初始值
//	static u8 RX_Flag = OFF;//开始接收标志位
//	static u8 SartBit_H = NULL;//起始位高位缓存区
	
	if(USART1->SR & (1 << 5))
	{
		USART1->SR &= ~(1 << 5);//清空中断标志位
		
		res = USART1->DR;//读取数据
		key = res;
//		if(RX_OK == OFF)//一次接收一帧数据 
//		{
//			if((RX_Flag == ON) && (RX_Count < RX_MAX))//如果数据帧格式正确 开始接收 (把此代码放前的目的是防止执行不必要的判断)
//				RX_BUF[RX_Count++] = res;//将数值存入数组
//			else if((res == (SartBit >> 8)) && (RX_Flag == OFF) && (SartBit_H == NULL))//如果第一字节是起始位高8位 
//				SartBit_H = res;//如果第一位是起始位高位 将其存入缓存区
//			else if((res == (SartBit & 0x00FF)) && (RX_Flag == OFF) && (SartBit_H == (SartBit >> 8)))//如果第二字节是起始位低8位
//				{
//					RX_BUF[0] = SartBit_H;//起始位高位高位存入buf[0]
//					RX_BUF[1] = res;//起始位低位存入buf[1]
//					SartBit_H = NULL;//起始位高位缓存区清零
//					RX_Flag = ON;//接收标志位开启
//				}
//				else //如果第起始位错误
//					SartBit_H = NULL;//起始高位缓存区清零 防止出错
//			
//			if(RX_Count == RX_MAX)//当一帧数据接收完成 所有标志位复位
//				{
//					RX_Flag = OFF;
//					RX_OK = ON;//一帧数据接收完成
//					RX_Count = 2;
//				}
		
		
//		}



		
	
	}

}	

