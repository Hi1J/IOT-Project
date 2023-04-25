#include "Zigbee.h"

#if Zigbee_Open

u8 Zigbee_Rx_Buf[128] = {0};
u8 Zigbee_Rx_OK = 0;

void Uart4_Init(u32 brr)
{
	GPIO_InitTypeDef GPIO_InitStruct;
 USART_InitTypeDef USART_InitStruct;
 NVIC_InitTypeDef NVIC_InitStruct;
 
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOA,ENABLE);
 RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4,ENABLE);

 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_10;//发送
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;
 GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
 GPIO_Init(GPIOC,&GPIO_InitStruct);
 GPIO_InitStruct.GPIO_Pin=GPIO_Pin_11;//接收
 GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
 GPIO_Init(GPIOC,&GPIO_InitStruct);
 
 //3:配置 USART4
 USART_InitStruct.USART_BaudRate=brr;//波特率
 
	USART_InitStruct.USART_HardwareFlowControl=USART_HardwareFlowControl_None;//没有硬件控制
 //RS232 有硬件引脚控制接收和发送
 USART_InitStruct.USART_Mode=USART_Mode_Tx|USART_Mode_Rx;
 //TX(发送) RX（接收） TX|RX(接收和发送)
 USART_InitStruct.USART_Parity=USART_Parity_No;//没有校验位
 USART_InitStruct.USART_StopBits=USART_StopBits_1;
 USART_InitStruct.USART_WordLength=USART_WordLength_8b;
 USART_Init(UART4,&USART_InitStruct);
/********************************************************************
*****/
//打开中断
 //1：打开外设的中断
 USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
 USART_ITConfig(UART4,USART_IT_IDLE,ENABLE);
 //2：设置中断的优先级
 NVIC_InitStruct.NVIC_IRQChannel=UART4_IRQn;
 NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
 NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
 NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
 NVIC_Init(&NVIC_InitStruct);
 //3:写中断服务函数
 //中断服务函数不声明 不调用

 //中断事件发生自然会执行
/*****************************************************************8*/ 
 //4：使能串口
 USART_Cmd(UART4,ENABLE);

}
void Uart4_Send_Data(u8 data)
{
	while(!(UART4->SR & (1 << 7)));
	UART4->DR = data;
}

void UART4_IRQHandler(void)
{
 uint8_t tmp;
 static uint8_t i=0;
 uint8_t count;
 //你要去判断发生的是哪一个中断
 if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
 {
 //进入中断的第一件事就是清除中断
	 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	 tmp = UART4->DR;
	 Zigbee_Rx_Buf[i++]=tmp;
	 //i++ :i 首先参与运算再自加
	 //++i ：i 先自加再参与运算
	 
	}
	 //空闲中断//只有当你的数据全部发生完后才会进去(数据总线空闲时触发)
 if(USART_GetITStatus(UART4,USART_IT_IDLE)==SET)
	 {
	 tmp=UART4->DR;
	 count = i;
	 i=0;
	 Zigbee_Rx_OK = 1;

 }

}

void Zigbee_Init(void)
{
	Uart4_Init(115200);

}

void Zigbee_Send_Data(u8 *data,u16 len)
{
	u8 i = 0;
	for(i=0;i<len;i++)
	{
		Uart4_Send_Data(*(data+i));
	}

}

#endif


