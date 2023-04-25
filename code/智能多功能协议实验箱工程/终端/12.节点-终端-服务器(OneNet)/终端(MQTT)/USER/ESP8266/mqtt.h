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

#define  R_NUM               5     //接收缓冲区个数
#define  RBUFF_UNIT          300   //接收缓冲区长度

#define  C_NUM               5     //命令缓冲区个数
#define  CBUFF_UNIT          300   //命令缓冲区长度


extern unsigned char  MQTT_RxDataBuf[R_NUM][RBUFF_UNIT];       //外部变量声明，数据的接收缓冲区,所有服务器发来的数据，存放在该缓冲区,缓冲区第一个字节存放数据长度
extern unsigned char *MQTT_RxDataInPtr;                        //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_RxDataOutPtr;                       //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_RxDataEndPtr;                       //外部变量声明，指向缓冲区结束的位置
extern unsigned char  MQTT_CMDBuf[C_NUM][CBUFF_UNIT];          //外部变量声明，命令数据的接收缓冲区
extern unsigned char *MQTT_CMDInPtr;                           //外部变量声明，指向缓冲区存放数据的位置
extern unsigned char *MQTT_CMDOutPtr;                          //外部变量声明，指向缓冲区读取数据的位置
extern unsigned char *MQTT_CMDEndPtr;                          //外部变量声明，指向缓冲区结束的位置


#define CLIENTID "1065278300"//设备ID
#define USERNAME "589281"   //产品ID
#define IP    "183.230.40.39"  //IP地址
#define PORT 6002         //端口

#define PASSWARD "123456"  //鉴权信息

#define S_TOPIC_NAME  "topic_one" //订阅路径

#define  Data_TOPIC_NAME      "$dp"

#define WIFI_Send_Buf UART5_Send_Buf

void AliIoT_Parameter_Init(void);
void MQTT_ConectPack(void);
void MQTT_Subscribe(char *topic_name, int QoS);
void MQTT_PublishQs0(char *topic, char *data, int data_len);
void MQTT_DealPushdata_Qs0(unsigned char *redata);
void CMDBuf_Deal(unsigned char *data, int size);

void send_data(uint8_t tem,uint8_t hum,uint16_t light);

extern uint8_t data_buf[256];


#endif