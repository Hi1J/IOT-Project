#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#define GET_PIN(PORTx,PIN) (rt_base_t)((16 * ( ((rt_base_t)__GD32_PORT(PORTx) - (rt_base_t)GPIO_BASE)/(0x0400UL) )) + PIN)
#define PIN_PORT(pin) ((uint8_t)(((pin) >> 4) & 0xFu))
#define PIN_NO(pin) ((uint8_t)((pin) & 0xFu))
#define PIN_GDPORT(pin) (GPIO_BASE + (0x400u * PIN_PORT(pin)))
#define PIN_GDPIN(pin) ((uint16_t)(1u << PIN_NO(pin)))


rt_thread_t th1_ptr = NULL;
struct rt_thread th2;

rt_uint8_t th2_stack[512] = {0};


void th1_entry(void *parameter)
{
    while(1)
    {
        rt_kprintf("th1_enter running...\n");
//				if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 0)
//				{gpio_bit_set(GPIOA,GPIO_PIN_1);rt_kprintf("LED_ON\n");}
//				else if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 1)
//				{gpio_bit_reset(GPIOA,GPIO_PIN_1);rt_kprintf("LED_0FF\n");}
					
        rt_thread_mdelay(1000);
    }
}

void th2_entry(void *parameter)
{
//	int i = 0;
//    for(i=0;i<10;i++)
		while(1)
    {
//        rt_kprintf("th2_enter running...\n");
				if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 0)
				{gpio_bit_set(GPIOA,GPIO_PIN_1);rt_kprintf("LED_ON\n");}
				else if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 1)
				{gpio_bit_reset(GPIOA,GPIO_PIN_1);rt_kprintf("LED_0FF\n");}
					
        rt_thread_mdelay(700);
    }
}



//GD32工程模板
int main()
{
	int ret = 0;
	th1_ptr = rt_thread_create("th1_demo", th1_entry, NULL, 1024, 20, 5);
  if(th1_ptr == RT_NULL){
     LOG_E("th1 create failed..\n");
     return -RT_ENOMEM;
   }

   LOG_D("th1 create successes..\n");
   rt_thread_startup(th1_ptr);//开始
	 
	 ret = rt_thread_init(&th2,"th2_demo",th2_entry,NULL,th2_stack,sizeof(th2_stack),19,5);
	 if(ret < 0)
	 {
			LOG_E("th2 create failed..\n");
			return ret;
	 }
	 LOG_D("th2 create successes..\n");
	rt_thread_startup(&th2);

}

