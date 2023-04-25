#include "drv_a72.h"

#define DBG_TAG "a72"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

uint8_t A72_send_order_buf[20];//发送指令BUF
uint8_t A72_send_data_buf[100];//发送数据BUF

/*
发送数据说明：
无线串口支持短地址、IEEE 地址、组播、广播等传输方式。
短地址和 IEEE 地址传输单次最大传输 79 字节。
组播传输单次最大传输 80 字节。
广播传输单次最大传输 81 字节。
*/

A72_Return_Value _value;


//恢复出场设置
A72_Instruction_Format _RFS = 
{.command = C_RFS,.length = L_RFS,.verification = V_RFS,.Order_Bytes = RFS_Bytes};


//读取地址指令初始化
A72_Instruction_Format READ_ADDERSS = 
{.command = C_Read_Adder,.length = L_Read_Adder,.verification = V_Read_Adder,.Order_Bytes = Read_Adder_Bytes};

//读取设备类型指令初始化
A72_Instruction_Format Device_Type = 
{.command = C_Device_Type,.length = L_Device_Type,.verification = V_Device_Type,.Order_Bytes = Device_Type_Bytes};

//创建网络
#if ZIGBEE_A72_COORDINATOR
A72_Instruction_Format CreateNet = 
{.command = C_CreateNet,.length = L_CreateNet,.verification = V_CreateNet,.Order_Bytes = CreateNet_Bytes};
#endif

//入网状态指令初始化
#if ZIGBEE_A72_NODE
A72_Instruction_Format Connect_status = 
{.command = C_Connect_status,.length = L_Connect_status,.verification = V_Connect_status,.Order_Bytes = Connect_status_Bytes};
#endif

//IEEE地址发送模式初始化
A72_SEND_MODE SEND_MODE_IEEE = 
{.L_Send_Data = L_Send_Data_IEEE,.M_Send_Data = M_Send_Data_IEEE,.Address_Length = 8};

//短地址发送模式初始化
A72_SEND_MODE SEND_MODE_SHORT = 
{.L_Send_Data = L_Send_Data_SHORT,.M_Send_Data = M_Send_Data_SHORT,.Address_Length = 2};

/*
函数功能：用于A72无回应的指令
参数：void
返回值：int
备注：
*/
int A72_Nothing(void)
{
	return SUCCESS;
}


/*
函数功能：A72复位
参数：void
返回值：void
备注：每次上电时需复位
*/
void A72_RESET(void)
{
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_SET();//唤醒
#endif
	memset(A72_send_order_buf,0,20);
	
	A72_send_order_buf[0] = A72_Sbit;
	A72_send_order_buf[1] = C_RESET >> 8;
	A72_send_order_buf[2] = C_RESET & 255;
	A72_send_order_buf[3] = L_RESET >> 8;
	A72_send_order_buf[4] = L_RESET & 255;
	A72_send_order_buf[5] = V_RESET;
	A72_send_order_buf[6] = A72_Ebit;
	
	A72_Send_Data(A72_send_order_buf,RESET_Bytes);
	
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_RESET();//休眠
#endif	
}

/*
函数功能：查看读取地址回应
参数：void
返回值：SUCCESS:成功 ERROR:失败
备注：
	注:该函数为A72_SEND_ORDER函数的参数 不可独自调用
*/
int A72_Read_Adderss(void)
{
	
	uint8_t Read_Data_OK = 0;//接收完成标志
	uint8_t RX_Correct = 0;//接收正确标志
	uint8_t i = 0;//临时变量
	uint8_t count = 0;//临时变量

//	memset(A72_RX_BUF,0,128);
	
	if(rt_sem_take(A72_Respond,50) == 0)//等待接收完成 等待时间50ms
	{
		for(i=0;(Read_Data_OK==0 && i < 20);i++)
		{
			
			if(A72_RX_BUF[i] == A72_Sbit && RX_Correct == 0)
				if(A72_RX_BUF[i+1] == 0x81 && A72_RX_BUF[i+2] == 0xC0)//回应命令：0x81C0
					RX_Correct = 1;//接收正确
				
			if(RX_Correct == 1)//如果接收到正确的数据
			{
				i += 6;//跳过命令位、长度位、校验位
				
				for(count=0;count<L_Adder_LEN;count++)//IEEE地址读取
					_value.L_Adder[count] = A72_RX_BUF[i++];
				
				for(count=0;count<S_Adder_LEN;count++)//短地址读取
					_value.S_Adder[count] = A72_RX_BUF[i++];
				
				Read_Data_OK = 1;//接收完成
				
				return SUCCESS;
			}
		}
	}
	else
		return ERROR;
				
	return ERROR;
}

