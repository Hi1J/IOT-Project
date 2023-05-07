#include "drv_uart.h"


//串口1
rt_device_t u1_dev;

uint8_t USART1_RX_BUF[128] = {0};
uint8_t USART1_RX_OK = 0;

/*! 
    \brief      USART0 configure 
    \param[in]  none
    \param[out] none
    \retval     none
*/
void dbgusart_config(void)
{
    /* enable USART clock */
    rcu_periph_clock_enable(RCU_USART0);
    
    #if defined USART0_REMAP
        /* enable GPIO clock */
        rcu_periph_clock_enable(RCU_GPIOB);
        rcu_periph_clock_enable(RCU_AF);
        /* USART0 remapping */
        gpio_pin_remap_config(GPIO_USART0_REMAP, ENABLE);
        /* connect port to USARTx_Tx */
        gpio_init(GPIOB, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_6);
        /* connect port to USARTx_Rx */
        gpio_init(GPIOB, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_7);
    #else
        /* enable GPIO clock */
        rcu_periph_clock_enable(RCU_GPIOA);
        
        /* connect port to USARTx_Tx */
        gpio_init(GPIOA, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, GPIO_PIN_9);
        /* connect port to USARTx_Rx */
        gpio_init(GPIOA, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, GPIO_PIN_10);
    #endif
    
    /* USART configure */
    usart_deinit(USART0);
    usart_baudrate_set(USART0, 115200U);
    usart_word_length_set(USART0, USART_WL_8BIT);
    usart_stop_bit_set(USART0, USART_STB_1BIT);
    usart_parity_config(USART0, USART_PM_NONE);
    usart_hardware_flow_rts_config(USART0, USART_RTS_DISABLE);
    usart_hardware_flow_cts_config(USART0, USART_CTS_DISABLE);
    usart_receive_config(USART0, USART_RECEIVE_ENABLE);
    usart_transmit_config(USART0, USART_TRANSMIT_ENABLE);
    usart_enable(USART0);
}

///* retarget the C library printf function to the USART */
//int fputc(int ch, FILE *f)
//{
//    usart_data_transmit(USART0, (uint8_t)ch);
//    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
//    return ch;
//}

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
//串口1printf重定向
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART1, (uint8_t)ch);
    while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
    return ch;
}

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
	static uint8_t count = 0;
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);//清标志位
		res = usart_data_receive(USART1);//读取数据
		USART1_RX_BUF[count++] = res;//存入数组
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
