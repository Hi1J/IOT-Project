#include "user_defined.h"

ALI_JSON ali_json = {.AirTem_Post = Param_Post1,.AirHum_Post = Param_Post2};

ZIGBEE_NODE_STATE STATE_NODE;

NODE_DATA node1_data = {0,0,0,0};
NODE_DATA node2_data;
NODE_DATA node3_data;
NODE_DATA node4_data;
NODE_DATA node5_data;
NODE_DATA node6_data;

char Json_Buf[1024];					 
					 
					 

					 
void JSON_Format(void)
{
	
	char buf[50];
	
	memset(Json_Buf,0,1024);
	
	strncpy(Json_Buf,"{params:{",9);
	
	sprintf(buf,"%s:%.1f,",Param_Post1,(float)(air_quality.TEM - 450) / 10.0);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%.1f,",Param_Post2,(float)(air_quality.HUM)/10.0);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post3,air_quality.PM1_0);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post4,air_quality.PM2_5);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post5,air_quality.PM10);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post6,air_quality.PM1_0_air);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post7,air_quality.PM2_5_air);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post8,air_quality.PM10_air);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post9,air_quality.TSP_0_3um);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post10,air_quality.TSP_0_5um);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post11,air_quality.TSP_1_0um);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post12,air_quality.TSP_2_5um);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post13,air_quality.TSP_5_0um);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post14,air_quality.TSP_10um);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post15,air_quality.TVOC);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d,",Param_Post16,air_quality.CO2);
	
	strcat(Json_Buf,buf);
	
	sprintf(buf,"%s:%d",Param_Post17,air_quality.CH2O);
	
	strcat(Json_Buf,buf);
	
	
	if(STATE_NODE.NODE1)
	{
		sprintf(buf,",\"%s\":%.2f,",Param_Post18,(float)(node1_data._Ph) / 100.0);
	
		strcat(Json_Buf,buf);
		
		sprintf(buf,"\"%s\":%.1f,",Param_Post19,(float)(node1_data._Hum) / 10.0);
		
		strcat(Json_Buf,buf);
		
		
		if(node1_data._Tem & (1 << 15))
		{
			sprintf(buf,"\"%s\":%.1f,",Param_Post20,(float)(node1_data._Tem - 0xFFFF) / 10.0);
		}
		else
		{
			sprintf(buf,"\"%s\":%.1f,",Param_Post20,(float)(node1_data._Tem) / 10.0);
		}
		
		strcat(Json_Buf,buf);
		
		sprintf(buf,"\"%s\":%d",Param_Post21,node1_data._Ec);
		
		strcat(Json_Buf,buf);
	}
	
	strcat(Json_Buf,"}}");
	
	
}

void SHUIfa_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOB);//开GPIOA时钟
	
	rcu_periph_clock_enable(RCU_GPIOC);//开GPIOC时钟
	
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_14);//GPIO工作模式配置
	
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_13);//GPIO工作模式配置
	
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_12);//GPIO工作模式配置
	
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_0);//GPIO工作模式配置
	
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);//GPIO工作模式配置
	
	
	gpio_init(GPIOB,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_5);//GPIO工作模式配置
}

