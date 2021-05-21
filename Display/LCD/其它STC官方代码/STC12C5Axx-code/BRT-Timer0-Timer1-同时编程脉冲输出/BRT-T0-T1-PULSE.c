
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

测试BRT输出脉冲大约		100.538KHZ @ 22.118400HZ。
测试Timer0输出脉冲大约	50.269KHZ  @ 22.118400HZ。
测试Timer1输出脉冲大约	201.076KHZ @ 22.118400HZ。

******************************************/


/*************	用户系统配置	**************/

#define MAIN_Fosc		22118400L	//定义主时钟, 延时会自动适应。5~35MHZ
#define D_TIMER0		5000			//	选择定时器时间，注意不要溢出，溢出时编译会报错。

/******************************************/

#define Timer0_Reload_s			((MAIN_Fosc / 1000) * D_TIMER0 / 12000)
#if (Timer0_Reload_s < 65536)
	#define	Timer0_Reload		(65536 - Timer0_Reload_s)
#endif


#include	<reg51.H>

sfr	AUXR = 0x8E;
sfr WAKE_CLKO = 0x8F;
sfr BRT   = 0x9C;

sfr P1M1 = 0x91;	//P1M1.n,P1M0.n 	= 00--->Standard,	01--->push-pull
sfr P1M0 = 0x92;	//					= 10--->pure input,	11--->open drain
sfr P3M1 = 0xB1;	//P3M1.n,P3M0.n 	= 00--->Standard,	01--->push-pull
sfr P3M0 = 0xB2;	//					= 10--->pure input,	11--->open drain



void main(void)
{
	P1M1 &= ~1;	//Set P1.0 Push-Pull Output
	P1M0 |=  1;
	P3M1 &= ~((1<<4) | (1<<5));	//Set P3.4 P3.5 Push-Pull Output
	P3M0 |=  ((1<<4) | (1<<5));

	AUXR |=  (1<<2);		//BRT 1T mode
	WAKE_CLKO |=  (1<<2);	//Enable BRT output frequency from P1.0
	BRT = 256 - 110;		//100.538KHZ @ 22.118400HZ
	AUXR |=  (1<<4);		//Enable BRT
	
	TMOD = (TMOD & ~0x03) | 0x02;	//T0 set as 8bit reload
	AUXR |= (1<<7);		//T0 1T mode
	TH0 = 256 - 220;	//50.269KHZ @ 22.118400HZ
	WAKE_CLKO |=  1;	//T0 pulse P34 enable
	TR0 = 1;
	
	TMOD = (TMOD & ~0x30) | 0x20;	//T1 set as 8bit reload
	AUXR |= (1<<6);		//T1 1T mode
	TH1 = 256 - 55;		//201.076KHZ @ 22.118400HZ
	WAKE_CLKO |= 2;		//T1 pulse P35 enable
	TR1 = 1;
	
	while(1)
	{
	
	}
}


	