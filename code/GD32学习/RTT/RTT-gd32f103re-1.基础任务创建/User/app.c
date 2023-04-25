#include "app.h"

//任务1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//任务2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};



//任务1函数
void th1_entry(void *parameter)
{
		while(1)
    {
				if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 0)
				{gpio_bit_set(GPIOA,GPIO_PIN_1);rt_kprintf("LED_ON\n");}
				else if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 1)
				{gpio_bit_reset(GPIOA,GPIO_PIN_1);rt_kprintf("LED_0FF\n");}
					
        rt_thread_mdelay(500);
    }
}
//任务2函数
void th2_entry(void *parameter)
{
		while(1)
    {
        rt_kprintf("th2_enter running...\n");	
        rt_thread_mdelay(1000);
    }
}
