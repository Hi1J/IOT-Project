#include "usart.h"

u8 USART1_RX_BUF[USART1_REC_LEN];     //接收缓冲,最大USART_REC_LEN个字节.
u8 USART2_RX_BUF[123];

u8 rx_ok;

u16 USART1_RX_STA=0;       //接收状态标记	

//串口初始化函数
void USART1_Config(void)
{
//1.配置GPIO——————————————————————
	//①开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//②配置GPIO模式
	GPIO_InitTypeDef GPIO_InitStructure;
		//PA9（发送）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//③初始化寄存器（PA9）
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		//PA10（接收）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	//④初始化寄存器（PA10）
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
//配置NVIC
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
	
	
//2.配置串口——————————————————————
	//①开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	//②配置串口模式
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = General_BaudRate;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//配置发送和接收（全双工）
	USART_InitStructure.USART_Parity = USART_Parity_No;//不使用奇偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//一帧中传输或接收8个数据位
	//③初始化寄存器
	USART_Init(USART1,&USART_InitStructure);
	//开中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//④使能串口
	USART_Cmd(USART1,ENABLE);
	
	


}

//3.数据发送函数和接收函数的编写————————————
  //①重定向printf函数
int fputc(int ch,FILE *fp)
{
	USART1 -> DR = ch;//如果发送完成，则把数据放入DR寄存器中
	while((USART1 -> SR & ( 1 << 7 )) == 0);//如果SR寄存器中的第六位TC为0，表示数据未发送完成，则会一直在此等待
	return ch;
}


//串口1中断函数
//接收不定长数据--定时器中断
void USART1_IRQHandler(void)
{
	u8 res;
	if(USART1->SR & (1 << 5))
	{
		res = USART1->DR;
		printf("%c",res);
		USART1->SR &= ~(1 << 5);//清空中断标志位
		
	}
	
}





//***************************************USART2**************************************//

void USART2_Config(void)
{
//1.配置GPIO——————————————————————
	//①开时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	//②配置GPIO模式
	GPIO_InitTypeDef GPIO_InitStructure;
		//PA2（发送）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽输出
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;			
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	//③初始化寄存器（PA9）
	GPIO_Init(GPIOA,&GPIO_InitStructure);
		//PA3（接收）
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	//④初始化寄存器（PA10）
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//中断
	NVIC_InitTypeDef NVIC_InitStructure;	
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器 
	
	
//2.配置串口——————————————————————
	//①开时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
	//②配置串口模式
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = General_BaudRate;//波特率
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;//配置发送和接收（全双工）
	USART_InitStructure.USART_Parity = USART_Parity_No;//不使用奇偶校验
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//1个停止位
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//一帧中传输或接收8个数据位
	//③初始化寄存器
	USART_Init(USART2,&USART_InitStructure);
	//④开中断
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//⑤使能串口
	USART_Cmd(USART2,ENABLE);

}


//串口2发送字符串函数
void Usart2_Send_String(uint8_t * str)
{
	while( *str != '\0')
	{
		USART2->DR = *str;
		while(!(USART2->SR & (1 << 7)));//等待发送完成
		str++;
	}

}

//串口2中断函数
void USART2_IRQHandler(void)
{
	uint8_t tmp;//数据临时存储区
	static uint8_t YJ_Rx_Count=1;//计数值 满8置0
	static uint8_t YJ_Rx_head = 0;//起始位接收缓存区
	static uint8_t YJ_Rx_flag = 0;//开始接收标志位
	if(USART2->SR & (1 << 5))
	{
		USART2->SR &= ~(1 << 5); //清空标志位
		
		tmp = UART4->DR;//临时记录接收的值
	 
	 //如果是首个进来的为起始位 将其放入起始位接收缓存区
	 if((tmp == 0xfa) && (YJ_Rx_flag == 0))YJ_Rx_head = tmp;

	 //如果第二个进来的为地址位 
	 if((tmp == 0xff) && (YJ_Rx_head == 0xfa) && (YJ_Rx_flag == 0))
	 {
			YJ_Rx_flag = 1;//接收标志位置1
//			if(Zigbee_Rx_OK == 1)P_RX = P_RX->next;//如果是连续的数据帧 则将数据保存到下一个buf
//			else P_RX->next = YJ_Link1;//否则指向buf1
			USART2_RX_BUF[0] = YJ_Rx_head;//将起始位放置buf[0]里面
			YJ_Rx_head = 0;//起始位接收缓存区置0
	 }

	//如果接收标志位为1 则开始接收
	 if(YJ_Rx_flag == 1)USART2_RX_BUF[YJ_Rx_Count++] = tmp;
	 
	 //当放满一个buf时 默认一帧数据接收完成 各标志位复位 以接收下一帧数据
	 if(YJ_Rx_Count == 8)
	 {
		 YJ_Rx_flag = 0;//接收开始位清0
		 YJ_Rx_Count = 1;//计数值复位
		 YJ_Rx_head = 0;//起始位缓存区复位 防止出错
		 rx_ok = 1;//接收完8个数据后 接收标志位置1
//		 for(u8 i=0;i<8;i++)
//		 {
//			(P_RX->next)->YJ_RX_Buf[i] = P_RX->YJ_RX_Buf[i];
//		 }
		 
	 }
	
	}

}

