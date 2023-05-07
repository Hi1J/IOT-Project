/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-07     y zy       the first version
 */
#include "app.h"


#define DBG_TAG "app"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>


rt_uint8_t people_flog = 0;
rt_uint16_t people_num = 0;

rt_uint16_t Dates1[1];
rt_uint8_t Dates[1];

rt_uint8_t A72_Device_Connect = 0;//ZIGBEE硬件状况标志位 0：正常 1：异常 

rt_err_t A72_Cheak;

rt_thread_t th1_ptr = RT_NULL;
struct rt_thread th2;
struct rt_thread th3;
struct rt_thread th4;
struct rt_thread th5;
struct rt_thread th6;
struct rt_thread th7;
struct rt_thread th8;
//struct rt_thread th8;

rt_uint8_t th2_stack[512] = {0};
rt_uint8_t th3_stack[512] = {0};
rt_uint8_t th4_stack[512] = {0};
rt_uint8_t th5_stack[512] = {0};
rt_uint8_t th6_stack[512] = {0};
rt_uint8_t th7_stack[512] = {0};
rt_uint8_t th8_stack[512] = {0};
//rt_uint8_t th8_stack[512] = {0};

rt_sem_t dynamic_sem = RT_NULL;

//动态信号量
rt_sem_t A72_Respond;//A72回应信号量
rt_sem_t A72_Connect;//A72连接信号量
rt_sem_t A72_Data_handle;

uint8_t Led_Status = 1;
uint8_t Turn_next = 0;

#ifdef FINSH_USING_MSH

void th1_test_start(void *paremeter)
{
    th1_ptr = rt_thread_create("th1", th1_test, NULL, 512, th1_test_priority, 5);//动态创建任务
    if(th1_ptr != RT_NULL){
        rt_thread_startup(th1_ptr);
        LOG_D("th1 created successfully\n");
    }else{
        LOG_E("th1 created failed\n");
    }
}

MSH_CMD_EXPORT(th1_test_start, th1 start);

#endif

void th1_test(void* paremeter)
{
    while(1){
		
        return;
    }
}

//ZIGBEE初始化
void WLAN_INIT(void* paremeter)
{
	
	rt_err_t _flag;
	
	while(1)
	{
		if(A72_Init() == ERROR)
		rt_kprintf("A72_Init create failed..\n");
		else
		{
			rt_kprintf("A72_Init create successed..\n");
			A72_Device_Connect = 1;//设备连接成功标志位置1
			
			rt_thread_startup(&th3);
			rt_thread_startup(&th4);
			rt_thread_startup(&th5);
			rt_thread_startup(&th6);
			
			_flag = rt_thread_suspend(&th2);//挂起自身
				
			if(_flag == RT_EOK)
				rt_kprintf("th2 rt_thread_suspend successed..\n");
			else
				rt_kprintf("th2 rt_thread_suspend failed..\n");
		}
		rt_thread_mdelay(2000);
	}
}

//ZIGBEE检测入网状态(入网前开启)
void Zigbee_Network_Status_Cheak_front(void* paremeter)
{

	while(1)
    {
		if(A72_SEND_ORDER(&Connect_status,A72_Read_Connect_status) == ERROR)
		{
			
			rt_kprintf("A72_Read_Connect_status failed..\n");
			
			rt_thread_mdelay(5000);
		}
		else
		{
			
			rt_kprintf("A72_Read_Connect_status successed..\n");
			
			if(_value.S_Network == 0)//如果未连接 5秒后再次尝试
			{
				rt_kprintf("A72_Connect_status: OFF\n");
				
				rt_thread_mdelay(5000);
			}
			else//若连接了 挂起自身 同时开启发送函数
			{
				rt_kprintf("A72_Connect_status: ON\n");
				rt_kprintf("A72_Connect successed..\n");
				
				rt_sem_release(A72_Connect);
				
				
				A72_Cheak = rt_thread_suspend(&th3);//挂起自身
				
				if(A72_Cheak == RT_EOK)
					rt_kprintf("th3 rt_thread_suspend successed..\n");
				else
					rt_kprintf("th3 rt_thread_suspend failed..\n");
					
			 }
		}	
    }
}

//ZIGBEE检测入网状态(入网后开启)
void Zigbee_Network_Status_Cheak_after(void *parameter)
{
	while(1)
	{
		
		if(_value.S_Network == 1)//如果入网
		{
			
			rt_thread_mdelay(45000);//45s一检测
			
			rt_kprintf("Connection status Cheaking...\nOnce every 45s\n");
			
			if(A72_SEND_ORDER(&Connect_status,A72_Read_Connect_status) == ERROR)
			{
				rt_kprintf("Cheak Connect failed..\n");
			}
			else
			{
				rt_kprintf("Cheak Connect successed..\n");
				
				if(_value.S_Network == 0)//如果A72断开
				{
					rt_thread_resume(&th3);//恢复th1 A72入网线程
				}
				
			}
		}
		else
		{
			rt_thread_mdelay(3000);//位入网时3秒检测一次
		}
	}
}

