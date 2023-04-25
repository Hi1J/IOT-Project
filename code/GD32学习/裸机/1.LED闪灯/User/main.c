#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>

//LED闪灯
int main()
{
	systick_config();//时钟初始化
	
	rcu_periph_clock_enable(RCU_GPIOA);//开GPIOA时钟
	
	gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);//GPIO工作模式配置
	
	while(1)
	{
		delay_1ms(1000);
		gpio_bit_set(GPIOA,GPIO_PIN_1);
		delay_1ms(1000);
		gpio_bit_reset(GPIOA,GPIO_PIN_1);
		
	}
}

