#include "sensor.h"





void Take_A_Data(uint16_t Type)
{
	
	MODBUS_HostToSlave_H03(Slave_address,Type,1);
	
	
}