//ZIGBEE发送数据
void Zigbee_Send_Data(void *parameter)
{
	
	uint8_t send_buf[6];
	
	
	while(1)
    {
		rt_sem_take(A72_Connect,RT_WAITING_FOREVER);//阻塞等待
		
		
		send_buf[0] = 0x02;
		send_buf[1] = Dates1[0] >> 8;
		send_buf[2] = Dates1[0] & 0xFF;
		send_buf[3] = people_num >> 8;
		send_buf[4] = people_num & 255;
		//send_buf[5] = _value.S_Network;
		send_buf[5] = Led_Status;
		
		if(A72_SEND_DATA(_COORDINATOR,&A72_SEND_MODE_SHORT,send_buf,6) == SUCCESS)
		{
			rt_kprintf("A72_Send_Data successed..\n");
		}
		else
		{
			rt_kprintf("A72_Send_Data fialed..\n");
		}

        rt_thread_mdelay(3000);//发送频率3s
		
		if(_value.S_Network == 1)//如果Zigbee已经入网
			rt_sem_release(A72_Connect);//解锁
    }
}

//ZIGBEE(数据处理)
void Zigbee_Data_handle(void *parameter)
{
	while(1)
    {
		rt_sem_take(A72_Data_handle,RT_WAITING_FOREVER);//阻塞等待
		
		A72_HANDLE_DATA();
		
		rt_thread_mdelay(500);
		
    }
}


void th7_test(void* paremeter)
{
	rt_device_t dev_light;
	rt_device_t dev_people;
	

	dev_light = rt_device_find("Light_Sensor");
	dev_people = rt_device_find("people_scanf");

	if(dev_light == RT_NULL){
		LOG_E("rt_device_find[Light_Sensor] failed...\n");
		}
	if(dev_people == RT_NULL){
		LOG_E("rt_device_find[Light_Sensor] failed...\n");
		}
	
	rt_device_init(dev_people);
	rt_device_init(dev_light);

	rt_device_open(dev_people,RT_DEVICE_OFLAG_RDWR);
	rt_device_open(dev_light,RT_DEVICE_OFLAG_RDWR);
	while (1)
	{
		
		rt_device_read(dev_light,0,Dates1,1);
		rt_device_read(dev_people,0,Dates,1);
		
		
		rt_thread_mdelay(500);
	}
}



void th8_test(void* paremeter)
{
		uint16_t untill = 0;
		PWM1_CH1_Crol(0);
    while(1){
			if (Dates1[0] <= 300){
					
					if(Turn_next == 1)
					{
						Turn_next = 0;
						PWM1_CH1_Crol(350);
						rt_kprintf("person is comming!\n");
						rt_thread_mdelay(2000);
					}
					else if(Dates[0] == 1 && Turn_next == 0)
					{
						people_flog ++;
						if(people_flog == 1)
						{
							people_num ++;
							if(_value.S_Network == 1)
							{
							   uint8_t buf[3] = {0x02,0x78,0x21};
							   if(A72_SEND_DATA(_NODE1,&A72_SEND_MODE_SHORT,buf,3) == SUCCESS)
								{
									rt_kprintf("A72_Send_Data successed..\n");
								}
								else
								{
									rt_kprintf("A72_Send_Data fialed..\n");
								}
								rt_kprintf("turn on 2->1\n");
								
								buf[2] = 0x23;
								if(A72_SEND_DATA(_NODE3,&A72_SEND_MODE_SHORT,buf,3) == SUCCESS)
								{
									rt_kprintf("A72_Send_Data successed..\n");
								}
								else
								{
									rt_kprintf("A72_Send_Data fialed..\n");
								}
								rt_kprintf("turn on 2->3\n");
							}
						   
						}
						if(untill<3)
						{
						rt_kprintf("Pedestrian detected !!!\n");
						PWM1_CH1_Crol(350);

						untill ++;
						}
						if(untill >= 3)
						{
							PWM1_CH1_Crol(500);
							rt_kprintf("Pedestrian detected biggg !!!\n");
						}
						
					}
					else
					{
						people_flog = 0;
						untill = 0;
			//			rt_kprintf("No Pedestrian detected !!!");
						PWM1_CH1_Crol(40);
					}
					//rt_thread_mdelay(1000);
			}
			else
			{
				
				PWM1_CH1_Crol(0);
				people_flog = 0;
				//rt_thread_mdelay(1000);
			}
			
			
			rt_thread_mdelay(1000);
		}

}	
