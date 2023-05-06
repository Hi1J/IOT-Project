#include "app.h"

#define DBG_TAG "app"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>



//任务4
struct rt_thread th4;
rt_uint8_t th4_stack[2048] = {0};

//任务5
struct rt_thread th5;
rt_uint8_t th5_stack[512] = {0};

//任务6
struct rt_thread th6;
rt_uint8_t th6_stack[512] = {0};

//任务7
struct rt_thread th7;
rt_uint8_t th7_stack[512] = {0};

struct rt_thread th8;
rt_uint8_t th8_stack[512] = {0};

struct rt_thread th9;
rt_uint8_t th9_stack[512] = {0};

struct rt_thread th10;
rt_uint8_t th10_stack[512] = {0};

uint8_t A72_flag = 0;
uint8_t A72_Device_Connect = 0;//A72硬件正常与否标志位
uint8_t NB73_Device_Connect = 0;//NB73硬件正常与否标志位
uint8_t MQTT_Connect = 0;//与服务器连接与否标志位

//节点1标志位
int NODE1_CONNECT_STATUS = 0;//(1:正常状态 0: 异常)
uint8_t NODE1_LED_STATUS = 0;
int NODE1_LIGHT = 28;
int NODE1_PEOPLE = 46;
int NODE1_LED_CONTROL = 0;
uint8_t RX_NODE1 = 0;

//节点2标志位
int NODE2_CONNECT_STATUS = 0; //(1:正常状态 0: 异常)
uint8_t NODE2_LED_STATUS = 0;
int NODE2_LIGHT = 72;
int NODE2_PEOPLE = 38;
int NODE2_LED_CONTROL = 0;
uint8_t RX_NODE2 = 0;

//动态信号量
rt_sem_t A72_Respond;//A72回应信号量
rt_sem_t A72_Connect;//A72连接信号量
rt_sem_t A72_Data_handle;
rt_sem_t NB73_Data_handle;
rt_sem_t LED_Control;//LED亮灭信号量
rt_sem_t NODE_Appear_person;

uint8_t NODE_Appear_dirction = 0;
uint8_t LED1_Control_do = 0;//LED1控制动作标志位
uint8_t LED2_Control_do = 0;//LED2控制动作标志位

//MODBUS接收定时器
struct rt_timer tm1;



