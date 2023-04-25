#include "adc.h"


//PB1 -- ADC1 CH9
void ADC1_CH9_Init(void)
{
	//时钟配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1,ENABLE);
	
	//GPIOB配置
	GPIO_InitTypeDef x;
	x.GPIO_Mode = GPIO_Mode_AIN;//模拟输入
	x.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOB,&x);
	
	//ADC1配置
	ADC_InitTypeDef y;
	y.ADC_ContinuousConvMode = DISABLE;//单次模式
	y.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
	y.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;//软件触发
	y.ADC_Mode = ADC_Mode_Independent;//独立工作
	y.ADC_NbrOfChannel = 1;//进行规则转换的ADC 通道数为1
	y.ADC_ScanConvMode = DISABLE;//单次模式（不扫描）
	ADC_Init(ADC1,&y);
	
	/*设置ADC1*/
	ADC_RegularChannelConfig(ADC1,ADC_Channel_9,1,ADC_SampleTime_239Cycles5);//ADC1 , 通道9 规则组采样顺序1 
	
	//使能ADC1
	ADC_Cmd(ADC1,ENABLE);
}

u16 MQ_2_GetValue(void)
{
	u16 value;
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);//软件启动
	
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));//等待SR硬件置1
	
	value = ADC_GetConversionValue(ADC1);//返回规则组转换结果 即读取DR寄存器的值
	
	return value;


}




