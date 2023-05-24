#define DBG_TAG "app"
#define DBG_LVL DBG_LOG

#include "app.h"

//����1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//����2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};

//����3
struct rt_thread th3;
rt_uint8_t th3_stack[512] = {0};

//����4
struct rt_thread th4;
rt_uint8_t th4_stack[1024] = {0};

//����5
struct rt_thread th5;
rt_uint8_t th5_stack[512] = {0};

//����6
struct rt_thread th6;
rt_uint8_t th6_stack[512] = {0};


rt_sem_t A72_Respond;//A72��Ӧ�ź���
rt_sem_t A72_Connect;//A72�����ź���
rt_sem_t A72_Data_handle;

rt_sem_t FS_Respond;


rt_uint8_t A72_Device_Connect = 0;//ZIGBEEӲ��״����־λ 0������ 1���쳣 

rt_err_t A72_Cheak;



//����1--ZIGBEE��ʼ��
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
			A72_Device_Connect = 1;//�豸���ӳɹ���־λ��1
			
			rt_thread_startup(&th2);
			rt_thread_startup(&th3);
			
			_flag = rt_thread_suspend(&th1);//��������
				
			if(_flag == RT_EOK)
				rt_kprintf("th2 rt_thread_suspend successed..\n");
			else
				rt_kprintf("th2 rt_thread_suspend failed..\n");
		}
		rt_thread_mdelay(3000);
	}
}


//����2--ZIGBEE(���ݴ���)
void Zigbee_Data_handle(void *parameter)
{
	while(1)
    {
		rt_sem_take(A72_Data_handle,RT_WAITING_FOREVER);//�����ȴ�
		
		A72_HANDLE_DATA();
		
		rt_thread_mdelay(5);
		
    }
}
//����3--ZIGBEE(������)
void Zigbee_Heartbeat(void *parameter)
{
	rt_uint8_t chakbuf[2] = {0x00,0x7A};
	
	while(1)
	{
		
		rt_thread_mdelay(30000);//30s���
		
		A72_SEND_DATA(_B_ALL,&A72_SEND_MODE_BROADCAST,chakbuf,2);//�㲥����
		
		rt_kprintf("ZIGBEE Checking...\n");
		
		
	}

}
//����4--��ȡ��������
void Air_quality_takeValue(void *parameter)
{
	
	int result;
	
	while(1)
	{
		
		rt_thread_mdelay(6000);//�ɼ�Ƶ�� 6s
		
		usart_enable(UART3);//�򿪴���
		
		rt_sem_take(FS_Respond,RT_WAITING_FOREVER);//���õȴ�
		
		usart_disable(UART3);//�رմ���
			
		result = FS_DATA_HANDLE(FS_RX_BUF,FS_RX_LEN);
		
		
		if(result == FS_SUCCESS)
		{
			
			rt_kprintf("Air quality take successed..\n\n");
			
			FS_DATA_DEBUG();//��ӡ����
			
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



