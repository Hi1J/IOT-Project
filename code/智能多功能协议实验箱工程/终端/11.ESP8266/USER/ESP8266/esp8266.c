#include "esp8266.h"


u8 WiFi_Rx_Buff[512];//接收缓存区
u8 WiFi_Tx_Buff[512];//发送缓存区
u8 WiFi_Rx_Flag = 0;
u32 Rev_Len=0;
uint8_t ESP8266_STATE;

/*底层串口初始化*/
void WIFI_USART_Init(u32 baud_rate)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	USART_InitTypeDef USART_InitStruct = {0};
	//1.打开 C/D 端口时钟和 UART5 端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5,ENABLE);
	//2.PC12 配置为复用推挽输出模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//通用推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;//速度 50MHz
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOC,&GPIO_InitStruct);
	//3.PD2 配置为浮空输入模式
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;
	GPIO_Init(GPIOD,&GPIO_InitStruct);
	//串口初始化
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	USART_InitStruct.USART_StopBits = USART_StopBits_1;
	USART_InitStruct.USART_Parity = USART_Parity_No;
	USART_InitStruct.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;
	USART_InitStruct.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None;
	USART_InitStruct.USART_BaudRate = baud_rate;
	USART_Init(UART5,&USART_InitStruct);
	USART_ITConfig(UART5,USART_IT_RXNE,ENABLE);
	USART_ITConfig(UART5,USART_IT_IDLE,ENABLE);
	USART_Cmd(UART5,ENABLE);
	NVIC_SetPriority(UART5_IRQn,1);
	NVIC_EnableIRQ(UART5_IRQn);
}
/*USART5 发送一字节数据*/
void UART5_Send_Data(uint8_t data)
{
	while(!(UART5->SR & (0x01<<7)));
	UART5->DR = data;
}
/*USART5 发送指定长度数据*/
void UART5_Send_Buf(uint8_t *buf , uint16_t len)
{
	while(len--) UART5_Send_Data(*buf++);
}
/*USART5 发送字符串数据*/
void UART5_Send_Str(uint8_t *buf )
{
	while(* buf !=0) 
	{UART5_Send_Data(*buf);buf++;}
}
u8 FindStr(char* dest,char* src,u16 retry_nms)
{
	 retry_nms/=10; //超时时间
	 while((strstr(dest,src)==0) && retry_nms--)//等待串口接收完毕或超时退出
	 {
		delay_ms(10);
	 }
	 retry_nms+=1;
	 if(retry_nms)
	 {
		return 1;
	 }
	return 0; 
}

//串口5中断服务函数
void UART5_IRQHandler(void)
{
	static u32 len = 0;
	uint8_t tmp;
	if(USART_GetITStatus(UART5,USART_IT_RXNE))
	{
		tmp=UART5->DR;
		WiFi_Rx_Buff[len++]=tmp;
		USART1->DR=tmp;
	}
	if(USART_GetITStatus(UART5,USART_IT_IDLE))
	{
		tmp = UART5->DR;
		WiFi_Rx_Flag = 1;
		Rev_Len = len;
		len = 0;
	}
	USART_ClearITPendingBit(UART5,USART_IT_RXNE);
}
/**
* 功能：检查 ESP8266 是否正常
* 参数：None
* 返回值：ESP8266 返回状态
* 非 0 ESP8266 正常
* 0 ESP8266 有问题 
*/
uint8_t Check_Esp8266(void)
{
	uint8_t check_cnt=5;
	while(check_cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); //清空接收缓冲
		UART5_Send_Str((uint8_t *)"AT\r\n"); //发送 AT 握手指令
		delay_ms(20);
		if(FindStr((char*)WiFi_Rx_Buff,"OK",200) != 0)
		{
			printf("检测到 ESP8266\r\n");
			return 1;
		}
	}
		printf("未检测到 ESP8266\r\n");
	return 0;
}
/**
* 功能：打开透传
* 参数：None
* 返回值：ESP8266 返回状态
* 非 0 ESP8266 正常
* 0 ESP8266 有问题 
*/
static uint8_t OpenTransmission(void)
{
//设置透传模式
	uint8_t cnt=2;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 

		UART5_Send_Str((uint8_t *)"AT+CIPMODE=1\r\n"); 
		if(FindStr((char*)WiFi_Rx_Buff,"OK",200)!=0)
		{
			return 1;
		}
	}
	return 0;
}
//退出透传
static void CloseTransmission(void)
{
	UART5_Send_Str((uint8_t *)"+++"); delay_ms(50);
	UART5_Send_Str((uint8_t *)"+++"); delay_ms(50);
}
/*
关闭 ESP8266 回显
*/
void close_esp8266_echo(void)
{
	UART5_Send_Str((uint8_t *)"ATE0\r\n");
}
/* 重启 ESP8266*/
void RESET_ESP8266(void)
{
	UART5_Send_Str((uint8_t *)"AT+RST\r\n"); 
}
static uint8_t ConnectAP(char* ssid,char* pswd)
{
	uint8_t cnt=5;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 

		UART5_Send_Str((uint8_t *)"AT+CWMODE_CUR=1\r\n"); 
		//设置为 STATION 模式
		if(FindStr((char*)WiFi_Rx_Buff,"OK",200) != 0)
		{
		break;
		} 
	}
	if(cnt == 0) 
		return 0;
	cnt=2;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); //清空发送缓冲
		memset(WiFi_Tx_Buff,0,512); //清空接收缓冲
		sprintf((char*)WiFi_Tx_Buff,"AT+CWJAP_CUR=\"%s\",\"%s\"\r\n",ssid,pswd);//连接目标 AP
		UART5_Send_Str(WiFi_Tx_Buff);
		if(FindStr((char*)WiFi_Rx_Buff,"OK",8000)!=0) 
		//连接成功且分配到 IP
		{
			return 1;
		}
	}
	return 0;
}
void Init_Esp8266(void)
{
	uint8_t flag=0;
	WIFI_USART_Init(115200); //跟 WIFI 通信 USART 初始化
	CloseTransmission(); //退出透传 防止系统复位，ESP8266 还处于透传模式
	RESET_ESP8266(); //重启 ESP8266 
	delay_ms(2000); //给 ESP8266 初始化时间
	flag=Check_Esp8266();

	if(flag==0)
	{
		ESP8266_STATE = 0; 
		return ;
	}
	//close_esp8266_echo();
	flag=ConnectAP(SSID,PWD);
	if(flag==0)
	{
		ESP8266_STATE = 0; 
		return ;
	}else 
	{
		printf("WIFI 连接成功\r\n");
	}
}

