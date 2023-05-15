#define DBG_TAG "app"
#define DBG_LVL DBG_LOG



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
	char buf[30];
	
	while(1)
	{
		
		if(Sensor_TakeAValue(TYPE_PH,REG_PH) == false)
			rt_kprintf("take ph failed..\n");
		
		if(Sensor_TakeAValue(TYPE_HUM,REG_HUM) == false)
			rt_kprintf("take hum failed..\n");
		
		if(Sensor_TakeAValue(TYPE_TEM,REG_TEM) == false)
			rt_kprintf("take tem failed..\n");
		
		if(Sensor_TakeAValue(TYPE_EC,REG_EC) == false)
			rt_kprintf("take ec failed..\n");
		
		rt_kprintf("\n");
		
		sprintf(buf,"PH:%.2fpH",debug_value._PH);
		rt_kprintf("%s\n",buf);
		
		sprintf(buf,"HUM:%.1f%%RH",debug_value._HUM);
		rt_kprintf("%s\n",buf);
		
		sprintf(buf,"TEM:%.1fC",debug_value._TEM);
		rt_kprintf("%s\n",buf);
		
		rt_kprintf("EC:%dus/cm\n",debug_value._EC);
		
		
		rt_thread_mdelay(5000);//5s
		
	}
	
}
//任务2函数
void th2_entry(void *parameter)
{
	
	
	while(1)
	{
		

		rt_thread_mdelay(500);
	}
	
}

