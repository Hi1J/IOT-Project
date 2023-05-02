#include "drv_usart.h"

//串口1
rt_device_t u1_dev;
rt_device_t u2_dev;

uint8_t USART1_RX_BUF[128] = {0};
uint8_t USART1_RX_OK = 0;

void usart0_init(uint32_t brr)
{
	//时钟配置
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART0);
	
	//GPIO模式配置
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);//TX-PA9
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//RX-PA10
	
	//串口工作模式配置
	usart_deinit(USART0);//复位外设USART
	usart_baudrate_set(USART0,brr);//配置波特率
	usart_word_length_set(USART0,USART_WL_8BIT);//8位数据位
	usart_stop_bit_set(USART0,USART_STB_1BIT);//1个停止位
	usart_parity_config(USART0,USART_PM_NONE);//无奇偶校验
	usart_hardware_flow_cts_config(USART0,USART_CTS_DISABLE);//无硬件控制流
	usart_hardware_flow_rts_config(USART0,USART_RTS_DISABLE);//无硬件控制流
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);//接收使能
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//发送使能
	
	
	
	//使能串口
	usart_enable(USART0);
	
}

void usart1_init(uint32_t brr)
{
	//时钟配置
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);
	
	//GPIO模式配置
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);//TX-PA2
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3);//RX-PA3
	
	//串口工作模式配置
	usart_deinit(USART1);//复位外设USART
	usart_baudrate_set(USART1,brr);//配置波特率
	usart_word_length_set(USART1,USART_WL_8BIT);//8位数据位
	usart_stop_bit_set(USART1,USART_STB_1BIT);//1个停止位
	usart_parity_config(USART1,USART_PM_NONE);//无奇偶校验
	usart_hardware_flow_cts_config(USART1,USART_CTS_DISABLE);//无硬件控制流
	usart_hardware_flow_rts_config(USART1,USART_RTS_DISABLE);//无硬件控制流
	usart_receive_config(USART1,USART_RECEIVE_ENABLE);//接收使能
	usart_transmit_config(USART1,USART_TRANSMIT_ENABLE);//发送使能
	
	//中断配置
	nvic_irq_enable(USART1_IRQn,0,0);//中断使能
	usart_interrupt_enable(USART1,USART_INT_RBNE);//接收中断使能
	usart_interrupt_enable(USART1,USART_INT_IDLE);//IDLE中断使能
	
	//使能串口
	usart_enable(USART1);
	
}
////串口1printf重定向
//int fputc(int ch, FILE *f)
//{
//    usart_data_transmit(USART1, (uint8_t)ch);
//    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
//    return ch;
//}

void USART1_Send_Data(uint8_t *data,uint16_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		
		usart_data_transmit(USART1, data[i]);
		while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
	}
}

//串口1中断服务函数
void USART1_IRQHandler(void)
{
	uint8_t res;
//	uint8_t i = 0;
	static uint8_t count = 0;
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);//清标志位
		res = usart_data_receive(USART1);//读取数据
		USART1_RX_BUF[count++] = res;//存入数组
//		usart_data_transmit(USART0,res);//Debug
//		rt_kprintf("%x ",res);
	}
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_IDLE) != RESET)
	{
		res = usart_data_receive(USART1);
		count = 0;//计数值复位
		if(USART1_RX_BUF[0] == 0x55 && USART1_RX_BUF[1] == 0xFF && USART1_RX_BUF[2] == 0x01)
			rt_sem_release(A72_Data_handle);//接收回应完成标志 释放处理数据信号量
		else
			rt_sem_release(A72_Respond);//接收回应完成标志 释放回应信号量
		
	}

}

int rt_usart1_init(void)
{
	rt_device_t U1_dev;
	U1_dev = rt_device_create(RT_Device_Class_Char,64);
	if(U1_dev == RT_NULL)
	{
		LOG_E("u1_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	usart1_init(115200);
	rt_device_register(U1_dev,"usart1",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_usart1_init);



void usart2_init(uint32_t brr)
{
	//时钟配置
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_USART2);
	
	//GPIO模式配置
	gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//TX-PB10
	gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);//RX-PB11
	
	//串口工作模式配置
	usart_deinit(USART2);//复位外设USART
	usart_baudrate_set(USART2,brr);//配置波特率
	usart_word_length_set(USART2,USART_WL_8BIT);//8位数据位
	usart_stop_bit_set(USART2,USART_STB_1BIT);//1个停止位
	usart_parity_config(USART2,USART_PM_NONE);//无奇偶校验
	usart_hardware_flow_cts_config(USART2,USART_CTS_DISABLE);//无硬件控制流
	usart_hardware_flow_rts_config(USART2,USART_RTS_DISABLE);//无硬件控制流
	usart_receive_config(USART2,USART_RECEIVE_ENABLE);//接收使能
	usart_transmit_config(USART2,USART_TRANSMIT_ENABLE);//发送使能
	
	//中断配置
	nvic_irq_enable(USART2_IRQn,0,0);//中断使能
	usart_interrupt_enable(USART2,USART_INT_RBNE);//接收中断使能
	usart_interrupt_enable(USART2,USART_INT_IDLE);//IDLE中断使能
	
	//使能串口
	usart_enable(USART2);
	
}
//串口2printf重定向
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART2, (uint8_t)ch);
    while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
    return ch;
}

void USART2_Send_Data(uint8_t *data,uint16_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		
		usart_data_transmit(USART2, data[i]);
		while(RESET == usart_flag_get(USART2, USART_FLAG_TBE));
	}
}

//串口2中断服务函数
void USART2_IRQHandler(void)
{
	uint8_t res;
	static uint8_t count = 1;
	static uint8_t iot_head = 0;
	static uint8_t iot_data = 0;
	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART2,USART_INT_FLAG_RBNE);//清标志位
		res = usart_data_receive(USART2);//读取数据
		//usart_data_transmit(USART0,res);//Debug
		//rt_kprintf("%x",res);
		

		if(MQTT_Connect == 1)//接收服务器发来的数据 协议：Modbus
		{
			if(iot_data == 0 && res == 0x01)iot_head = res;
			if((res == 0x46 || res == 0x03 || res == 0x06) && iot_data == 0 && iot_head == 0x01)
			{
				NB72_Receive_Buf[0] = iot_head;
				iot_head = 0;
				iot_data = 1;
			}
			if(iot_data == 1)
				NB72_Receive_Buf[count++] = res;
		}
		else//接收NB73会有数据
		{
			if(strNB73_Fram_Record .InfBit .FramLength < (RX_BUF_MAX_LEN - 1))  //预留1个字节写结束符
				strNB73_Fram_Record .Data_RX_BUF [strNB73_Fram_Record .InfBit .FramLength ++] = res;
		
		}
		
	}
	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_IDLE) != RESET)
	{
		res = usart_data_receive(USART2);
		
		if(MQTT_Connect == 1 && iot_data == 1)
		{
			IOT_DATA_LEN = count;
			count = 1;
			iot_data = 0;
			rt_sem_release(NB73_Data_handle);
		}
		else
			strNB73_Fram_Record .InfBit .FramFinishFlag = 1;
			
		
		
	}

}

int rt_usart2_init(void)
{
	rt_device_t U2_dev;
	U2_dev = rt_device_create(RT_Device_Class_Char,64);
	if(U2_dev == RT_NULL)
	{
		LOG_E("u2_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	usart2_init(9600);
	rt_device_register(U2_dev,"usart2",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_usart2_init);

