#ifndef __MENU_H
#define __MENU_H 	


#include "oled0561.h"
#include "rtc.h"
#include "TM1640.h"
#include "encoder.h"
#include "touch_key.h"
#include "NVIC.h"
#include "delay.h"
#include "lm75a.h"
#include "TimeSetting.h"

void Main_MENU(u8 *b,u8 *c);
void Auxiliary_MENU(u8 *b,u8 *c);

#endif
