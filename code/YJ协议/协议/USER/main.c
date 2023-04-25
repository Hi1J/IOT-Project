#include "stm32f10x.h"
#include "delay.h"
#include "usart.h"
#include "Zigbee.h"
#include "new.h"





//Zigbee
int main(void)
{

	Usart1_Init(115200);
	printf("串口1初始化成功\r\n");
	Zigbee_Init();
	YJ_Link_Init();//链表初始化
	
	while(1)
	{
		if(Zigbee_Rx_OK == 1)
		{
			if(YJ_Link1->YJ_RX_Buf[1] == Zigbee_Address)
			{
				printf("YJ_Link1->YJ_RX_Buf:");
				for(u8 i=0;i<8;i++)
				{
					printf("%x ",YJ_Link1->YJ_RX_Buf[i]);
				}
				printf("\r\n");
			}
			if(YJ_Link2->YJ_RX_Buf[1] == Zigbee_Address)
			{
				printf("YJ_Link2->YJ_RX_Buf:");
				for(u8 i=0;i<8;i++)
				{
					printf("%x ",YJ_Link2->YJ_RX_Buf[i]);
				}
				printf("\r\n");
			}

			Zigbee_Rx_OK = 0;
//			P_RX->next = YJ_Link1;
		}
		
			
		
	}

}
