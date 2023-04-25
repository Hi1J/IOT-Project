#ifndef __NEW_H_
#define __NEW_H_

#include "stm32f10x.h"
#include "string.h"
#include "stdlib.h"

typedef struct Yj_link{
	u8 YJ_RX_Buf[8];
	struct Yj_link* next;
}YJ_Link;



extern YJ_Link *YJ_Link1;
extern YJ_Link *YJ_Link2;
extern YJ_Link *YJ_Link3;
extern YJ_Link *P_RX;


extern uint8_t YJ_RX_Buf1[8];
extern uint8_t YJ_RX_Buf2[8];

void YJ_Link_Init(void);






#endif