#include "gd32f10x.h"
#include "systick.h"
#include <stdio.h>
#include <rtthread.h>
#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>
#include "app.h"



rt_thread_t start = NULL;



void AppTaskStart(void *parameter)
{
	int ret = 0;//临时变量
	rt_err_t ret1 = 0;
	
	//串口1设备初始化
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

	//串口2设备初始化
	u2_dev = rt_device_find("usart2");
	if(u2_dev == RT_NULL)
		LOG_E("rt_device_find[usart2] failed..\n");
	else
	{
		LOG_D("rt_device_find[usart2] successed..\n");
		ret1 = rt_device_init(u2_dev);
		if(ret1 < 0)
			LOG_E("rt_device_init[usart2] failed..\n");
		else
			LOG_D("rt_device_init[usart2] successed..\n");
		ret1 = rt_device_open(u2_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret1 < 0)
			LOG_E("rt_device_open[usart2] failed..\n");
		else
			LOG_D("rt_device_open[usart2] successed..\n");		
	}

	//A72设备初始化
	A72_dev = rt_device_find("A72");
	if(A72_dev == RT_NULL)
		LOG_E("rt_device_find[A72] failed..\n");
	else
	{
		LOG_D("rt_device_find[A72] successed..\n");
		ret1 = rt_device_init(A72_dev);
		if(ret1 < 0)
			LOG_E("rt_device_init[A72] failed..\n");
		else
			LOG_D("rt_device_init[A72] successed..\n");
		ret1 = rt_device_open(A72_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret1 < 0)
			LOG_E("rt_device_open[A72] failed..\n");
		else
			LOG_D("rt_device_open[A72] successed..\n");		
	}

	//NB73设备初始化
	NB73_dev = rt_device_find("NB73");
	if(NB73_dev == RT_NULL)
		LOG_E("rt_device_find[NB73] failed..\n");
	else
	{
		LOG_D("rt_device_find[NB73] successed..\n");
		ret1 = rt_device_init(NB73_dev);
		if(ret1 < 0)
			LOG_E("rt_device_init[NB73] failed..\n");
		else
			LOG_D("rt_device_init[NB73] successed..\n");
		ret1 = rt_device_open(NB73_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret1 < 0)
			LOG_E("rt_device_open[NB73] failed..\n");
		else
			LOG_D("rt_device_open[NB73] successed..\n");		
	}
	
	//动态信号量创建
	A72_Respond = rt_sem_create("A72_Respond",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Respond == RT_NULL)
		LOG_E("dynamic A72_Respond rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Respond rt_sem_create successed..\n");	
	
	A72_Connect = rt_sem_create("A72_Connect",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Connect == RT_NULL)
		LOG_E("dynamic A72_Connect rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Connect rt_sem_create successed..\n");
	
	A72_Data_handle = rt_sem_create("A72_Data_handle",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Data_handle == RT_NULL)
		LOG_E("dynamic A72_Data_handle rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Data_handle rt_sem_create successed..\n");
	
	NB73_Data_handle = rt_sem_create("NB73_Data_handle",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(NB73_Data_handle == RT_NULL)
		LOG_E("dynamic NB73_Data_handle rt_sem_create failed..\n");
	else
		LOG_D("dynamic NB73_Data_handle rt_sem_create successed..\n");
	
	NODE_Appear_person = rt_sem_create("NODE_Appear_person",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(NODE_Appear_person == RT_NULL)
		LOG_E("dynamic NODE1_Appear_person rt_sem_create failed..\n");
	else
		LOG_D("dynamic NODE1_Appear_person rt_sem_create successed..\n");
	
	LED_Control = rt_sem_create("LED_Control",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(LED_Control == RT_NULL)
		LOG_E("dynamic LED_Control rt_sem_create failed..\n");
	else
		LOG_D("dynamic LED_Control rt_sem_create successed..\n");

	
	//任务4创建
	ret = rt_thread_init(&th4,"Zigbee_Data_handle",Zigbee_Data_handle,NULL,th4_stack,sizeof(th4_stack),16,5);
	if(ret < 0)
		LOG_E("th4 create failed..\n");
	else
		LOG_D("th4 create successed..\n");
	
	//任务5创建
	ret = rt_thread_init(&th5,"NB73_Send_Data",NB73_Send_Data,NULL,th5_stack,sizeof(th5_stack),23,5);
	if(ret < 0)
		LOG_E("th5 create failed..\n");
	else
		LOG_D("th5 create successed..\n");
	
	//任务6创建
	ret = rt_thread_init(&th6,"IOT_Data_handle",IOT_Data_handle,NULL,th6_stack,sizeof(th6_stack),19,5);
	if(ret < 0)
		LOG_E("th6 create failed..\n");
	else
		LOG_D("th6 create successed..\n");
	
	//任务7创建
	ret = rt_thread_init(&th7,"WALN_Init",WALN_Init,NULL,th7_stack,sizeof(th7_stack),24,5);
	if(ret < 0)
		LOG_E("th7 create failed..\n");
	else
		LOG_D("th7 create successed..\n");
	
	//任务8创建
	ret = rt_thread_init(&th8,"CHEK_NODE",CHEK_NODE,NULL,th8_stack,sizeof(th8_stack),25,5);
	if(ret < 0)
		LOG_E("th8 create failed..\n");
	else
		LOG_D("th8 create successed..\n");
	
	//任务9创建
	ret = rt_thread_init(&th9,"Turn_on_next",Turn_on_next,NULL,th9_stack,sizeof(th9_stack),17,5);
	if(ret < 0)
		LOG_E("th9 create failed..\n");
	else
		LOG_D("th9 create successed..\n");
	
	//任务10创建
	ret = rt_thread_init(&th10,"LED_CONTROL",LED_CONTROL,NULL,th10_stack,sizeof(th10_stack),18,5);
	if(ret < 0)
		LOG_E("th10 create failed..\n");
	else
		LOG_D("th10 create successed..\n");
	
		
	rt_thread_startup(&th7);

}





//ZIGBEE-A72
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

