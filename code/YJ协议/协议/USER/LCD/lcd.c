#include "lcd.h"
#include "font.h"
#include "delay.h"
#include "math.h"
#include "stdio.h"


/****************************************************
函数名：Delay
形参： 无
返回值：无
函数功能：延时函数
****************************************************/
static void Delay(u32 time)
{    
   u16 i=0;  
   while(time--)
   {
	  i=12000;  //自己定义
	  while(i--) ;    
   }
}

//管理LCD重要参数、默认为竖屏
_lcd_dev lcddev;
/****************************************************
函数名：LCD_WR_REG
形参：  regval:寄存器值
返回值：无
函数功能：写寄存器函数
****************************************************/
void LCD_WR_REG(u16 regval)
{ 
	LCD->LCD_REG=regval;//写入要写的寄存器序号	 
}
/****************************************************
函数名：LCD_WR_DATA
形参： data:要写入的值
返回值：无
函数功能：写LCD数据
****************************************************/
void LCD_WR_DATA(u16 data)
{										    	   
	LCD->LCD_RAM=data;		 
}
/****************************************************
函数名：LCD_RD_DATA
形参：无
返回值：读取的16位颜色值
函数功能：读LCD数据
****************************************************/
u16 LCD_RD_DATA(void)
{										    	   
	return LCD->LCD_RAM;		 
}					   
/****************************************************
函数名：LCD_WriteReg
形参：LCD_Reg:寄存器地址   LCD_RegValue:要写入的数据
返回值：无
函数功能：写寄存器
****************************************************/
void LCD_WriteReg(u8 LCD_Reg, u16 LCD_RegValue)
{	
	LCD->LCD_REG = LCD_Reg;		//写入要写的寄存器序号		 
	LCD->LCD_RAM = LCD_RegValue;//写入数据	    		 
}	   
/****************************************************
函数名：LCD_ReadReg
形参：LCD_Reg:寄存器地址
返回值：读到的数据
函数功能：读寄存器
****************************************************/
u16 LCD_ReadReg(u8 LCD_Reg)
{										   
	LCD_WR_REG(LCD_Reg);		//写入要读的寄存器序号
	delay_us(5);		  
	return LCD_RD_DATA();		//返回读到的值
}   
/****************************************************
函数名：LCD_WriteRAM_Prepare
形参：无
返回值：无
函数功能：开始写GRAM
****************************************************/
void LCD_WriteRAM_Prepare(void)
{
 	LCD->LCD_REG=lcddev.wramcmd;	  
}	
/****************************************************
函数名：LCD_WriteRAM
形参：RGB_Code:颜色值
返回值：无
函数功能：LCD写GRAM
****************************************************/
void LCD_WriteRAM(u16 RGB_Code)
{							    
	LCD->LCD_RAM = RGB_Code;//写十六位GRAM
}
/***************************************************************************************
2^26 =0X0400 0000 = 64MB,每个 BANK 有4*64MB = 256MB
64MB:FSMC_Bank1_NORSRAM1:0X6000 0000 ~ 0X63FF FFFF
64MB:FSMC_Bank1_NORSRAM2:0X6400 0000 ~ 0X67FF FFFF
64MB:FSMC_Bank1_NORSRAM3:0X6800 0000 ~ 0X6BFF FFFF
64MB:FSMC_Bank1_NORSRAM4:0X6C00 0000 ~ 0X6FFF FFFF

选择BANK1-BORSRAM4 连接 TFT，地址范围为0X6C00 0000 ~ 0X6FFF FFFF
FSMC_A23 接LCD的RS(寄存器/数据选择)脚
寄存器基地址 = 0X6C00 0000
RAM基地址 = 0X6D00 0000 = 0X6C00 0000+2^23*2 = 0X6C00 0000 + 0X100 0000 = 0X6D00 0000
当选择不同的地址线时，地址要重新计算  
****************************************************************************************/
/****************************************************
函数名：LCD_GPIO_Config
形参：无
返回值：无
函数功能：初始化LCD屏接口的GPIO口
****************************************************/
void LCD_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

    /* 使能FSMC时钟*/
    
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB |RCC_APB2Periph_GPIOD \
	 					  |RCC_APB2Periph_GPIOE |RCC_APB2Periph_GPIOG,\
	                       ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	/* 配置LCD背光控制管脚*/

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/* 配置LCD复位控制管脚*/
    //用的是RESET管脚


	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode =  GPIO_Mode_AF_PP;
	/* 配置FSMC相对应的数据线,FSMC-D0~D15: PD 14 15 0 1 8 9 10,PE 7 8 9 10 11 12 13 14 15*/	

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1\
	                              |GPIO_Pin_8| GPIO_Pin_9\
	                              |GPIO_Pin_10|GPIO_Pin_14\
	                              |GPIO_Pin_15;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8\
	 							| GPIO_Pin_9 | GPIO_Pin_10\
	 						    | GPIO_Pin_11 | GPIO_Pin_12\
	 						    | GPIO_Pin_13 | GPIO_Pin_14\
	   						    | GPIO_Pin_15;
	GPIO_Init(GPIOE, &GPIO_InitStructure);

	/* 配置FSMC相对应的控制线
	* PD4-FSMC_NOE   :LCD-RD
	* PD5-FSMC_NWE   :LCD-WR
	* PG12-FSMC_NE4  :LCD-CS
	* PE2-FSMC_A23   :LCD-DC
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//PD4-FSMC_NOE   :LCD-RD

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_Init(GPIOD, &GPIO_InitStructure);	//PD5-FSMC_NWE   :LCD-WR
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //PG12-FSMC_NE4  :LCD-CS

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOG, &GPIO_InitStructure);  //PE2-FSMC_A10   :LCD-DC发送的数据代表是命令还是显示数据

 		    
}
/****************************************************
函数名：LCD_FSMC_Config
形参：无
返回值：无
函数功能：FSMC的初始化
****************************************************/
void LCD_FSMC_Config(void)
{
	
	FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  WRITE_USER; 
	FSMC_NORSRAMTimingInitTypeDef  READ_USER; 
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_FSMC, ENABLE);

	READ_USER.FSMC_AddressSetupTime = 0x01;	 //地址建立时间
    READ_USER.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    READ_USER.FSMC_DataSetupTime = 0x0f;		 //数据建立时间
    READ_USER.FSMC_BusTurnAroundDuration = 0x00;
    READ_USER.FSMC_CLKDivision = 0x00;
    READ_USER.FSMC_DataLatency = 0x00;
    READ_USER.FSMC_AccessMode = FSMC_AccessMode_A;	 // 

	WRITE_USER.FSMC_AddressSetupTime = 0x00;	 //地址建立时间
    WRITE_USER.FSMC_AddressHoldTime = 0x00;	 //地址保持时间
    WRITE_USER.FSMC_DataSetupTime = 0x03;		 //数据建立时间
    WRITE_USER.FSMC_BusTurnAroundDuration = 0x00;//
    WRITE_USER.FSMC_CLKDivision = 0x00;//
    WRITE_USER.FSMC_DataLatency = 0x00;//
    WRITE_USER.FSMC_AccessMode = FSMC_AccessMode_A;	 // 
    
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;  //选择外接存储区域的地址
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;//配置数据线和地址线是否复用
//    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
	FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;//配置外接存储器的类型
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;//设置FSMC接口的数据宽度
	
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;//配置访问模式
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;//配置等待信号的极性
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;//配置是否使用非对齐方式
	FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;//配置等待信号什么时间产生
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;//配置是否使用等待信号
	FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;//配置是否允许突发写操作
	
	FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;//配置写操作使能
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Enable ;//配置是否使用扩展模式
    
	
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &READ_USER;//读时序
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &WRITE_USER;  //写时序
    
    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure); 
    
    /* 使能 FSMC Bank1_SRAM Bank */
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);

}
/****************************************************
函数名：LCD_init_RGB
形参：无
返回值：无
函数功能：LCD屏ILI9341的初始化
****************************************************/
void LCD_init_RGB(void)
{
	Delay(50); // delay 50 ms 
	
 	LCD_WriteReg(0x0000,0x0001);
	 
	Delay(50); // delay 50 ms 
  	lcddev.id = LCD_ReadReg(0x0000);  
	 
  	if(lcddev.id<0XFF||lcddev.id==0XFFFF||lcddev.id==0X9300)//读到ID不正确,新增lcddev.id==0X9300判断，因为9341在未被复位的情况下会被读成9300
	{	
 		//尝试9341 ID的读取		
		LCD_WR_REG(0XD3);				   
		LCD_RD_DATA(); 				//dummy read 	
 		LCD_RD_DATA();   	    	//读到0X00
  		lcddev.id=LCD_RD_DATA();   	//读取93								   
 		lcddev.id<<=8;
		lcddev.id|=LCD_RD_DATA();  	//读取41 	   			   
 		
		if(lcddev.id!=0X9341)lcddev.id=0x9341;//用于识别9341 	     
	}
	 
	if(lcddev.id==0X9341)	//9341初始化  使用的是扩展模式
	{	 
		LCD_WR_REG(0xCF);      
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0xC1); 
		LCD_WR_DATA(0X30); 
		
		LCD_WR_REG(0xED);  
		LCD_WR_DATA(0x64); 
		LCD_WR_DATA(0x03); 
		LCD_WR_DATA(0X12); 
		LCD_WR_DATA(0X81);
		
		LCD_WR_REG(0xE8);  
		LCD_WR_DATA(0x85); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x7A); 
		
		LCD_WR_REG(0xCB);  
		LCD_WR_DATA(0x39); 
		LCD_WR_DATA(0x2C); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x34); 
		LCD_WR_DATA(0x02);
		
		LCD_WR_REG(0xF7);  
		LCD_WR_DATA(0x20);
		
		LCD_WR_REG(0xEA);  
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00);
		
		LCD_WR_REG(0xC0);    //Power control 
		LCD_WR_DATA(0x1B);   //VRH[5:0] 
		
		LCD_WR_REG(0xC1);    //Power control 
		LCD_WR_DATA(0x01);   //SAP[2:0];BT[3:0] 
		
		LCD_WR_REG(0xC5);    //VCM control 
		LCD_WR_DATA(0x30); 	 //3F
		LCD_WR_DATA(0x30); 	 //3C
		
		LCD_WR_REG(0xC7);    //VCM control2 
		LCD_WR_DATA(0XB7); 
		
		LCD_WR_REG(0x36);    // Memory Access Control 
		LCD_WR_DATA(0x48); 
		
		LCD_WR_REG(0x3A);   
		LCD_WR_DATA(0x55); 
		
		LCD_WR_REG(0xB1);   
		LCD_WR_DATA(0x00);   
		LCD_WR_DATA(0x1A); 
		
		LCD_WR_REG(0xB6);    // Display Function Control 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0xA2); 
		
		LCD_WR_REG(0xF2);    // 3Gamma Function Disable 
		LCD_WR_DATA(0x00); 
		
		LCD_WR_REG(0x26);    //Gamma curve selected 
		LCD_WR_DATA(0x01); 
		
		LCD_WR_REG(0xE0);    //Set Gamma 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x2A); 
		LCD_WR_DATA(0x28); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x0E); 
		LCD_WR_DATA(0x08); 
		LCD_WR_DATA(0x54); 
		LCD_WR_DATA(0XA9); 
		LCD_WR_DATA(0x43); 
		LCD_WR_DATA(0x0A); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x00); 
		
		LCD_WR_REG(0XE1);    //Set Gamma 
		LCD_WR_DATA(0x00); 
		LCD_WR_DATA(0x15); 
		LCD_WR_DATA(0x17); 
		LCD_WR_DATA(0x07); 
		LCD_WR_DATA(0x11); 
		LCD_WR_DATA(0x06); 
		LCD_WR_DATA(0x2B); 
		LCD_WR_DATA(0x56); 
		LCD_WR_DATA(0x3C); 
		LCD_WR_DATA(0x05); 
		LCD_WR_DATA(0x10); 
		LCD_WR_DATA(0x0F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x3F); 
		LCD_WR_DATA(0x0F); 
		
		LCD_WR_REG(0x2B); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x01);
		LCD_WR_DATA(0x3f);
		
		LCD_WR_REG(0x2A); 
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0x00);
		LCD_WR_DATA(0xef);
		
		LCD_WR_REG(0x11); //Exit Sleep
		Delay(120);
		LCD_WR_REG(0x29); //display on	
	}
}
/****************************************************
函数名：LCD_Init
形参：无
返回值：无
函数功能：LCD屏的初始化
****************************************************/
void LCD_Init(void)
{
	LCD_GPIO_Config();
	
	LCD_FSMC_Config();
	
	LCD_init_RGB();
		
	/* 开背光 */
	LCD_DisplayOn();
	
     LCD_Display_Dir(0);		 	//竖屏
		//LCD_Display_Dir(1);		 	//横屏
	
	LCD_Clear(WHITE);	
}


