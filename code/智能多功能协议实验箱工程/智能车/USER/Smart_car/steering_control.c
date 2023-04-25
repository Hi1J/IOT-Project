#include "steering_control.h"
//转向控制 舵机部分

u16 Steering_current = Mid_value;

u16 Steering_Control(u8 value)
{
	switch(value)
	{
		case 0://右转
			Steering_current--;
			if(Steering_current <= Right_Max)
			{Steering_current = Right_Max;}
			break;
		case 1://左转
			Steering_current++;
			if(Steering_current >= Left_Max)
			{Steering_current = Left_Max;}
			break;
		case 2://直线
			Steering_current = Mid_value;
			break;
		case 3://方向不变
			break;
	}
	return Steering_current;

}


void Car_Steering(u8 value)
{
	switch(value)
	{
		case 0x00://直线
			TIM3->CCR3 = Steering_Control(2);
		break;
		case 0x01://左转
			TIM3->CCR3 = Steering_Control(1);
		break;
		case 0x10://右转
			TIM3->CCR3 = Steering_Control(0);
		break;
		case 0x11:
			TIM3->CCR3 = Steering_Control(3);
		break;
		default:
			break;
	
	}

}