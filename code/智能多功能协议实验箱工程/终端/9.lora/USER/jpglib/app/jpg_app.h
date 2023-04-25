#ifndef _JPG_APP_H_
#define _JPG_APP_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "lcd.h"
#include "ff.h"
#include "tjpgd.h"


void load_jpg(FIL *fp,void *work,UINT sz_work);

void JPG_ShowFunc (const char *fn,u16 start_x,u16 start_y);


#endif