//Xpos:横坐标

//Ypos:纵坐标
/****************************************************
函数名：LCD_SetCursor
形参：u16 Xpos -- X坐标, u16 Ypos -- Y坐标
返回值：无
函数功能：设置LCD屏坐标值
****************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{    
      if(lcddev.id==0X9341)
      {               
				LCD_WR_REG(lcddev.setxcmd);
				LCD_WR_DATA(Xpos>>8);
				LCD_WR_DATA(Xpos&0XFF);
				LCD_WR_DATA(240>>8);
				LCD_WR_DATA(240&0XFF);
				LCD_WR_REG(lcddev.setycmd);
				LCD_WR_DATA(Ypos>>8);
				LCD_WR_DATA(Ypos&0XFF);
				LCD_WR_DATA(320>>8);
				LCD_WR_DATA(320&0XFF);
      }
} 
/****************************************************
函数名：LCD_Scan_Dir
形参：dir:0~7,代表8个方向(具体定义见lcd.h) 
返回值：无
函数功能：设置LCD的自动扫描方向（一般设置为L2R_U2D即可,如果设置为其他扫描方式,可能导致显示不正常.）
****************************************************/  
void LCD_Scan_Dir(u8 dir)
{
	u16 regval=0;
	u8 dirreg=0;
	u16 temp;  
	if(lcddev.dir==1&&lcddev.id!=0X6804)//横屏时，对6804不改变扫描方向！
	{			   
		switch(dir)//方向转换
		{
			case 0:dir=6;break;
			case 1:dir=7;break;
			case 2:dir=4;break;
			case 3:dir=5;break;
			case 4:dir=1;break;
			case 5:dir=0;break;
			case 6:dir=3;break;
			case 7:dir=2;break;	     
		}
	}
	if(lcddev.id==0x9341||lcddev.id==0X6804)//9341/6804,很特殊
	{
		switch(dir)
		{
			case L2R_U2D://从左到右,从上到下
				regval|=(0<<7)|(0<<6)|(0<<5); 
				break;
			case L2R_D2U://从左到右,从下到上
				regval|=(1<<7)|(0<<6)|(0<<5); 
				break;
			case R2L_U2D://从右到左,从上到下
				regval|=(0<<7)|(1<<6)|(0<<5); 
				break;
			case R2L_D2U://从右到左,从下到上
				regval|=(1<<7)|(1<<6)|(0<<5); 
				break;	 
			case U2D_L2R://从上到下,从左到右
				regval|=(0<<7)|(0<<6)|(1<<5); 
				break;
			case U2D_R2L://从上到下,从右到左
				regval|=(0<<7)|(1<<6)|(1<<5); 
				break;
			case D2U_L2R://从下到上,从左到右
				regval|=(1<<7)|(0<<6)|(1<<5); 
				break;
			case D2U_R2L://从下到上,从右到左
				regval|=(1<<7)|(1<<6)|(1<<5); 
				break;	 
		}
		dirreg=0X36;
 		regval|=0X08;//BGR   
		if(lcddev.id==0X6804)regval|=0x02;//6804的BIT6和9341的反了	   
		LCD_WriteReg(dirreg,regval);
 		if((regval&0X20)||lcddev.dir==1)
		{
			if(lcddev.width<lcddev.height)//交换X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}else  
		{
			if(lcddev.width>lcddev.height)//交换X,Y
			{
				temp=lcddev.width;
				lcddev.width=lcddev.height;
				lcddev.height=temp;
 			}
		}  
		LCD_WR_REG(lcddev.setxcmd); 
		LCD_WR_DATA(0);
		LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.width-1)>>8);
		LCD_WR_DATA((lcddev.width-1)&0XFF);
		
		LCD_WR_REG(lcddev.setycmd); 
		LCD_WR_DATA(0);LCD_WR_DATA(0);
		LCD_WR_DATA((lcddev.height-1)>>8);LCD_WR_DATA((lcddev.height-1)&0XFF);  
  	}
} 
/****************************************************
函数名：LCD_Display_Dir
形参：dir:0,竖屏；1,横屏
返回值：无
函数功能：设置LCD显示方向
****************************************************/
void LCD_Display_Dir(u8 dir)
{
	if(dir==0)			//竖屏
	{
		lcddev.dir=0;	//竖屏
		lcddev.width=240;
		lcddev.height=320;
		if(lcddev.id==0X9341||lcddev.id==0X6804)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
			if(lcddev.id==0X6804)
			{
				lcddev.width=320;
				lcddev.height=480;
			}
		}
	}
	else 				//横屏
	{	  				
		lcddev.dir=1;	//横屏
		lcddev.width=320;
		lcddev.height=240;
		if(lcddev.id==0X9341)
		{
			lcddev.wramcmd=0X2C;
	 		lcddev.setxcmd=0X2A;
			lcddev.setycmd=0X2B;  	 
		}
	} 
	LCD_Scan_Dir(DFT_SCAN_DIR);	//默认扫描方向
}
/****************************************************
函数名：LCD_DrawPoint
形参：x,y:坐标
返回值：POINT_COLOR:此点的颜色
函数功能：画点函数
****************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 c)
{	 
	LCD_SetCursor(x,y);		//设置光标位置 
	LCD_WriteRAM_Prepare();	//开始写入GRAM
	LCD->LCD_RAM=c; 
}
/****************************************************
函数名：LCD_ReadPoint
形参：x,y:坐标
返回值：此点的颜色
函数功能：读取个某点的颜色值
****************************************************/
u16 LCD_ReadPoint(u16 x,u16 y)
{
 	u16 r=0,g=0,b=0;
	if(x>=lcddev.width||y>=lcddev.height)return 0;	//超过了范围,直接返回		   
	LCD_SetCursor(x,y);	 
	
	if(lcddev.id==0X9341)LCD_WR_REG(0X2E);//9341/6804/3510 发送读GRAM指令	    
	if(LCD->LCD_RAM)r=0;							//dummy Read	   
	Delay(2);	  
 	r=LCD->LCD_RAM;  		  						//实际坐标颜色
 	if(lcddev.id==0X9341)		//9341/NT35310/NT35510要分2次读出
 	{
		Delay(2);	  
		b=LCD->LCD_RAM; 
		g=r&0XFF;		//对于9341,第一次读取的是RG的值,R在前,G在后,各占8位
		g<<=8;
	}
	b=b;//防止编译器报警
	return g;
}
/****************************************************
函数名：LCD_DisplayOn
形参：无
返回值：无
函数功能：打开LCD屏显示背光
****************************************************/
void LCD_DisplayOn(void)//开显示
{
	/* 开背光 */
	GPIO_SetBits(GPIOB, GPIO_Pin_0);	
}
/****************************************************
函数名：LCD_DisplayOff
形参：无
返回值：无
函数功能：关闭LCD屏显示背光
****************************************************/
void LCD_DisplayOff(void)//关显示
{
	/*关背光 */
	GPIO_ResetBits(GPIOB, GPIO_Pin_0);
}



