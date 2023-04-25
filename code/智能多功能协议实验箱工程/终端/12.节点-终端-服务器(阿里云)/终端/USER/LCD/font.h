#ifndef __FONT_H
#define __FONT_H 	

#include "stm32f10x.h"

struct _FONT
{
	uint16_t width;//宽度
	uint16_t high; //高度
	union 
	{
		uint8_t *zfaddr;//保存zf模集合的首地址
    char *hzaddr;  //保存汉字文件的目录和名字
	}hz_zf_addr;
};

extern struct _FONT font_zf816;
extern struct _FONT font_zf1624;
extern struct _FONT font_hz1616;

extern const uint8_t zf_8_16[][16];
extern const uint8_t zf_16_24[][48];
uint8_t Font_ZfFind(char ch);
#endif