//任务4函数(ZIGBEE数据处理)
void Zigbee_Data_handle(void *parameter)
{
	static uint8_t flag_one_hand = 0;//临界区标志位 保证每次只处理一次 防止出错
	
	while(1)
    {
		rt_sem_take(A72_Data_handle,RT_WAITING_FOREVER);//阻塞等待
		
		if(flag_one_hand == 0)
		{
			flag_one_hand = 1;
			
			A72_HANDLE_DATA();
			
			flag_one_hand = 0;
			
			rt_thread_mdelay(5);
		}
		else
			rt_thread_mdelay(5);
		
		
    }
}
//任务5函数 上传有人云服务器
void NB73_Send_Data(void *parameter)
{
    uint8_t buff[60] = {0};
	uint8_t i = 0;
	int len = 0;
	int databuf[3];
	static int led1_control_frist = 1;
	static int led2_control_frist = 1;
	while(1)
	{
		rt_thread_mdelay(3000);
		
		
		if(RX_NODE1 == 1)
		{
			databuf[0] = NODE1_LED_STATUS;
			databuf[1] = NODE1_LIGHT;
			databuf[2] = NODE1_PEOPLE;
			len = MODBUS_Data_Reporting(databuf,3,reg_addre1,buff);
			for(i=0;i<len;i++)
			{
				rt_thread_mdelay(1);
				NB73_Send_A_Data(buff[i]);
			}
			rt_kprintf("NB send node1 data\n");
			RX_NODE1 = 0;
			rt_thread_mdelay(2500);
			if(led1_control_frist == 1)//第一次记录灯的状态给开关
			{
				memset(databuf,0,3);
				memset(buff,0,60);
				databuf[0] = NODE1_LED_STATUS;
				len = MODBUS_Data_Reporting(databuf,1,0x0006,buff);
				for(i=0;i<len;i++)
				{
					rt_thread_mdelay(1);
					NB73_Send_A_Data(buff[i]);
				}
				rt_kprintf("NB send node1 led frist control\n");
				led1_control_frist = 0;
				rt_thread_mdelay(3000);//必须加上
			}
		}
		
		if(RX_NODE2 == 1)
		{
			databuf[0] = NODE2_LED_STATUS;
			databuf[1] = NODE2_LIGHT;
			databuf[2] = NODE2_PEOPLE;
			len = MODBUS_Data_Reporting(databuf,3,reg_addre2,buff);
			for(i=0;i<len;i++)
			{
				rt_thread_mdelay(1);
				NB73_Send_A_Data(buff[i]);
				
			}
			rt_kprintf("NB send node2 data\n");
			RX_NODE2 = 0;
			rt_thread_mdelay(2500);
			if(led2_control_frist == 1)//第一次记录灯的状态给开关
			{
				memset(databuf,0,3);
				memset(buff,0,60);
				databuf[0] = NODE2_LED_STATUS;
				len = MODBUS_Data_Reporting(databuf,1,0x0007,buff);
				for(i=0;i<len;i++)
				{
					rt_thread_mdelay(1);
					NB73_Send_A_Data(buff[i]);
				}
				rt_kprintf("NB send node2 led frist control\n");
				led2_control_frist = 0;
				rt_thread_mdelay(3000);//必须加上
			}
		}
		
		
	}

}
//任务6函数(IOT数据处理)
void IOT_Data_handle(void *parameter)
{
	while(1)
	{
		rt_sem_take(NB73_Data_handle,RT_WAITING_FOREVER);//阻塞等待
		
		NB73_HANDLE_DATA();
		
		
		rt_thread_mdelay(5);
		
	}
}
//任务7 通信模块初始化
void WALN_Init(void *parameter)
{
	rt_err_t _flag;
	static uint8_t count = 0;
	bool mqtt_flag = false;
	
	while(1)
	{
		if(A72_Device_Connect == 0)//A72初始化
		{
			if(A72_Init() == ERROR)
			LOG_E("A72_Init create failed..\n");
			else
			{
				LOG_D("A72_Init create successed..\n");
				A72_Device_Connect = 1;//设备连接成功标志位置1
				
				rt_thread_startup(&th4);
				rt_thread_startup(&th8);
				rt_thread_startup(&th9);
			}
		}
		if(NB73_Device_Connect == 0 || MQTT_Connect == 0)//NB73初始化
		{
			if(NB73_IOT_Init() == false)
				LOG_E("NB73_Init create failed..\n");
			else
			{
				
				LOG_D("NB73_Init create successed..\n");
				
				NB73_Device_Connect = 1;
				
				strNB73_Fram_Record .InfBit .FramLength = 0;
				strNB73_Fram_Record.InfBit.FramFinishFlag = 0;
				memset(strNB73_Fram_Record .Data_RX_BUF,0,300);
				
				count = 0;
				
				while(count < 300)//等待NB连接服务器成功(最长等待时间 30s)
				{
					rt_thread_mdelay(100);
					mqtt_flag = NB73_MQTT_CHACK();
					strNB73_Fram_Record .InfBit .FramLength = 0;
					strNB73_Fram_Record.InfBit.FramFinishFlag = 0;
					if(mqtt_flag != false)
					{
						MQTT_Connect = 1;
						break;
					}
					count++;
				}
				if(MQTT_Connect == 1)
				{
					rt_kprintf("mqtt Connected..\n");
					rt_thread_startup(&th5);
					rt_thread_startup(&th6);
					rt_thread_startup(&th10);
				}
			}
		}
		
		if(A72_Device_Connect == 1 && NB73_Device_Connect == 1 && MQTT_Connect == 1)
		{
			_flag = rt_thread_suspend(&th7);//挂起自身
				
			if(_flag == RT_EOK)
				rt_kprintf("th7 rt_thread_suspend successed..\n");
			else
				rt_kprintf("th7 rt_thread_suspend failed..\n");
		}
		rt_thread_mdelay(5000);//若失败 5s后重新初始化
	}
}	

