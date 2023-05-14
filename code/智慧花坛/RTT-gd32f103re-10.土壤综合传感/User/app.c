#define DBG_TAG "app"
#define DBG_LVL DBG_LOG



#include "app.h"

//任务1
struct rt_thread th1;
rt_uint8_t th1_stack[512] = {0};

//任务2
struct rt_thread th2;
rt_uint8_t th2_stack[512] = {0};






//任务1函数
void th1_entry(void *parameter)
{
	while(1)
	{
		rt_thread_mdelay(3000);//5s
		
	}
	
}
//任务2函数
void th2_entry(void *parameter)
{
	
	int result = 0;
	
	while(1)
	{
		
		rt_sem_take(modbus_respond,RT_WAITING_FOREVER);
		
//		rt_kprintf("receive data:\n");
//		for(uint8_t i = 0; i < modbus_rxdata_len; i ++ )
//			rt_kprintf("%x ",modbus_rx_buf[i]);
//		rt_kprintf("\n");
		
		result = MODBUS_Handle_Reply(Slave_address);
		
		switch(result)
		{
			case modbus_reply_h03:
				rt_kprintf("receive successed..\n");
				rt_kprintf("Parsed data:\n");
				rt_kprintf("%d %d\n",DataBuff[0],DataBuff[1]);
				break;
			case modbus_reply_h06:
				rt_kprintf("Change successed..\n");
				break;
			case modbus_reply_noreg:
				rt_kprintf("no this reg!\n");
				break;
			case modbus_reply_cfailed:
				rt_kprintf("reg read only or no this reg\n");
				break;
			case modbus_reply_data_err:
				rt_kprintf("data error\n");
				break;
			case modbus_reply_err:
				rt_kprintf("error\n");
				break;
			default:
				break;
		}
		
		
		modbus_rxdata_len = 0;
		
		rt_memset(modbus_rx_buf,0,modbus_rx_max_len);
		
		rt_thread_mdelay(5);
	}
	
}

