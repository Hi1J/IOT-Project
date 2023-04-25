#include "key.h"


void KEY_Indenpendent_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
	GPIO_InitStructure.GPIO_Pin = KEY1 | KEY2 | KEY3 | KEY4 | KEY5 | KEY6 | KEY7 | KEY8;
	
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	


}

//按键扫描
u8 Key_GetValue(void)
{
	u8 KeyValue = 0;
	u8 PinValue = 0;
	
	PinValue = GPIO_ReadInputData(GPIOA);//读取GPIOA状态
	
	PinValue = PinValue & 0xFF;//读取某个值
	
	if(PinValue)//如果有按键按下
	{
		delay_ms(15);//延时15ms
		
		PinValue = GPIO_ReadInputData(GPIOA);
	
		PinValue = PinValue & 0xFF;//读取某个值
		
		if(PinValue)//再次判断
		{
			for(KeyValue = 0;KeyValue < 8;KeyValue++)
			{
				if(PinValue & (1 << KeyValue))//判断是哪个值
				{
					KeyValue = KeyValue + 1; //返回值加1
					break;//跳出循环
				}
			
			}
			do{//判断是否放手
				PinValue = GPIO_ReadInputData(GPIOA);
				PinValue = PinValue & 0xFF;
			}while(PinValue);//如果没放手 在这里等待
		}
	
	}
	return KeyValue;//返回哪个按键
	
}