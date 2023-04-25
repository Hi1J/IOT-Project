#include "Zigbee.h"

#if Zigbee_Open

u8 Zigbee_Rx_Buf[128] = {0};
u8 Zigbee_Rx_OK = 0;

void Usart3_Init(u32 brr)
{
	GPIO_InitTypeDef GPIO_InitStruct;
 USART_InitTypeDef USART_InitStruct;
 NVIC_InitTypeDef NVIC_InitStruct;
 
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
	
	USART_InitStruct.USART_BaudRate = brr;//波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件流
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//全双工模式
	USART_InitStruct.USART_Parity = USART_Parity_No;//无奇偶校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;//1+8 字长
	USART_Init(USART3,&USART_InitStruct);
/********************************************************************
*****/
//打开中断
 //1：打开外设的中断
 USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
 USART_ITConfig(USART3,USART_IT_IDLE,ENABLE);
 //2：设置中断的优先级
 NVIC_InitStruct.NVIC_IRQChannel=USART3_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
 NVIC_Init(&NVIC_InitStruct);
 //3:写中断服务函数
 //中断服务函数不声明 不调用

 //中断事件发生自然会执行
/*****************************************************************8*/ 
 //4：使能串口
 USART_Cmd(USART3,ENABLE);

}
void Usart3_Send_Data(u8 data)
{
	while(!(USART3->SR & (1 << 7)));
	USART3->DR = data;
}

void UART4_IRQHandler(void)
{
 uint8_t tmp;
 static uint8_t i=0;
 uint8_t count;
 //你要去判断发生的是哪一个中断
 if(USART_GetITStatus(USART3,USART_IT_RXNE)==SET)
 {
 //进入中断的第一件事就是清除中断
	 USART_ClearITPendingBit(USART3,USART_IT_RXNE);
	 tmp = USART3->DR;
	 Zigbee_Rx_Buf[i++]=tmp;
	 //i++ :i 首先参与运算再自加
	 //++i ：i 先自加再参与运算
	 
	}
	 //空闲中断//只有当你的数据全部发生完后才会进去(数据总线空闲时触发)
 if(USART_GetITStatus(USART3,USART_IT_IDLE)==SET)
	 {
	 tmp=USART3->DR;
	 count = i;
	 i=0;
	 Zigbee_Rx_OK = 1;

 }

}

void Zigbee_Init(void)
{
	Usart3_Init(115200);

}

void Zigbee_Send_Data(u8 *data,u16 len)
{
	u8 i = 0;
	for(i=0;i<len;i++)
	{
		Usart3_Send_Data(*(data+i));
	}

}

#endif


