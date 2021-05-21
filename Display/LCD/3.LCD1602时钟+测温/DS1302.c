#include "DS1302.h"
#include<intrins.h>

//定义变量

SYSTEMTIME DS1302Buffer;
bit Flag_Time_Refresh = 1;

//****************************************************
//实时时钟写入一字节(内部函数)
//****************************************************
void DS1302_InputByte(unsigned char dat) 	
{ 
    unsigned char i;
    for(i=0; i<8; i++)
    {
        DS1302_IO = dat & 0x01;   
		
		DS1302_CLK = 0;				//上升沿写入数据
		_nop_();
		_nop_();        
        
        DS1302_CLK = 1;
		_nop_();
		_nop_();
        dat >>= 1; 
    } 
}

//****************************************************
//实时时钟读取一字节(内部函数)
//****************************************************
unsigned char DS1302_OutputByte(void) 	
{ 
    unsigned char i;
	unsigned char dat;
    for(i=0; i<8; i++)
    {
		DS1302_CLK = 1;				    //下降沿读出数据
		_nop_();
		_nop_();
        DS1302_CLK = 0;
		_nop_();
		_nop_();
        dat >>= 1;         			
        if( DS1302_IO == 1 )
			dat |= 0x80;				//最高位置一
		else
			dat &= 0x7F;				//最高位清零       
    } 
    return(dat); 
}

//****************************************************
//ucAddr: DS1302地址, ucData: 要写的数据
//****************************************************
void DS1302_Write(unsigned char ucAddr, unsigned char ucDa)	
{
    DS1302_RST = 0;
	_nop_();
    DS1302_CLK = 0;
	_nop_();
    DS1302_RST = 1;
	_nop_();
    DS1302_InputByte(ucAddr);       	// 地址，命令 
    DS1302_InputByte(ucDa);       		// 写1Byte数据
    DS1302_CLK = 1;
	_nop_();
    DS1302_RST = 0;
	_nop_();
}

//****************************************************
//读取DS1302某地址的数据
//****************************************************
unsigned char DS1302_Read(unsigned char ucAddr)	
{
    unsigned char ucData;
    DS1302_RST = 0;
	_nop_();
    DS1302_CLK = 0;
	_nop_();
    DS1302_RST = 1;
	_nop_();
    DS1302_InputByte(ucAddr|0x01);        // 地址，命令 
    ucData = DS1302_OutputByte();         // 读1Byte数据
    DS1302_CLK = 1;
	_nop_();
    DS1302_RST = 0;
	_nop_();
    return(ucData);
}

//****************************************************
//是否写保护
//****************************************************
void DS1302_SetProtect(unsigned char flag)       
{
	if(flag)
		DS1302_Write(0x8E,0x10);			//保护
	else
		DS1302_Write(0x8E,0x00);		   	//不保护
} 

//****************************************************
// 设置时间函数
//****************************************************
void DS1302_SetTime(unsigned char Address, unsigned char Value)        
{
	DS1302_SetProtect(0);
	DS1302_Write(Address, ((Value/10)<<4 | (Value%10))); 
}

//****************************************************
//获取实时时间
//****************************************************
void DS1302_GetTime(SYSTEMTIME *Time)
{
	unsigned char ReadValue;
	ReadValue = DS1302_Read(DS1302_SECOND);
	ReadValue = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);			//BCD码转换十进制
	if(ReadValue != DS1302Buffer.Second)								//跟上一次状态不同，置位刷新标志位
		Flag_Time_Refresh = 1;
	Time->Second = ReadValue;

	ReadValue = DS1302_Read(DS1302_MINUTE);
	Time->Minute = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = DS1302_Read(DS1302_HOUR);
	Time->Hour = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = DS1302_Read(DS1302_DAY);
	Time->Day = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);	
	ReadValue = DS1302_Read(DS1302_WEEK);
	Time->Week = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = DS1302_Read(DS1302_MONTH);
	Time->Month = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);
	ReadValue = DS1302_Read(DS1302_YEAR);
	Time->Year = ((ReadValue&0x70)>>4)*10 + (ReadValue&0x0F);	
}

//****************************************************
//初始化
//****************************************************
void DS1302_Init(void)
{
	unsigned char Second;
	Second = DS1302_Read(DS1302_SECOND);
	if(Second&0x80)		  
		DS1302_SetTime(DS1302_SECOND,Second & 0x7f);	//开启振荡	  

	
}
////****************************************************
////DS1302更新数据
////****************************************************
//void DS1302_Update(SYSTEMTIME *Time)
//{
//	DS1302_SetTime( DS1302_YEAR,Time->Year );					//设置年
//	DS1302_SetTime( DS1302_MONTH,Time->Month );					//设置月
//	DS1302_SetTime( DS1302_DAY,Time->Day );						//设置日
//	DS1302_SetTime( DS1302_WEEK,Time->Week );					//设置星期
//	DS1302_SetTime( DS1302_HOUR,Time->Hour );					//设置时
//	DS1302_SetTime( DS1302_MINUTE,Time->Minute );				//设置分
//	DS1302_SetTime( DS1302_SECOND,Time->Second );				//设置秒	
//}
////****************************************************
////DS1302振荡器停止
////****************************************************
//void DS1302_ON_OFF(bit FLAG_ON_OFF)
//{
//	unsigned char Second;
//	Second = DS1302_Read(DS1302_SECOND);
//	if(FLAG_ON_OFF == 0)
//		DS1302_Write(DS1302_SECOND,Second | 0x80);				//关闭振荡
//	else
//		DS1302_Write(DS1302_SECOND,Second & 0x7f);				//开启振荡
//}


