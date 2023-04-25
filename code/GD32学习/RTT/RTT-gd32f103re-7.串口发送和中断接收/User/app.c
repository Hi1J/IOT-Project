#include "app.h"

//任务1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//任务2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};

//任务3
struct rt_thread th3;
rt_uint8_t th3_stack[512] = {0};

//动态信号量
rt_sem_t sem1;



//任务1函数
void th1_entry(void *parameter)
{
	while(1)
    {
		if(rt_sem_take(sem1,RT_WAITING_NO) == 0)
			rt_kprintf("yes\n");
        rt_thread_mdelay(1);
    }
}
//任务2函数
void th2_entry(void *parameter)
{
	while(1)
    {
	
		
    }
}
//任务3函数
void th3_entry(void *parameter)
{
	while(1)
    {

		
    }
}
