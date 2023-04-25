#include "mq2.h"


u16 mq2_val = 0;
/*****************************************************
函数名称：MQ2_Gpio_Init
函数功能：MQ2传感器使用的GPIO初始化
函数参数：无
函数返回值：无
备注：
	接在板子中间的传感器接口就是
	PB1-----ADC12_IN9
	接在板子右边的传感器接口就是
	PA0-----ADC12_IN0
版本：V1.0
作者：ZZXYD
*******************************************************/		

void MQ2_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	
	RCC_APB2PeriphClockCmd(MQ2_CLK|RCC_APB2Periph_ADC1,ENABLE);
	
	//GPIO口配置为模拟模式
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin = MQ2_PIN;
	GPIO_Init(MQ2_PORT,&GPIO_InitStruct);
	//ADC1的配置
	ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;//ADC独立模式
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//单次循环模式还是连续循环模式
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;     //多通道还是单通道
	ADC_InitStruct.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//有事件触发还是软件触发
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐 -- 低12位
	ADC_InitStruct.ADC_NbrOfChannel = 1;//顺序转换的规则组的通道数目
	
	ADC_Init(ADC1,&ADC_InitStruct);     //给相关寄存器赋值
	
  //设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,MQ2_ADC_CHANNEL,1,ADC_SampleTime_239Cycles5);
	
	ADC_Cmd(ADC1,ENABLE);               //开启ADC1
	
	ADC_ResetCalibration(ADC1);         //使用复位校准，就是初始化校准寄存器
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	
	ADC_StartCalibration(ADC1);         //A/D校准
	while(ADC_GetCalibrationStatus(ADC1) == SET);
}

/*****************************************************
函数名称：MQ2_GetValue
函数功能：获取MQ2传感器数据
函数参数：获取到的ADC数据
函数返回值：成功返回0，失败返回-1
备注：
	接在板子中间的传感器接口就是
	PB1-----ADC12_IN9
	接在板子右边的传感器接口就是
	PA0-----ADC12_IN0
版本：V1.0
作者：ZZXYD
*******************************************************/	
int MQ2_GetValue(u16 *adc_val)
{
	u32 timeout = 0;
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//开启转换
	while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC) != SET)
	{
		timeout++;
		if(timeout>0xffff)
			return -1;
	}
	*adc_val = ADC_GetConversionValue(ADC1);
	return 0;
}

/*****************************************************
函数名称：MQ2_Show_Usart
函数功能：获取MQ2传感器数据并且在串口显示
函数参数：无
函数返回值：失败返回-1，成功返回MQ2浓度值
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/	
u16 MQ2_Show_Usart(void)
{
	if(MQ2_GetValue(&mq2_val))
	{
		printf("MQ2传感器数据读取失败\r\n");
		return (u16)-1;
	}
	else
	{
		printf("MQ2传感器数据读取成功\r\n");
		printf("adc_val:%d\r\n",mq2_val);
		return mq2_val;
	}
}



