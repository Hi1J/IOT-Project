#ifndef __DRV_A72_H_
#define __DRV_A72_H_

#include "gd32f10x.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>
#include "app.h"


/*
功能：A72设备选择
说明：置1 开启 置0 关闭 
      通过使用的A72类型改以下的宏定义
	  每次只开启1个 不然会出错
*/
#define ZIGBEE_A72_NODE 0          //终端节点
#define ZIGBEE_A72_COORDINATOR 1   //协调器

/*
功能：地址
说明：
需要添加节点数量时在以下添加
例如：添加节点5
#define A72_NODE5_ADDERSS_IEEE                       0x1111111111111111      //节点5IEEE地址    
#define A72_NODE5_ADDERSS_SHORT                      0x1111                  //节点5短地址     
*/

#define A72_COORDINATOR_ADDERSS_IEEE                 0xa4c1381cae35506b      /*!<协调器IEEE地址      */
#define A72_COORDINATOR_ADDERSS_SHORT                0xffff                 /*!<协调器短地址          */

#define A72_NODE1_ADDERSS_IEEE                       0x1111111111111111     /*!<节点1IEEE地址      */
#define A72_NODE1_ADDERSS_SHORT                      0x1111                /*!<节点1短地址          */

#define A72_NODE2_ADDERSS_IEEE                       0x1111111111111111     /*!<节点2IEEE地址      */
#define A72_NODE2_ADDERSS_SHORT                      0x1111                /*!<节点2短地址          */

#define A72_NODE3_ADDERSS_IEEE                       0x1111111111111111     /*!<节点3IEEE地址      */
#define A72_NODE3_ADDERSS_SHORT                      0x1111                /*!<节点3短地址          */


/*
功能：发送目标
说明：
需要添加节点数量时在以下添加
例如：添加节点4
#define _NODE4             4      //目标:节点4
*/
#define _COORDINATOR       0      /*!<目标:协调器         */
#define _NODE1             1      /*!<目标:节点1          */
#define _NODE2             2      /*!<目标:节点2          */
#define _NODE3             3      /*!<目标:节点3          */

#define s_adderss(x)         A72_NODE##x##_ADDERSS_SHORT
#define ieee_adderss(x)      A72_NODE##x##_ADDERSS_IEEE


/*
说明：
需要添加节点数量时在以下添加
例如：": (x) == _NODE5 ? s_adderss(5)\"     //添加节点5
放置": A72_COORDINATOR_ADDERSS_SHORT)"的上一行
*/
#define S_adderss(x)     ((x) == _NODE1 ? s_adderss(1)\
                        : (x) == _NODE2 ? s_adderss(2)\
						: (x) == _NODE3 ? s_adderss(3)\
						: A72_COORDINATOR_ADDERSS_SHORT)


/*
说明：
需要添加节点数量时在以下添加
例如：": (x) == _NODE5 ? ieee_adderss(5)\"   //添加节点5
放置": A72_COORDINATOR_ADDERSS_IEEE)"的上一行
*/
#define IEEE_adderss(x)  ((x) == _NODE1 ? ieee_adderss(1)\
                        : (x) == _NODE2 ? ieee_adderss(2)\
						: (x) == _NODE3 ? ieee_adderss(3)\
						: A72_COORDINATOR_ADDERSS_IEEE)

/*
功能：发送模式设置
参数：x:目的地 y:地址长
*/
#define mode_adderss(x,y) ((y) > 2 ? IEEE_adderss(x) : S_adderss(x)) 




/*函数名重定义*/
#define A72_Send_Data USART1_Send_Data
#define A72_RX_BUF USART1_RX_BUF
#define A72_Delay_ms TIM6_Delay_ms

#if ZIGBEE_A72_NODE
/*
A72终端节点 唤醒PIN:PA1
*/
#define A72_WAKE_UP_CLOCK   RCU_GPIOA
#define A72_WAKE_UP_PORT    GPIOA
#define A72_WAKE_UP_PIN     GPIO_PIN_1

#define A72_WAKE_UP_SET()     gpio_bit_reset(A72_WAKE_UP_PORT,A72_WAKE_UP_PIN);
#define A72_WAKE_UP_RESET()   gpio_bit_set(A72_WAKE_UP_PORT,A72_WAKE_UP_PIN);

/*查询入网状态*/
#define Connect_status_Bytes 7 //指令字节个数
#define C_Connect_status 0x000D //命令
#define L_Connect_status 0x0000 //长度
#define V_Connect_status 0x00 //校验

#endif


/*起始结束位*/
#define A72_Sbit 0x55
#define A72_Ebit 0xAA

/*恢复出厂设置*/
#define RFS_Bytes 7  //指令字节个数
#define C_RFS 0x0005//命令
#define L_RFS 0x0000//长度
#define V_RFS 0x00//校验

