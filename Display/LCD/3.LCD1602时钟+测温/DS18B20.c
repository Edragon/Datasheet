#include "DS18B20.h"
#include<intrins.h>

//定义变量
unsigned char flag_temper = 0;

//****************************************************
//DS18B20延时函数
//****************************************************
void DS18B20_Delay( unsigned int n )
{
	unsigned int i;
	for(i = 0 ; i < n ; i++ );
}
//****************************************************
//DS18B20写1字节
//****************************************************
void DS18B20_Write_Byte( unsigned char dat)
{
	unsigned char i;
	for( i = 0 ; i < 8 ; i++ )
	{
		DS18B20_DQ = 0;
		_nop_();			//延时>1us
		_nop_();
		
		DS18B20_DQ = dat&0x01;	 	//先写低位
		dat >>= 1;

		DS18B20_Delay(6);	//延时60~120us
		
		DS18B20_DQ = 1;		//释放总线
		_nop_();			//延时>1us
		_nop_();	
	}		
}

//****************************************************
//DS18B20读1字节
//****************************************************
unsigned char DS18B20_Read_Byte( )
{
	unsigned char dat,i;
	for( i = 0 ; i < 8 ; i++ )
	{
		DS18B20_DQ = 0;
		_nop_();			//延时>1us
		_nop_();
				
		DS18B20_DQ = 1;		//释放总线
		_nop_();			//延时>1us
		_nop_();

		dat >>= 1;
		if( DS18B20_DQ == 1)
		{
			dat |= 0X80;
		}
		else
		{
			dat &= 0x7f;
		}

		DS18B20_Delay(6);	//延时60~120us			
	}
	return dat;		
}

//****************************************************
//DS18B20初始化
//****************************************************
bit DS18B20_Init()
{
	bit Flag_exist = 1;
	DS18B20_DQ = 1;			//释放总线
	_nop_();				//延时>1us
	_nop_();

	DS18B20_DQ = 0;	
	DS18B20_Delay(50);		//延时480~960us	

	DS18B20_DQ = 1;			//释放总线
	DS18B20_Delay(3);		//延时15~60us

	Flag_exist = DS18B20_DQ;
	DS18B20_Delay(10);		//延时60~240us

	DS18B20_DQ = 1;			//释放总线
	return Flag_exist;			
}

//**********************************************************
//读取温度函数，返回温度的绝对值，并标注flag_temper，flag_temper=1表示负，flag_temper=0表示正
//**********************************************************
unsigned int Get_temp(void)         //读取温度值 
{  
	float tt;
	unsigned char a,b;
	unsigned int temp;
	if( DS18B20_Init() == 0 )                //初始化
	{
		DS18B20_Write_Byte(0xcc);          //忽略ROM指令
		DS18B20_Write_Byte(0x44);          //温度转换指令
	
	//	_delay_ms(750);				//PROTEUS仿真需要加
	
		if( DS18B20_Init() == 0 )                 //初始化
		{
			DS18B20_Write_Byte(0xcc);          //忽略ROM指令
			DS18B20_Write_Byte(0xbe);          //读暂存器指令
			a = DS18B20_Read_Byte();           //读取到的第一个字节为温度LSB
			b = DS18B20_Read_Byte();           //读取到的第一个字节为温度MSB
			temp = b;                      //先把高八位有效数据赋于temp
			temp <<= 8;                    //把以上8位数据从temp低八位移到高八位
			temp = temp|a;                //两字节合成一个整型变量
			
			if(temp>0xfff)
			{
				flag_temper=1;				//温度为负数
				temp=(~temp)+1;
			}
			else
			{																			   
				flag_temper=0;				//温度为正或者0
			}
			
			tt = temp*0.0625;              //得到真实十进制温度值
			                                //因为DS18B20可以精确到0.0625度
			                                //所以读回数据的最低位代表的是0.0625度
			temp = tt*10+0.5;               //放大十倍
			                                //这样做的目的将小数点后第一位也转换为可显示数字
		                              	    //同时进行一个四舍五入操作。
		}
	}
	return temp;
}


