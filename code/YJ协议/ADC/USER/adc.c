#include "adc.h"


void ADC1_CH4_Init(void)
{
	/*GPIOA配置*/
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;//ADC1_CH4
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	/*配置ADC1工作方式*/
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//单次模式
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发启动
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;//独立工作
	ADC_InitStructure.ADC_NbrOfChannel = 1;//进行规则转换的ADC 通道数为1
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;//单次模式
	ADC_Init(ADC1,&ADC_InitStructure);
	
	
	/*设置ADC1*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_4,1,ADC_SampleTime_239Cycles5);//通道4 
	
	ADC_Cmd(ADC1,ENABLE);//使能ADC1
}

u16 ADC1_CH4_GetValue(void)
{
	u16 value;
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件启动
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待SR硬件置1
	
	value = ADC_GetConversionValue(ADC1);//返回规则组转换结果 即读取DR寄存器的值
	
	return value;
}

