#include "illu.h"

u8  BUF[8];
u16 illu_val = 0;
/*****************************************************
函数名称：Illu_Gpio_Init
函数功能：光照强度传感器BH1750使用的GPIO初始化
			IIC通信
函数参数：无
函数返回值：无
备注：
	接在板子中间的传感器接口就是
	PB12---SCL
	PB13---SDA
	接在板子右边的传感器接口就是
	PB6----SCL
	PB7----SDA
版本：V1.0
作者：ZZXYD
*******************************************************/
void ILLU_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(SDA_CLK|SCL_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	GPIO_Init(SDA_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SCL_PIN;
	GPIO_Init(SCL_PORT,&GPIO_InitStructure);
}

/*****************************************************
函数名称：SDA_Gpio_init
函数功能：IIC通信SDA引脚模式配置
函数参数：int flag
		flag == 1--》SDA引脚配置为输出模式
		flag == 0--》SDA引脚配置为输入模式
函数返回值：无
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
void SDA_Gpio_init(int flag)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;
	if(flag == 1)
	{
		//输出
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	}
	else if(flag == 0)
	{
		//输入
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	}
	GPIO_Init(SDA_PORT,&GPIO_InitStructure);
}

/*****************************************************
函数名称：IIC_Start
函数功能：IIC通信起始信号
函数参数：无
函数返回值：无
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
static void IIC_Start(void)
{
  SDA_Gpio_init(1);    //SDA为写模式
  LIGHT_SDA(1);//SDA拉高
  LIGHT_SCL(1);//SCL拉高

  delay_us(5); //延时5us

  LIGHT_SDA(0);//SDA拉低
  delay_us(5); //延时5us
  LIGHT_SCL(0);//SCL拉低

}

/*****************************************************
函数名称：IIC_Start
函数功能：IIC通信停止信号
函数参数：无
函数返回值：无
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
static void IIC_Stop(void)
{
  SDA_Gpio_init(1);    //SDA为写模式
  LIGHT_SDA(0);//SDA拉低
  LIGHT_SCL(1);//SCL拉高
  delay_us(5); //延时5us
  LIGHT_SDA(1);//SDA拉高 
  delay_us(5); //延时5us
}


/*****************************************************
函数名称：IIC_SendACK
函数功能：IIC通信应答信号
函数参数：u8 ack
			0:ACK 
			1:NAK
函数返回值：无
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
void IIC_SendACK(u8 ack)
{
    SDA_Gpio_init(1);
    LIGHT_SDA(ack);                  //写应答信号
    LIGHT_SCL(1);                    //拉高时钟线
    delay_us(5);             //延时5us
    LIGHT_SCL(0);                    //拉低时钟线
    delay_us(5);             //延时
}

/*****************************************************
函数名称：IIC_RecvACK
函数功能：IIC通信接收应答信号
函数参数：无
函数返回值：返回应答信号
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
u8 IIC_RecvACK(void)
{
    u8 ack = 0;
    
    SDA_Gpio_init(0);                    //SDA为读模式

    LIGHT_SCL(1);              //拉高时钟线
    delay_us(10);       //延时
    ack = IN_SDA;            //读应答信号
    LIGHT_SCL(0);              //拉低时钟线
    delay_us(10);       //延时

    SDA_Gpio_init(1);
    return ack;
}


/*****************************************************
函数名称：IIC_SendByte
函数功能：向IIC总线发送一个字节数据
函数参数：u8 dat 要发送的数据
函数返回值：返回应答信号
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
u8 IIC_SendByte(u8 dat)
{
    u8 ack = 0;
    u8 i;
    SDA_Gpio_init(1);                    //SDA为写模式
    for (i=0; i<8; i++)         //8位计数器
    {
      if(dat & 0x80)            //送数据口
        LIGHT_SDA(1);
      else
        LIGHT_SDA(0);
      dat <<= 1;                //移出数据的最高位              
      LIGHT_SCL(1);            //拉高时钟线
      delay_us(5);     //延时        
      LIGHT_SCL(0);            //拉低时钟线
      delay_us(5);     //延时
    }
    ack = IIC_RecvACK();
    return ack;
}


/*****************************************************
函数名称：IIC_RecvByte
函数功能：从IIC总线接收一个字节数据
函数参数：无
函数返回值：返回读取到的数据
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
u8 IIC_RecvByte(void)
{
    unsigned char i;
    unsigned char dat = 0;

    LIGHT_SDA(1);                    //使能内部上拉,准备读取数据,
    SDA_Gpio_init(0);                    //SDA为读模式
    for (i=0; i<8; i++)         //8位计数器
    {
        dat <<= 1;
        LIGHT_SCL(1);                //拉高时钟线
        delay_us(5);             //延时
        dat |= GPIO_ReadInputDataBit(SDA_PORT,SDA_PIN);             //读数据               
        LIGHT_SCL(0);                //拉低时钟线
        delay_us(5);             //延时
    }
    SDA_Gpio_init(1);
    return dat;
}

/*****************************************************
函数名称：Single_Write_BH1750
函数功能：往BH1750传感器写入一字节数据
函数参数：u8 REG_Address
			要写入的寄存器地址
函数返回值：无
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
void Single_Write_BH1750(u8 REG_Address)
{
    IIC_Start();                  //起始信号
    IIC_SendByte(SlaveAddress);   //发送设备地址+写信号              //没有接收到应答信号，返回0xff
    IIC_SendByte(REG_Address);   //内部寄存器地址，请参考中文pdf22页 
                     //没有收到应答信号，返回0xfe

  //  BH1750_SendByte(REG_data);       //内部寄存器数据，请参考中文pdf22页 
    IIC_Stop();                   //发送停止信号
}



/*****************************************************
函数名称：Multiple_Read_BH1750
函数功能：连续读出BH1750内部数据
函数参数：无
函数返回值：无
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
void Multiple_Read_BH1750(void)
{   
    u8 i;	
    IIC_Start();                          //起始信号
    IIC_SendByte(SlaveAddress+1);         //发送设备地址+读信号
     
	for (i=0; i<2; i++)                      //连续读取6个地址数据，存储中BUF
    {
        BUF[i] = IIC_RecvByte();          //BUF[0]存储0x32地址中的数据
        if (i == 1)
        {
           IIC_SendACK(1);                //最后一个数据需要回NOACK
        }
        else
        {		
          IIC_SendACK(0);                //回应ACK
        }
   }
   IIC_Stop();                          //停止信号
   delay_ms(5);                           //延时5ms
}


/*****************************************************
函数名称：ILLU_ReadData
函数功能：BH1750读取光照强度数据
函数参数：无
函数返回值：光照值
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
u16 ILLU_ReadData(void)
{
   Single_Write_BH1750(0x01);   // power on
   Single_Write_BH1750(0x10);   // H- resolution mode
   delay_ms(180);               //延时180ms
   Multiple_Read_BH1750();      //连续读出数据，存储在BUF中  

   return  (u16)(((BUF[0]<<8)+BUF[1])/1.2);
} 

/**********************


        

*******************************
函数名称：ILLU_Show_Usart
函数功能：读取光照数据并且在串口显示
函数参数：无
函数返回值：返回值是光照强度
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
u16 ILLU_Show_Usart(void)
{
	illu_val = ILLU_ReadData();
	printf("illu_val = %dlx\r\n",illu_val);
	return illu_val;
}



