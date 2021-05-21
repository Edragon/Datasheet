
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

����Timer0�ж�ȡ��P1.1�����Լ100HZ����, ��P1.2���������. 

����BRT��������Լ10KHZ������5ms�źŵ��ơ�


******************************************/


/*************	�û�ϵͳ����	**************/

#define MAIN_Fosc		22118400L	//������ʱ��, ��ʱ���Զ���Ӧ��5~35MHZ
#define D_TIMER0		5000		//	ѡ��ʱ��ʱ��us��ע�ⲻҪ��������ʱ����ᱨ��

/******************************************/

#define Timer0_Reload_s			((MAIN_Fosc / 1000) * D_TIMER0 / 12000)
#if (Timer0_Reload_s < 65536)
	#define	Timer0_Reload		(65536 - Timer0_Reload_s)
#endif


#include	"reg51.H"

sfr	AUXR = 0x8E;
sfr WAKE_CLKO = 0x8F;
sfr BRT   = 0x9C;

/*************	���ر�������	**************/
sbit	P10 = P1^0;
sbit	P11 = P1^1;
sbit	P12 = P1^2;
unsigned char ms_cnt;

/*************	���غ�������	**************/
void  	delay_ms(unsigned char ms);


//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��.
// ����: none.
// �汾: VER1.0
// ����: 2010-12-15
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}





void main(void)
{
	WAKE_CLKO |=  (1<<2);	//Enable BRT output frequency from P1.0
	BRT = 256 - 92;			//10KHZ @ 22.118400HZ 12T mode
	AUXR |=  (1<<4);		//Enable BRT
	
	TMOD |= 0x01;			//Timer0 set as 16bit timer
	TH0  = Timer0_Reload / 256;
	TL0  = Timer0_Reload % 256;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
	
	while(1)
	{
		AUXR |=  (1<<4);	//Enable BRT
		delay_ms(5);
		AUXR &=  ~(1<<4);	//Disable BRT
		P10 = 0;			//ǿ�����0��ƽ
		delay_ms(5);
	
	}
}


/**********************************************/
void timer0 (void) interrupt 1
{
	TR0 = 0;
	TH0  = Timer0_Reload / 256;
	TL0  = Timer0_Reload % 256;
	TR0 = 1;
	
	P11 = ~P11;		//Timer0�ж�ȡ��P1.1��� 100HZ
	if(++ms_cnt >= 100)
	{
		ms_cnt = 0;
		P12 = ~P12;		//P1.2���������
	}
}
	
	
	
	