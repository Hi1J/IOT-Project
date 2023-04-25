#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.h"
#include "drv_usart.h"



rt_thread_t start = NULL;



void AppTaskStart(void *parameter)
{
	int ret = 0;//临时变量
	rt_err_t ret1 = 0;
	
	//串口1初始化
	u1_dev = rt_device_find("usart1");
	if(u1_dev == RT_NULL)
		LOG_E("rt_device_find[usart1] failed..\n");
	else
	{
		LOG_D("rt_device_find[usart1] successed..\n");
		ret1 = rt_device_init(u1_dev);
		if(ret1 < 0)
			LOG_E("rt_device_init[usart1] failed..\n");
		else
			LOG_D("rt_device_init[usart1] successed..\n");
		ret1 = rt_device_open(u1_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret1 < 0)
			LOG_E("rt_device_open[usart1] failed..\n");
		else
			LOG_D("rt_device_open[usart1] successed..\n");		
	}
	
	//动态信号量创建
	sem1 = rt_sem_create("sem1",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(sem1 == RT_NULL)
		LOG_E("dynamic sem1 rt_sem_create failed..\n");
	else
		LOG_D("dynamic sem1 rt_sem_create successed..\n");	
		
	//任务1创建
	ret = rt_thread_init(&th1,"th1_",th1_entry,NULL,th1_stack,sizeof(th1_stack),20,5);
	if(ret < 0)
		LOG_E("th1 create failed..\n");
	else
		LOG_D("th1 create successed..\n");
	
//	//任务2创建
//	ret = rt_thread_init(&th2,"th2_",th2_entry,NULL,th2_stack,sizeof(th2_stack),20,5);
//	if(ret < 0)
//		LOG_E("th2 create failed..\n");
//	else
//		LOG_D("th2 create successed..\n");
//	
//	//任务3创建
//	ret = rt_thread_init(&th3,"th2_",th3_entry,NULL,th3_stack,sizeof(th3_stack),20,5);
//	if(ret < 0)
//		LOG_E("th3 create failed..\n");
//	else
//		LOG_D("th3 create successed..\n");
	
	//任务启动
//	rt_thread_startup(&th2);
//	rt_thread_startup(&th3);
	rt_thread_startup(&th1);
	


}





//串口发送和中断接收
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

