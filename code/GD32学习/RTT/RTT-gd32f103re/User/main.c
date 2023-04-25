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
void th_entry(void *parameter)
{
    while(1)
    {
        rt_kprintf("th_enter running...\n");
				if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 0)
				{gpio_bit_set(GPIOA,GPIO_PIN_1);rt_kprintf("LED_ON\n");}
				else if(gpio_output_bit_get(GPIOA,GPIO_PIN_1) == 1)
				{gpio_bit_reset(GPIOA,GPIO_PIN_1);rt_kprintf("LED_0FF\n");}
					
        rt_thread_mdelay(1000);
    }
}


//GD32工程模板
int main()
{
	th1_ptr = rt_thread_create("th_demo", th_entry, NULL, 1024, 20, 5);
  if(th1_ptr == RT_NULL){
     LOG_E("create failed..\n");
     return -RT_ENOMEM;
   }

   LOG_D("create successes\n");
   rt_thread_startup(th1_ptr);//开始

	

}

