#include "dht11.h"

u16 tem = 0,hum = 0;

/*******************************
函数名称：DHT11_Config
函数功能：DHT11 gpio口初始化
函数参数：flag 	-- 0 输入模式
				        -- 1 输出输出
函数返回值：无
备注：
	接在板子中间的传感器接口就是
	PB12
	接在板子右边的传感器接口就是
	PB6
作者:ZZXYD
*******************************/
void DHT11_Config(u8 flag)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(DHT11_CLK,ENABLE);
	if(flag == 1)
	{
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	}
	else if(flag == 0)
	{
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	}
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Pin = DHT11_PIN;
	GPIO_Init(DHT11_PORT,&GPIO_InitStruct);
}


/***************************************
函数名称：Get_DHT11_Data
函数功能：读取温湿度数据
函数参数：
	u16 *t--存放温度数据
	u16* h--存放湿度数据
函数返回值：成功返回0，失败返回-1
备注：
作者:ZZXYD
****************************************/
int Get_DHT11_Data(u16 *t,u16* h)
{
	u8 i = 0,j=0;
	u32 timeout = 0;
	u8 DHT_DATA[5]={0};
	DHT11_Config(1);//输出
	DATA(1);//拉高
	DATA(0);//拉低
	delay_ms(20);
	DATA(1);
	delay_us(30);
	DHT11_Config(0);//输入
	timeout = 0;
	while(DATA_IN()==1)//等待信号拉低
	{
		timeout++;
		if(timeout>0XFFFFF)
			return -1;
	}
	timeout = 0;
	while(DATA_IN()==0)//等待信号拉高
	{
		timeout++;
		if(timeout>0XFFFFF)
			return -1;
	}
	/*读取数据*/
	for(i=0;i<5;i++)
	{
		for(j=0;j<8;j++)
		{
			timeout = 0;
			while(DATA_IN()==1)//等待信号拉低
			{
				timeout++;
				if(timeout>0XFFFFF)
					return -1;
			}
			timeout = 0;
			while(DATA_IN()==0)//等待信号拉高
			{
				timeout++;
				if(timeout>0XFFFFF)
					return -1;
			}
			delay_us(40);
			if(DATA_IN()==1)//数据为1
			{	
				DHT_DATA[i]<<=1;
				DHT_DATA[i]|=1;
			}
			else if(DATA_IN()==0)//数据为0
					DHT_DATA[i]<<=1;
		}
	}
	/*数据校验*/
	if(DHT_DATA[4]==(DHT_DATA[0]+DHT_DATA[1]+DHT_DATA[2]+DHT_DATA[3]))
	{
		*t = DHT_DATA[2];
		*h = DHT_DATA[0];
		return 0;
	}
	return -1;
}

/***************************************
函数名称：DHT11_Show_Usart
函数功能：获取温湿度数据并且通过串口输出
函数参数：	
			u16 *t-->存放温度
			u16 *h-->存放湿度
函数返回值：成功返回0，失败返回-1
备注：
作者:ZZXYD
****************************************/
int DHT11_Show_Usart(u16 *t,u16 *h)
{
	if(Get_DHT11_Data(&tem,&hum))
	{
		printf("获取温湿度数据失败\r\n");
		return -1;
	}
	else
	{
		printf("获取温湿度数据成功\r\n");
		printf("温度:%d℃\r\n湿度:%d%%RH\r\n",tem,hum);
		if(t != NULL) *t = tem;
		if(h != NULL) *h = hum;
		return 0;
	}
}




	
