
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU RC Demo -----------------------------------*/
/* --- Mobile: (86)13922805190 -------------------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*
	���Է�����
	����������ʱ�ӺͲ������޸��������ض��壬��������ص�MCU��
	ͨ������������MCU�������ݣ�MCU�յ���ԭ�����ء�
	Timer0�жϲ���, ȡ��һ��IO�����
*/

#include	<reg51.h>
#include	<math.h>
#include	<stdlib.h>

sfr	AUXR  = 0x8E;
sfr BRT   = 0x9C;

#define		BUF_LENTH	128		//���崮�ڽ��ջ��峤��
unsigned char 	uart0_wr;		//дָ��
unsigned char 	uart0_rd;		//��ָ��
unsigned char 	xdata RX0_Buffer[BUF_LENTH];
bit		B_TI;

sbit	P_PULSE = P1^0;		//Timer0�ж�ȡ��һ��IO�������.


/*************** �û�������� *****************************/

//#define MAIN_Fosc		22118400L	//define main clock
#define MAIN_Fosc		11059200L	//define main clock
//#define MAIN_Fosc		18432000L	//define main clock

#define Baudrate0		9600		//define the baudrate,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ

#define	Timer0_Devide	12			//	Timer0��Ƶϵ����1: 1T,  12: 12T
#define D_TIMER0		10000		//	ѡ��ʱ��ʱ�䣬������С25us

/**********************************************************/


/****************** �������Զ������û������޸� ************************************/

#define T1_TimerReload	(256 - MAIN_Fosc / 192 / Baudrate0)			//Calculate the timer1 reload value	at 12T mode

#if (D_TIMER0 >= 25)
	#define Timer0_CLK			(MAIN_Fosc / 100 / Timer0_Devide)
	#define Timer0_CNT			(D_TIMER0 * Timer0_CLK / 10000)
	#if (Timer0_CNT >= 65536)
		#define Timer0_Reload	65535
	#else
		#define Timer0_Reload	Timer0_CNT
	#endif
#endif

/**********************************************************/



/******************** ���غ������� ***************/
void	uart0_init(void);
void	Timer0_init(void);



void UART0_TxByte(unsigned char dat)
{
	B_TI = 0;
	SBUF = dat;
	while(!B_TI)	;
	B_TI = 0;
}

void	main(void)
{

	Timer0_init();
	uart0_init();
	
//	AUXR1 |= 0x80;		//UART0 switch to P1.7 P1.6

	while(1)
	{
		if(uart0_rd != uart0_wr)	//����0ת��
		{
			UART0_TxByte(RX0_Buffer[uart0_rd]);
			if(++uart0_rd >= BUF_LENTH)		uart0_rd = 0;
		}
	}
}


//========================================================================
// ����: void	Timer0_init(void)
// ����: ��ʼ��Timer0��
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2010-12-15
// ��ע: 
//========================================================================

void	Timer0_init(void)
{
	#if (Timer0_Devide == 1)
		AUXR |= (1 << 7);	//1T mode
	#endif

	#if (Timer0_Reload >= 256)
		TMOD = (TMOD & ~0x03) | 0x01;		//Timer0���ó�16λ
		TH0  = (65536-Timer0_Reload) / 256;	//Timer0 16 bits
		TL0  = (65536-Timer0_Reload) % 256;
	#else
		TMOD = (TMOD & ~0x03) | 0x02;		//Timer0���ó�8λ�Զ���װ
		TH0  = 256 - Timer0_Reload;		//Timer0 8 bits auto re-load
	#endif

	ET0 = 1;			//����T0�ж�
	TR0 = 1;			//����T0
	EA  = 1;			//����ȫ���ж�
}



//========================================================================
// ����: void timer0 (void) interrupt 1
// ����: Timer0�жϺ�����
// ����: none.
// ����: none.
// �汾: VER1.0
// ����: 2010-12-15
// ��ע: 
//========================================================================
void timer0 (void) interrupt 1
{
	#if (Timer0_Reload >= 256)
		TR0 = 0;
		TH0  = (65536-Timer0_Reload) / 256;	//Timer0 16 bits
		TL0  = (65536-Timer0_Reload) % 256;
		TR0 = 1;
	#endif

	P_PULSE = ~P_PULSE;
}

/*************** UART0 init ***************/
void	uart0_init(void)
{
	PCON |= 0x80;		//UART0 Double Rate Enable
	SCON = 0x50;		//UART0 set as 10bit , UART0 RX enable
	TMOD &= ~(1<<6);		//Timer1 Set as Timer, 12T
	TMOD = (TMOD & ~0x30) | 0x20;	//Timer1 set as 8 bits auto relaod
	TH1 = T1_TimerReload;		//Load the timer
	TR1  = 1;
	ES  = 1;
}


/**********************************************/
void UART0_RCV (void) interrupt 4
{
	if(RI)
	{
		RI = 0;
		RX0_Buffer[uart0_wr] = SBUF;
		if(++uart0_wr >= BUF_LENTH)	uart0_wr = 0;
	}

	if(TI)
	{
		TI = 0;
		B_TI = 1;
	}
}


