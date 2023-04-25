#include "drv_a72.h"

#define DBG_TAG "a72"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

uint8_t A72sendbuf[20];
A72_Return_Value _value;

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
	memset(A72sendbuf,0,20);
	A72sendbuf[0] = A72_Sbit;
	A72sendbuf[1] = C_RESET >> 8;
	A72sendbuf[2] = C_RESET & 0x00FF;
	A72sendbuf[3] = L_RESET >> 8;
	A72sendbuf[4] = L_RESET & 0x00FF;
	A72sendbuf[5] = V_RESET;
	A72sendbuf[6] = A72_Ebit;
	A72_Send_Data(A72sendbuf,RESET_Bytes);
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_RESET();//休眠
#endif	
}


/*
函数功能：读取地址
参数：void
返回值：int (返回0：成功 返回1：失败)
备注：适用：协调器、终端节点、路由器
*/
int A72_Read_Address(void)
{	
	uint8_t Read_Adder_OK = 0;//接收完成标志
	uint8_t RX_Correct = 0;//接收正确标志
	uint8_t i = 0;//临时变量
	uint8_t count = 0;//临时变量
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_SET();//唤醒
#endif
	memset(A72sendbuf,0,20);
	A72sendbuf[0] = A72_Sbit;
	A72sendbuf[1] = C_Read_Adder >> 8;
	A72sendbuf[2] = C_Read_Adder & 0x00FF;
	A72sendbuf[3] = L_Read_Adder >> 8;
	A72sendbuf[4] = L_Read_Adder & 0x00FF;
	A72sendbuf[5] = V_Read_Adder;
	A72sendbuf[6] = A72_Ebit;
	A72_Send_Data(A72sendbuf,Read_Adder_Bytes);
	if(rt_sem_take(sem1,50) == 0)//等待接收完成 等待时间50ms
	{
		for(i=0;(Read_Adder_OK==0 && i < 20);i++)
		{
			if(A72_RX_BUF[i] == A72_Sbit && RX_Correct == 0)
				if(A72_RX_BUF[i+1] == 0x81 && A72_RX_BUF[i+2] == 0xC0)
					RX_Correct = 1;//接收正确
			if(RX_Correct == 1)//如果接收到正确的数据
			{
				i += 6;//跳过命令位、长度位、校验位
				for(count=0;count<L_Adder_LEN;count++)//IEEE地址读取
					_value.L_Adder[count] = A72_RX_BUF[i++];
				for(count=0;count<S_Adder_LEN;count++)//短地址读取
					_value.S_Adder[count] = A72_RX_BUF[i++];
				Read_Adder_OK = 1;//接收完成
			}
			if(i >= 35)
			{
#if ZIGBEE_A72_NODE
				A72_WAKE_UP_RESET();//休眠
#endif
				return ERROR;//返回失败		
			}
		}

	}
	else//如果A72不回应 则读取失败
	{
#if ZIGBEE_A72_NODE
		A72_WAKE_UP_RESET();//休眠
#endif
		return ERROR;//返回失败
	}
	
#if ZIGBEE_A72_NODE
	A72_WAKE_UP_RESET();//休眠
#endif		
	return SUCCESS;	//返回成功
}



void A72_Init(void)
{
	uint8_t count;
#if ZIGBEE_A72_NODE
	//配置GPIO
	rcu_periph_clock_enable(RCU_GPIOA);//开GPIOA时钟
	gpio_init(GPIOA,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_1);//GPIO工作模式配置
	gpio_bit_reset(GPIOA,GPIO_PIN_1);
#endif
	while(1)
	{
		A72_RESET();//复位
		rt_thread_mdelay(100);
		if(A72_Read_Address() == SUCCESS)
		{
			LOG_D("A72_Read_Address successed..\n");
			rt_kprintf("IEEE: ");
			for(count=0;count<L_Adder_LEN;count++)
				rt_kprintf("%x ",_value.L_Adder[count]);
			rt_kprintf("\nshort adderss: ");
			for(count=0;count<S_Adder_LEN;count++)
				rt_kprintf("%x ",_value.S_Adder[count]);
			rt_kprintf("\n");
			break;
		}
		else
			LOG_E("A72_Read_Address failed..\n");
	
	}
	
}

