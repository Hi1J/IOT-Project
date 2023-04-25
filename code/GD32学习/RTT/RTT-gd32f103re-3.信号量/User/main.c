#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.h"



rt_thread_t start = NULL;

int flags = 0;//定义一个全局变量 目的是让线程1和线程2都用到


void AppTaskStart(void *parameter)
{
	int ret = 0;//临时变量
	
	//动态软定时创建
//	tm1 = rt_timer_create("tm1_demo",tm1_callback,NULL,3000, //3s执行一次   
//	RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);//周期性 软定时
//	if(tm1 == RT_NULL)
//		LOG_E("rt_timer_create failed..\n");
//	else
//		LOG_D("rt_timer_create successed..\n");
	
	//静态软定时创建
//	rt_timer_init(&tm2,"tm2_demo",tm2_callback,NULL,3000,//3s执行一次 
//	RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);//周期性 软定时 
	
	//动态信号量创建
	sem1 = rt_sem_create("sem1",1,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(sem1 == RT_NULL)
		LOG_E("dynamic sem1 rt_sem_create failed..\n");
	else
		LOG_D("dynamic sem1 rt_sem_create successed..\n");
	
	//静态信号量创建
	ret = rt_sem_init(&sem2,"sem2",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(ret < 0)
		LOG_E("static sem2 rt_sem_create failed..\n");
	else
		LOG_D("static sem2 rt_sem_create successed..\n");
		
	
	//任务1创建
	ret = rt_thread_init(&th1,"th1_",th1_entry,NULL,th1_stack,sizeof(th1_stack),20,5);
	if(ret < 0)
		LOG_E("th1 create failed..\n");
	else
		LOG_D("th1 create successed..\n");
	
	//任务2创建
	ret = rt_thread_init(&th2,"th2_",th2_entry,NULL,th2_stack,sizeof(th2_stack),19,5);
	if(ret < 0)
		LOG_E("th2 create failed..\n");
	else
		LOG_D("th2 create successed..\n");
	
	//任务启动
	rt_thread_startup(&th1);
	rt_thread_startup(&th2);
	
	//软定时启动
//	rt_timer_start(tm1);
//	rt_timer_start(&tm2);
}





//信号量
int main()
{
	//开始任务创建
  start = rt_thread_create("App Task Start", AppTaskStart, NULL,1024, 20, 5);
  if(start == RT_NULL){
     LOG_E("App Task Start create failed..\n");
     return -RT_ENOMEM;
   }

   LOG_D("App Task Start create successed..\n");
   rt_thread_startup(start);//启动
	 
}

