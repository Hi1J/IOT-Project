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

//动态事件集
rt_event_t event1;

//静态事件集
struct rt_event event2;




//任务1函数
void th1_entry(void *parameter)
{
	while(1)
    {
		rt_event_recv(event1,EVENT_FLAGS_1,RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND,//接收事件1
		RT_WAITING_FOREVER,NULL);//阻塞等待
		rt_kprintf("111111th1_entry...\n");
		rt_event_send(event1,EVENT_FLAGS_2);//发送事件2
		rt_thread_mdelay(1000);
        
    }
}
//任务2函数
void th2_entry(void *parameter)
{
	while(1)
    {
		rt_event_recv(event1,EVENT_FLAGS_2,RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND,//接收事件2
		RT_WAITING_FOREVER,NULL);//阻塞等待
		rt_kprintf("222222th2_entry...\n");
		rt_event_send(event1,EVENT_FLAGS_3);//发送事件3
		rt_thread_mdelay(1000);
		
    }
}
//任务3函数
void th3_entry(void *parameter)
{
	while(1)
    {
		rt_event_recv(event1,EVENT_FLAGS_3,RT_EVENT_FLAG_CLEAR | RT_EVENT_FLAG_AND,//接收事件3
		RT_WAITING_FOREVER,NULL);//阻塞等待
		rt_kprintf("333333th3_entry...\n");
		rt_event_send(event1,EVENT_FLAGS_1);//发送事件1
		rt_thread_mdelay(1000);
		
    }
}
