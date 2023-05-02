#include "drv_demo.h"

rt_device_t dev;

rt_err_t demo_init(rt_device_t dev)
{
	rt_kprintf("demo_init..\n");
	return 0;
}
rt_err_t demo_open(rt_device_t dev, rt_uint16_t oflag)
{
	rt_kprintf("demo_open..\n");
	return 0;
}
rt_err_t demo_close(rt_device_t dev)
{
	rt_kprintf("demo_close..\n");
	return 0;
}


int rt_demo_init(void)
{
	rt_device_t demo_dev;
	demo_dev = rt_device_create(RT_Device_Class_Char,32);
	if(demo_dev == RT_NULL)
	{
		LOG_E("rt_device_create failed..\n");
		return -RT_ENOMEM;
	}
	demo_dev->init = demo_init;
	demo_dev->open = demo_open;
	demo_dev->close = demo_close;
	
	rt_device_register(demo_dev,"demo",RT_DEVICE_FLAG_RDWR);//ע��  ��д
	
	return 0;
}

//INIT_DEVICE_EXPORT(rt_demo_init);



