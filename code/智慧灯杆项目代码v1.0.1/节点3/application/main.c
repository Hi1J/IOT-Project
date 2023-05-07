#include "gd32f10x.h"
#include "rtthread.h"
#include "app.h"

#define DBG_TAG "main"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

#include "drv_pin.h"
#define PB10_m GET_PIN(B,10)

rt_thread_t apptaskstart_ptr = RT_NULL;
void AppTaskStart(void* paremeter);

void AppTaskStart(void* paremeter)
{
    rt_err_t ret_app = 0;
	rt_err_t ret1 = 0;
	
    rt_kprintf("Start creating and starting tasks ...\n");
	
	//串口1设备初始化
	u1_dev = rt_device_find("usart1");
	if(u1_dev == RT_NULL)
		LOG_E("rt_device_find[usart1] failed..\n");
	else
	{
		LOG_D("rt_device_find[usart1] successed..\n");
		ret1 = rt_device_init(u1_dev);
		if(ret1 < 0)
			LOG_E("rt_device_init[usart1] failed..\n");
		else
			LOG_D("rt_device_init[usart1] successed..\n");
		ret1 = rt_device_open(u1_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret1 < 0)
			LOG_E("rt_device_open[usart1] failed..\n");
		else
			LOG_D("rt_device_open[usart1] successed..\n");		
	}
	
	//A72设备初始化
	A72_dev = rt_device_find("A72");
	if(A72_dev == RT_NULL)
		LOG_E("rt_device_find[A72] failed..\n");
	else
	{
		LOG_D("rt_device_find[A72] successed..\n");
		ret1 = rt_device_init(A72_dev);
		if(ret1 < 0)
			LOG_E("rt_device_init[A72] failed..\n");
		else
			LOG_D("rt_device_init[A72] successed..\n");
		ret1 = rt_device_open(A72_dev,RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
		if(ret1 < 0)
			LOG_E("rt_device_open[A72] failed..\n");
		else
			LOG_D("rt_device_open[A72] successed..\n");		
	}
	
	//动态信号量创建
	A72_Respond = rt_sem_create("A72_Respond",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Respond == RT_NULL)
		LOG_E("dynamic A72_Respond rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Respond rt_sem_create successed..\n");	
	
	A72_Connect = rt_sem_create("A72_Connect",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Connect == RT_NULL)
		LOG_E("dynamic A72_Connect rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Connect rt_sem_create successed..\n");
	
	A72_Data_handle = rt_sem_create("A72_Data_handle",0,RT_IPC_FLAG_FIFO);//按照先进先出方式获取资源
	if(A72_Data_handle == RT_NULL)
		LOG_E("dynamic A72_Data_handle rt_sem_create failed..\n");
	else
		LOG_D("dynamic A72_Data_handle rt_sem_create successed..\n");
	
	
    //静态创建任务2
    ret_app = rt_thread_init(&th2, "th2", WLAN_INIT, NULL, th2_stack, sizeof(th2_stack), th2_test_priority, 5);
    if(ret_app == RT_EOK){
        rt_thread_startup(&th2);
        rt_kprintf("th2 initialized successfully\n");
    }else{
        LOG_E("th2 initialized failed\n");
    }
    //静态创建任务3
    ret_app = rt_thread_init(&th3, "th3", Zigbee_Network_Status_Cheak_front, NULL, th3_stack, sizeof(th3_stack), th3_test_priority, 5);
        if(ret_app == RT_EOK){
            rt_kprintf("th3 initialized successfully\n");
        }else{
            LOG_E("th3 initialized failed\n");
        }
		
	//静态创建任务4
    ret_app = rt_thread_init(&th4, "Zigbee_Network_Status_Cheak_after", Zigbee_Network_Status_Cheak_after, NULL, th4_stack, sizeof(th4_stack), th4_test_priority, 5);
        if(ret_app == RT_EOK){
            rt_kprintf("th4 initialized successfully\n");
        }else{
            LOG_E("th4 initialized failed\n");
        }
		
	//静态创建任务5
    ret_app = rt_thread_init(&th5, "Zigbee_Send_Data", Zigbee_Send_Data, NULL, th5_stack, sizeof(th5_stack), th5_test_priority, 5);
        if(ret_app == RT_EOK){
            rt_kprintf("th5 initialized successfully\n");
        }else{
            LOG_E("th5 initialized failed\n");
        }
		
	//静态创建任务6
    ret_app = rt_thread_init(&th6, "Zigbee_Data_handle", Zigbee_Data_handle, NULL, th6_stack, sizeof(th6_stack), th6_test_priority, 5);
        if(ret_app == RT_EOK){
            rt_kprintf("th6 initialized successfully\n");
        }else{
            LOG_E("th6 initialized failed\n");
        }
		
	//静态创建任务7
    ret_app = rt_thread_init(&th7,"th7",th7_test, NULL, th7_stack, sizeof(th7_stack), th7_test_priority, 5);
		if(ret_app == RT_EOK){
        rt_thread_startup(&th7);
        rt_kprintf("th7 initialized successfully\n");
    }else{
        LOG_E("th7 initialized failed\n");
    }
		
		ret_app = rt_thread_init(&th8,"th8",th8_test, NULL, th8_stack, sizeof(th8_stack), th8_test_priority, 5);
		if(ret_app == RT_EOK){
        rt_thread_startup(&th8);
        rt_kprintf("th8 initialized successfully\n");
    }else{
        LOG_E("th8 initialized failed\n");
    }
	
	
	

//		rt_pin_mode(PB10_m,GPIO_MODE_OUT_PP);
//		
//		rt_pin_write(PB10_m,PIN_HIGH);
}

#include "ledblink.h"
#define LED0_PIN    GET_PIN(C, 13)

rt_device_t dev;
//static	uint8_t temp1[2];

int main(){
	//rt_err_t ret;

	apptaskstart_ptr = rt_thread_create("AppTaskStart", AppTaskStart, NULL, 512, 18, 5);
    rt_thread_startup(apptaskstart_ptr);

//    led_add_device(LED0_PIN);
//	
//	dev = rt_device_find("led");
//	if(dev == RT_NULL){
//		LOG_E("rt_device_find[led] failed...\n");
//		return -RT_EINVAL;
//	}
	
	
    while (1)
    {
		rt_thread_mdelay(5000);
    }

}
