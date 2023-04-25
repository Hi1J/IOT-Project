#include "stm32f10x.h"
#include "delay.h"
#include "lcd.h"
#include "usart.h"
#include "jpg_app.h"
#include "fatfs_app.h"
#include "lora.h"
#include "hal_wireless.h"
#include "string.h"
//LORAc传输数据
int main(void)
{
	Usart1_Init(115200);
	printf("串口1初始化成功\r\n");
//	LCD_Init();
//	SD_Fatfs_Init();
//	JPG_ShowFunc("0:/pic2/cat2.jpg",0,0);
//	LCD_ShowStr(70,240,(u8 *)"你好，猫咪！");
	Lora_Config();
	printf("Lora初始化成功\r\n");
	
	while(1)
	{
		if(wireless_data.rx_over == 1)
		{
			printf("%s\r\n",wireless_data.rx_buff);
			memset(wireless_data.rx_buff,0,64);
			wireless_data.rx_over = 0;
			wireless_data.rx_count = 0;
			
		}
	}

}
