#include "app.h"

//任务1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//任务2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};

//动态软定时
rt_timer_t tm1;

//静态软定时
struct rt_timer tm2;
int tm2_flags = 0;


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

//调度钩子函数
void scheduler_hook(struct rt_thread *from,struct rt_thread *to)
{
	rt_kprintf("from:%s ---> to:%s\n",from->name,to->name);
}

//动态软定时回调函数
void tm1_callback(void *parameter)
{
	rt_kprintf("111111111111tm1_callback running....\n");
}

//静态软定时回调函数
void tm2_callback(void *parameter)
{
	rt_tick_t timeout = 1000;//时间改为1s
	tm2_flags++;
	if(tm2_flags == 10)//如果执行了10次 则改为单次触发
	{
		rt_timer_control(&tm2,RT_TIMER_CTRL_SET_ONESHOT,NULL);
		tm2_flags = 0;
	}
	rt_timer_control(&tm2,RT_TIMER_CTRL_SET_TIME,(void *)&timeout);//修改时间
	rt_kprintf("[%u]222222222222tm2_callback running....[%d]\n",rt_tick_get(),tm2_flags);
}
