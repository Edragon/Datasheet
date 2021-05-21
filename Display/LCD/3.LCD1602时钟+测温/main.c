//****************************************************
//****************************************************
//			小强电子设计最小系统例程
//****************************************************
//****************************************************

#include "main.h"
#include "LCD1602.h"
#include "DS1302.h"
#include "DS18B20.h"

//定义变量
unsigned int Temp_Buffer = 0;
unsigned char KEY_NUM = 0;
bit Flag_KEY_Set = 0;
unsigned char KEY2_Count = 0;


//****************************************************
//主函数
//****************************************************
void main()
{
	LCD1602_Init();									//初始化LCD1602
	DS1302_Init();									//初始化DS1302

	LCD1602_write_com(0x80);
	LCD1602_write_word("Welcome to use!");

	Temp_Buffer = Get_temp();  					//读取DS18B20的值
	Delay_ms(1000);	

	LCD1602_write_com(0x01);					//清屏

	while(1)
	{				
		DS1302_GetTime(&DS1302Buffer);							//获取当前RTCC值

		if(Flag_Time_Refresh == 1 && Flag_KEY_Set == 0)			//数据更新时才刷新LCD
		{
			Flag_Time_Refresh = 0;
			Display_RTCC();										//显示实时时钟
			Display_Temp();										//获取并显示温度值
		}

		Scan_Key();											//按键扫描

		switch( KEY_NUM )
		{
			case 1:						//按键1执行程序
					KEY_NUM = 0;		//清空按键标志
					Flag_KEY_Set = ~Flag_KEY_Set;	//设置按键标志位翻转
					if(Flag_KEY_Set)
					{						
						LCD1602_write_com(0x0d);	//LCD1602指针闪烁开
						LCD1602_write_com(0x80+ 3);	//指针设置到第一行第4列
						KEY2_Count = 0;				//按键S2计数清零
					}
					else
					{
						LCD1602_write_com(0x0c);	//LCD1602指针闪烁关
					}
					break;
			case 2:						//按键2执行程序
					KEY_NUM = 0;		//清空按键标志
					if(Flag_KEY_Set)			//在设置状态时运行
					{
						KEY2_Count++;
						if(KEY2_Count == 7)
							KEY2_Count = 0;
						switch(KEY2_Count)
						{
							case 0:
									LCD1602_write_com(0x80+ 3);	//指针指向年，闪烁
									break;
							case 1:
									LCD1602_write_com(0x80+ 6);	//指针指向月，闪烁
									break;
							case 2:
									LCD1602_write_com(0x80+ 9);	//指针指向日，闪烁
									break;
							case 3:
									LCD1602_write_com(0x80+ 15);	//指针指向星期，闪烁
									break;
							case 4:
									LCD1602_write_com(0x80+ 0x40 + 1);	//指针指向小时，闪烁
									break;
							case 5:
									LCD1602_write_com(0x80+ 0x40 + 4);	//指针指向分钟，闪烁
									break;
							case 6:
									LCD1602_write_com(0x80+ 0x40 + 7);	//指针指向秒钟，闪烁
									break;
							default : break;
						}	
					}					
					break;
			case 3:						//按键3执行程序
					KEY_NUM = 0;		//清空按键标志
					if(Flag_KEY_Set)			//在设置状态时运行
					{
						switch(KEY2_Count)
						{
							case 0:
									DS1302Buffer.Year++;
									if(DS1302Buffer.Year >= 100)
									{
										DS1302Buffer.Year = 0;
									}
									DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );

									Display_RTCC();
									
									LCD1602_write_com(0x80+ 3);	//指针指向年，闪烁
									break;
							case 1:
									DS1302Buffer.Month++;
									if(DS1302Buffer.Month >= 13)
									{
										DS1302Buffer.Month = 1;
									}
									DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 6);	//指针指向月，闪烁
									break;
							case 2:
									DS1302Buffer.Day++;
									if(DS1302Buffer.Day >= 32)
									{
										DS1302Buffer.Day = 1;
									}
									DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 9);	//指针指向日，闪烁
									break;
							case 3:
									DS1302Buffer.Week++;
									if(DS1302Buffer.Week >= 8)
									{
										DS1302Buffer.Week = 1;
									}
									DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 15);	//指针指向星期，闪烁
									break;
							case 4:
									DS1302Buffer.Hour++;
									if(DS1302Buffer.Hour >= 24)
									{
										DS1302Buffer.Hour = 0;
									}
									DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 1);	//指针指向小时，闪烁
									break;
							case 5:
									DS1302Buffer.Minute++;
									if(DS1302Buffer.Minute >= 60)
									{
										DS1302Buffer.Minute = 0;
									}
									DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 4);	//指针指向分钟，闪烁
									break;
							case 6:
									DS1302Buffer.Second++;
									if(DS1302Buffer.Second >= 60)
									{
										DS1302Buffer.Second = 0;
									}
									DS1302_SetTime( DS1302_SECOND , DS1302Buffer.Second);
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 7);	//指针指向秒钟，闪烁
									break;
							default : break;
						}
					}
					break;
			case 4:						//按键4执行程序
					KEY_NUM = 0;		//清空按键标志
					if(Flag_KEY_Set)			//在设置状态时运行
					{
						switch(KEY2_Count)
						{
							case 0:
									DS1302Buffer.Year--;
									if(DS1302Buffer.Year == 255)
									{
										DS1302Buffer.Year = 99;
									}
									DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );
									Display_RTCC();
									
									LCD1602_write_com(0x80+ 3);	//指针指向年，闪烁
									break;
							case 1:
									DS1302Buffer.Month--;
									if(DS1302Buffer.Month == 0)
									{
										DS1302Buffer.Month = 12;
									}
									DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 6);	//指针指向月，闪烁
									break;
							case 2:
									DS1302Buffer.Day--;
									if(DS1302Buffer.Day == 0)
									{
										DS1302Buffer.Day = 31;
									}
									DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 9);	//指针指向日，闪烁
									break;
							case 3:
									DS1302Buffer.Week--;
									if(DS1302Buffer.Week == 0)
									{
										DS1302Buffer.Week = 7;
									}
									DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 15);	//指针指向星期，闪烁
									break;
							case 4:
									DS1302Buffer.Hour--;
									if(DS1302Buffer.Hour == 255)
									{
										DS1302Buffer.Hour = 23;
									}
									DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 1);	//指针指向小时，闪烁
									break;
							case 5:
									DS1302Buffer.Minute--;
									if(DS1302Buffer.Minute == 255)
									{
										DS1302Buffer.Minute = 59;
									}
									DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 4);	//指针指向分钟，闪烁
									break;
							case 6:
									DS1302Buffer.Second--;
									if(DS1302Buffer.Second == 255)
									{
										DS1302Buffer.Second = 59;
									}
									DS1302_SetTime( DS1302_SECOND , DS1302Buffer.Second);
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 7);	//指针指向秒钟，闪烁
									break;
							default : break;
						}
					}
					break;
			default : break;
		}				
	}
}

