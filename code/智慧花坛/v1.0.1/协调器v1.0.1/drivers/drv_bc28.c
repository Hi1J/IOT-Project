#include "drv_bc28.h"

rt_device_t BC28_dev;


//BC28Timer回调函数
void bc28_callback(void *parameter)
{
	if(BC28_RX_FLAG == BC28_RX_ON)
	{
		BC28_RX_FLAG = BC28_RX_OFF;
		BC28_RX_DATA_LEN = 0;
		rt_sem_release(BC28_Respond);
	}
}



//阿里云测试温度  格式 5位或4位 例如 +23.3 表示23.3
static void aliyun_tem(int argc,char**argv)
{
	
	if(argc != 2)
	{
		rt_kprintf("Please enter Please enter the correct format\n");
		rt_kprintf("For example +23.6 or -3.0(-40<tem<40)\n");
		return;
	}
	
	int len = strlen(argv[1]);
	
	if(len != 5 && len != 4)
	{
		rt_kprintf("error!\n");
		rt_kprintf("Please enter Please enter the correct format\n");
		rt_kprintf("For example +23.6 or -3.0 (-40<tem<40)\n");
		return;
	}
	
	rt_uint8_t sendbuf[5] = {0};
	
	rt_uint8_t i = 0;
	
	for(i = 0;i < len;i ++)
	{
		sendbuf[i] = argv[1][i];
	}
	
	if((sendbuf[0] != '+' && sendbuf[0] != '-') || sendbuf[len - 2] != '.')
	{
		rt_kprintf("error!\n");
		rt_kprintf("Please enter Please enter the correct format\n");
		rt_kprintf("For example +23.6 or -03.0 (-40<tem<40)\n");
		return;
	}
	
	rt_uint8_t value[3];
	
	if(len == 4)
	{
		value[0] = '0';
		value[1] = sendbuf[1];
		value[2] = sendbuf[3];
	}
	else
	{
		value[0] = sendbuf[1];
		value[1] = sendbuf[2];
		value[2] = sendbuf[4];
	}
	
	
	rt_uint8_t number;
	
	for(i = 0;i < 3;i++)
	{
		number = value[i] - '0';
		if(number > 9)
		{
			rt_kprintf("error!\n");
			rt_kprintf("Please enter Please enter the correct format\n");
			rt_kprintf("For example +23.6 or -3.0 (-40<tem<40)\n");
			return;
		}
	}
	
	rt_uint8_t tem_h = (value[0] - '0') * 10 + (value[1] - '0');
	
	if(tem_h > 40)
	{
		rt_kprintf("error!\n");
		rt_kprintf("(-40<tem<40)\n");
		return;
	}
	
	
	
	float tem = (float)tem_h + (float)(value[2] - '0') / 10.0;
	
	if(sendbuf[0] == '-')
		tem = tem * (-1);
	
//	Debug_printf("tem:%.1f C ",tem);
//	rt_kprintf("\n");
	
	char tembuf[10];
	
	char buf[200];
	
	sprintf(tembuf,"%.2f", tem);//写入温度
	
	sprintf(buf,"{params:{%s:%s}}",  Param_Post1, tembuf);
	
	
	if(!(MQTT_PUB(buf)))
	{
		rt_kprintf("send data failed..\n");
		return;
	}
	else
	{
		rt_kprintf("send data successed..\n");
	}
	
}


/*BC28设备初始化*/
int rt_BC28_init(void)
{
	rt_device_t bc28_dev;
	bc28_dev = rt_device_create(RT_Device_Class_Block,256);
	if(bc28_dev == RT_NULL)
	{
		LOG_E("bc28_dev rt_device_create failed..\n");
		return -RT_ENOMEM;
	}	
	rt_device_register(bc28_dev,"BC28",RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX);//注册  读写
	return 0;
}

INIT_DEVICE_EXPORT(rt_BC28_init);

MSH_CMD_EXPORT(aliyun_tem,For example +23.6 || -3.0  -40<tem<40);

