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

//void UART4_IRQHandler(void)
//{
// uint8_t tmp;
// static uint8_t i=0;
// static u8 flag = 0;//数据正确标志位
// //你要去判断发生的是哪一个中断
// if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
// {
// //进入中断的第一件事就是清除中断
//	 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
//	 tmp = UART4->DR;
//	 Zigbee_Rx_Buf[i++]=tmp;
//	 
//	 //判断数据是否是发来本机
//	 if(flag == 0)
//	 {
//		 if(Zigbee_Rx_Buf[0] == (Zigbee_Address >> 8))//判断地址高八位
//		 {
//				if(i == 2)//
//				{
//					if(Zigbee_Rx_Buf[1] == (Zigbee_Address & 0x00ff))//判断地址低八位
//					{
//						flag = 1;//确定数据是发来本机的
//					}
//				}
//		 }
//	 }
//	 
//	}
//	 //空闲中断//只有当你的数据全部发生完后才会进去(数据总线空闲时触发)
// if(USART_GetITStatus(UART4,USART_IT_IDLE)==SET)
//	 {
//	 tmp=UART4->DR;
//	 i=0;
//		 if(flag == 1)//如果数据正确
//		 {
//				Zigbee_Rx_OK = 1;//接收标志位至1
//				flag = 0;//数据正确标志位清零
//		 }
//		 else
//			 {
//				memset(Zigbee_Rx_Buf,0,sizeof(Zigbee_Rx_Buf));
//			 }
//		}

//}
void UART4_IRQHandler(void)
{
 uint8_t tmp;
 static uint8_t i=0;
 static u8 flag = 1;//数据正确标志位
 //你要去判断发生的是哪一个中断
 if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
 {
 //进入中断的第一件事就是清除中断
	 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	 
	 if(flag == 1)//如果数据正确才放入接收数组 防止接收数组溢出错误
	 {
			tmp = UART4->DR;
			Zigbee_Rx_Buf[i++]=tmp;
	 }
	 
	 //判断数据是否是正确Zigbee发来的
	 if(Zigbee_Rx_Buf[0] == 0xFA)//如果起始位正确 则是正确Zigbee发来的
	 {
		if(i == 2)//判断是否是发来本机的
		{
			if(!(Zigbee_Rx_Buf[1] == Zigbee_Address))//如果不是发来本机的
			{
				flag = 0;//如果不是发来本机的 则不接收此数据帧
			}
		}
	 }
	 else  //如果不是正确Zigbee发来的
	 {
			flag = 0;//如果不是正确Zigbee发来的 则不接收此数据帧
	 }
	
	}
	 //空闲中断//一帧数据接收完成后 不管正确与否 会在此函数中判断处理
 if(USART_GetITStatus(UART4,USART_IT_IDLE)==SET)
	 {
	 tmp=UART4->DR;
	 i=0;
		 if(flag == 1)//如果数据正确（确定是正确Zigbee发来的且是发来本机的）
		 {
				Zigbee_Rx_OK = 1;//接收标志位至1
		 }
		 else//如果数据不正确
			 {
					flag = 1;//一次数据帧传输完成 但并不是正确的 故flag重新置1以接收下一数据帧
					memset(Zigbee_Rx_Buf,0,sizeof(Zigbee_Rx_Buf));//清空数组
			 }
		}

}//Zigbee初始化
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
/*
Zigbee数据分析函数

Zigbee数据帧格式规范：
-------------------------------------------------------------------------------------------
|	    1    |   /     2               3         		  4                5          6-n
| 地址高位 / 目标地址低位     发送地址高位     发送地址地位        功能码     数据内容


功能码：
0x03:温湿光照度发送
0x04:控制灯亮灭

数据内容：
0x03:
5:温度
6:湿度
7:光照高位
8:光照低位

0x04:


*/
void Zigbee_Data_Analysis(void)
{
	if(Zigbee_Rx_OK == 1)//如果接收到发来本机的数据
		{
			if(Zigbee_Rx_Buf[2] == 0x03)//如果发来的是温湿光强度
			{
				
				//写入数据
				sprintf(Tembuf,"%2d",Zigbee_Rx_Buf[3]);
				sprintf(Humbuf,"%2d",Zigbee_Rx_Buf[4]);
				sprintf(Lxbuf,"%-5d",(Zigbee_Rx_Buf[5]<<8) | Zigbee_Rx_Buf[6]);
				
				//LCD显示数据
				LCD_ShowStr(97,160+9,(u8 *)Tembuf);
				LCD_ShowStr(97,220+9,(u8 *)Humbuf);
				LCD_ShowStr(97,280+9,(u8 *)Lxbuf);
			
			}
			else if(Zigbee_Rx_Buf[2] == 0x04)
			{
				
			}
			else
			{
				
			}
			
			Zigbee_Rx_OK = 0;//清空标志位
			memset(Zigbee_Rx_Buf,0,sizeof(Zigbee_Rx_Buf));//清空接收数组
		}

}

#endif


