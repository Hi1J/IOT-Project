#include "lora.h"

/*
Lora 管脚的初始化
Tx -- USART4_RX -- PC10(串口 4 功能)
Rx -- USART4_TX -- PC11(串口 4 功能)
M0 -- SPI_CS -- PA8 (通用推挽输出模式)
M1 -- SPI_SCK -- PA11 (通用推挽输出模式)
AUX - SPI_MISO -- PA12 (浮空输入模式)
*/


uint8_t usart4_buf[128]={0};
u8 Rx_OK = 0;

void Init_USART4_Lora(uint32_t brr)
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

void Send_data_Usart4(uint8_t c)
{
	 while(!(UART4->SR & (0x01<<7)));
	 UART4->DR = c;
}
//lora发送数据
void Send_Data_Lora(uint8_t *data ,uint8_t size)
{
	 uint8_t i=0;
	 for(i=0;i<size;i++)
	 {
	 Send_data_Usart4(*(data+i));
	 }
}

void Send_String_lora(uint8_t *data)
{
	 while(*data !=0)
	 {
	 Send_data_Usart4(*(data));
	 data+=1;
	 }
}

void Init_Lora(void)
{
 //1:需要设置 Lora 的工作模式
 //需要看 M0 和 M1 的状态
 //设置 M0 和 M1 为推挽、AUX 为浮空输入
/* PC10 uart4-tx
 PC11 uart4-rx
 M0 --CS--PA8
 M1 --SCK--PA11
 AUX --SIP1-MISO-->PA12
*/
	 u8 RxCount = 0;
	 u8 rx_buf[6];
	 GPIO_InitTypeDef GPIO_InitTStruct;
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	 
	 GPIO_InitTStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_11;
	 GPIO_InitTStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	 GPIO_InitTStruct.GPIO_Speed=GPIO_Speed_50MHz;
	 GPIO_Init(GPIOA,&GPIO_InitTStruct);
	 GPIO_InitTStruct.GPIO_Pin=GPIO_Pin_12;
	 GPIO_InitTStruct.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	 GPIO_Init(GPIOA,&GPIO_InitTStruct);
	 
	 //1:设置 Lora 的工作模式
	 Init_USART4_Lora(9600);//默认通信速度是 9600
	 //一般模式
	 while(AUX_Sta==RESET);//等待 AUX 引脚拉高
	 M0(1);M1(1);//休眠模式
	 delay_ms(20);//1ms 之后就会工作在新模式
	 //要去设置 他的工作参数 你最好把模块设置成休眠状态
	 Send_data_Usart4(0xc0);//掉电保存
	 Send_data_Usart4(0xFF);//地址高位
	 Send_data_Usart4(0xFF);//地址地位
	 Send_data_Usart4(0x1A);//9600 波特率，无奇偶检验、19.2kb
	 Send_data_Usart4(0x17);//
	 Send_data_Usart4(0xC4);//FEC 纠错打开、20dbm、定向传输
	 
	 delay_ms(20);//等待
	 
	 //检测发送数据是否写入LORA
	 printf("发送指令:C1 C1 C1\r\n");
	 
	 Send_data_Usart4(0xc1);
	 Send_data_Usart4(0xc1);
	 Send_data_Usart4(0xc1);
	 

	 
	 
	 while(AUX_Sta==RESET);//等待 AUX 引脚拉高
	 M0(0);M1(0);//一般模式
	 delay_ms(20);//1ms 之后就会工作在新模式
	 

	 
	 //串口调试
	 if(Rx_OK == 1)
	 {
		 printf("回应数据：\r\n");
		for(u8 i=0;i < 6;i++)
		 {
			printf("%x ",usart4_buf[i]);
		 }
		 printf("\r\n");
		 Rx_OK = 0;
		 memset(usart4_buf,0,sizeof(usart4_buf));//清空数组
	 }
	 else
	 {
		memset(usart4_buf,0,sizeof(usart4_buf));//清空数组
	 }
	 
	 
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
	 usart4_buf[i++]=tmp;
	 //i++ :i 首先参与运算再自加
	 //++i ：i 先自加再参与运算
	 
	}
	 //空闲中断//只有当你的数据全部发生完后才会进去(数据总线空闲时触发)
 if(USART_GetITStatus(UART4,USART_IT_IDLE)==SET)
	 {
	 tmp=UART4->DR;
	 count = i;
	 i=0;
	 Rx_OK = 1;
//	 printf("%s\r\n",usart4_buf);
//	 memset(usart4_buf,0,sizeof(usart4_buf));//清空数组
 }

}

