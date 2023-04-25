#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "W25Q64.h"


//FLASH保存流水灯状态
int main(void)
{
	u8 i=0;//流水灯标志位
	u8 wirtebuf[1];//定义一个临时存储状态的数组
	Usart1_Init(115200);//串口初始化
	LED_Init();//LED初始化
	sFLASH_Init();//FLASH初始化
	Flash_Test();//FLASH测试
	i = sFLASH_Read_Byte(0x0000fd);//读取关机前的数据
	
	while(1)
	{
		//------------------流水灯程序---------------------//
		if(i == 7)
		{
			i = 0;
			LED1_7_Port->ODR = ~(0 << 6);//第7灯关闭
			LED8_Port->ODR = ~(1 << 13);//第8灯开启
		}
		else
		{
			LED1_7_Port->ODR = ~(1 << (i++));//1-7灯流水
			LED8_Port->ODR = ~(0 << 13);//第8灯关闭
		
		}
		//------------------流水灯程序---------------------//
	
		//-----------------FLASH保存数据-------------------//
		wirtebuf[0] = i;//存数据
		sFLASH_EraseSector(0x0000fd);//擦除扇区
		sFLASH_WriteBuffer(wirtebuf,0x0000fd,1);//写数据
		//-----------------FLASH保存数据-------------------//
		
		delay_ms(300);//300ms延迟
	}
	
	

}
