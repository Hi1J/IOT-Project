#include "jpg_app.h"
static int MaskL = 0; 
static int MaskR = LCD_WIDTH - 1; 
static int MaskT = 0; 
static int MaskB = LCD_HEIGHT - 1;
u16 jpg_start_x=0,jpg_start_y=0;
/*********************************************************************************************
  函数名称:    STM32_Display
  函数功能:    在TFTLCD屏幕上显示图片 
  入口参数:    见函数头
  出口参数:    无
  全局变量:    无
  备注说明:    无
*********************************************************************************************/
void Lcd_Display_Rect( 
                            int left,                            /*图片左方起始点，即一行的起始点 */ 
                            int right,                           /*图片右方的结束点，即一行的结束点*/ 
                            int top,                             /* 图片上方的起始点，即一列的起始点 */ 
                            int bottom,                          /*图像下方的结束点，即一列的结束点 */ 
                            const uint16_t * RGB_Data_Pointer    /* 待显示的图像数据，RGB格式*/ 
                        ) 
{ 
	  u32 total_pixelpoint = 0;//统计显示图片一共的像素点的个数
    int hight_jpg,width_jpg; //记录显示图片的高度和宽度
	  left += jpg_start_x;     //修改x轴显示位置
	  right += jpg_start_x;
	  top += jpg_start_y;      //修改y轴显示位置
	  bottom += jpg_start_y;
    if (left > right || top > bottom)  
    { 
        return;             // Check varidity 
    } 
    
    if (left > MaskR || right < MaskL  || top > MaskB || bottom < MaskT)  
    { 
        return;                // Check if in active area  
    }
		hight_jpg = bottom - top + 1 ;           /* Vertical size *///计算图像的高度
    width_jpg = right - left + 1;            /* Horizontal size and skip *///计算图像的宽度
		LCD_Address_Set(left,top,right,bottom);  //设置显示区域
		total_pixelpoint = hight_jpg*width_jpg;  //计算显示区域中像素点的个数
		for(u32 i=0;i<total_pixelpoint;i++)      //开始往屏幕上显示数据
		{	
				LCD_WR_DATA(*RGB_Data_Pointer++);	 			 
		}		
} 
/* 
User defined input funciton                          
jd        //储存待解码的对象信息的结构体  
buff,     //输入数据缓冲区 (NULL:删除数据)
nbyte     //需要从输入数据流读出/删除的数据量
*/

uint16_t in_func (JDEC* jd, uint8_t* buff, uint16_t nbyte)
{
	UINT  rb; 
	FIL *dev = (FIL *)jd->device;   /* Device identifier for the session (5th argument of jd_prepare function) */

	if (buff) {
			/* Read bytes from input stream */
		  f_read(dev, buff, nbyte, &rb); //调用FATFS的f_read函数，用于把jpeg文件的数据读取出来
			return rb;                     // 返回读取到的字节数目 
	} else {
			/* Remove bytes from input stream */
			return (f_lseek(dev, f_tell(dev) + nbyte) == FR_OK) ? nbyte : 0;/* 重新定位数据点，相当于删除之前的n字节数据 */ 
	}
}

/*------------------------------*/
/* User defined output funciton */
/*------------------------------*/

uint16_t out_func (JDEC* jd, void* bitmap, JRECT* rect)
{
		jd = jd;    /* 说明：输出函数中JDEC结构体没有用到 */ 
    //显示一块区域到LCD上
		Lcd_Display_Rect(rect->left, rect->right, rect->top, rect->bottom, (uint16_t*)bitmap);
    return 1;    /* Continue to decompress */ /*返回1，使解码工作继续执行 */ 
}

//加载并显示jpg文件
void load_jpg (
    FIL *fp,        /* 指向打开的文件执政 */
    void *work,     /*指向四字节对其的工作区缓存 */
    UINT sz_work    /*工作区的大小 */
)
{
    JDEC jd;        /* Decoder object (124 bytes) */
    JRESULT rc;
    BYTE scale;
    /* Prepare to decompress the file */
    rc = jd_prepare(&jd, in_func, work, sz_work, fp);
    if (rc == JDR_OK) 
    {
			  printf("Image dimensions: %u by %u. %u bytes used.\r\n", jd.width, jd.height, 4096 - jd.sz_pool);
        /* 根据图片大小选选择一个刚刚好能够缩放的图片比例 */
        for (scale = 0; scale < 3; scale++) 
        {
						
            if ((jd.width >> scale) <= LCD_WIDTH && (jd.height >> scale) <= LCD_HEIGHT)break;
						
        }

        rc = jd_decomp(&jd, out_func, scale);    /* Start to decompress */

    } else 
    {
        //显示错误,将错误信息打印在屏幕上
        printf("jpg error %d\r\n",rc);
    }
}
#define SIZE_OF_FILE_BUFFER        4096
BYTE jpegbuffer[SIZE_OF_FILE_BUFFER]; //图像文件数据缓冲区
/*参数:指向文件名 */
void JPG_ShowFunc (const char *fn,u16 start_x,u16 start_y)
{
    FIL fil;            /* Pointer to a file object */
    
    if (f_open(&fil, fn, FA_READ) == FR_OK) 
    {
			jpg_start_x=start_x;//给显示图片设置起始的行地址和列地址
			jpg_start_y=start_y;
			load_jpg(&fil, (void *)jpegbuffer, SIZE_OF_FILE_BUFFER );  //打开jpg文件并解码显示
      f_close(&fil);
    }
    else
    {
        printf("open file failed\r\n");
    }
}


