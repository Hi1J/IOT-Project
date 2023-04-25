#include "stm32f10x.h"
#include "delay.h"
#include "led.h"
#include "key.h"

//按键1 - 8 控制 LED 1 - 8
int main(void)
{
	u8 KeyValue;//设置一个值记录哪个按键按下
	LED_Init(); //LED初始化
	
	KEY_Indenpendent_Init();//独立按键初始化
	
	while(1)
	{
		KeyValue = Key_GetValue();//扫描
		switch(KeyValue)
		{
			case 1:LED1_TOGGLE;break;
			case 2:LED2_TOGGLE;break;
			case 3:LED3_TOGGLE;break;
			case 4:LED4_TOGGLE;break;
			case 5:LED5_TOGGLE;break;
			case 6:LED6_TOGGLE;break;
			case 7:LED7_TOGGLE;break;
			case 8:LED8_TOGGLE;break;
			default:break;
		}
		
		
	}
	
	

}
