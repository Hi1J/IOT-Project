/************************************************************
编写人：李宇杰
日期：2022年11月19日
内容：多功能电子表调时函数
修改日志：
1-2022年11月20日16点25分-内容：增加了调年月日的函数

**************************************************************/

#include "TimeSetting.h"





void TimeSettingA(void){
		u8 z,s,m,h,c,i;//设置标志量
//		delay_ms(200);  //给ENCODER_READ()返回值反应时间 若没有这语句 会出错 经测试 延时最低极限大约为200ms 低于会出错 (当不是按键触发时省略)
		z=1;	  //循环标志位
		s=rsec;m=rmin;h=rhour;	//记录下此刻的时间
		i=0;				 //默认调时模式
		TM1640_led(0x80);   //默认调秒指示灯亮  
		OLED_DISPLAY_CLEAR();  //清屏
		OLED_DISPLAY_16x16(2,2*16,27);	  //正
		OLED_DISPLAY_16x16(2,3*16,28);	  //在
		OLED_DISPLAY_16x16(2,4*16,29);	  //设
		OLED_DISPLAY_16x16(2,5*16,30);	  //置
		OLED_DISPLAY_16x16(2,6*16,33);	  //秒（默认）
		OLED_DISPLAY_8x16(6,0*8,'A');	  //A
		OLED_DISPLAY_8x16(6,6*8,'B');	  //B
		OLED_DISPLAY_8x16(6,12*8,'C');	  //C 
		OLED_DISPLAY_16x16(6,1*16,31);	  //时
		OLED_DISPLAY_16x16(6,4*16,32);	  //分
		OLED_DISPLAY_16x16(6,7*16,33);	  //秒
		while(z==1)	  //
		{
			c=ENCODER_READ();	  //再次读取编码器返回值	若没有上面的delay_ms(200);这里读出来的值仍为3 程序直接退出！


			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){	  //按A 调时
			i=2;TM1640_led(0x01);OLED_DISPLAY_16x16(2,6*16,31);}	  
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)){	  //按B 调分
			i=1;TM1640_led(0x20);OLED_DISPLAY_16x16(2,6*16,32);}	  
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)){	  //按C 调秒
			i=0;TM1640_led(0x80);OLED_DISPLAY_16x16(2,6*16,33);}	  



			if(c==1){	 //右转	 加
				if(i==0){s++;if(s==60)s=0;}
				if(i==1){m++;if(m==60)m=0;}
				if(i==2){h++;if(h==24)h=0;}		}
			
			if(c==2){	  //左转  减
				if(i==0){if(s==0)s=60;s--;}
				if(i==1){if(m==0)m=60;m--;}
				if(i==2){if(h==0)h=24;h--;}		 }
									
			if(c==3)z=0;		   		//调时后再次按下编码器可退出调时模式从而继续走时
	
			TM1640_display(0,h/10);		//显示数值
			TM1640_display(1,h%10);
			TM1640_display(2,21);
			TM1640_display(3,m/10);
			TM1640_display(4,m%10);
			TM1640_display(5,21);
			TM1640_display(6,s/10);
			TM1640_display(7,s%10);
		}

		
		//rsec=s;rmin=m;rhour=h;		  
		if(RTC_Get()==0)RTC_Set(ryear,rmon,rday,h,m,s);	   //将修改后的时间值写入	 时间的写入大概需要1s 也就是说 若想调的准 得调比预调值多1s
		OLED_DISPLAY_CLEAR(); //清屏！
		//delay_ms(200);		//作用同上，这些函数不会影响rtc走时	
		TM1640_led(0x00);	 //退出调时模式后调时指示灯灭	

}



