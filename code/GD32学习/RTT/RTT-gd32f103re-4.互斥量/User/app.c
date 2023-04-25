#include "app.h"

//任务1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//任务2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};

//动态互斥锁
rt_mutex_t mutex1;

//静态互斥锁
struct rt_mutex mutex2;




//任务1函数
void th1_entry(void *parameter)
{
	while(1)
    {
		rt_mutex_take(mutex1,RT_WAITING_FOREVER);//上锁 阻塞等待
		flag1++;
		rt_thread_mdelay(1000);
		flag2++;
		rt_mutex_release(mutex1);//解锁			
        
    }
}
//任务2函数
void th2_entry(void *parameter)
{
	while(1)
    {
		rt_mutex_take(mutex1,RT_WAITING_FOREVER);//上锁 阻塞等待
		flag1++;
		flag2++;
		rt_mutex_release(mutex1);//解锁	
		rt_kprintf("flag1:%d,flag2:%d\n",flag1,flag2);
		rt_thread_mdelay(1000);
		
    }
}