/*
函数功能：查看读取设备类型回应
参数：void
返回值：SUCCESS:成功 ERROR:失败
备注：
	注:该函数为A72_SEND_ORDER函数的参数 不可独自调用
*/
int A72_Read_Device_Type(void)
{
	uint8_t Read_Data_OK = 0;//接收完成标志
	uint8_t RX_Correct = 0;//接收正确标志
	uint8_t i = 0;//临时变量

//	memset(A72_RX_BUF,0,128);
	
	if(rt_sem_take(A72_Respond,50) == 0)//等待接收完成 等待时间50ms
	{
		for(i=0;(Read_Data_OK==0 && i < 20);i++)
		{
			
			if(A72_RX_BUF[i] == A72_Sbit && RX_Correct == 0)
				if(A72_RX_BUF[i+1] == 0x80 && A72_RX_BUF[i+2] == 0x0E)//回应命令：0x800E
					RX_Correct = 1;//接收正确
				
			if(RX_Correct == 1)//如果接收到正确的数据
			{
				i += 7;//跳过命令位、长度位、校验位、保留位
				
				switch(A72_RX_BUF[i])
				{
					case 0x00://协调器
						sprintf(_value.Type_Device,"Coordinator");
						break;
					case 0x01://路由器
						sprintf(_value.Type_Device,"Router");
						break;
					case 0x02://终端结点
						sprintf(_value.Type_Device,"Node");
						break;
					default:
						break;
				
				}
				
				Read_Data_OK = 1;//接收完成
				
				return SUCCESS;
			}
		}
	}
	else
		return ERROR;
		
	return ERROR;
}


#if ZIGBEE_A72_NODE
/*
函数功能：查看节点入网状态
参数：void
返回值：SUCCESS:成功 ERROR:失败
备注：适用：节点
		注:该函数为A72_SEND_ORDER函数的参数 不可独自调用
*/
int A72_Read_Connect_status(void)
{
	uint8_t Read_Data_OK = 0;//接收完成标志
	uint8_t RX_Correct = 0;//接收正确标志
	uint8_t i = 0;//临时变量

//	memset(A72_RX_BUF,0,128);
	
	if(rt_sem_take(A72_Respond,50) == 0)//等待接收完成 等待时间50ms
	{
		for(i=0;(Read_Data_OK==0 && i < 20);i++)
		{
			
			if(A72_RX_BUF[i] == A72_Sbit && RX_Correct == 0)
				if(A72_RX_BUF[i+1] == 0x80 && A72_RX_BUF[i+2] == 0x0D)//回应命令：0x800D
					RX_Correct = 1;//接收正确
				
			if(RX_Correct == 1)//如果接收到正确的数据
			{
				i += 6;//跳过命令位、长度位、校验位
				
				_value.S_Network = A72_RX_BUF[i];
				
				Read_Data_OK = 1;//接收完成
				
				return SUCCESS;
			}
		}
	}
	else
		return ERROR;
	
	return ERROR;
}
#endif

#if ZIGBEE_A72_COORDINATOR

int A72_PermitJoin(uint8_t time)
{
	
	memset(A72_send_order_buf,0,20);
	
	A72_send_order_buf[0] = A72_Sbit;
	A72_send_order_buf[1] = C_PermitJoin >> 8;
	A72_send_order_buf[2] = C_PermitJoin & 255;
	A72_send_order_buf[3] = L_PermitJoin >> 8;
	A72_send_order_buf[4] = L_PermitJoin & 255;
	A72_send_order_buf[5] = V_PermitJoin;
	A72_send_order_buf[7] = PermitJoin_Destin_adder >> 8;
	A72_send_order_buf[8] = PermitJoin_Destin_adder & 255;
	A72_send_order_buf[9] = PermitJoin_Time(time);
	A72_send_order_buf[10] = R_PermitJoin;
	A72_send_order_buf[11] = A72_Ebit;
	
	A72_Send_Data(A72_send_order_buf,PermitJoin_Bytes);

}

#endif

