/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "sd_driver.h"
#include <stdlib.h>
//#include "usbdisk.h"	/* Example: Header file of existing USB MSD control module */
//#include "atadrive.h"	/* Example: Header file of existing ATA harddisk control module */
//#include "sdcard.h"		/* Example: Header file of existing MMC/SDC contorl module */

/* Definitions of physical drive number for each drive */
#define SD    0
#define ATA		1	/* Example: Map ATA harddisk to physical drive 0 */
#define MMC		2	/* Example: Map MMC/SD card to physical drive 1 */
#define USB		3	/* Example: Map USB MSD to physical drive 2 */


/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	return 0;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	u8 result;
 
	switch (pdrv)
	{  
		case SD :	result = SD_Initialize();			 //SD¿¨³õÊ¼»¯
							break;
		case ATA :
							break;
		case MMC :
							break;
		case USB :
							break;
	}
	if(!result)return RES_OK;
	return STA_NOINIT;
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	u8 result;
	
	switch (pdrv)
	{
		case SD : result = SD_ReadDisk(buff,sector,count);		  //¶Á¿é		       
							break;
		case ATA :
							break;
		case MMC :
							break;
		case USB :
							break;
	}
	if(!result)return RES_OK;
	return RES_PARERR;
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	u8 result;
	switch (pdrv)
	{
		case SD : result = SD_WriteDisk((u8 *)buff,sector,count);		//Ð´¿é
							break;
		case ATA :
							break;
		case MMC :
							break;
		case USB :
							break;
	}
	if(!result)return RES_OK;
	return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	u8 result= RES_OK;
	switch (pdrv)
	{
		case SD : switch(cmd)
							{
								case CTRL_SYNC:
										SD_CS=0;
										if(SD_WaitReady())result = RES_ERROR;
										SD_CS=1;
										break;	 
								case GET_SECTOR_SIZE:
										*(WORD*)buff = 512;
										break;	 
								case GET_BLOCK_SIZE:
										*(WORD*)buff = 8;
										break;	 
								case GET_SECTOR_COUNT:
										*(DWORD*)buff = SD_GetSectorCount();
										break;
								default:
										result = RES_PARERR;
										break;
							}
							break;
		case ATA :
							break;
		case MMC :
							break;
		case USB :
							break;
	}
	if(!result)return RES_OK;

	return RES_PARERR;
}
#endif

//31-25: Year(0-127 org.1980), 24-21: Month(1-12), 20-16: Day(1-31) */                                                                                                                                                                                                                                          
//15-11: Hour(0-23), 10-5: Minute(0-59), 4-0: Second(0-29 *2) */   
DWORD get_fattime (void)
{
	 u32 date;
	 date = 
	 (
	   ((2015 - 1980) << 25)  |
	              (7 <<  21 ) |
	              (9 <<  16 ) |
	              (12 << 11 ) |
	              ( 4 << 5  ) |
	              ( 0 )
	 );
	
   return date;
}

void* ff_memalloc (UINT msize)		/* Allocate memory block */
{
    return malloc(msize);
}

void ff_memfree (void* mblock)		/* Free memory block */
{
     free(mblock);
}