//任务8函数 ZIGBEE发送心跳包
void CHEK_NODE(void *parameter)
{
	int data[1] = {0};
	uint8_t chakbuf[2] = {0x00,0x7A};
	uint8_t buff1[60] = {0};
	int len = 0;
	uint8_t i = 0;
	data[0] = 0x0000;
	while(1)
	{
		rt_thread_mdelay(45000);//45s检测1次
		
		rt_kprintf("ZIGBEE Checking...\n");
		
		NODE1_CONNECT_STATUS = 0;
		NODE2_CONNECT_STATUS = 0;
		
		A72_SEND_DATA(_NODE1,&A72_SEND_MODE_SHORT,chakbuf,2);
		rt_thread_mdelay(100);
		A72_SEND_DATA(_NODE2,&A72_SEND_MODE_SHORT,chakbuf,2);
		
		rt_thread_mdelay(5000);//等待回应周期(必须大于节点休眠时间 建议所有节点休眠时间设置为一致)
		
		if(NODE1_CONNECT_STATUS == 0)//若得不到回应 上传故障信息至服务器
		{
			rt_kprintf("node1 is abnormal\n");
			if(MQTT_Connect == 1)
			{
				len = MODBUS_Data_Reporting(data,1,0x0000,buff1);
				for(i=0;i<len;i++)
				{
					rt_thread_mdelay(1);
					NB73_Send_A_Data(buff1[i]);
				}
			}
			rt_thread_mdelay(2000);
			
		}
		else
		{
			rt_kprintf("node1 connection is normal\n");
		}
			
		
		if(NODE2_CONNECT_STATUS == 0)//若得不到回应 上传故障信息至服务器
		{
			
			rt_kprintf("node2 is abnormal\n");
			if(MQTT_Connect == 1)
			{
				len = MODBUS_Data_Reporting(data,1,0x0003,buff1);
				for(i=0;i<len;i++)
				{
					rt_thread_mdelay(1);
					NB73_Send_A_Data(buff1[i]);
				}
			}
			
			rt_thread_mdelay(2000);
		}
		else
		{
			rt_kprintf("node2 connection is normal\n");
		}
			
		
	}

}

//任务9函数 发送预测信息
void Turn_on_next(void *parameter)
{
	uint8_t buf[3] = {0};
	buf[0] = 0x00;//协调器发送
	buf[1] = 0x78;//转向标志位
	while(1)
	{
		rt_sem_take(NODE_Appear_person,RT_WAITING_FOREVER);
		buf[2] = NODE_Appear_dirction;
		switch(NODE_Appear_dirction)
		{
			case 0x12:
				A72_SEND_DATA(_NODE2,&A72_SEND_MODE_SHORT,buf,3);
				rt_kprintf("turn:1-->2");
				break;
			case 0x21:
				A72_SEND_DATA(_NODE1,&A72_SEND_MODE_SHORT,buf,3);
				rt_kprintf("turn:2-->1");
				break;
			default:
				break;
		}
		rt_thread_mdelay(100);
	}
}

void LED_CONTROL(void *parameter)
{
	uint8_t buf[3] = {0};
	buf[0] = 0x00;//协调器发送
	buf[1] = 0x8A;//控制灯亮灭标志位
	
	while(1)
	{
		
		rt_sem_take(LED_Control,RT_WAITING_FOREVER);
		
		if(LED1_Control_do == 1)
		{
			buf[2] = NODE1_LED_CONTROL;
			A72_SEND_DATA(_NODE1,&A72_SEND_MODE_SHORT,buf,3);
			LED1_Control_do = 0;
			if(NODE1_LED_CONTROL == 0)
				rt_kprintf("Control LED1 0FF\n");
			else
				rt_kprintf("Control LED1 ON\n");
		}
		
		
		if(LED2_Control_do == 1)
		{
			buf[2] = NODE2_LED_CONTROL;
			A72_SEND_DATA(_NODE2,&A72_SEND_MODE_SHORT,buf,3);
			LED2_Control_do = 0;
			if(NODE2_LED_CONTROL == 0)
				rt_kprintf("Control LED2 0FF\n");
			else
				rt_kprintf("Control LED2 ON\n");
		}
			
		
		rt_thread_mdelay(5);
	}
}
