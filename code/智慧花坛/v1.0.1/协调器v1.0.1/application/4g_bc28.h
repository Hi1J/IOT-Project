#ifndef __4G_BC28_H_
#define __4G_BC28_H_

#include "app.h"


#define ProductKey "i3uocf6XqNy"						
#define DeviceName "bc26_1"					
#define DeviceSecret "8150afbe44b5c6ed14fd16adf40e4b72" 

//#define Domain "iot-06z00dymnfjkl1d.mqtt.iothub.aliyuncs.com"	
#define Domain "139.196.135.135"
#define Port "1883"														                    //端口
#define ConnectID "bc26_1"	//设备名称
//#define ConnectID "i3uocf6XqNy.bc26_1"
#define PubTopic "/sys/i3uocf6XqNy/bc26_1/thing/event/property/post"			      //发送路径
#define SubTopic "/i3uocf6XqNy/bc26_1/user/get" 							//订阅路径
#define Param_Post1 "CurrentTemperature"			                            //标签名称
#define Param_Post2 "CurrentHumidity"	                                //


#define BC28_RX_ON    0
#define BC28_RX_OFF   1
#define BC28_RX_OK    2


//变量声明
extern rt_uint8_t BC28_RX_BUF[200];
extern struct rt_timer bc28_10ms;
extern rt_uint8_t BC28_RX_FLAG;
extern rt_uint16_t BC28_RX_DATA_LEN;


//函数声明
void bc28_callback(void *parameter);
int BC28_Init(void);
int MQTT_Init(void);
int MQTT_PUB(char *data);

#endif
