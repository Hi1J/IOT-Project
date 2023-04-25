#include "new.h"



YJ_Link *YJ_Link1;
YJ_Link *YJ_Link2;
YJ_Link *YJ_Link3;
YJ_Link *P_RX;
//YJ_Link* P_RX = (struct YJ_Link*)malloc(sizeof(YJ_Link));


void YJ_Link_Init(void)//链表初始化
{
	P_RX = (YJ_Link*)malloc(sizeof(YJ_Link));//动态分配空间
	YJ_Link1 = (YJ_Link*)malloc(sizeof(YJ_Link));
	YJ_Link2 = (YJ_Link*)malloc(sizeof(YJ_Link));
	YJ_Link3 = (YJ_Link*)malloc(sizeof(YJ_Link));
	P_RX->next = YJ_Link1;
	YJ_Link1->next = YJ_Link2;
	YJ_Link2->next = YJ_Link3;
	YJ_Link3->next = NULL;
}


uint8_t YJ_RX_Buf1[8];
uint8_t YJ_RX_Buf2[8];





