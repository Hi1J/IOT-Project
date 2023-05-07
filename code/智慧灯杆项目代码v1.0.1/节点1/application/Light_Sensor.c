#include "Light_Sensor.h"

#define DBG_TAG "Light_Sensor"
#define DBG_LVL DBG_LOG
#include <rtdbg.h>

rt_uint8_t Mode_Selet = Default_Mode; //传感器模式
/***************************控制命令****************************/

/************look_Date查看传感器数据*************/

void look_Date(void)
{
	rt_uint16_t Dates = Light_Read_Date(Mode_Selet);
	rt_kprintf("The current light intensity is %d\n",Dates);
	
}


/*************************检测模式更改*************************/	
static void Control_Mode(int argc, char**argv)
{
	
	if ( argc < 2 )
    {
        rt_kprintf( "Please input'Control_Mode <COUT_H/L_x|ONCE_H/L_x>'\n\n" );
        return;
    }
   
	if ( !rt_strcmp( argv[1], "COUT_H_1" ) )
    {
			  Mode_Selet = Cont_H_1_Mode;
		}
	else if ( !rt_strcmp( argv[1], "COUT_H_2" ) )
    {
			  Mode_Selet = Cont_H_2_Mode;
		}
	else if ( !rt_strcmp( argv[1], "COUT_L" ) )
    {
			  Mode_Selet = Cont_L_Mode;
		}
	else if ( !rt_strcmp( argv[1], "ONCE_H_1" ) )
    {
			  Mode_Selet = Once_H_1_Mode;
		}
	else if ( !rt_strcmp( argv[1], "ONCE_H_2" ) )
    {
			  Mode_Selet = Once_H_2_Mode;
		}
	else if ( !rt_strcmp( argv[1], "ONCE_L" ) )
	{
		Mode_Selet = Once_L_Mode;
	}
	Light_Serior(Mode_Selet);
	rt_kprintf("mode select flashing! %d\n",Mode_Selet);
}





/****************************************************************/


rt_err_t Light_Sensor_init(rt_device_t dev)
{

	Light_Serior(Mode_Selet);
	return RT_EOK;
}



rt_err_t Light_Sensor_open(rt_device_t dev, rt_uint16_t oflag)
{
	rt_kprintf("coming soon ....");
	return RT_EOK;
}

//rt_err_t Light_Sensor_close(rt_device_t dev)
//{
//	rt_kprintf("coming soon ....");
//	return RT_EOK;
//}

rt_size_t Light_Sensor_read(rt_device_t dev, rt_off_t pos, void *buffer, rt_size_t size)
{
	
	*(rt_uint16_t *)buffer = Light_Read_Date(Mode_Selet);
	
	return RT_EOK;
}



/************************************************************************
*								    	设备写入，buffer储存要写入的模式	  								*
*************************************************************************/

rt_size_t Light_Sensor_write(rt_device_t dev, rt_off_t pos, const void *buffer, rt_size_t size)
{
	Mode_Selet = *(rt_uint8_t *)buffer;
	Light_Serior(Mode_Selet);
	return RT_EOK;
}

//rt_err_t Light_Sensor_control(rt_device_t dev, int cmd, void *args)
//{
//	rt_kprintf("coming soon ....");
//	return RT_EOK;
//}

int rt_Light_Sensor_init(void)
{

	rt_err_t err;
	rt_device_t Light_Sensor_dev;
	
	Light_Sensor_dev = rt_device_create(RT_Device_Class_Char,8);
	
	if(Light_Sensor_dev == RT_NULL){
		LOG_E("rt_device_create[Light_Sensor_dev] failed...\n");
		return -RT_ENOMEM;
	}
	Light_Sensor_dev ->init = Light_Sensor_init;
	
	Light_Sensor_dev->open = Light_Sensor_open;
//	Light_Sensor_dev->close = Light_Sensor_close;
	Light_Sensor_dev->read = Light_Sensor_read;
	Light_Sensor_dev->write = Light_Sensor_write;

//	Light_Sensor_dev->control = Light_Sensor_control;
	
	err = rt_device_register(Light_Sensor_dev, "Light_Sensor", RT_DEVICE_FLAG_RDWR);
	if(err != RT_EOK){
		LOG_E("rt_device_register[Light_Sensor_dev] failed...\n");
		return -RT_ERROR;
	}
		
	return RT_EOK;
	

}

INIT_DEVICE_EXPORT(rt_Light_Sensor_init)


/***********************命令初始化**********************/
MSH_CMD_EXPORT(look_Date,Look This Date);	
MSH_CMD_EXPORT(Control_Mode, cmd sample:Control_Mode < COUT_H/L_x|ONCE_H/L_x );

