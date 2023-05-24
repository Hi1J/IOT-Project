#include "sensor.h"


STRUCT_DEBUGTypeDef   debug_value;
STRUCT_ZIGBEETypeDef  zigbee_value;



/*
函数功能：获取传感器某个数据
参数：enumTYPE ： 类型    REG ：对应地址
返回值：true : 成功  false :失败
备注：
enumTYPE ：               REG:
PH：	   TYPE_PH            REG_PH    0x0006
湿度：     TYPE_HUM			  REG_HUM   0x0012
温度：	   TYPE_TEM           REG_TEM   0x0013
电导率：	   TYPE_EC            REG_EC    0x0015
地址：     TYPE_ADD           REG_ADD   0x0100
波特率：	   TYPE_BAUD          REG_BAUD  0x0101
		   TYPE_FREE          想查找的寄存器
		   
注：当enumTYPE为TYPE_FREE时，意为查找任意寄存器 这时候REG需输入任意寄存器对应地址
	而此时赋给_FREE的值为无符号16位的数
*/
bool Sensor_TakeAValue(ENUM_TYPETypeDef enumTYPE,unsigned short REG)
{
	
	MODBUS_HostToSlave_H03(Slave_address,REG,1);//modbus发送
	
	rt_hw_us_delay(60);
	
	RS485_CON_TX();
	
	if(rt_sem_take(modbus_respond,100) == 0)//最长等待时间100ms
	{
		
		rt_hw_us_delay(40);
		
		RS485_CON_RX();
		
		if(MODBUS_Handle_Reply(Slave_address) != modbus_reply_h03)//验证数据
		{
			modbus_rxdata_len = 0;//标志位清零
		
			rt_memset(modbus_rx_buf,0,modbus_rx_max_len);//清除数组
			
			return false;
		}
			
		
		modbus_rxdata_len = 0;//标志位清零
		
		rt_memset(modbus_rx_buf,0,modbus_rx_max_len);//清除数组
		
		switch(enumTYPE)//判断类型
		{
			case TYPE_PH:
				zigbee_value.PH = Modbus_DataBuff[0];
				debug_value._PH = (float)(Modbus_DataBuff[0]) / 100.0;
				return true;
			case TYPE_HUM:
				zigbee_value.HUM = Modbus_DataBuff[0];
				debug_value._HUM = (float)(Modbus_DataBuff[0]) / 10.0;
				return true;
			case TYPE_TEM:
				zigbee_value.TEM = Modbus_DataBuff[0];
				if(Modbus_DataBuff[0] & (1 << 15))//当温度低于零下时以补码的形式上传
				{
					debug_value._TEM = (float)(Modbus_DataBuff[0] - 0xFFFF) /10.0;//补码求原码
				}
				else
				{
					debug_value._TEM = (float)(Modbus_DataBuff[0]) / 10.0;
				}
				return true;
			case TYPE_EC:
				zigbee_value.EC = Modbus_DataBuff[0];
				debug_value._EC = Modbus_DataBuff[0];
				return true;
			case TYPE_ADD:
				debug_value._ADD = Modbus_DataBuff[0];
				return true;
			case TYPE_BAUD:
				if(Modbus_DataBuff[0] == 0)
					debug_value._BAUD = 2400;
				else if(Modbus_DataBuff[0] == 1)
					debug_value._BAUD = 4800;
				else if(Modbus_DataBuff[0] == 2)
					debug_value._BAUD = 9600;
				else
					debug_value._BAUD = 0;
				return true;
			case TYPE_FREE:
				debug_value._FREE = Modbus_DataBuff[0];
				return true;
			default:
				return false;

		}
		
	
	}
	else
		return false;
	
	
}
/*
函数功能：修改从机单个寄存器值
参数：  REG ：对应地址 value ：修改的值
返回值：int
备注：
返回值：
-999：                 传感器无回应
modbus_reply_h06：     06功能码修改寄存器成功
modbus_reply_cfailed： 06功能码修改失败 原因可能为:该寄存器只读或不存在的寄存器等
modbus_reply_data_err：数据错误
modbus_reply_err：     未知错误

*/
int Sensor_MakeAValue(unsigned short REG,unsigned short value)
{
	
	int result;
	
	MODBUS_HostToSlave_H06(Slave_address,REG,value);
	
	if((rt_sem_take(modbus_respond,100) == 0) || modbus_rx_falg == modbus_rx_ok)//最长等待时间100ms
	{
		
		result = MODBUS_Handle_Reply(Slave_address);
		
		modbus_rxdata_len = 0;//标志位清零
		
		rt_memset(modbus_rx_buf,0,modbus_rx_max_len);//清除数组
		
		return result;
		
	}
	else
		return -999;
	
}

/*
函数功能：修改从机波特率
参数：  baud 波特率（仅支持 2400  4800 9600）
返回值：true ： 修改成功  false ：修改失败
备注：
*/
bool Modify_Baud_Rate(unsigned int baud)
{
	
	unsigned short data;
	
	switch(baud)
	{
		case 2400:
			data = 0;
			break;
		case 4800:
			data = 1;
			break;
		case 9600:
			data = 2;
			break;
		default:
			return false;
	}
	
	
	if(Sensor_MakeAValue(REG_BAUD,data) == modbus_reply_h06)
		return true;
	else
		return false;

}

/*
函数功能：修改从机地址
参数：  address ：地址（输入小于256）
返回值：true ： 修改成功  false ：修改失败
备注：
*/
bool Modify_Address(unsigned short addre)
{
	if(Sensor_MakeAValue(REG_ADD,addre) == modbus_reply_h06)
		return true;
	else
		return false;

}
	


