#include "stm32f10x.h"
#include "delay.h"
#include "smg.h"

//8段数码管SMG显示内容
int main(void)
{
	char data[8] = {'2','0','0','1','0','4','1','2'};
	u8 data_code[8] = {0};//设一个数组接收数组data对于的code码
	Smg_Config();//smg初始化
	
	Seg_Tran(data,data_code);//转换
	
	
	while(1)
	{
		Digitron_Display(data_code,8);//动态显示
		delay_ms(2);//1ms-2ms调用一次就可以正常显示
		
		
		
	}
	
	

}
