#include "mqtt.h"

char OneNet_tmp[512] = {0};
char OneNet_data_buf[123] = {0};
uint8_t OneNet_Init(void)
{
	
	uint8_t cnt = 5;
	uint8_t flag = 0;
	
	ConnectServer("TCP",OneNet,Prot);//连接OneNET
	 
	 sprintf(OneNet_tmp,
	"POST /devices/%s/datapoints?type=3 HTTP/1.1\r\n"
	"api-key:%s\r\n"
	"Host:api.heclouds.com\r\n"
	"Connection:close\r\n"
	"Content-Length:59\r\n"
	,DeviceID,Api_key);
	
	sprintf(OneNet_data_buf,"\r\n{\"tem\":%d;\"hum\":%d;\"light\":%d}\r\n",0,0,0);
	while(cnt--)//发送数据
	{
		memset(WiFi_Rx_Buff,0,sizeof(WiFi_Rx_Buff));
		UART5_Send_Str((uint8_t *)OneNet_tmp);
		UART5_Send_Str((uint8_t *)OneNet_data_buf);
		if(FindStr((char*)WiFi_Rx_Buff,"{\"errno\":0",500)!=0)
		{
			flag = 1;
			break;
		}
		
	}
	
	if(flag == 1)
		return 1;
	else
		return 0;
	
		

}



