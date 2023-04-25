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
rt_err_t A72_Cheak;

//任务3
struct rt_thread th3;
rt_uint8_t th3_stack[512] = {0};

//任务4
struct rt_thread th4;
rt_uint8_t th4_stack[512] = {0};

uint8_t A72_flag = 0;
uint8_t A72_Device_Connect = 0;//A72硬件正常与否标志位


//动态信号量
rt_sem_t A72_Respond;//A72回应信号量
rt_sem_t A72_Connect;//A72连接信号量
rt_sem_t A72_Data_handle;



////任务1 检测入网状态(入网前开启)
//void Zigbee_Network_Status_Cheak_front(void *parameter)
//{
//	while(1)
//    {
//		if(A72_SEND_ORDER(&Connect_status,A72_Read_Connect_status) == ERROR)
//		{
//			
//			rt_kprintf("A72_Read_Connect_status failed..\n");
//			
//			rt_thread_mdelay(5000);
//		}
//		else
//		{
//			
//			rt_kprintf("A72_Read_Connect_status successed..\n");
//			
//			if(_value.S_Network == 0)//如果未连接 5秒后再次尝试
//			{
//				rt_kprintf("A72_Connect_status: OFF\n");
//				
//				rt_thread_mdelay(5000);
//			}
//			else//若连接了 挂起自身 同时开启发送函数
//			{
//				rt_kprintf("A72_Connect_status: ON\n");
//				rt_kprintf("A72_Connect successed..\n");
//				
//				rt_sem_release(A72_Connect);
//				
//				
//				A72_Cheak = rt_thread_suspend(&th1);//挂起自身
//				
//				if(A72_Cheak == RT_EOK)
//					rt_kprintf("th3 rt_thread_suspend successed..\n");
//				else
//					rt_kprintf("th3 rt_thread_suspend failed..\n");
//					
//			 }
//		}	
//    }
//}


////任务2 检测入网状态(入网后开启)
//void Zigbee_Network_Status_Cheak_after(void *parameter)
//{
//	while(1)
//	{
//		
//		if(_value.S_Network == 1)//如果入网
//		{
//			
//			rt_thread_mdelay(45000);//45s一检测
//			
//			rt_kprintf("Connection status Cheaking...\nOnce every 45s\n");
//			
//			if(A72_SEND_ORDER(&Connect_status,A72_Read_Connect_status) == ERROR)
//			{
//				rt_kprintf("Cheak Connect failed..\n");
//			}
//			else
//			{
//				rt_kprintf("Cheak Connect successed..\n");
//				
//				if(_value.S_Network == 0)//如果A72断开
//				{
//					rt_thread_resume(&th1);//恢复th1 A72入网线程
//				}
//				
//			}
//		}
//		else
//		{
//			rt_thread_mdelay(3000);//位入网时3秒检测一次
//		}
//	}
//}

////任务3函数 发送数据
//void Zigbee_Send_Data(void *parameter)
//{
//	
//	uint8_t buf[2] = {0x11,0x10};
//	
//	while(1)
//    {
//		rt_sem_take(A72_Connect,RT_WAITING_FOREVER);//阻塞等待
//		
//		if(A72_SEND_DATA(_COORDINATOR,&A72_SEND_MODE_SHORT,buf,2) == SUCCESS)
//		{
//			rt_kprintf("A72_Send_Data successed..\n");
//		}
//		else
//		{
//			rt_kprintf("A72_Send_Data fialed..\n");
//		}

//        rt_thread_mdelay(6000);//发送频率6s
//		
//		if(_value.S_Network == 1)//如果Zigbee已经入网
//			rt_sem_release(A72_Connect);//解锁
//    }
//}

//任务4函数(数据处理)
void Zigbee_Data_handle(void *parameter)
{
	while(1)
    {
		rt_sem_take(A72_Data_handle,RT_WAITING_FOREVER);//阻塞等待
		
		A72_HANDLE_DATA();
		
		rt_thread_mdelay(500);
		
    }
}