/*复位*/
#define RESET_Bytes 7  //指令字节个数
#define C_RESET 0x000B//命令
#define L_RESET 0x0000//长度
#define V_RESET 0x00//校验

/*读取地址*/
#define Read_Adder_Bytes 7    //指令字节个数
#define C_Read_Adder 0x01C0   //命令
#define L_Read_Adder 0x0000   //长度
#define V_Read_Adder 0x00    //校验
#define L_Adder_LEN 8        //IEEE地址长度
#define S_Adder_LEN 2       //短地址长度

/*设备类型*/
#define Device_Type_Bytes 7    //指令字节个数
#define C_Device_Type 0x000E  //命令
#define L_Device_Type 0x0000 //长度
#define V_Device_Type 0x00   //校验

/*发送数据*/
#define C_Send_Data 0x0044        //命令
#define L_Send_Data_SHORT 0x0007  //短地址模式长度初始值(1个字节)
#define L_Send_Data_IEEE 0x000D   //IEEE模式长度初始值(1个字节)
#define V_Send_Data 0x00          //校验
#define M_Send_Data_SHORT 0x02    //地址模式:目标短地址和目标端口
#define M_Send_Data_IEEE 0x03     //地址模式:目标IEEE地址和目标端口
#define SPORT_Send_Data 0x01      //源端口
#define TPORT_Send_Data 0x01      //目标端口
#define User_Send_Data_LEN(x) x   //用户数据长度

/*发送模式*/
#define A72_SEND_MODE_IEEE   SEND_MODE_IEEE  //IEEE发送
#define A72_SEND_MODE_SHORT  SEND_MODE_SHORT //短地址发送

/*接收数据*/
#define C_Receive_Data 0xFF01 //命令
#define L_Receive_Data 0x0003 //长度初始值(1个字节)

#if ZIGBEE_A72_COORDINATOR

/*创建网络*/
#define CreateNet_Bytes 7  //指令字节个数
#define C_CreateNet 0x0001//命令
#define L_CreateNet 0x0000//长度
#define V_CreateNet 0x00//校验

/*允许入网*/
#define PermitJoin_Bytes 11  //指令字节个数
#define C_PermitJoin 0x0034//命令
#define L_PermitJoin 0x0004//长度
#define V_PermitJoin 0x00//校验
#define PermitJoin_Destin_adder  0xFFFC//目标地址
#define PermitJoin_Time(x) x  //时间
#define R_PermitJoin 0x01//保留


#endif

/*A72回应信息结构体定义*/
typedef struct
{
	uint8_t L_Adder[L_Adder_LEN];//IEEE
	uint8_t S_Adder[S_Adder_LEN];//短地址
	char Type_Device[20];//设备类型
	uint8_t Channel;//信道
	uint8_t N_nodes;//节点数量(协调器)
	uint8_t S_nodes;//节点状态(协调器)
	uint8_t S_Network;//入网状态(终端节点)
	uint8_t Sleep_Time;//休眠时间(终端节点)
	
}A72_Return_Value;

/*A72指令格式*/
typedef struct
{
	uint16_t command;//命令
	uint16_t length;//长度
	uint8_t verification;//检验
	uint8_t Order_Bytes;//发送该指令长度（不属于指令内容）
}A72_Instruction_Format;


/*A72发送模式*/
typedef struct
{
	uint16_t L_Send_Data;
	uint8_t M_Send_Data;
	uint8_t Address_Length;
}A72_SEND_MODE;

/*变量声明*/
extern A72_Return_Value _value;
extern A72_Instruction_Format _RFS;
extern A72_Instruction_Format READ_ADDERSS;
extern A72_Instruction_Format Device_Type;
extern A72_SEND_MODE SEND_MODE_IEEE;
extern A72_SEND_MODE SEND_MODE_SHORT;
#if ZIGBEE_A72_COORDINATOR
extern A72_Instruction_Format CreateNet;
#endif
#if ZIGBEE_A72_NODE
extern A72_Instruction_Format Connect_status;
#endif

/*函数声明*/
int A72_Init(void);
int A72_SEND_ORDER(A72_Instruction_Format *order,int(*ORDER)(void));
int A72_Nothing(void);
int A72_Read_Adderss(void);
int A72_Read_Device_Type(void);
void A72_RESET(void);
void A72_Print_Information(void);
int A72_SEND_DATA(int target,A72_SEND_MODE *mode,uint8_t *data,uint8_t len);
void A72_HANDLE_DATA(void);
#if ZIGBEE_A72_COORDINATOR
int A72_Read_CreateNet(void);
int A72_PermitJoin(uint8_t time);
#endif
#if ZIGBEE_A72_NODE
int A72_Read_Connect_status(void);
#endif

#endif
