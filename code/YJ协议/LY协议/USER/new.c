#include "new.h"

uint8_t YJ_RX_Aframe_OK = 0;//接收一帧数据完成标志位（也作为接收完成标志位） 在处理完接收到的数据后需手动清零 否则会出错
static uint8_t Multipe_data_frames_Flag = 1;//一次性传来的数据帧个数标志位 默认为1
static uint8_t YJ_TASK_FALG = YJ_OFF;//处理函数是否在运行标志位

struct Yj_node Yjnode1 = {{0},NULL};//数组1
struct Yj_node Yjnode2 = {{0},NULL};//数组2
struct Yj_node Yjnode3 = {{0},NULL};//数组3


//链表初始化
void YJ_Init(void)
{

	Yjnode1.next = &Yjnode2;
	Yjnode2.next = &Yjnode3;
	
}

//链表测试
void YJ_Tset(void)
{
	struct Yj_node *p = &Yjnode1;
	while(p != NULL)
	{
		printf("%s\r\n",p->YJ_RX_Buf);
		p = p->next;
	}

}
/*
函数功能：单片机毫秒级软延时
参数：延迟毫秒数
返回值：void
备注：此函数非必须使用 
*/
void YJ_Delay_Ms(uint16_t time)
{
	uint8_t MHz = System_Frequency_MHz;
	time = time * 100;
	while(time--)
	{
		while(MHz--)
		{
			/*什么也不做！*/
		}
		MHz = System_Frequency_MHz;
	}
}

/*
函数功能：数据处理函数开始执行
参数：void
返回值：void
备注：数据处理函数 开始执行时将 数据处理函数状态标志位 置0
*/
void YJ_TASK_START(void)
{
	YJ_TASK_FALG = YJ_ON;
}

/*
函数功能：数据处理函数结束执行
参数：void
返回值：void
备注：数据处理函数 结束执行时将 数据处理函数状态标志位 置0
*/
void YJ_TASK_END(void)
{
	YJ_TASK_FALG = YJ_OFF;
}

/*
函数功能：查看数据处理函数是否在进行
参数：void
返回值：YJ_TASK_FALG
备注：查看当前是系统是否有在处理之前接收的数据
*/
uint8_t YJ_TASK_STATUS(void)
{
	return YJ_TASK_FALG;
}


/*
函数功能：用于串口接收数据
参数：data —— 取自串口的DR寄存器（stm32平台）  其他平台取相关寄存器的数据
返回值：void
备注：此函数基于单片机的串口接收中断 一次只处理一个字节
     用户若需要用到串口空闲中断可自行根据需求来写
*/
void YJ_Data_reception(uint8_t data)
{
	static uint8_t YJ_Rx_Count= 1;//计数值 满8置1
	static uint8_t YJ_Rx_head = 0;//起始位接收缓存区
	static uint8_t YJ_Rx_flag = 0;//开始接收标志位
	static struct Yj_node *P_RX = &Yjnode1; //默认将数据放于buf1
	
	//如果第一个字节是开始位
	 if((data == Start_bit) && (YJ_Rx_flag == 0))YJ_Rx_head = data;//将该字节保存至起始位缓存区 
	 else if((data == Address) && (YJ_Rx_head == Start_bit) && (YJ_Rx_flag == 0))//如果第二个字节是地址位 
	 {
			YJ_Rx_flag = 1;//接收标志位置1 开始接收
			if(((YJ_RX_Aframe_OK == 1)|| (YJ_TASK_STATUS() == YJ_ON)) && (P_RX->next != NULL)){P_RX = P_RX->next;Multipe_data_frames_Flag++;}//如果是连续的数据帧或者当前有任务在处理并且还有足够的储存空间 则将数据保存到下一个buf 并且数据帧个数标志位加1
			else P_RX = &Yjnode1;//否则指向buf1
			P_RX->YJ_RX_Buf[0] = YJ_Rx_head;//将起始位放置buf[0]里面
			YJ_Rx_head = 0;//起始位接收缓存区置0
	 }
	 else  //如果第二个字节不是地址位 则数据帧错误或不是传来本机的
		 YJ_Rx_head = 0;//起始位接收缓存区置0

	//如果接收标志位为1 则开始接收
	 if(YJ_Rx_flag == 1)
	 {
		P_RX->YJ_RX_Buf[YJ_Rx_Count++] = data;//将数据放置BUF里面
		 //当放满一个buf时 默认一帧数据接收完成 各标志位复位 以接收下一帧数据
		 if(YJ_Rx_Count == Data_Frame_Max)
		 {
			 YJ_Rx_flag = 0;//接收开始位清0
			 YJ_Rx_Count = 1;//计数值复位
			 YJ_Rx_head = 0;//起始位缓存区复位 防止出错
			 YJ_RX_Aframe_OK = 1;//接收完一帧数据后 接收标志位置1
		 }
	 }
}

/*
函数功能：数据处理函数
参数：用户自定义（默认void）
返回值：用户自定义 （默认void）
备注：此函数可以作为单纯的数据解析函数 真正的数据处理可以在其他地方写（由用户自定义）
			此函数也可以作为数据处理函数
*/
void YJ_Data_Analysis(void)
{
		static struct Yj_node *P_handle = &Yjnode1; //默认处理buf1的数据
		
		YJ_Delay_Ms(1);//为防止出错 此处可放置1ms延时 具体延时时间可根据串口波特率来调 用哪种类型的延时函数根据用户具体情况使用
	//根据串口波特率115200、数据传输格式为1个起始位、1个停止位、8个数据位来算 1ms可接收11.5个字节 刚好比一帧数据接收的时间长 
		
		YJ_TASK_START();//数据处理函数开始
		
		while(Multipe_data_frames_Flag--)//处理次数
		{
			//************用户自定义区*****************//
			
			for(u8 i=0;i<Data_Frame_Max;i++)
				{
					printf("%x ",P_handle->YJ_RX_Buf[i]);
				}
			printf("\r\n");
			
			memset(P_handle->YJ_RX_Buf,0,Data_Frame_Max);//此函数放置用户定义区 用户可根据情况自行删减 （一般情况下会清空数组以防止出错）
			
			//************用户自定义区*****************//
			
			if(Multipe_data_frames_Flag != 0)//如果还有未处理的数据
				P_handle = P_handle->next;//则指向下一组数据
		}
		P_handle = &Yjnode1;//当处理完数据后 重新指向buf1 以便后续数据的处理
		Multipe_data_frames_Flag = 1;//数据帧个数标志位重新置1
		YJ_RX_Aframe_OK = 0;//接收一帧标志位清零
		
		YJ_TASK_END();//数据处理函数结束
}





