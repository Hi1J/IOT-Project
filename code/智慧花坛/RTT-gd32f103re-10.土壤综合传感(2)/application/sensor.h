#ifndef __SENSOR_H
#define __SENSOR_H

#include <app.h>


#define REG_PH    0x0006
#define REG_HUM   0x0012
#define REG_TEM   0x0013
#define REG_EC    0x0015

#define REG_ADD   0x0100
#define REG_BAUD  0x0101


typedef enum
{
	TYPE_PH,
	TYPE_HUM,
	TYPE_TEM,
	TYPE_EC,
	TYPE_ADD,
	TYPE_BAUD,
	TYPE_FREE,
}ENUM_TYPETypeDef;

typedef struct
{
	float _PH;
	float _HUM;
	float _TEM;
	unsigned short _EC;
	unsigned short _ADD;
	unsigned short _FREE;
	int _BAUD;
	
}STRUCT_DEBUGTypeDef;

typedef struct
{
	unsigned short PH;
	unsigned short HUM;
	unsigned short TEM;
	unsigned short EC;
}STRUCT_ZIGBEETypeDef;

extern STRUCT_DEBUGTypeDef debug_value;
extern STRUCT_ZIGBEETypeDef  zigbee_value;

bool Sensor_TakeAValue(ENUM_TYPETypeDef enumTYPE,unsigned short REG);
int Sensor_MakeAValue(unsigned short REG,unsigned short value);
bool Modify_Baud_Rate(unsigned int baud);
bool Modify_Address(unsigned int addre);


#endif 

