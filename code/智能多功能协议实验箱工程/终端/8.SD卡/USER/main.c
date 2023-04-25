#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"
#include "jpg_app.h"
#include "fatfs_app.h"
//LCD显示SD卡中图片和文字
int main(void)
{
	Usart1_Init(115200);
	printf("串口1初始化成功\r\n");
	LCD_Init();
	SD_Fatfs_Init();
	JPG_ShowFunc("0:/pic2/cat2.jpg",0,0);
	LCD_ShowStr(70,240,(u8 *)"你好，猫咪！");
	
	
	
	while(1)
	{
		
	}

}