void  TimeSettingB(void){
		u8 m,d,i,c,j=0;
		u16 y;
		i=0;
		y=ryear;m=rmon;d=rday;		//记录当前年月日
	
		OLED_DISPLAY_CLEAR();  //清屏
		OLED_DISPLAY_16x16(2,1*16,27);	  //正
		OLED_DISPLAY_16x16(2,2*16,28);	  //在
		OLED_DISPLAY_16x16(2,3*16,29);	  //设
		OLED_DISPLAY_16x16(2,4*16,30);	  //置
		OLED_DISPLAY_8x16(6,0*8,'A');	  //A
		OLED_DISPLAY_8x16(6,6*8,'B');	  //B
		OLED_DISPLAY_8x16(6,12*8,'C');	  //C
		OLED_DISPLAY_16x16(6,1*16,12);	  //年
		OLED_DISPLAY_16x16(6,4*16,13);	  //月
		OLED_DISPLAY_16x16(6,7*16,14);	  //日

		OLED_DISPLAY_16x16(2,5*16,14);	  //日（默认）

		TM1640_led(0x80); 

		while(1){

			c=ENCODER_READ();
			if((y%4==0&&y%100!=0)||y%400==0)i=1;//判断是否为闰年	1为闰年，0为平年
			else i=0;

			//方法1
			if(m==4||m==6||m==9||m==11)if(d==31)d=30;		//当设置小月时，若设置前日期为31 将其设为30
			if(m==2){if(d>28){if(i==1)d=29;if(i==0)d=28;}}	//当设置2月时，若设置前日期大于28，闰年设为29，平年设为28

			//方法2
//			if(m==1||m==3||m==5||m==7||m==8||m==10||m==12)if(d>31)d=1;		 //大月时，超过31减为0（或可以设置最高值）
//			if(m==4||m==6||m==9||m==11)if(d>30)d=1;							 //小月时，超过31减为0（或可以设置最高值）
//			if(m==2&&i==1)if(d>29)d=1;										 //
//			if(m==2&&i==0)if(d>28)d=1;										 //

			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)){	  //按A 调年
			j=2;TM1640_led(0x01);OLED_DISPLAY_16x16(2,5*16,12);}	  
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1)){	  //按B 调月
			j=1;TM1640_led(0x20);OLED_DISPLAY_16x16(2,5*16,13);}	  
			if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)){	  //按C 调日
			j=0;TM1640_led(0x80);OLED_DISPLAY_16x16(2,5*16,14);}

			if(c==1){	   //右转加
				if(j==0){                          //d++;	   //方法2					     		  
					switch(m){						 //方法1
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						d++;if(d==32)d=1;break;			  //31天的月份
	
					case 4:
					case 6:
					case 9:
					case 11:
						d++;if(d==31)d=1;break;			  //30天的月份
	
					case 2:								  
						if(i==1){d++;if(d==30)d=1;};		 //闰年
						if(i==0){d++;if(d==29)d=1;};break;	 //平年

					default:break;
					}
				}
				if(j==1){m++;if(m==13)m=1;}					//调月
				if(j==2){y++;if(y==2100)y=1970;}			//调年
			}
			
			if(c==2){
				if(j==0){              //{if(d==0)d=31;d--;}	 //方法2
					switch(m){
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						if(d==1)d=32;d--;break;			  //31天的月份
	
					case 4:
					case 6:
					case 9:
					case 11:
						if(d==1)d=31;d--;break;			  //30天的月份
	
					case 2:								  
						if(i==1){if(d==1)d=30;d--;};		 //闰年
						if(i==0){if(d==1)d=29;d--;};break;	 //平年

					default:break;
					}
				}
				if(j==1){if(m==1)m=13;m--;}					  //调月
				if(j==2){if(y==1970)y=2100;y--;}			  //调年
			}
			if(c==3)break;	  //按编码器确认退出

			TM1640_display(0,y/1000);		//显示数值
			TM1640_display(1,(y/100)%10);
			TM1640_display(2,(y/10)%10);
			TM1640_display(3,y%10);
			TM1640_display(4,m/10);
			TM1640_display(5,m%10);
			TM1640_display(6,d/10);
			TM1640_display(7,d%10);
			
			

		}

		if(RTC_Get()==0)RTC_Set(y,m,d,rhour,rmin,rsec+1); //写入	  +1是为了保证时间接近同步 因为写入时间大概为1s
		OLED_DISPLAY_CLEAR(); //清屏！
		TM1640_led(0x00);		 //指示灯灭
		
		
		
	
	
	
	
	}

