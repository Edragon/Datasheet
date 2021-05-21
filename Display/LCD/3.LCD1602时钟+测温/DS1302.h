#ifndef __DS1302_H__
#define __DS1302_H__

#include <reg52.h>

//DS1302 IO设置
sbit DS1302_RST = P1^2;      //实时时钟复位线引脚
sbit DS1302_IO = P1^1;      //实时时钟数据线引脚
sbit DS1302_CLK = P1^0;      //实时时钟时钟线引脚 
 

//宏定义
#define DS1302_SECOND	0x80
#define DS1302_MINUTE	0x82
#define DS1302_HOUR		0x84 
#define DS1302_DAY		0x86
#define DS1302_MONTH	0x88
#define DS1302_WEEK		0x8a
#define DS1302_YEAR		0x8c 


typedef struct __SYSTEMTIME__
{
	unsigned char Second;
	unsigned char Minute;
	unsigned char Hour;
	unsigned char Week;
	unsigned char Day;
	unsigned char Month;
	unsigned char  Year;
}SYSTEMTIME;	//定义的时间类型


//函数或者变量声明
extern void DS1302_InputByte(unsigned char dat);
extern unsigned char DS1302_OutputByte(void) ;
extern void DS1302_Write(unsigned char ucAddr, unsigned char ucDa);
extern unsigned char DS1302_Read(unsigned char ucAddr);
extern void DS1302_SetProtect(unsigned char flag) ;
extern void DS1302_SetTime(unsigned char Address, unsigned char Value);
extern void DS1302_GetTime(SYSTEMTIME *Time);
extern void DS1302_Init(void);
//extern void DS1302_ON_OFF(bit FLAG_ON_OFF);
//extern void DS1302_Update(SYSTEMTIME *Time);


extern SYSTEMTIME DS1302Buffer;
extern bit Flag_Time_Refresh;

#endif