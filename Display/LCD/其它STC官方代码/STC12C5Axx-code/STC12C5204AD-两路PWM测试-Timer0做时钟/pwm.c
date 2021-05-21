
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

测试硬件PWM. 适用STC12C5204AD系列。

可以通过设置Timer0的溢出率来改变PWM周期。

******************************************/


#include	<reg51.h>
#define	uchar	unsigned char
#define uint	unsigned int

#define		Timer0_Reload	18432	//T0 interrupt at 10ms @22.1184MHZ

sfr	AUXR = 0x8E;
sfr CCON = 0xD8;
sfr CMOD = 0xD9;
sfr CCAPM0 = 0xDA;	//PCA module 0 work mode
sfr CCAPM1 = 0xDB;	//PCA module 1 work mode
sfr CL     = 0xE9;	//PCA counter
sfr CCAP0L = 0xEA;	//PCA模块0的捕捉/比较寄存器低8位。
sfr CCAP1L = 0xEB;	//PCA模块1的捕捉/比较寄存器低8位。
sfr PCA_PWM0 = 0xF2;	//PCA模块0 PWM寄存器。
sfr PCA_PWM1 = 0xF3;	//PCA模块1 PWM寄存器。
sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;		//PCA模块0的捕捉/比较寄存器高8位。
sfr CCAP1H = 0xFB;		//PCA模块1的捕捉/比较寄存器高8位。

sbit CCF0  = CCON^0;	//PCA 模块0中断标志，由硬件置位，必须由软件清0。
sbit CCF1  = CCON^1;	//PCA 模块1中断标志，由硬件置位，必须由软件清0。
sbit CR    = CCON^6;	//1: 允许PCA计数器计数，必须由软件清0。
sbit CF    = CCON^7;	//PCA计数器溢出（CH，CL由FFFFH变为0000H）标志。PCA计数器溢出后由硬件置位，必须由软件清0。

#define		PWM0_NORMAL()	PCA_PWM0 = 0	//PWM0正常输出(默认)
#define		PWM0_OUT_0()	PCA_PWM0 = 3	//PWM0一直输出0
#define		PWM1_NORMAL()	PCA_PWM1 = 0	//PWM1正常输出(默认)
#define		PWM1_OUT_0()	PCA_PWM1 = 3	//PWM1一直输出0


/*************	本地函数声明	**************/

void	en_PCA(void);
void	en_PWM(uchar channel,uchar start_value);

void	main(void)
{

	en_PWM(0,128);		//load PWM value
	en_PWM(1,128);		//load PWM value
	en_PCA();			//enable PCA

	while (1)
	{

	}
}




//Note: CCAP0H and CCAP1H load the PWM output low time.

void	en_PCA(void)
{
	CMOD = (CMOD & 0x81) + 4;	//PCA clocj select Timer0 overfollow，Timer0 can select 1T mode or 12T through set AUXR.
	AUXR |=  (1<<7);	//Timer0 set as 1T
	TMOD |= 0x02;		//Timer0 set as 8 bit reload
	TH0 = 256- 10;		//Set reload value
	TR0 = 1;			//Start Timer0

	CL = 0x00;		//clear PCA counter
	CH = 0x00;
	CR = 1;			//Start PCA counter
}

void	en_PWM(uchar channel,uchar start_value)
{
	if(channel == 0)
	{
		CCAP0L = start_value;	//Set the initial value same as CCAP0H
		CCAP0H = start_value;	//25% Duty Cycle
		CCAPM0 = 0x42;			//0x42 Setup PCA module 0 in PWM mode
	}

	if(channel == 1)
	{
		CCAP1L = start_value;	//Set the initial value same as CCAP1H
		CCAP1H = start_value;	//25% Duty Cycle
		CCAPM1 = 0x42;			//0x42 Setup PCA module 1 in PWM mode
	}
}