//****************************************************
//显示温度
//****************************************************
void Display_Temp()
{
	Temp_Buffer = Get_temp();  					//读取DS18B20的值
	LCD1602_write_com(0x80+0x40+10);				//设置LCD1602指针到第二行第11列
	if(flag_temper == 1)						//根据温度标志位显示温度正负
	{
		LCD1602_write_data('-');	
	}
	if( Temp_Buffer/1000 != 0 )					//如果第一位为0，忽略显示
	{
		LCD1602_write_data(Temp_Buffer/1000+0X30);	   //显示温度百位值
	}
	LCD1602_write_data(Temp_Buffer%1000/100+0X30);	   //显示温度十位值
	LCD1602_write_data(Temp_Buffer%100/10+0X30);	   //显示温度个位值
	LCD1602_write_data('.');						   //显示小数点
	LCD1602_write_data(Temp_Buffer%10+0X30);		   //显示温度十分位值
	LCD1602_write_data('C');						   //显示字符C
		
}

//****************************************************
//显示实时时钟
//****************************************************
void Display_RTCC()
{
	LCD1602_write_com(0x80);							//指针设置到第一行第1列
	
	LCD1602_write_word("20");
	LCD1602_write_data(DS1302Buffer.Year/10 + 0x30);	 //显示Year高位
	LCD1602_write_data(DS1302Buffer.Year%10 + 0x30);	 //显示Year低位
	LCD1602_write_data('-');
	LCD1602_write_data(DS1302Buffer.Month/10 + 0x30);	 //显示Month高位
	LCD1602_write_data(DS1302Buffer.Month%10 + 0x30);	 //显示Month低位
	LCD1602_write_data('-');
	LCD1602_write_data(DS1302Buffer.Day/10 + 0x30);		 //显示Day高位
	LCD1602_write_data(DS1302Buffer.Day%10 + 0x30);		 //显示Day低位

	LCD1602_write_com(0x80+13);							 //指针设置到第一行第14列
	switch(DS1302Buffer.Week)
	{
		case 1:
				LCD1602_write_word("Mon");				 //显示星期一
				break;
		case 2:
				LCD1602_write_word("Tue");				 //显示星期二
				break;
		case 3:
				LCD1602_write_word("Wed");				 //显示星期三
				break;
		case 4:
				LCD1602_write_word("Thu");				 //显示星期四
				break;
		case 5:
				LCD1602_write_word("Fri");				 //显示星期五
				break;
		case 6:
				LCD1602_write_word("Sat");				 //显示星期六
				break;
		case 7:
				LCD1602_write_word("Sun");				 //显示星期日
				break;
		default : break;						
	}		


	LCD1602_write_com(0x80 + 0x40);				 //指针设置到第二行第1列

	LCD1602_write_data(DS1302Buffer.Hour/10 + 0x30);	 //显示Hour高位
	LCD1602_write_data(DS1302Buffer.Hour%10 + 0x30);	 //显示Hour低位
	LCD1602_write_data(':');
	LCD1602_write_data(DS1302Buffer.Minute/10 + 0x30);	 //显示Minute高位
	LCD1602_write_data(DS1302Buffer.Minute%10 + 0x30);	 //显示Minute低位
	LCD1602_write_data(':');
	LCD1602_write_data(DS1302Buffer.Second/10 + 0x30);	 //显示Second高位
	LCD1602_write_data(DS1302Buffer.Second%10 + 0x30);	 //显示Second低位	
}

//****************************************************
//蜂鸣器程序
//****************************************************
void Buzzer_Di()
{
	Buzzer = 0;
	Delay_ms(3);
	Buzzer = 1;
	Delay_ms(3);
}
//****************************************************
//按键扫描程序
//****************************************************
void Scan_Key()
{
	if( KEY1 == 0 )	 					//按键1扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY1 == 0 )
		{
			Buzzer_Di();
			while(KEY1 == 0);			//等待松手
			KEY_NUM = 1;
		}
	}

	if( KEY2 == 0 )						//按键2扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY2 == 0 )
		{
			Buzzer_Di();
			while(KEY2 == 0);			//等待松手
			KEY_NUM = 2;
		}
	}

	if( KEY3 == 0 )						//按键2扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY3 == 0 )
		{
			Buzzer_Di();
			while(KEY3 == 0);			//等待松手
			KEY_NUM = 3;
		}
	}

	if( KEY4 == 0 )						//按键2扫描
	{
		Delay_ms(10);					//延时去抖
		if( KEY4 == 0 )
		{
			Buzzer_Di();
			while(KEY4 == 0);			//等待松手
			KEY_NUM = 4;
		}
	}
}

//****************************************************
//MS延时函数(12M晶振下测试)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}