/**
* 功能：使用指定协议(TCP/UDP)连接到服务器
* 参数：
* mode:协议类型 "TCP","UDP"
* ip:目标服务器 IP
* port:目标是服务器端口号
* 返回值：
* 连接结果,非 0 连接成功,0 连接失败
* 说明：
* 失败的原因有以下几种(UART 通信和 ESP8266 正常情况下)
* 1. 远程服务器 IP 和端口号有误
* 2. 未连接 AP
* 3. 服务器端禁止添加(一般不会发生)
*/
uint8_t ConnectServer(char* mode,char* ip,uint16_t port)
{
	uint8_t cnt;
	 

	 // CloseTransmission(); //多次连接需退出透传
	 delay_ms(500);
	//连接服务器
	cnt=2;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 
		memset(WiFi_Tx_Buff,0,512); 
		sprintf((char*)WiFi_Tx_Buff,"AT+CIPSTART=\"%s\",\"%s\",%d\r\n",mode,ip,port);
		UART5_Send_Str(WiFi_Tx_Buff);
		if(FindStr((char*)WiFi_Rx_Buff,"CONNECT",8000) !=0 )
		{
			break;
		}
	}
	if(cnt == 0) 
			return 0;
	//设置透传模式
	if(OpenTransmission()==0) return 0;
	//开启发送状态
	cnt=2;
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 
		UART5_Send_Str((uint8_t *)"AT+CIPSEND\r\n");//开始处于透传发送状态
		if(FindStr((char*)WiFi_Rx_Buff,">",200)!=0)
		{
			return 1;
		}

	}
	return 0;
}
/**
* 功能：主动和服务器断开连接
* 参数：None
* 返回值：
* 连接结果,非 0 断开成功,0 断开失败
*/
uint8_t DisconnectServer(void)
{
	uint8_t cnt;
	CloseTransmission(); //退出透传
	delay_ms(500);
	while(cnt--)
	{
		memset(WiFi_Rx_Buff,0,512); 
		UART5_Send_Str((uint8_t *)"AT+CIPCLOSE\r\n");//关闭链接
		if(FindStr((char*)WiFi_Rx_Buff,"CLOSED",200)!=0)//操作成功,和服务器成功断开
		{
			break;
		}
	}
	if(cnt) return 1;
	return 0;
}

void Smart_Config(void)
{

	uint8_t flag=0;
	CloseTransmission(); //退出透传 防止系统复位，ESP8266 还处于透传

	RESET_ESP8266(); //重启 ESP8266 
	delay_ms(2000); //给 ESP8266 初始化时间
	flag=Check_Esp8266();
	if(flag==0)
	{
		ESP8266_STATE = 0; 
		return ;
	}
	memset(WiFi_Rx_Buff,0,512);
	UART5_Send_Str((uint8_t *)"AT+CWSTARTSMART\r\n");//智能配网
	//smartconfig connected wifi
	while(FindStr((char*)WiFi_Rx_Buff,"smartconfig connected wifi",200)==0)
	{
		delay_ms(1);
	}
	printf("配网完成\r\n");
}
