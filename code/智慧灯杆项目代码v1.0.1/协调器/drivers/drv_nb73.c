#include "drv_nb73.h"

#define DBG_TAG "nb73"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

rt_device_t NB73_dev;

struct  STRUCT_USARTx_Fram strNB73_Fram_Record = { 0 };
uint8_t NB72_Receive_Buf[50];
uint8_t IOT_DATA_LEN = 0;
int reg_addre1 = 0x0000;
int reg_addre2 = 0x0004;
int reg_addre3 = 0x0008;

/*
�������ܣ�NB73��ʼ��
������void
����ֵ��bool  0:ʧ��  1:�ɹ�
��ע��
*/
bool NB73_IOT_Init(void)
{
	
	rt_kprintf("�˳�͸��ģʽ\n");
	NB73_ExitUnvarnishSend();
	
	rt_kprintf("CoAP͸��ģʽ\n");
	if(NB73_WKMODSet(COAP) == false)
		return false;
	
	rt_kprintf("����������\n");
	if(NB73_Cmd("AT+NCDP=117.60.157.137,5683","OK",NULL,2500) == false)
		return false;
	
	rt_kprintf("��λ\n");
	NB73_Rst();
	
	
	return 1;
}
/*
�������ܣ��˳�͸��ģʽ
������void
����ֵ��void
��ע��
*/
void NB73_ExitUnvarnishSend (void)
{
	
	rt_thread_mdelay(1000);
	
	printf("+++");
	
	rt_thread_mdelay(1000);
	
	printf("a");
	
	rt_thread_mdelay(500); 
	
	
}

/*
�������ܣ�����͸��ģʽ
������void
����ֵ��bool  0:ʧ��  1:�ɹ�
��ע��
*/
bool NB73_UnvarnishSend(void)
{
	if (!NB73_Cmd("AT+ENTM","OK",0,500))
		return false;
	
	return 1;
}
/*
�������ܣ���λ
������void
����ֵ��void
��ע��
*/
void NB73_Rst(void)
{
	NB73_Cmd ("AT+S","OK","LoRa Start!",1000);   	
//	NB73_Cmd ("AT+Z","OK","LoRa Start!",1000);   	
	rt_thread_mdelay(500); 
}
/*
�������ܣ�����ģʽѡ��
������enumWKMOD
����ֵ��bool  0:ʧ��  1:�ɹ�
��ע������ѡ��CMD:ָ��ģʽ NET:NET͸��ģʽ COAP:COAP͸��ģʽ
*/
bool NB73_WKMODSet(ENUM_WKMODTypeDef enumWKMOD)
{
	switch (enumWKMOD)
	{
		case CMD:
			return NB73_Cmd("AT+WKMOD=CMD","OK",NULL,2500); 

		case NET:
		  return NB73_Cmd("AT+WKMOD=NET","OK",NULL,2500); 

		case COAP:
		  return NB73_Cmd("AT+WKMOD=COAP","OK",NULL,2500); 

		default:
		  return false;
  }
}
/*
�������ܣ�����ATָ��
������cmd�������͵�ָ�� reply1��reply2���ڴ�����Ӧ waittime���ȴ���Ӧ��ʱ��
����ֵ��bool  0:ʧ��  1:�ɹ�
��ע��
*/
bool NB73_Cmd(char * cmd,char * reply1,char * reply2,uint32_t waittime)
{  
	
	uint8_t MAX_Repeat = 5;//����ظ�����
	bool NB73_OK_Flag = false;
	
	while(--MAX_Repeat)
	{
		
		strNB73_Fram_Record .InfBit .FramLength = 0; //���¿�ʼ�����µ����ݰ�

		printf("%s\r\n",cmd);

		if ((reply1 == NULL) && (reply2 == NULL))       //����Ҫ��������
			return true;
		
		rt_thread_mdelay(waittime);                 //��ʱ
		
		strNB73_Fram_Record .Data_RX_BUF [ strNB73_Fram_Record .InfBit .FramLength ]  = '\0';

	//	rt_kprintf("%s",strNB73_Fram_Record .Data_RX_BUF);
	  
		if (( reply1 != NULL ) && ( reply2 != NULL ))
			NB73_OK_Flag = ((bool)strstr(strNB73_Fram_Record .Data_RX_BUF,reply1) || 
							  (bool)strstr(strNB73_Fram_Record .Data_RX_BUF,reply2)); 
		
		else if(reply1 != 0)
			NB73_OK_Flag = ((bool)strstr(strNB73_Fram_Record .Data_RX_BUF,reply1));
		
		else
			NB73_OK_Flag = ((bool)strstr(strNB73_Fram_Record .Data_RX_BUF,reply2));
		
		if(NB73_OK_Flag != false)break;
		
	}
	
	if(MAX_Repeat > 0)
		return true;	
	else
		return false;
	

}
/*
�������ܣ�����1��8λ����
������data�������͵�����
����ֵ��bool  0:ʧ��  1:�ɹ�
��ע��
*/
void NB73_Send_A_Data(uint8_t data)
{
	usart_data_transmit(USART2,data);
}

/*
�������ܣ�IOT-NB73���ݴ���
�������û��Զ��� Ĭ�ϣ�void
����ֵ���û��Զ��� Ĭ�ϣ�void
��ע���ú�����Ҫ���ڴ��������������������
	  �������ݷ�ʽΪ�û��Զ��� 
*/
void NB73_HANDLE_DATA(void)
{
	
	uint8_t i = 0;
	
	rt_kprintf("��������������:\n");
	for(i=0;i<IOT_DATA_LEN;i++)
		rt_kprintf("%x ",NB72_Receive_Buf[i]);
	
	rt_kprintf("\n");
	
	MODBUS_DATA_HANDLE(NB72_Receive_Buf);
	
	memset(NB72_Receive_Buf,0,50);
	
	IOT_DATA_LEN = 0;
}

/*
�������ܣ�����Ƿ��ڷ�����Connected�ɹ�
������void
����ֵ��bool  false��ʧ��
��ע��
*/
bool NB73_MQTT_CHACK(void)
{
	bool MQTT_Flag = false;
	
	if(strNB73_Fram_Record .InfBit .FramFinishFlag == 1)
	{
		//rt_kprintf("%s\n",strNB73_Fram_Record .Data_RX_BUF);//Debug
		MQTT_Flag = (bool)strstr(strNB73_Fram_Record .Data_RX_BUF,"Connected");
	}
	if(MQTT_Flag != false)return 1;
	
	return false;
}

int rt_NB73_init(void)
{
	rt_device_t nb73_dev;
	nb73_dev = rt_device_create(RT_Device_Class_Block,64);
	if(nb73_dev == RT_NULL)
	{
		LOG_E("nb73_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	rt_device_register(nb73_dev,"NB73",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//ע��  ��д
	return 0;
}

INIT_DEVICE_EXPORT(rt_NB73_init);