//清屏函数
//color:要清屏的填充色
/****************************************************
函数名：LCD_Clear
形参：u16 color -- 清屏的颜色值
返回值：无
函数功能：清屏
****************************************************/
void LCD_Clear(u16 color)
{
	u32 index=0;      
	u32 totalpoint=lcddev.width;
	totalpoint*=lcddev.height; 			//得到总点数
	LCD_SetCursor(0x00,0x0000);	//设置光标位置 
	LCD_WriteRAM_Prepare();     		//开始写入GRAM	 	  
	for(index=0;index<totalpoint;index++)
	{
		LCD->LCD_RAM=color;	   
	}
}  

/****************************************************
函数名：my_itoa_n
形参：  num(要转化的数)
        str(转化好的数组的地址)
        radix(进制 2 8 10 16)
返回值：
函数功能：把数字转化为字符串
****************************************************/
char *my_itoa_n(int num,char *str,int radix)
{
	const char table[] = "0123456789ABCDEF";
	char *ptr = str;
	char *start = NULL;
    char temp;
	char negative = 0;
	if(num == 0)
	{
		*ptr++ = '0';
//		*ptr='\0';
		return str;
	}
	if(num<0)
	{
		*ptr++='-';
		num = num*(-1);
		negative = 1;
	}
	while(num)
	{
		*ptr++ = table[num % radix];
		num/=radix;
	}
//	*ptr = '\0';
	start = (negative ? str+1:str);
	ptr--;
	while(start<ptr)
	{
		temp = *start;
		*start = *ptr;
		*ptr = temp;
		start++;
		ptr--;
	}
	return str;
}

