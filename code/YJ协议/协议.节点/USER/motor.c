#include "motor.h"

//保存窗帘的状态
unsigned char curtain_state = CURTAIN_OPEN;
//相序  
const unsigned int eighttb[8][4]=
{
	{0,1,1,0},
	{0,1,0,0},
	{0,1,0,1},
	{0,0,0,1},
	{1,0,0,1},
	{1,0,0,0},
	{1,0,1,0},
	{0,0,1,0}
};

/*****************************************************
函数名称：Motor_Gpio_Init
函数功能：丝杆电机使用的GPIO初始化
函数参数：无
函数返回值：无
备注：
	接在板子中间的传感器接口就是
	PB12---B+
	PB13---A+
	PB1----B-
	PA8----A-
	接在板子右边的传感器接口就是
	PB6----B+
	PB7----A+
	PA0----B-
	PB5----A-
版本：V1.0
作者：ZZXYD
*******************************************************/
void Motor_Gpio_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_APB2PeriphClockCmd(A1_CLK|A0_CLK|B1_CLK|B0_CLK,ENABLE);
	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;

	GPIO_InitStruct.GPIO_Pin = A1_PIN;
	GPIO_Init(A1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = A0_PIN;
	GPIO_Init(A0_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = B1_PIN;
	GPIO_Init(B1_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin = B0_PIN;
	GPIO_Init(B0_PORT,&GPIO_InitStruct);
	
	//全部清零
	A1_OPT(0);A0_OPT(0);B1_OPT(0);B0_OPT(0);
	
}

/*****************************************************
函数名称：Motorcw
函数功能：驱动丝杆电机转动
函数参数：int anti
		anti == 0---》正转
		anti == 1---》反转
函数返回值：无
备注：
版本：V1.0
作者：ZZXYD
*******************************************************/
u8 Motorcw(unsigned char anti)  
{  
	
  int i; 	
	int pos = 0;
	for(i=0;i<8;i++)
	{
		switch(anti)
		{
			case 0:pos = i;break;
			case 1:pos = 7 - i;break;
			default:return 0;
		}
		A1_OPT(eighttb[pos][0]);
		A0_OPT(eighttb[pos][1]);
		B1_OPT(eighttb[pos][2]);
		B0_OPT(eighttb[pos][3]);
		delay_ms(10);
	}
	return 1;
	
	
}  



