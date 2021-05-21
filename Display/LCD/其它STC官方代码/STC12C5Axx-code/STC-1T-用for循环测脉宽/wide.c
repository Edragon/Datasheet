
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU Programme Demo ----------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*      本程序经过测试完全正常, 不提供电话技术支持, 如不能理解, 请自行补充相关基础.  */


/*************	本程序功能说明	**************

用for循环测脉冲宽度.  通用各种1T的MCU和。

******************************************/

#define		MCU_MODE	1	//1: 1T,   6: 6T,   12: 12T

#include	<reg51.h>

//#define		MAIN_Fosc	 5529600L	//定义时钟
//#define		MAIN_Fosc	 6000000L	//定义时钟
//#define		MAIN_Fosc	11059200L	//定义时钟
//#define		MAIN_Fosc	12000000L	//定义时钟
//#define		MAIN_Fosc	16000000L	//定义时钟
//#define		MAIN_Fosc	16384000L	//定义时钟
//#define		MAIN_Fosc	18432000L	//定义时钟
//#define		MAIN_Fosc	20000000L	//定义时钟
#define		MAIN_Fosc	22118400L	//定义时钟
//#define		MAIN_Fosc	24000000L	//定义时钟
//#define		MAIN_Fosc	27000000L	//定义时钟
//#define		MAIN_Fosc	30000000L	//定义时钟
//#define		MAIN_Fosc	32768000L	//定义时钟
//#define		MAIN_Fosc	33000000L	//定义时钟
//#define		MAIN_Fosc	33177600L	//定义时钟

sbit	P_PULSE = P1^0;	//检测脚输入
sbit	P_TXD1  = P3^1;	//模拟串口发送

void	Tx1Send(unsigned char dat);
void	PrintString(unsigned char code *puts);
void  	delay_ms(unsigned char ms);

unsigned int wide_H;	//高脉冲时间
unsigned int wide_L;	//低脉冲时间


void main(void)
{
	unsigned int j;

	PrintString("模拟串口发送，延时程序.  通用各种IC和频率。 2011-2-2\r\n");

	while(1)
	{
		delay_ms(250);	//延时0.5秒
		delay_ms(250);
		
		for(wide_H=0; wide_H<=65000; wide_H++)	//等待一个低电平, 无信号超时退出
		{
			if(!P_PULSE)	break;
		}
		
		for(wide_H=0; wide_H<=65000; wide_H++)	//等待一个高电平, 无信号超时退出
		{
			if(P_PULSE)	break;
		}
		
		for(wide_H=0; wide_H<=65000; wide_H++)	//测一个高电平
		{
			if(!P_PULSE)	break;
		}
		
		for(wide_L=0; wide_L<=65000; wide_L++)	//测一个低电平
		{
			if(P_PULSE)	break;
		}
		
		if(wide_H > 65000)	PrintString("测量高电平超时!\r\n");
		else
		{
			PrintString("测量到高电平计数 = ");
			j = wide_H;
			Tx1Send(j / 10000 + '0');
			Tx1Send(j % 10000 / 1000 + '0');
			Tx1Send(j % 1000 / 100 + '0');
			Tx1Send(j % 100 / 10 + '0');
			Tx1Send(j % 10 + '0');
			Tx1Send(0x0d);
			Tx1Send(0x0a);
		}

		if(wide_L > 65000)	PrintString("测量低电平超时!\r\n");
		else
		{
			PrintString("测量到低电平计数 = ");
			j = wide_L;
			Tx1Send(j / 10000 + '0');
			Tx1Send(j % 10000 / 1000 + '0');
			Tx1Send(j % 1000 / 100 + '0');
			Tx1Send(j % 100 / 10 + '0');
			Tx1Send(j % 10 + '0');
			Tx1Send(0x0d);
			Tx1Send(0x0a);
		}
			Tx1Send(0x0d);
			Tx1Send(0x0a);
		
	}
}


//========================================================================
// 函数: void  delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数.
// 返回: none.
// 版本: VER1.0
// 日期: 2010-12-15
// 备注: 
//========================================================================
void  delay_ms(unsigned char ms)
{
	unsigned int i;
	do
	{
		i = MAIN_Fosc / 14000L;	//1T
		while(--i)	;   //8T per loop at 12T or 6T mode, 14T at 1T mode
     }while(--ms);
}

/********************** 模拟串口相关函数************************/

void	BitTime(void)	//位时间函数 9600bps
{
	unsigned int i;

	i = ((MAIN_Fosc / 100) * 104) / 140000L - 1;	//根据主时钟来计算位时间 1T
	while(--i)	;   //8T per loop at 12T or 6T mode, 14T at 1T mode
}

//模拟串口发送
void	Tx1Send(unsigned char dat)		//9600，N，8，1		发送一个字节
{
	unsigned char	i;
	EA = 0;
	P_TXD1 = 0;
	BitTime();
	i = 8;
	do
	{
		dat >>= 1;
		P_TXD1 = CY;
		BitTime();
	}while(--i);
	P_TXD1 = 1;
	EA = 1;
	BitTime();	//stop bit
	BitTime();	//stop bit
}

void PrintString(unsigned char code *puts)		//发送一串字符串
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//遇到停止符0结束
}
