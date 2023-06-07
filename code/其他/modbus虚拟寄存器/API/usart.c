#include "usart.h"

uint8_t UART3_RX_BUF[128] = {0};
uint16_t UART3_RX_LEN = 0;
uint8_t UART3_RX_FLAG = UART3_RX_NO;

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
	//usart_receive_config(USART0,USART_RECEIVE_ENABLE);//����ʹ��
	usart_transmit_config(USART0,USART_TRANSMIT_ENABLE);//����ʹ��
	
	//�ж�����
	nvic_irq_enable(USART0_IRQn,0,0);//�ж�ʹ��
	usart_interrupt_enable(USART0,USART_INT_RBNE);//�����ж�ʹ��
	
	//ʹ�ܴ���
	usart_enable(USART0);
	
}

//����0printf�ض���
int fputc(int ch, FILE *f)
{
    usart_data_transmit(USART0, (uint8_t)ch);
    while(RESET == usart_flag_get(USART0, USART_FLAG_TBE));
    return ch;
}


//����0�жϷ�����
void USART0_IRQHandler(void)
{
	uint8_t res;
	if(usart_interrupt_flag_get(USART0,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART0,USART_INT_FLAG_RBNE);
		res = usart_data_receive(USART0);
		printf("%c",res);
		
	}

}



void usart1_init(uint32_t brr)
{
	//ʱ������
	rcu_periph_clock_enable(RCU_GPIOA);
	rcu_periph_clock_enable(RCU_USART1);
	
	//GPIOģʽ����
	gpio_init(GPIOA,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_2);//TX-PA2
	gpio_init(GPIOA,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_3);//RX-PA3
	
	//���ڹ���ģʽ����
	usart_deinit(USART1);//��λ����USART
	usart_baudrate_set(USART1,brr);//���ò�����
	usart_word_length_set(USART1,USART_WL_8BIT);//8λ����λ
	usart_stop_bit_set(USART1,USART_STB_1BIT);//1��ֹͣλ
	usart_parity_config(USART1,USART_PM_NONE);//����żУ��
	usart_hardware_flow_cts_config(USART1,USART_CTS_DISABLE);//��Ӳ��������
	usart_hardware_flow_rts_config(USART1,USART_RTS_DISABLE);//��Ӳ��������
	usart_receive_config(USART1,USART_RECEIVE_ENABLE);//����ʹ��
	usart_transmit_config(USART1,USART_TRANSMIT_ENABLE);//����ʹ��
	
	//�ж�����
	nvic_irq_enable(USART1_IRQn,0,0);//�ж�ʹ��
	usart_interrupt_enable(USART1,USART_INT_RBNE);//�����ж�ʹ��
	
	//ʹ�ܴ���
	usart_enable(USART1);
	
}


void USART1_Send_A_Data(uint8_t data)
{
	usart_data_transmit(USART1, data);
	while(RESET == usart_flag_get(USART1, USART_FLAG_TBE));
}


void USART1_Send_Data_buf(uint8_t *data,uint16_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		USART1_Send_A_Data(*data);
		data++;
	}
}

void USART1_IRQHandler(void)
{
	uint8_t res;
	if(usart_interrupt_flag_get(USART1,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART1,USART_INT_FLAG_RBNE);
		res = usart_data_receive(USART1);
		USART1_Send_A_Data(res);
		
	}

}

void usart2_init(uint32_t brr)
{
	//ʱ������
	rcu_periph_clock_enable(RCU_GPIOB);
	rcu_periph_clock_enable(RCU_USART2);
	
	//GPIOģʽ����
	gpio_init(GPIOB,GPIO_MODE_AF_PP,GPIO_OSPEED_50MHZ,GPIO_PIN_10);//TX-PB10
	gpio_init(GPIOB,GPIO_MODE_IN_FLOATING,GPIO_OSPEED_50MHZ,GPIO_PIN_11);//RX-PB11
	
	//���ڹ���ģʽ����
	usart_deinit(USART2);//��λ����USART
	usart_baudrate_set(USART2,brr);//���ò�����
	usart_word_length_set(USART2,USART_WL_8BIT);//8λ����λ
	usart_stop_bit_set(USART2,USART_STB_1BIT);//1��ֹͣλ
	usart_parity_config(USART2,USART_PM_NONE);//����żУ��
	usart_hardware_flow_cts_config(USART2,USART_CTS_DISABLE);//��Ӳ��������
	usart_hardware_flow_rts_config(USART2,USART_RTS_DISABLE);//��Ӳ��������
	usart_receive_config(USART2,USART_RECEIVE_ENABLE);//����ʹ��
	usart_transmit_config(USART2,USART_TRANSMIT_ENABLE);//����ʹ��
	
	//�ж�����
	nvic_irq_enable(USART2_IRQn,0,0);//�ж�ʹ��
	usart_interrupt_enable(USART2,USART_INT_RBNE);//�����ж�ʹ��
	usart_interrupt_enable(USART2,USART_INT_IDLE);//IDLE�ж�ʹ��
	
	//ʹ�ܴ���
	usart_enable(USART2);
	
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

volatile unsigned char u2_ok = 0;

//����2�жϷ�����
void USART2_IRQHandler(void)
{
	uint8_t res;
//	uint8_t i = 0;
//	static uint8_t count = 0;
	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(USART2,USART_INT_FLAG_RBNE);//���־λ
		res = usart_data_receive(USART2);//��ȡ����
		usart_data_transmit(USART0,res);//Debug
		
		
	}
	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_IDLE) != RESET)
	{
		res = usart_data_receive(USART2);
		
		u2_ok = 1;
		
//		count = 0;//����ֵ��λ
	}

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
	usart_interrupt_enable(UART3,USART_INT_IDLE);//IDLE�ж�ʹ��
	
	//ʹ�ܴ���
//	usart_enable(UART3);
	
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


int TSET_INTERRUPT = 0;
uint16_t FS_Ver = 0;

//����3�жϷ�����
void UART3_IRQHandler(void)
{
	uint8_t res;
//	uint8_t i = 0;
	static uint16_t count = 0;
	static uint16_t fs_ver = 0;
	if(usart_interrupt_flag_get(UART3,USART_INT_FLAG_RBNE) != RESET)
	{
		usart_interrupt_flag_clear(UART3,USART_INT_FLAG_RBNE);//���־λ
		res = usart_data_receive(UART3);//��ȡ����
		UART3_RX_BUF[count++] = res;
		fs_ver += res;
//		usart_data_transmit(USART0,res);//Debug
		TSET_INTERRUPT ++;
	}
	if(usart_interrupt_flag_get(UART3,USART_INT_FLAG_IDLE) != RESET)
	{
		res = usart_data_receive(UART3);
		
		UART3_RX_LEN = count;

		count = 0;//����ֵ��λ
		
		FS_Ver = fs_ver;
		
		fs_ver = 0;
		
		UART3_RX_FLAG = UART3_RX_OK;
		
		
	}

}
