#include "drv_usart.h"


rt_device_t u3_dev;

rt_uint8_t UART3_RX_BUF[64] = {0};
rt_uint16_t rs485_data_len = 0;

void usart0_init(uint32_t brr)
{
	//ʱ������
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART0);
	
	//GPIOģʽ����
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_9);//TX-PA9
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//RX-PA10
	
	//���ڹ���ģʽ����
	usart_deinit(USART0);//��λ����USART
	usart_baudrate_set(USART0,brr);//���ò�����
	usart_word_length_set(USART0,USART_WL_8BIT);//8λ����λ
	usart_stop_bit_set(USART0,USART_STB_1BIT);//1��ֹͣλ
	usart_parity_config(USART0,USART_PM_NONE);//����żУ��
	usart_hardware_flow_cts_config(USART0,USART_CTS_DISABLE);//��Ӳ��������
	usart_hardware_flow_rts_config(USART0,USART_RTS_DISABLE);//��Ӳ��������
	usart_receive_config(USART0,USART_RECEIVE_ENABLE);//����ʹ��
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//����ʹ��
	
	
	
	//ʹ�ܴ���
	usart_enable(USART0);
	
}

void uart3_init(uint32_t brr)
{
	//ʱ������
	rcu_periph_clock_enable(RCU_GPIOC);
	rcu_periph_clock_enable(RCU_UART3);
	
	//GPIOģʽ����
	gpio_init(GPIOC,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//TX-PC10
	gpio_init(GPIOC,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);//RX-PC11
	
	//���ڹ���ģʽ����
	usart_deinit(UART3);//��λ����USART
	usart_baudrate_set(UART3,brr);//���ò�����
	usart_word_length_set(UART3,USART_WL_8BIT);//8λ����λ
	usart_stop_bit_set(UART3,USART_STB_1BIT);//1��ֹͣλ
	usart_parity_config(UART3,USART_PM_NONE);//����żУ��
	usart_hardware_flow_cts_config(UART3,USART_CTS_DISABLE);//��Ӳ��������
	usart_hardware_flow_rts_config(UART3,USART_RTS_DISABLE);//��Ӳ��������
	usart_receive_config(UART3,USART_RECEIVE_ENABLE);//����ʹ��
	usart_transmit_config(UART3,USART_TRANSMIT_ENABLE);//����ʹ��
	
	//�ж�����
	nvic_irq_enable(UART3_IRQn,0,0);//�ж�ʹ��
	usart_interrupt_enable(UART3,USART_INT_RBNE);//�����ж�ʹ��
	
	//ʹ�ܴ���
	usart_enable(UART3);
	
}
//����3printf�ض���
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

//����2�жϷ�����
void UART3_IRQHandler(void)
{
	uint8_t res;
	if(usart_interrupt_flag_get(UART3,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(UART3,USART_INT_FLAG_RBNE);//���־λ
		modbus_rx_falg = modbus_rx_on;
		modbus_time = 0;
		res = usart_data_receive(UART3);//��ȡ����
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
	rt_device_register(U3_dev,"uart3",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//ע��  ��д
	return 0;
}

INIT_DEVICE_EXPORT(rt_uart3_init);




