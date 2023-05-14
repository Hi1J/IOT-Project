#include "drv_sensor.h"


rt_device_t Sensor_dev;

//读ph
static void take_ph(void)
{
	Take_A_Data(0x0006);

}

//读湿度
static void take_hum(void)
{
	Take_A_Data(0x0012);
}
//读温度
static void take_tem(void)
{
	Take_A_Data(0x0013);
}

//读温湿度
static void take_hum_tum(void)
{
	MODBUS_HostToSlave_H03(Slave_address,0x0012,2);
}



//读任意寄存器（例：take_ 0013 则表示读0013寄存器）
static void take_(int argc,char**argv)
{
	if(argc < 2)
	{
		rt_kprintf("Please enter the starting address(4 bit) for example:0001\n");
		return;
	}
	
	int len = strlen(argv[1]);
	
	if(len != 4)
	{
		rt_kprintf("erron!\nPlease enter the starting address(4 bit) for example:0001\n");
		return;
	}
	
	
    rt_uint16_t numer[4] = {0};
	rt_uint16_t address = 0;
	for(rt_uint8_t i = 0;i < 4;i ++)
	{
		numer[i] = (rt_uint16_t)(argv[1][i] - '0');
		if(numer[i] <= 9)
		{
			address |= (numer[i] << (4 * (4 - 1 - i)));
		}	
		else if(numer[i] >= 49 && numer[i] <= 54)
		{
			numer[i] -= 39;
			address |= (numer[i] << (4 * (4 - 1 - i)));
		}
		else
		{
			rt_kprintf("erron!\nPlease enter the starting address(4 bit) for example:0001\n");
			return;
		}
		
		
	}
	
	
	//rt_kprintf("%x \n",address);
	Take_A_Data(address);
}
//修改单个寄存器的值（例：make_ 0001 0005 把0001寄存器的值修改为0x0005）
static void make_(int argc,char**argv)
{
	
	if(argc < 2)
	{
		rt_kprintf("Please enter address & data for example:0001 0007\n");
		return;
	}
	
	int len1 = strlen(argv[1]);
	int len2 = strlen(argv[2]);
	
	
	if((len1 != 4) || (len2 != 4))
	{
		rt_kprintf("erron:number\nPlease enter address & data for example:0001 0007\n");
		return;
	}
	
	
    rt_uint16_t numer[8] = {0};
	rt_uint16_t address = 0;
	rt_uint16_t data = 0;
	for(rt_uint8_t i = 0;i < 8;i ++)
	{
		if(i < 4)
			numer[i] = (rt_uint16_t)(argv[1][i] - '0');
		else
			numer[i] = (rt_uint16_t)(argv[2][i - 4] - '0');
		
		if(numer[i] <= 9)
		{
			if(i < 4)
				address |= (numer[i] << (4 * (4 - 1 - i)));
			else
				data |= (numer[i] << (4 * (8 - 1 - i)));
		}	
		else if(numer[i] >= 49 && numer[i] <= 54)
		{
			numer[i] -= 39;
			if(i < 4)
				address |= (numer[i] << (4 * (4 - 1 - i)));
			else
				data |= (numer[i] << (4 * (8 - 1 - i)));
		}
		else
		{
			rt_kprintf("erron:format\nPlease enter address & data for example:0001 0007\n");
			return;
		}
		
		
	}
	
	MODBUS_HostToSlave_H06(Slave_address,address,data);
	//rt_kprintf("%x %x \n",address,data);
	
}

int rt_sensor_init(void)
{
	rt_device_t sensor_dev;
	sensor_dev = rt_device_create(RT_Device_Class_Block,128);
	if(sensor_dev == RT_NULL)
	{
		LOG_E("sensor_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	rt_device_register(sensor_dev,"sensor",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_sensor_init);

MSH_CMD_EXPORT(take_ph,only take ph);
MSH_CMD_EXPORT(take_hum,only take hum);
MSH_CMD_EXPORT(take_tem,only take tem);
MSH_CMD_EXPORT(take_hum_tum,take tem&hum);
MSH_CMD_EXPORT(take_,Please enter the starting address 4 bit);
MSH_CMD_EXPORT(make_,Please enter address & data);
