#ifndef __USER_DEFINED_H
#define __USER_DEFINED_H


#include "app.h"

/*���ܣ�ȡ��ֵַ ȡֵ���ͣ�char*  ������ x:��ַ y :ƫ��  */
#define CharValue(x,y)  (*(char **)(x + y))


#define Zigbee_soildata     0x8A
#define Zigbee_check        0x7A


typedef struct
{
	char *AirTem_Post;
	char *AirTem_Value;
	char *AirHum_Post;
	char *AirHum_Value;
	
}ALI_JSON;

typedef struct 
{
	rt_uint16_t _Tem;
	rt_uint16_t _Hum;
	rt_uint16_t _Ph;
	rt_uint16_t _Ec;
}NODE_DATA;



/*�ڵ�״̬*/
typedef struct
{
	rt_uint8_t NODE1;
	rt_uint8_t NODE2;
	rt_uint8_t NODE3;
	rt_uint8_t NODE4;
	rt_uint8_t NODE5;
	rt_uint8_t NODE6;
}ZIGBEE_NODE_STATE;

extern ALI_JSON ali_json;
extern char Json_Buf[1024];
extern ZIGBEE_NODE_STATE STATE_NODE;
extern NODE_DATA node1_data;
extern NODE_DATA node2_data;
extern NODE_DATA node3_data;
extern NODE_DATA node4_data;
extern NODE_DATA node5_data;
extern NODE_DATA node6_data;


void JSON_Format(void);
void SHUIfa_init(void);



#endif

