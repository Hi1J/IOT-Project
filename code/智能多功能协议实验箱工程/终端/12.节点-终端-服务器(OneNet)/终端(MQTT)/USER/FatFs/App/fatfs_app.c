#include "fatfs_app.h"

//文件类型列表
const u8 *File_type_table[6][13]=
{
    {"BIN","HEX"},		 
    {"LRC"},			     
    {"NES"},			     
    {"TXT","C","H"},	 
    {"MP1","MP2","MP3","MP4","M4A","3GP","3G2","OGG","ACC","WMA","WAV","MID","FLAC"},//????
    {"BMP","JPG","JPEG","GIF"},
};


#define printstrCom(str) printf("%s\r\n",str)



u8 char_upper(u8 c)
{
    if(c<'A')return c;//??,????.
    if(c>='a')return c-0x20;//????.
    else return c;//??,????
}


u8 mf_getfree(u8 *drv,u32 *total,u32 *free)
{
    FATFS *fs1;
    u8 res;
    u32 fre_clust=0, fre_sect=0, tot_sect=0;
    res = f_getfree((const TCHAR*)drv, (DWORD *)&fre_clust, &fs1);
    if(res==0)
    {
        tot_sect=(fs1->n_fatent-2)*fs1->csize;	
        fre_sect=fre_clust*fs1->csize;			
#if FF_MAX_SS != FF_MIN_SS				  				
        tot_sect*=fs1->ssize/512;
        fre_sect*=fs1->ssize/512;
#endif
        *total=tot_sect>>1;	
        *free=fre_sect>>1;	
    }
    return res;
}


u8 f_typetell(u8 *fname)
{
    u8 tbuf[6];
    u8 i=0,j;
	 
	  while((*fname++ != '\0') && i<250)i++;
		if(i==250)return 0XFF;    
	   
	  i=0;
	  while(*fname !='.')
		{
		  fname--;
			if(++i>6)return 0XFF;
		}
	
		strcpy((char *)tbuf,(const char*)(fname+1));
		for(i=0; i<strlen((const char*)tbuf); i++)tbuf[i]=char_upper(tbuf[i]); 
    
    for(i=0; i<6; i++)
    {
        for(j=0; j<13; j++)
        {
            if(*File_type_table[i][j]==0)break;
            if(strcmp((const char *)File_type_table[i][j],(const char *)tbuf)==0)
            {
                return (i<<4)|j;
            }
        }
    }
    return 0XFF;
}


//FRESULT f_scanfiles (char* path, char *strbuf,u8 mode)/* Start node to be scanned (also used as work area) */
//{
//    FRESULT res;
//    FILINFO fno;
//    DIR dir;
//    int i;
//	  int fnum=0;
//    char *fn; /* This function is assuming non-Unicode cfg. */
//    char *buf;
//    buf = malloc(64);
//    strcpy(buf,path);

//    res = f_opendir(&dir, buf); /* Open the directory */
//    if (res == FR_OK)
//    {
//        i = strlen(buf);
//        while(1)
//        {
//            res = f_readdir(&dir, &fno); /* Read a directory item */
//            if (res != FR_OK || fno.fname[0] == 0) break; /* Break on error or end of dir */
//            if (fno.fname[0] == '.') continue; /* Ignore dot entry */
//            fn = fno.fname;
//            if (fno.fattrib & AM_DIR) 
//						{ /* It is a directory */
//                sprintf(&buf[i], "/%s", fn);
//                res = f_scanfiles(buf,strbuf,mode);
//                if (res != FR_OK) break;
//                buf[i] = 0;
//            }
//            else
//            {   /* It is a file. */
//							if(!mode)
//						    sprintf(strbuf+strlen(strbuf),"%s/%s\r\n",buf,fn);//????+???
//							else 
//								sprintf(strbuf+strlen(strbuf),"%u.%s\r\n",++fnum,fn);//????+???
//            }
//        }
//    }
//    return res;
//}


/*
path:路径
ftpye：类型变化
*/
u16 f_getflietnum(char *path,u8 ftpye)
{
    u8 res;
    u16 rval=0;
    DIR tdir;	 		
    FILINFO tfileinfo;	
    res=f_opendir(&tdir,(const TCHAR*)path); 					
    if(res==FR_OK)
    {
        while(1)
        {
            res=f_readdir(&tdir,&tfileinfo);       		
            if(res != FR_OK || tfileinfo.fname[0] == 0)break;	
            res=f_typetell((u8*)tfileinfo.fname);
            if(res == ftpye)
            {
                rval++;
            }
        }
    }
    return rval;
}

FATFS myfs;
void SD_Fatfs_Init()
{
  __IO u32 total,free;
	__IO u8 filenum = 0;
	char dispbuf[100];
	if(FR_OK !=SD_Initialize()) 
	{
	   printstrCom((u8*)"sd init err!!!!!!!!!!!!!!!!!!!");
		 
		 return ;
	}
	 printstrCom((u8*)"sd init ok!");
   if(FR_OK != f_mount(&myfs, "0:", 1)) 
	 {
		  	 
      printstrCom((u8*)"sd mount err!!!!!!!!!!!!!!!!!!!");
		  return ;
	 }
	 mf_getfree("/",&total,&free);
	 filenum = f_getflietnum("/",0x42);
	 memset(dispbuf,0,sizeof(dispbuf));
   sprintf(dispbuf,"sd mount ok! disk total size:%f M,free size:%f M. %d MP3 file founded!",total / 1024.,free / 1024.,filenum)	; 
   printstrCom((u8*)dispbuf); 
}
void Fatfs_TestMy(void)
{
	FATFS myfs1;
	FIL fp;
	u32 bw;
	if(FR_OK !=SD_Initialize())//初始化SD卡
	{
		printstrCom("sd init err !!!\r\n");
		return ;
	}
	if(f_mount(&myfs1,"0:",1) != FR_OK)
	{
		printstrCom("fatfs mount err !!!\r\n");
		return ;
	}
	if(f_open(&fp,"new.txt",FA_WRITE|FA_READ|FA_OPEN_ALWAYS)!= FR_OK)
	{
		printstrCom("文件打开失败\r\n");
		return ;
	}
	printstrCom("文件打开成功\r\n");

	if(f_write(&fp,"我爱中国",8,&bw) != FR_OK)
	{
		printstrCom("文件写入失败\r\n");
		return ;
	}
	printstrCom("文件写入成功\r\n");
	f_lseek(&fp,0);//回到文件首
	char buff[32];
	if(f_read(&fp,buff,8,&bw) != FR_OK)
	{
		printstrCom("文件读取失败\r\n");
		return ;
	}
	printstrCom("文件读取成功\r\n");
	buff[8]='\0';
	printf("%s\r\n",buff);
	f_close(&fp);
}