/*
函数功能：发送指令
参数：A72_Instruction_Format *order,int(*ORDER)(void)
返回值：int (返回0：成功 返回1：失败)
备注：适用：协调器、终端节点、路由器
		该函数用于发送A72有回应的指令	
*/
int A72_SEND_ORDER( A72_Instruction_Format *order,int(*ORDER)(void) )
{	
	
	uint8_t MAX_Repeat = 5;//最大重复次数
	
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_SET();//唤醒
#endif
	
	memset(A72_send_order_buf,0,20);
	
	A72_send_order_buf[0] = A72_Sbit;
	A72_send_order_buf[1] = order->command >> 8;
	A72_send_order_buf[2] = order->command & 255;
	A72_send_order_buf[3] = order->length >> 8;
	A72_send_order_buf[4] = order->length & 255;
	A72_send_order_buf[5] = order->verification;
	A72_send_order_buf[6] = A72_Ebit;
	
	
	while(--MAX_Repeat)
	{
		
		memset(A72_RX_BUF,0,128);
		
		A72_Send_Data(A72_send_order_buf,order->Order_Bytes);
		
		if(ORDER() == SUCCESS)break;
				
	}
	
//	rt_kprintf("MAX_Repeat:%d\n",MAX_Repeat);
	
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_RESET();//休眠
#endif
	
	if(MAX_Repeat > 0)
		return SUCCESS;	//返回成功
	else
		return ERROR;//返回失败
}

/*
函数功能：发送数据函数
参数：int target:发送目标 A72_SEND_MODE *mode:发送模式  uint8_t *data:发送数据   uint8_t len:数据长度
返回值：int   SUCCESS:成功   ERROR:失败
备注：该函数同样可以发送1个字节的数据 当发送1个字节的数据时 需创建一个BUF[1] 来储存 
	  target的取值有：_COORDINATOR：0
	                 _NODE1 ：1  _NODE2 ：2  _NODE3 ：3
	  *mode的取值有：&A72_SEND_MODE_IEEE ：IEEE地址发送   &A72_SEND_MODE_SHORT ：短地址发送   
	  注：如果输入了其他target值 则默认发送给协调器
*/
int A72_SEND_DATA(int target,A72_SEND_MODE *mode,uint8_t *data,uint8_t len)
{
	
	uint8_t Read_Data_OK = 0;//接收完成标志
	uint8_t RX_Correct = 0;//接收正确标志
	uint8_t i = 0;//临时变量
	uint8_t MAX_Repeat = 5;//最大重复次数
	uint8_t count_num;//临时变量(记录指令长度)
	uint8_t count_bit;//临时变量
	
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_SET();//唤醒
#endif
	
	memset(A72_send_data_buf,0,100);
	
	mode->L_Send_Data += len - 1;//L_Send_Data计算
	
	/*****************发送数据指令初始化*************************/
	
	A72_send_data_buf[0] = A72_Sbit;
	A72_send_data_buf[1] = C_Send_Data >> 8;
	A72_send_data_buf[2] = C_Send_Data & 255;
	A72_send_data_buf[3] = mode->L_Send_Data >> 8;
	A72_send_data_buf[4] = mode->L_Send_Data & 255;
	A72_send_data_buf[5] = V_Send_Data;
	A72_send_data_buf[6] = mode->M_Send_Data;
	for(count_num=0,count_bit=mode->Address_Length-1;count_num<mode->Address_Length;count_num++,count_bit--)
	{
		A72_send_data_buf[count_num+7] = (mode_adderss(target,mode->Address_Length) >> (count_bit * 8)) & 255;
	}
	A72_send_data_buf[7+count_num] = SPORT_Send_Data;
	A72_send_data_buf[8+count_num] = TPORT_Send_Data;
	A72_send_data_buf[9+count_num] = len;
	for(count_num=0;count_num<len;count_num++)
	{
		A72_send_data_buf[count_num+10+mode->Address_Length] = *data;
		data++;
	}
	A72_send_data_buf[count_num+10+mode->Address_Length] = A72_Ebit;
	
	/*****************发送数据指令初始化*************************/
	
	count_num = 11+len+mode->Address_Length;//指令个数计算
	
	mode->L_Send_Data -= len - 1;//L_Send_Data复位
	
//Debug:
//	rt_kprintf("send order:\n");
//	for(uint8_t count=0;count<count_num;count++)
//	{
//		rt_kprintf("%x ",A72_send_data_buf[count]);
//	}
//	rt_kprintf("\n");
	
	memset(A72_RX_BUF,0,128);
	
	while(--MAX_Repeat)
	{
		
		A72_Send_Data(A72_send_data_buf,count_num);
		
		if(rt_sem_take(A72_Respond,50) == 0)//等待接收完成 等待时间50ms
		{
			for(i=0;(Read_Data_OK==0 && i < 20);i++)
			{
				if(A72_RX_BUF[i] == A72_Sbit && RX_Correct == 0)
					if(A72_RX_BUF[i+1] == 0x80 && A72_RX_BUF[i+2] == 0x00)//回应命令：0x800D
						RX_Correct = 1;//接收正确
					
				if(RX_Correct == 1)//如果接收到回应的数据
				{
					
					i += 5;//跳位
					
					if(A72_RX_BUF[i] == 0xC0 && A72_RX_BUF[i+1] == 0x00 && A72_RX_BUF[i+2] == 0x44)
					{
						Read_Data_OK = 1;//接收正确
						break;
					}
					else//否则接收失败
					{
#if ZIGBEE_A72_NODE
						A72_WAKE_UP_RESET();//休眠
#endif					
						return ERROR;
					}		
				}
			}
		}
		
		
		if(RX_Correct == 1 && Read_Data_OK == 1)break;//跳出循环
	
	}


#if ZIGBEE_A72_NODE
	A72_WAKE_UP_RESET();//休眠
#endif	
	
	if(MAX_Repeat > 0)
		return SUCCESS;	
	else
		return ERROR;
		
}

