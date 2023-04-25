#include "lora.h"


#define LORA_SETPRAM_TIME 100   //等待Lora模块内部接收完成
u16 lora_addr = 0xffff;//lora地址

LORA_WORKDATA lora_data;
char *air_sped[] = {"0.3","1.2","2.4","4.8","9.6","19.2","19.2","19.2"};
char *baudrate[] = {"1200","2400","4800","9600","19200","38400","57600","115200"};
char *uartmode[] = {"8N1","801","8E1","8N1"};
char *Tran_power[] = {"20","17","14","10"};

/*
Lora管脚的初始化
Tx -- USART4_RX -- PC10(串口4功能)
Rx -- USART4_TX -- PC11(串口4功能)
M0 -- SPI_CS    -- PA8 (通用推挽输出模式)
M1 -- SPI_SCK   -- PA11 (通用推挽输出模式)
AUX - SPI_MISO  -- PA12 (浮空输入模式)
*/
void Lora_Config(void)
{
	//相关管脚初始化
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(LORA_M0_CLK|LORA_M1_CLK|LORA_AUX_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStruct.GPIO_Pin = LORA_M0_PIN;
	GPIO_Init(LORA_M0_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = LORA_M1_PIN;
	GPIO_Init(LORA_M1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = LORA_AUX_PIN;
	GPIO_Init(LORA_AUX_PORT,&GPIO_InitStruct);	
	
	UART4_Config(9600);//串口3初始化+接收中断初始化
	Time4_Config(7200,200);//定时器4初始化
	
	Lora_SetWorkParameters();//设置Lora模块的参数
	Lora_CurrentPram();//读取Lora模块的参数
}
void Lora_SendData(u8 *data,u16 len)
{
	uint8_t buff[256] = {0xff,0xff,PRM_CHANNEL_DEFAULT};
	for(u8 i = 0;i<len;i++)
		buff[i+3] = data[i];
	UART_SendData(UART4, buff, len+3);
}
void Lora_SendCmd(u8 *data,u16 len)
{
	UART_SendData(UART4, data, len);
}
//设置Lora的模式
void Lora_SetMode(u8 mode)
{
	if((mode != LORA_MODE_PATTERN) && (mode != LORA_MODE_WARKUP) && (mode != LORA_MODE_POWERSAVE) && (mode != LORA_MODE_SLEEP))  return ;
	GPIO_WriteBit(LORA_M0_PORT,LORA_M0_PIN,(BitAction)(mode & 0x1));
	GPIO_WriteBit(LORA_M1_PORT,LORA_M1_PIN,(BitAction)((mode>>4) & 0x1));
	delay_ms(100);
}
//Lora模块复位
void Lora_Reset(void)
{
	u8 tx_buff[] = {0xC4,0xC4,0xC4};
	Lora_SetMode(LORA_MODE_SLEEP);//进入休眠模式
	Lora_SendCmd(tx_buff,sizeof(tx_buff));//发送重启命令
}
//获取当前参数
void Lora_CurrentPram(void)
{
	uint32_t timeout = 0x00ffffff;
	u8 RxBuff[6] = {0};
	u8 RxCount = 0;
	u8 tx_buff[] = {0xC1,0xC1,0xC1};
		//关闭串口3中断
	USART_ITConfig(UART4, USART_IT_RXNE, DISABLE);
//	NVIC_DisableIRQ(UART4_IRQn);
	
	Lora_SetMode(LORA_MODE_SLEEP);//进入休眠模式
	Lora_SendCmd(tx_buff,sizeof(tx_buff));//发送获取参数命令
	memset(lora_data.buff,0,6);
	//串口等待获取数据
	do{
		if(USART_GetFlagStatus(UART4,USART_FLAG_RXNE) == SET)
		{
			lora_data.buff[RxCount++] = UART4->DR;
		}
		if(RxCount == 6) break;//接收完数据正常退出
	}while(timeout--);
	
	printf("\r\n接收到的数据为：\r\n");
	for(u8 i = 0;i<6;i++)
			printf("%x ",lora_data.buff[i]);
	printf("\r\n");
	
	if(RxCount != 6) 
		printf("Lora模组读取参数超时。\r\n");
	else//解析Lora的参数
		Lora_WorkParameters();
	Lora_SetMode(LORA_MODE_PATTERN);//进入一般模式
	//开中断
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);
//	NVIC_EnableIRQ(UART4_IRQn);
}


void Lora_SetWorkParameters(void)
{
	//关闭串口4中断
	USART_ITConfig(UART4,USART_IT_RXNE,DISABLE);
//	NVIC_DisableIRQ(UART4_IRQn);
	
	Lora_SetMode(LORA_MODE_SLEEP);//进入休眠模式

	memset(lora_data.buff,0,sizeof(lora_data.buff));
	//修改地址和信道
  lora_data.lora_workprm.head = CMD_STOREPRM;
	lora_data.lora_workprm.addh = (lora_addr>>8) & 0xff;
	lora_data.lora_workprm.addl = lora_addr & 0xff;
	lora_data.lora_workprm.sped = (PRM_UART_Parity_8N1<<6)|(PRM_UART_BaudRate_9600<<3)|(PRM_Sky_Speed_2_4);
	lora_data.lora_workprm.chan = PRM_CHANNEL_DEFAULT;
	lora_data.lora_workprm.option = ((PRM_TRAN_MODE_FP<<7)|(PRM_IO_Driver_PP<<6)|
											             (PRM_WarkUp_Time_250<<3)|(PRM_FEC_OPEN<<2)|(PRM_EmissionPower_20));
  
	printf("写入的数据为：\r\n");
	for(u8 i = 0;i<6;i++)
		printf("%x ",lora_data.buff[i]);
	
	Lora_SendCmd(lora_data.buff,sizeof(lora_data.buff));

	delay_ms(2);//延时等待Lora内部保存完成
	Lora_SetMode(LORA_MODE_PATTERN);//进入一般模式

	//开中断
	USART_ITConfig(UART4,USART_IT_RXNE,ENABLE);

//	NVIC_EnableIRQ(UART4_IRQn);
}

void Lora_WorkParameters(void)
{
	//打印原始数据
	printf("原始数据：");
	for(u8 i = 0;i<6;i++)
		printf("%x ",lora_data.buff[i]);
	printf("\r\n");
	//解析数据
	printf("频率\t地址\t信道\t空中速率\t波特率\t串口格式\t发射功率\r\n");
	printf("%dMHz\t",lora_data.lora_workprm.chan+410);//工作频率
	printf("0x%x\t",((lora_data.lora_workprm.addh<<8) + lora_data.lora_workprm.addl));//地址
	printf("0x%x\t",lora_data.lora_workprm.chan);//信道
	printf("%skbps\t",air_sped[(lora_data.lora_workprm.sped & 0x7)]);//空中速率
	printf("%s\t",baudrate[((lora_data.lora_workprm.sped>>3)& 0x7)]);//波特率
	printf("%s\t",uartmode[(lora_data.lora_workprm.sped>>6) & 0x3 ]);//串口格式
	printf("%sdBm\t",Tran_power[lora_data.lora_workprm.option & 0x3]);//发射功率
  printf("\r\n");
}


