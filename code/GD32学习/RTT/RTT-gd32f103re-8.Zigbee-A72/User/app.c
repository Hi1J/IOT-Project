#include "app.h"

#define DBG_TAG "app"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

//任务1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//任务2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};



//动态信号量
rt_sem_t sem1;



//任务1函数
void th1_entry(void *parameter)
{
	while(1)
    {
//		rt_kprintf("hello\n");
        rt_thread_mdelay(1000);
    }
}
//任务2函数
void th2_entry(void *parameter)
{
	while(1)
    {
//		if(A72_Read_Address() == SUCCESS)
//			rt_kprintf("successed..\n");
		rt_thread_mdelay(1000);
		
    }
}

