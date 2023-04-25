#ifndef __MQTT_H_
#define __MQTT_H_

#include "stm32f10x.h"
#include "delay.h"
#include "stdio.h"
#include "string.h"
#include "esp8266.h"
#include "utils_hmac.h"

typedef struct {
    uint32_t total[2];          /*!< number of bytes processed  */
    uint32_t state[5];          /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */
} iot_sha1_context;

#define KEY_IOPAD_SIZE 64
#define MD5_DIGEST_SIZE 16
#define SHA1_DIGEST_SIZE 20


#define CLIENTID "esp8266_1|securemode=3,signmethod=hmacsha1|"
#define USERNAME "esp8266_1&i3uoZcZ9tZw"
#define PORT 1883

#define DEVICENAME "esp8266_1"  //设备名
#define PRODUCTKEY "i3uoZcZ9tZw" //产品名
#define DEVICESECRE "819d15626d523aa43cb79ebd25ffd7d9" //设备密码
#define DEVICESECRE_LEN      strlen(DEVICESECRE)

#define S_TOPIC_NAME  "/i3uoZcZ9tZw/esp8266_1/user/get" //订阅路径
#define P_TOPIC_NAME  "/sys/i3uoZcZ9tZw/esp8266_1/thing/event/property/post"        //需要发布的主题 

#define WIFI_Send_Buf UART5_Send_Buf

void AliIoT_Parameter_Init(void);
void MQTT_ConectPack(void);
void MQTT_Subscribe(char *topic_name, int QoS);




#endif