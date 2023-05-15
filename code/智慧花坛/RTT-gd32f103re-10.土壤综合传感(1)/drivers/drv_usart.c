#include "drv_usart.h"


rt_device_t u3_dev;

rt_uint8_t UART3_RX_BUF[64] = {0};
rt_uint16_t rs485_data_len = 0;

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

void uart3_init(uint32_t brr)
{
	//时钟配置
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_UART3);
	
	//GPIO模式配置
	gpio_init(GPIOC,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//TX-PC10
	gpio_init(GPIOC,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);//RX-PC11
	
	//串口工作模式配置
	usart_deinit(UART3);//复位外设USART
	usart_baudrate_set(UART3,brr);//配置波特率
	usart_word_length_set(UART3,USART_WL_8BIT);//8位数据位
	usart_stop_bit_set(UART3,USART_STB_1BIT);//1个停止位
	usart_parity_config(UART3,USART_PM_NONE);//无奇偶校验
	usart_hardware_flow_cts_config(UART3,USART_CTS_DISABLE);//无硬件控制流
	usart_hardware_flow_rts_config(UART3,USART_RTS_DISABLE);//无硬件控制流
	usart_receive_config(UART3,USART_RECEIVE_ENABLE);//接收使能
	usart_transmit_config(UART3,USART_TRANSMIT_ENABLE);//发送使能
	
	//中断配置
	nvic_irq_enable(UART3_IRQn,0,0);//中断使能
	usart_interrupt_enable(UART3,USART_INT_RBNE);//接收中断使能
	
	//使能串口
	usart_enable(UART3);
	
}
//串口3printf重定向
int fputc(int ch, FILE *f)
{
    usart_data_transmit(UART3, (uint8_t)ch);
    while(RESET == usart_flag_get(UART3, USART_FLAG_TBE));
    return ch;
}

void UART3_Send_Data(uint8_t *data,uint16_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		
		usart_data_transmit(UART3, data[i]);
		while(RESET == usart_flag_get(UART3, USART_FLAG_TBE));
	}
}

//串口2中断服务函数
void UART3_IRQHandler(void)
{
	uint8_t res;
	if(usart_interrupt_flag_get(UART3,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(UART3,USART_INT_FLAG_RBNE);//清标志位
		modbus_rx_falg = modbus_rx_on;
		modbus_time = 0;
		res = usart_data_receive(UART3);//读取数据
		modbus_rx_buf[modbus_rxdata_len++] = res;
		
		
	}
}

int rt_uart3_init(void)
{
	rt_device_t U3_dev;
	U3_dev = rt_device_create(RT_Device_Class_Char,64);
	if(U3_dev == RT_NULL)
	{
		LOG_E("u3_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	uart3_init(modbus_uart_baud);
	rt_device_register(U3_dev,"uart3",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_uart3_init);




