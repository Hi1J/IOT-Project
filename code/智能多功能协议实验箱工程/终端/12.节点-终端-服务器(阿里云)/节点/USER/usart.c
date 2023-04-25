#include "usart.h"

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

//USART1串口中断函数
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART1->SR & (1 << 5))
	{
		res = USART1->DR;//读取数据
		printf("%c",res);//返回数据
		
		USART1->SR &= ~(1 << 5);//清空中断标志位
	
	}

}	

