#include "people_scanf.h"

#define DBG_TAG "people_scanf"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

rt_uint8_t device_state = DeviceOpen;

/* people_scanf初始化函数 */
rt_err_t people_scanf_init(rt_device_t dev)
{
	rt_people_init();
	return RT_EOK;
}

/* 打开people_scanf设备函数 */
rt_err_t people_scanf_open(rt_device_t dev, rt_uint16_t oflag)
{
	if(device_state == DeviceClose)
	{
		device_state = DeviceOpen;
		LOG_D("people_scanf_open success...\n");
	}
	else if(device_state == DeviceOpen) 
	{
		LOG_D("people_scanf_open has been opened");
	}
	
	return RT_EOK;
}

/* 关闭people_scanf设备函数 */
rt_err_t people_scanf_close(rt_device_t dev)
{
		if(device_state == DeviceOpen)
	{
		device_state = DeviceClose;
		LOG_D("people_scanf_close success...\n");
	}
	else if(device_state == DeviceClose) 
	{
		LOG_D("people_scanf_open has been closed");
	}
	
	return RT_EOK;
}

/* 读取people_scanf设备状态函数 */
rt_size_t people_scanf_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
	if(device_state == DeviceOpen)
	{
		*(rt_uint8_t *)buffer = People_scanf();
	
	}
	else
	{
		LOG_D("failled!!!people_scanf_open is closed\n");
	
	}

	return RT_EOK;
}

///* 改变people_scanf设备状态函数 */
//rt_size_t people_scanf_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
//{
//	
//	return RT_EOK;
//}

/* people_scanf设备控制函数 */
//rt_err_t people_scanf_control(rt_device_t dev, int cmd, void *args)
//{

//		return RT_EOK;
//	}
	



int rt_people_scanf_init(void)
{
	rt_err_t err;
	rt_device_t people_scanf_dev;
	
	people_scanf_dev = rt_device_create(RT_Device_Class_Char, 32);
	if(people_scanf_dev == RT_NULL){
		LOG_E("rt_device_create[people_scanf] failed...\n");
		return -RT_ENOMEM;
	}
	
	people_scanf_dev->init = people_scanf_init;
	people_scanf_dev->open = people_scanf_open;
	people_scanf_dev->close = people_scanf_close;
	people_scanf_dev->read = people_scanf_read;


	
	err = rt_device_register(people_scanf_dev, "people_scanf", RT_DEVICE_FLAG_RDWR);
	if(err != RT_EOK){
		LOG_E("rt_device_register[people_scanf] failed...\n");
		return -RT_ERROR;
	}
	
	
	return RT_EOK;
}

void scanf_close(void)
{
	device_state = DeviceClose;

}

void scanf_open(void)
{
	device_state = DeviceOpen;

}

void read_scanf(void)
{
	rt_uint8_t Date = People_scanf();
	if(Date == 1)
	{
		rt_kprintf("Pedestrian detected !!!");
	
	}
	else
	{
		rt_kprintf("No Pedestrian detected !!!");
	}


}

INIT_DEVICE_EXPORT(rt_people_scanf_init)
MSH_CMD_EXPORT(scanf_open,ps:open the people_scanf)
MSH_CMD_EXPORT(scanf_close,ps:close the people_scanf)
MSH_CMD_EXPORT(read_scanf,ps:read the result of people_scanf)

