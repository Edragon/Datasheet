

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

使用24MHZ时钟时，Timer0从P3.4  Timer1从P3.5分别输出100KHZ和200KHZ的方波.

******************************************/


#include	<reg51.h>

sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain
sfr	AUXR = 0x8E;
sfr WAKE_CLKO = 0x8F;

void main(void)
{
	P3M1 &= ~((1<<4) | (1<<5));	//Set P3.4 P3.5 Push-Pull Output
	P3M0 |=  ((1<<4) | (1<<5));

	TMOD = (TMOD & ~0x03) | 0x02;	//T0 set as 8bit reload
	AUXR |= (1<<7);		//T0 1T mode
	TH0 = 256 - 120;
	WAKE_CLKO |=  1;	//T0 pulseP34 enable
	TR0 = 1;
	
	TMOD = (TMOD & ~0x30) | 0x20;	//T1 set as 8bit reload
	AUXR |= (1<<6);		//T1 1T mode
	TH1 = 256 - 60;
	WAKE_CLKO |=  2;	//T1 pulse P35 enable
	TR1 = 1;
	
	while(1)
	{
	
	}
}	
	
	
	
