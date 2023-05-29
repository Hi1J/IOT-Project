#define DBG_TAG "app"
#define DBG_LVL DBG_LOG

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

//任务4
struct rt_thread th4;
rt_uint8_t th4_stack[1024] = {0};

//任务5
struct rt_thread th5;
rt_uint8_t th5_stack[512] = {0};

//任务6
struct rt_thread th6;
rt_uint8_t th6_stack[1024] = {0};


rt_sem_t A72_Respond;//A72回应信号量
rt_sem_t A72_Connect;//A72连接信号量
rt_sem_t A72_Data_handle;

rt_sem_t FS_Respond;

rt_sem_t BC28_Respond;

rt_uint8_t A72_Device_Connect = 0;//ZIGBEE硬件状况标志位 0：正常 1：异常 

rt_err_t A72_Cheak;

rt_uint8_t BC28_Connect = 0;

rt_uint8_t MQTT_Connect = 0;


//任务1--ZIGBEE初始化
void WLAN_INIT(void *parameter)
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
			
			rt_thread_startup(&th2);
			rt_thread_startup(&th3);
			
			_flag = rt_thread_suspend(&th1);//挂起自身
				
			if(_flag == RT_EOK)
				rt_kprintf("th2 rt_thread_suspend successed..\n");
			else
				rt_kprintf("th2 rt_thread_suspend failed..\n");
		}
		rt_thread_mdelay(3000);
	}
}


//任务2--ZIGBEE(数据处理)
void Zigbee_Data_handle(void *parameter)
{
	while(1)
    {
		rt_sem_take(A72_Data_handle,RT_WAITING_FOREVER);//阻塞等待
		
		A72_HANDLE_DATA();
		
		rt_thread_mdelay(5);
		
    }
}
//任务3--ZIGBEE(心跳包)
void Zigbee_Heartbeat(void *parameter)
{
	rt_uint8_t chakbuf[2] = {0x00,0x7A};
	
	while(1)
	{
		
		rt_thread_mdelay(30000);//30s检测
		
		A72_SEND_DATA(_B_ALL,&A72_SEND_MODE_BROADCAST,chakbuf,2);//广播发送
		
		rt_kprintf("ZIGBEE Checking...\n");
		
		
	}

}
//任务4--获取空气质量
void Air_quality_takeValue(void *parameter)
{
	
	int result;
	
	
	while(1)
	{
		
		rt_thread_mdelay(6000);//采集频率 6s
		
		usart_enable(UART3);//打开串口
		
		rt_sem_take(FS_Respond,RT_WAITING_FOREVER);//永久等待
		
		usart_disable(UART3);//关闭串口
			
		result = FS_DATA_HANDLE(FS_RX_BUF,FS_RX_LEN);
		
		
		if(result == FS_SUCCESS)
		{
			
			rt_kprintf("Air quality take successed..\n\n");
			
			FS_DATA_DEBUG();//打印数据
			
			rt_kprintf("\n");
		}
		else if(result == FS_LEN_ERROR)
		{
			rt_kprintf("Air quality take failed..\n");
			rt_kprintf("reason: data len error\n");
		}
		else if(result == FS_FRAME_ERROR)
		{
			rt_kprintf("Air quality take failed..\n");
			rt_kprintf("reason: Frame format error\n");
		}
		else if(result == FS_CHECK_ERROR)
		{
			rt_kprintf("Air quality take failed..\n");
			rt_kprintf("reason: Verification error\n");
		}
		
		rt_memset(FS_RX_BUF,0,64);
				
	}
}
//任务5--MQTT初始化
void MQTT_INIT(void *parameter)
{
	
	rt_err_t _flag;
	
	while(1)
	{
		if(BC28_Connect == 0 && BC28_Init() != SUCCESS )
		{
			rt_kprintf("\nBC28 initialization failed!\nTry again after 10 s...\n\n");
		}
		else
		{
			BC28_Connect = 1;
		}
		
		if(BC28_Connect == 1 && MQTT_Connect == 0 && MQTT_Init() != SUCCESS)
		{
			rt_kprintf("\nMQTT initialization failed!\nTry again after 10 s...\n\n");
		}
		else
		{
			MQTT_Connect = 1;
		}
		
		if(BC28_Connect == 1 && MQTT_Connect == 1)
		{
			rt_kprintf("\nMQTTinitialization successed!\n");
			
			rt_thread_startup(&th6);//启动任务6
			
			_flag = rt_thread_suspend(&th5);//挂起自身
				
			if(_flag == RT_EOK)
				rt_kprintf("th5 rt_thread_suspend successed..\n");
			else
				rt_kprintf("th5 rt_thread_suspend failed..\n");
			
		}
		else
		{
			rt_thread_mdelay(10000);
		}
		
	}
	
}
//任务6--上传数据至阿里云
void MQTT_PUB_DATA(void *parameter)
{
	
	char data[100];
	
	char air_tem[10];
	
	char air_hum[10];
	
	while(1)
	{
		
		if(MQTT_Connect == 1)
		{
			
			sprintf(air_tem,"%.1f", (float)(air_quality.TEM - 450) / 10.0);//写入温度
			
			sprintf(air_hum,"%.1f",(float)(air_quality.HUM)/10.0);
		
			sprintf(data,"{params:{%s:%s,%s:%s}}",Param_Post1,air_tem,Param_Post2,air_hum);
			
			if(MQTT_PUB(data) == SUCCESS)
				rt_kprintf("Data upload to Aliyun successed!\n\n");
			else
				rt_kprintf("Data upload to Aliyun failed!\n\n");

		}
		
		rt_thread_mdelay(5000);
	
	}
}