//在指定区域内填充单个颜色
//(sx,sy),(ex,ey):填充矩形对角坐标,区域大小为:(ex-sx+1)*(ey-sy+1)   
//color:要填充的颜色
void LCD_Fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color)
{          
	u16 i,j;
	u16 xlen=0;
	xlen=ex-sx+1;	 
	for(i=sy;i<=ey;i++)
	{
		LCD_SetCursor(sx,i);      				//设置光标位置 
		LCD_WriteRAM_Prepare();     			//开始写入GRAM	  
		for(j=0;j<xlen;j++)LCD->LCD_RAM=color;	//显示颜色 	    
	} 
}

/*
函数功能：画线
函数参数：x1,y1:起点坐标
					x2,y2:终点坐标
					color：颜色值

返回值：无
*/
void LCD_DrawLine(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol; 
	delta_x=x2-x1; //计算坐标增量 
	delta_y=y2-y1; 
	uRow=x1; 
	uCol=y1; 
	if(delta_x>0)incx=1; //设置单步方向 
	else if(delta_x==0)incx=0;//垂直线 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//水平线 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //选取基本增量坐标轴 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//画线输出 
	{  
		LCD_DrawPoint(uRow,uCol,color);//画点 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}





//设置显示区域
void LCD_Address_Set(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end)
{
	LCD_WR_REG(lcddev.setxcmd);
	LCD_WR_DATA(x_start>>8);
	LCD_WR_DATA(x_start&0XFF); 
	LCD_WR_DATA(x_end>>8);
	LCD_WR_DATA(x_end&0XFF); 
	LCD_WR_REG(lcddev.setycmd);
	LCD_WR_DATA(y_start>>8);
	LCD_WR_DATA(y_start&0XFF);
	LCD_WR_DATA(y_end>>8);
	LCD_WR_DATA(y_end&0XFF); 	
  LCD_WriteRAM_Prepare();
}



/*
x -- x轴起始坐标
y -- y轴起始坐标 
width -- 模的宽度
high -- 模的高度
p -- 字模
*/

uint16_t LCD_Color = BLACK;
uint16_t LCD_BackColor= WHITE;

void LCD_ShowxXx(uint16_t x, uint16_t y, uint16_t width, uint16_t high, char *p)
{
	uint8_t i = 0,j=0,k=0;
	LCD_Address_Set(x,y,x+width-1,y+high-1);
	for(i=0;i<high;i++)
	{
		for(j=0;j<width/8;j++)
		{
			for(k=0;k<8;k++)
			{
				if(*p & (0x80 >> k))
					LCD_WR_DATA(LCD_Color);
        else
					LCD_WR_DATA(LCD_BackColor);
			}
			p++;
		}
	}
}

/*
col -- 开始的行
page - 开始的页
p    - 指向要显示的字符串
*/
/*
col -- 开始的行
page - 开始的页
p    - 指向要显示的字符串
*/
#include "ff.h"

struct _FONT *PHZ = &font_hz1616;//设置汉字的字体
struct _FONT *PZF = &font_zf816; //设置字符的字体



void LCD_ShowStr(uint16_t col,uint16_t page,uint8_t *p)
{
	uint8_t hz_buff[72]="\0";//保存汉字的模
	uint16_t newcol = col;
	uint16_t newpage = page;
	uint32_t offset = 0;
	struct _FONT *pfont = NULL;
	uint8_t *pfontaddr = NULL;
	FIL fil;            /* Pointer to a file object */
	UINT  rb; 
  u8 hz_flag = 0;//汉字是否打开标志 0 -- 打开失败  1 -- 打开成功
  if (f_open(&fil, PHZ->hz_zf_addr.hzaddr, FA_READ) == FR_OK) 
	{
//		printf("打开%d*%d汉字库成功！\r\n",PHZ->width,PHZ->high);
		hz_flag = 1;
	}
//	else 
//		printf("打开%d*%d汉字库失败，没办法显示汉字！\r\n",PHZ->width,PHZ->high);
	while(*p)//判断字符串是否为空 -- '\0'
	{
		if((*p > 0xA1) && hz_flag)//汉字
		{ 
      pfont = PHZ;	
      //字库偏移位置         
			offset = ((*p-0xa1)*94+(*(p+1)-0xa1))*pfont->high*pfont->width/8;;//模的偏移位置
			f_lseek(&fil,offset);
			f_read(&fil,hz_buff,pfont->high*pfont->width/8,&rb);
//			printf("读到%d个字节\r\n",rb);
			pfontaddr = hz_buff;
			p+=2;
		}
		else
		{
			pfont = PZF;
			offset = Font_ZfFind(*p);
			if(offset != 0xff) 
			{
				offset *= pfont->width*pfont->high/8;
				pfontaddr = (uint8_t *)(pfont->hz_zf_addr.zfaddr+offset);
			}
			p++;//指针向后移动
		}
		LCD_ShowxXx(newcol,newpage,pfont->width,pfont->high,(char *)pfontaddr);
		newcol += pfont->width;
		if(newcol > (LCD_WIDTH - pfont->width))
		{
			newcol = 0;
			newpage += pfont->high;
		}
	}
	f_close(&fil);
}



