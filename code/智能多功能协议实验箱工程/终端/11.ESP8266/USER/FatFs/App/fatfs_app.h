#ifndef __FATFS_APP_H
#define __FATFS_APP_H
#include "stm32f10x.h"                  // Device header
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "ff.h"
#include "sd_driver.h"
//////////////////////////////////////////////////////////////////////////////////

//f_typetell返回的类型定义
//根据表FILE_TYPE_TBL获得.
#define F_BIN		0X00	//bin文件
#define F_LRC		0X10	//lrc文件
#define F_NES		0X20	//nes文件
#define F_TEXT	0X30	//.txt文件
#define F_C			0X31	//.c文件
#define F_H			0X32    //.h文件
#define F_FLAC	0X4C	//flac文件
#define F_BMP		0X50	//bmp文件
#define F_JPG		0X51	//jpg文件
#define F_JPEG	0X52	//jpeg文件		 
#define F_GIF		0X53	//gif文件  
#define F_MUSIC 0X40  //音乐文件

void SD_Fatfs_Init(void);
extern u8 f_typetell(u8 *fname);						//识别文件类型
extern u8 mf_getfree(u8 *drv,u32 *total,u32 *free);	//得到磁盘总容量和剩余容量
extern u32 f_fdsize(u8 *fdname);						//得到文件夹大小
extern FRESULT f_scanfiles(char* path,char *strbuf,u8 mode);  //扫描文件夹里面的文件名称
extern u16 f_getflietnum(char *path,u8 ftpye);                //获取文件夹中文件总数量
extern u8 f_getfileindext(char *path,u16 *flieindextbl,u8 ftpye); //获取文件夹中文件索引
extern FRESULT f_getfile_spec_data(char *path,u8 *outbuf,u32 offset,u32 fsize);//读取文件的指定位置指定长度的数据

int GetGBKCode_from_sd(unsigned char* pBuffer,const unsigned char * c);


void Fatfs_TestMy(void);
#endif


