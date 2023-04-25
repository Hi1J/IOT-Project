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

//UART4中断服务函数
void UART4_IRQHandler(void)
{
	uint8_t tmp;//数据临时存储区
	static uint8_t YJ_Rx_Count=1;//计数值 满8置0
	static uint8_t YJ_Rx_head = 0;//起始位接收缓存区
	static uint8_t YJ_Rx_flag = 0;//开始接收标志位
 //接收中断
 if(USART_GetITStatus(UART4,USART_IT_RXNE)==SET)
 {
 //进入中断的第一件事就是清除中断
	 USART_ClearITPendingBit(UART4,USART_IT_RXNE);
	 
	 tmp = UART4->DR;//临时记录接收的值
	 
	 //如果是首个进来的为起始位 将其放入起始位接收缓存区
	 if((tmp == 0xfa) && (YJ_Rx_flag == 0))YJ_Rx_head = tmp;

	 //如果第二个进来的为地址位 
	 if((tmp == Zigbee_Address) && (YJ_Rx_head == 0xfa) && (YJ_Rx_flag == 0))
	 {
			YJ_Rx_flag = 1;//接收标志位置1
			if(Zigbee_Rx_OK == 1)P_RX = P_RX->next;//如果是连续的数据帧 则将数据保存到下一个buf
			else P_RX->next = YJ_Link1;//否则指向buf1
			P_RX->YJ_RX_Buf[0] = YJ_Rx_head;//将起始位放置buf[0]里面
//			Zigbee_Rx_Buf[0] = YJ_Rx_head;//将起始位放置buf[0]里面
			YJ_Rx_head = 0;//起始位接收缓存区置0
	 }

	//如果接收标志位为1 则开始接收
	 if(YJ_Rx_flag == 1)P_RX->YJ_RX_Buf[YJ_Rx_Count++] = tmp;
	 
	 //当放满一个buf时 默认一帧数据接收完成 各标志位复位 以接收下一帧数据
	 if(YJ_Rx_Count == 8)
	 {
		 YJ_Rx_flag = 0;//接收开始位清0
		 YJ_Rx_Count = 1;//计数值复位
		 YJ_Rx_head = 0;//起始位缓存区复位 防止出错
		 Zigbee_Rx_OK = 1;//接收完8个数据后 接收标志位置1
		 for(u8 i=0;i<8;i++)
		 {
			(P_RX->next)->YJ_RX_Buf[i] = P_RX->YJ_RX_Buf[i];
		 }
		 
	 }

	
	}

 
	

}
//Zigbee初始化
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
			if(Zigbee_Rx_Buf[3] == 0x03)//如果发来的是温湿光强度
			{
				
				//写入数据
				sprintf(Tembuf,"%2d",Zigbee_Rx_Buf[4]);
				sprintf(Humbuf,"%2d",Zigbee_Rx_Buf[5]);
				sprintf(Lxbuf,"%-5d",(Zigbee_Rx_Buf[6]<<8) | Zigbee_Rx_Buf[7]);
				
				//LCD显示数据
				LCD_ShowStr(97,160+9,(u8 *)Tembuf);
				LCD_ShowStr(97,220+9,(u8 *)Humbuf);
				LCD_ShowStr(97,280+9,(u8 *)Lxbuf);
			
			}
			else if(Zigbee_Rx_Buf[3] == 0x04)
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


