/************************************************************
编写人：李宇杰
日期：2022年11月19日
内容：多功能电子表菜单显示函数
修改日志：
1-2022年11月20日16点26分—内容：加入调日期函数于D菜单


************************************************************/
#include "MENU.h"

void Main_MENU(u8 *b,u8 *c){
	u8 buffer[3];	//用于存放LM75A给出的温度数据 
	while(*b==0){  //主菜单显示
		LM75A_GetTemp(buffer); //读取LM75A的温度数据

		OLED_DISPLAY_16x16(2,5*8,18);	//oled  “环境温度:”和“星期” 
		OLED_DISPLAY_16x16(2,7*8,19);
		OLED_DISPLAY_16x16(6,0*16,16);  
		OLED_DISPLAY_16x16(6,1*16,17);
		OLED_DISPLAY_16x16(6,2*16,8);
		OLED_DISPLAY_16x16(6,3*16,9);
		OLED_DISPLAY_8x16(6,8*8,':');
	
		
		
		//显示温度	
		if(buffer[0])OLED_DISPLAY_8x16(6,9*8,'-'); //如果第1组为1即是负温度
		else  OLED_DISPLAY_8x16(6,9*8,'+');		   //否则为正温度
		OLED_DISPLAY_8x16(6,10*8,buffer[1]/10+0x30);//显示温度值
		OLED_DISPLAY_8x16(6,11*8,buffer[1]%10+0x30);//
		OLED_DISPLAY_8x16(6,12*8,'.');//
		OLED_DISPLAY_8x16(6,13*8,buffer[2]/10+0x30);//
		OLED_DISPLAY_8x16(6,14*8,buffer[2]%10+0x30);//
		OLED_DISPLAY_8x16(6,15*8,'C');//
	
		//显示时间
		if(RTC_Get()==0){
		//oled显示：
		OLED_DISPLAY_8x16(0,1*8,ryear/1000+0x30); //第一位
		OLED_DISPLAY_8x16(0,2*8,(ryear/100)%10+0x30);		
		OLED_DISPLAY_8x16(0,3*8,(ryear/10)%10+0x30);
		OLED_DISPLAY_8x16(0,4*8,ryear%10+0x30);
		OLED_DISPLAY_16x16(0,5*8,12);//年
		OLED_DISPLAY_8x16(0,7*8,rmon/10+0x30);
		OLED_DISPLAY_8x16(0,8*8,rmon%10+0x30);
		OLED_DISPLAY_16x16(0,9*8,13);//月
		OLED_DISPLAY_8x16(0,11*8,rday/10+0x30);
		OLED_DISPLAY_8x16(0,12*8,rday%10+0x30);
		OLED_DISPLAY_16x16(0,13*8,14);//日
		//数码管显示：
		TM1640_display(0,rhour/10);
		TM1640_display(1,rhour%10);
		TM1640_display(2,21);
		TM1640_display(3,rmin/10);
		TM1640_display(4,rmin%10);
		TM1640_display(5,21);
		TM1640_display(6,rsec/10);
		TM1640_display(7,rsec%10);
	
		}
	
		//显示星期几
		switch(rweek){
			case 1:OLED_DISPLAY_16x16(2,9*8,20);break;
			case 2:OLED_DISPLAY_16x16(2,9*8,21);break;
			case 3:OLED_DISPLAY_16x16(2,9*8,22);break;
			case 4:OLED_DISPLAY_16x16(2,9*8,23);break;
			case 5:OLED_DISPLAY_16x16(2,9*8,24);break;
			case 6:OLED_DISPLAY_16x16(2,9*8,25);break;
			case 0:OLED_DISPLAY_16x16(2,9*8,26);break;
			default:break;
		}
	
		//调时
//			if(ENCODER_READ()==3)TimeSettingA();//当按钮按下，进入调时模式
	
	
		//流水灯 
		TM1640_led(*c);
		*c<<=1;
		if(*c==0x00)*c=0x01;
	
		if(k==1){*b=1;k=0;}
		delay_ms(150); //延迟

	}
} 

void Auxiliary_MENU(u8 *b,u8 *c){
	while(*b==1){	 //调时菜单显示
		OLED_DISPLAY_8x16(0,0*8,'A');		 //A
		OLED_DISPLAY_8x16(0,1*8,':');		 //:
		OLED_DISPLAY_16x16(0,2*16,29);		 //设
		OLED_DISPLAY_16x16(0,3*16,30);		 //置
		OLED_DISPLAY_16x16(0,4*16,31);		 //时
		OLED_DISPLAY_16x16(0,5*16,32);		 //分
		OLED_DISPLAY_16x16(0,6*16,33);		 //秒

		OLED_DISPLAY_8x16(2,0*8,'B');		 //B
		OLED_DISPLAY_8x16(2,1*8,':');		 //:
		OLED_DISPLAY_16x16(2,2*16,29);		 //设
		OLED_DISPLAY_16x16(2,3*16,30);		 //置
		OLED_DISPLAY_16x16(2,4*16,12);		 //时
		OLED_DISPLAY_16x16(2,5*16,13);		 //分
		OLED_DISPLAY_16x16(2,6*16,14);		 //秒
		
		OLED_DISPLAY_8x16(6,0*8,'D');		 //D
		OLED_DISPLAY_8x16(6,1*8,':');		 //:
		OLED_DISPLAY_16x16(6,2*16,34);		 //返
		OLED_DISPLAY_16x16(6,3*16,35);		 //回

		if(RTC_Get()==0){
		//数码管显示：
		TM1640_display(0,rhour/10);
		TM1640_display(1,rhour%10);
		TM1640_display(2,21);
		TM1640_display(3,rmin/10);
		TM1640_display(4,rmin%10);
		TM1640_display(5,21);
		TM1640_display(6,rsec/10);
		TM1640_display(7,rsec%10);
		}
		
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0))TimeSettingA();	//按A键进入调时分秒模式
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))TimeSettingB();	//按A键进入调年月日模式

		//流水灯
		TM1640_led(*c);	  
		*c<<=1;
		if(*c==0x00)*c=0x01;
	
		if(k==1){*b=0;k=0;}
		delay_ms(150); //延迟	
	}
}

