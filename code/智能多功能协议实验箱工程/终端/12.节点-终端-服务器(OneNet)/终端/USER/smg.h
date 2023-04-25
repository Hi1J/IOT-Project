#ifndef _SMG_H_
#define _SMG_H_

#include "stm32f10x.h"


extern uint8_t smgcodebuff[8];
extern char smgshowbuff[17];


void Smg_Config(void);
void Smg_SlectSegment(uint8_t num);
void Smg_SlectPosition(uint8_t num);

void Digitron_Display(uint8_t * buff, uint8_t len);
void Seg_Tran(char *pucSeg_Buf, uint8_t *pucSeg_Code);
#endif

