#ifndef __SPI_H_
#define __SPI_H_

#include "stm32f10x.h"

void SPI2_Init(void);
u8 SPI2_Send_Recv(u8 data);

#endif


