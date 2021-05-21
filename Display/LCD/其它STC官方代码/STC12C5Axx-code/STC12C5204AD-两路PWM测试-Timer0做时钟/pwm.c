
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU Programme Demo ----------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*      �����򾭹�������ȫ����, ���ṩ�绰����֧��, �粻�����, �����в�����ػ���.  */


/*************	��������˵��	**************

����Ӳ��PWM. ����STC12C5204ADϵ�С�

����ͨ������Timer0����������ı�PWM���ڡ�

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
sfr CCAP0L = 0xEA;	//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1L = 0xEB;	//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��
sfr PCA_PWM0 = 0xF2;	//PCAģ��0 PWM�Ĵ�����
sfr PCA_PWM1 = 0xF3;	//PCAģ��1 PWM�Ĵ�����
sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;		//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1H = 0xFB;		//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��

sbit CCF0  = CCON^0;	//PCA ģ��0�жϱ�־����Ӳ����λ�������������0��
sbit CCF1  = CCON^1;	//PCA ģ��1�жϱ�־����Ӳ����λ�������������0��
sbit CR    = CCON^6;	//1: ����PCA�����������������������0��
sbit CF    = CCON^7;	//PCA�����������CH��CL��FFFFH��Ϊ0000H����־��PCA�������������Ӳ����λ�������������0��

#define		PWM0_NORMAL()	PCA_PWM0 = 0	//PWM0�������(Ĭ��)
#define		PWM0_OUT_0()	PCA_PWM0 = 3	//PWM0һֱ���0
#define		PWM1_NORMAL()	PCA_PWM1 = 0	//PWM1�������(Ĭ��)
#define		PWM1_OUT_0()	PCA_PWM1 = 3	//PWM1һֱ���0


/*************	���غ�������	**************/

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
	CMOD = (CMOD & 0x81) + 4;	//PCA clocj select Timer0 overfollow��Timer0 can select 1T mode or 12T through set AUXR.
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
