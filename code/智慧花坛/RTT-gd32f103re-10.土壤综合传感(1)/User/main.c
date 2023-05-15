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
	
	//U3设备初始化
	u3_dev = rt_device_find("uart3");
	if(u3_dev == RT_NULL)
		LOG_E("rt_device_find[uart3] failed..\n");
	else
	{
		LOG_D("rt_device_find[uart3] successed..\n");
		ret = rt_device_init(u3_dev);
		if(ret < 0)
			LOG_E("rt_device_init[uart3] failed..\n");
		else
			LOG_D("rt_device_init[uart3] successed..\n");
		ret = rt_device_open(u3_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret < 0)
			LOG_E("rt_device_open[uart3] failed..\n");
		else
			LOG_D("rt_device_open[uart3] successed..\n");		
	}
	
	//Senor设备初始化
	Sensor_dev = rt_device_find("sensor");
	if(Sensor_dev == RT_NULL)
		LOG_E("rt_device_find[sensor] failed..\n");
	else
	{
		LOG_D("rt_device_find[sensor] successed..\n");
		ret = rt_device_init(Sensor_dev);
		if(ret < 0)
			LOG_E("rt_device_init[sensor] failed..\n");
		else
			LOG_D("rt_device_init[sensor] successed..\n");
		ret = rt_device_open(Sensor_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret < 0)
			LOG_E("rt_device_open[sensor] failed..\n");
		else
			LOG_D("rt_device_open[sensor] successed..\n");		
	}
	
	//动态信号量创建
	modbus_respond = rt_sem_create("modbus_respond",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(modbus_respond == RT_NULL)
		LOG_E("dynamic modbus_respond rt_sem_create failed..\n");
	else
		LOG_D("dynamic modbus_respond rt_sem_create successed..\n");
	
	//动态硬定时创建
	modbus_timer = rt_timer_create("modbus_timer",modbus_callback,NULL,1, //1ms执行一次   
	RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_HARD_TIMER);//周期性 软定时
	if(modbus_timer == RT_NULL)
		LOG_E("rt_timer_create failed..\n");
	else
		LOG_D("rt_timer_create successed..\n");
	
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
	rt_timer_start(modbus_timer);
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

