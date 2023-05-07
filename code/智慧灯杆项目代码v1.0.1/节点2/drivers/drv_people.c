#include "drv_people.h"

void rt_people_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA,GPIO_MODE_IPD,GPIO_OSPEED_50MHZ,GPIO_PIN_1);
	
	
}

uint8_t People_scanf(void)
{
	uint8_t IEXIT;
	IEXIT = gpio_input_bit_get(GPIOA,GPIO_PIN_1);

	return IEXIT;
}


void People_ISEIT(void)
{
	if(People_scanf() != SET)
	{
		rt_kprintf("NO Pedestrian detection ......\n");
	}
	else
	{
		rt_kprintf("Pedestrian detection ......\n");
	}

}
MSH_CMD_EXPORT(People_ISEIT,CMD:Detect a pedestrian is passing);	