/*
函数功能：Zigbee-A72数据处理
参数：void
返回值：void
备注：该函数主要用于处理其他Zigbee发过来的数据
	  处理数据方式为用户自定义 
*/
void A72_HANDLE_DATA(void)
{
	
	uint8_t len;//数据长度存储变量
	uint8_t count;//临时变量
	uint8_t rx_data_buf[81];
	
	len = A72_RX_BUF[4] - L_Receive_Data + 1;
	
	rt_kprintf("receive data:\n");//Debug
	for(count=0;count<len;count++)
	{
		rx_data_buf[count] = A72_RX_BUF[count+6];
		rt_kprintf("%x ",A72_RX_BUF[count+6]);//Debug
	}
	rt_kprintf("\n");//Debug
	
	
	/******************用户定义区******************/
	
	if(rx_data_buf[0] == 0x00)
		rt_kprintf("yes\n");
	
	/******************用户定义区******************/
	memset(A72_RX_BUF,0,128);
	
}





/*
函数功能：Zigbee-A72初始化
参数：void
返回值：int (返回0：成功 返回1：失败)
备注：适用：协调器、终端节点、路由器
*/
int A72_Init(void)
{
#if ZIGBEE_A72_NODE
	//配置GPIO(唤醒引脚)
	rcu_periph_clock_enable(A72_WAKE_UP_CLOCK);//开GPIO时钟
	gpio_init(A72_WAKE_UP_PORT,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,A72_WAKE_UP_PIN);//GPIO工作模式配置
#endif
	
	A72_RESET();//复位
	
	rt_thread_mdelay(100);//延时
	
	A72_SEND_ORDER(&_RFS,A72_Nothing);//恢复出场设置
	
	rt_thread_mdelay(100);//延时
	
	if(A72_SEND_ORDER(&READ_ADDERSS,A72_Read_Adderss) == SUCCESS)//读地址
		rt_kprintf("A72_Read_Address successed..\n");
	else
	{
		rt_kprintf("A72_Read_Address failed..\n");
		return ERROR;//失败返回
	}
	
	rt_thread_mdelay(100);//延时
	
	if(A72_SEND_ORDER(&Device_Type,A72_Read_Device_Type) == SUCCESS)//读设备类型
		rt_kprintf("A72_Read_Device_Type successed..\n");
	else
	{
		rt_kprintf("A72_Read_Device_Type failed..\n");
		return ERROR;//失败返回
	}
	
	
	
#if ZIGBEE_A72_COORDINATOR
	
	rt_thread_mdelay(100);//延时
	
	A72_SEND_ORDER(&CreateNet,A72_Nothing);
	
	rt_thread_mdelay(100);//延时
	
	A72_PermitJoin(180);
	
#endif
		
	A72_Print_Information();//打印信息
	
	return SUCCESS;
}


/*
函数功能：打印A72回应信息
参数：void
返回值：void
备注：需要打印时调用
	  打印前应先读取A72相应数据
*/
void A72_Print_Information(void)
{
	uint8_t count;
	
	rt_kprintf("IEEE: ");
	for(count=0;count<L_Adder_LEN;count++)
		rt_kprintf("%x ",_value.L_Adder[count]);
	rt_kprintf("\nshort adderss: ");
	for(count=0;count<S_Adder_LEN;count++)
		rt_kprintf("%x ",_value.S_Adder[count]);
	rt_kprintf("\nDevice Type:%s\n",_value.Type_Device);
}
