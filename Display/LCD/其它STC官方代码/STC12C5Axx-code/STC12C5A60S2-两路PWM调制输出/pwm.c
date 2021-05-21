
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

��·PWM�������.

**********************************************/


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

bit		B_T0_interrupt;

void	main(void)
{
	uint	i;

	en_PWM(0,128);	//load PWM value
	en_PWM(1,128);	//load PWM value
	en_PCA();		//enable PCA
	EA  = 1;		//enable all interrupt

	while (1)
	{
		TMOD = 0x02;	//T0 as 8 bit reload
		TH0  = 0 - 10;	//Reload time
		AUXR |=  0x80;	//T0 as 1T
		ET0 = 0;		//disable T0 interrupt
		TR0 = 1;		//enable T0
		PWM0_NORMAL();	//PWM0 normal output
		PWM1_NORMAL();	//PWM1 normal output
		for(i=0; i<10000; i++)	;	//delay for output PWM
		TR0 = 0;		//disable T0
		PWM0_OUT_0();	//PWM0 stop and putput 0
		PWM1_OUT_0();	//PWM1 stop and putput 0
		
		TMOD = 0x01;	//T0 as 16bit timer
		TL0  = (0 - Timer0_Reload) % 256;	//load the 16bit time
		TH0  = (0 - Timer0_Reload) / 256;
	
		AUXR &=  ~0x80;	//T0 as 12T
		ET0 = 1;		//enable T0 interrupt
		TR0 = 1;		//enable T0
		B_T0_interrupt = 0;	//clear the T0 interrupt flag
		while(!B_T0_interrupt);	//wait the T0 interrupt
		TR0 = 0;		//disable T0
	}
}



/**********************************************/
void timer0 (void) interrupt 1
{
	B_T0_interrupt = 1;
}



//Note: CCAP0H and CCAP1H load the PWM output low time.

void	en_PCA(void)
{
	CMOD = (CMOD & 0x81) + 4;	//PCA clocj select Timer0 overfollow��Timer0 can select 1T mode or 12T through set AUXR.
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
