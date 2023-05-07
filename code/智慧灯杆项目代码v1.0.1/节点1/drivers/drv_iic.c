#include "drv_iic.h"

void rt_iic_init(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);
	gpio_init(GPIOA,GPIO_MODE_OUT_OD,GPIO_OSPEED_50MHZ,I2C_SCL_PIN|I2C_SDA_PIN);

	
	gpio_bit_set(I2C_GPIO,I2C_SCL_PIN|I2C_SDA_PIN);
	
	I2C_TIM_Delay(1);	
}

void IIC_W_SCL(uint8_t BitValue)
{
	gpio_bit_write(I2C_GPIO,I2C_SCL_PIN,(FlagStatus)BitValue);
	I2C_TIM_Delay(1);

}


void IIC_W_SDA(uint8_t BitValue)
{
	gpio_bit_write(I2C_GPIO,I2C_SDA_PIN,(FlagStatus)BitValue);
	I2C_TIM_Delay(1);
}

uint8_t IIC_R_SDA(void)
{
	uint8_t BitValue;
	BitValue = gpio_input_bit_get(I2C_GPIO,I2C_SDA_PIN);
	I2C_TIM_Delay(1);
	return BitValue;
}



void IIC_Start(void)
{
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	
	IIC_W_SDA(0); 
	IIC_W_SCL(0);
}


void IIC_Stop(void)
{
		IIC_W_SDA(0); 
		IIC_W_SCL(1);
		IIC_W_SDA(1);
}

void IIC_SendByte(uint8_t Byte)
{
	uint8_t i;
	for(i = 0; i < 8;i ++)
	{
		IIC_W_SDA(Byte & (0x80 >> i));
		IIC_W_SCL(1);
		IIC_W_SCL(0);
	}
}


void IIC_SendAddress(uint8_t Byte)
{
	uint8_t i;
	for(i = 0; i < 7;i ++)
	{
		IIC_W_SDA(Byte & (0x80 >> i));
		IIC_W_SCL(1);
		IIC_W_SCL(0);
	}
}

uint8_t IIC_ReceiveByte(void)
{
	uint8_t i,Byte = 0x00;
	IIC_W_SDA(1);
	for(i = 0; i < 8;i ++)
	{
		IIC_W_SCL(1);
		if(IIC_R_SDA() == 1){Byte |= (0x80 >> i);}
		IIC_W_SCL(0);
	}
	return Byte;
	
}



void IIC_SendAsk(uint8_t AskByte)
{
		IIC_W_SDA(AskByte);
		IIC_W_SCL(1);
		IIC_W_SCL(0);
}


uint8_t IIC_ReceiveAsk(void)
{
	uint8_t AskByte;
	IIC_W_SDA(1);
	IIC_W_SCL(1);
	AskByte = IIC_R_SDA(); 
	IIC_W_SCL(0);

	return AskByte;
	
}

void Send_W_Adress(void)		//指定地址写
{
	IIC_Start();
	IIC_SendByte(Light_W_Adress);
}

void Send_R_Adress(void)		//指定地址读
{
	IIC_Start();
	IIC_SendByte(Light_R_Adress);
}


void Mode_Read_Wait_Time(uint8_t Mode_Selet)
{
		switch (Mode_Selet){
		case Cont_H_1_Mode:
			I2C_TIM_Delay(150);
			break;
		case Cont_H_2_Mode:
			I2C_TIM_Delay(150);
			break;
		case Cont_L_Mode:
			I2C_TIM_Delay(20);
			break;
		case Once_H_1_Mode:
			I2C_TIM_Delay(150);
			break;
		case Once_H_2_Mode:
			I2C_TIM_Delay(150);
			break;
		case Once_L_Mode:
			I2C_TIM_Delay(20);
			break;
		default:
			break;
	}

}

void Light_Serior(uint8_t Mode_Selet)
{
		rt_iic_init();
		Send_W_Adress();
	
		IIC_ReceiveAsk();
		IIC_SendByte(Mode_Selet);					//传感器默认模式
		IIC_ReceiveAsk();
		IIC_Stop();
}


uint16_t Light_Read_Date(uint8_t Mode_Selet)
{
		Mode_Read_Wait_Time(Mode_Selet);
		uint16_t Dates;
		uint8_t Date[2];
		Send_R_Adress();		//指定地址读
	             
		IIC_ReceiveAsk();
		Date[0] = IIC_ReceiveByte();		//高8位数据
		IIC_SendAsk(0);
		Date[1] = IIC_ReceiveByte();		//低8位数据
		IIC_SendAsk(1);
		
		IIC_Stop();
		Dates = (Date[0] << 8) | Date[1];
		Dates = (uint16_t)((float)Dates / 1.2);
//		rt_kprintf("The current light intensity is %d\n",Dates);
		return Dates;
}


