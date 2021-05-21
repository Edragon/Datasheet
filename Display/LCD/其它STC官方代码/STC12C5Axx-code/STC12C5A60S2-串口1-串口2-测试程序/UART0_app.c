
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

	���Է�����

	����������ʱ�ӺͲ������޸�"�û��������"�����ض��壬��������ص�MCU��

	ͨ������������MCU�������ݣ�MCU�յ���ԭ�����ء�
*/


/*************** �û�������� *****************************/

#define MAIN_Fosc		22118400L	//define main clock

#define Baudrate1		9600		//define the baudrate, ���ʹ��BRT�������ʷ�����,�����ʸ�����2һ��
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ

#define Baudrate2		19200		//define the baudrate2,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ

#define		BUF_LENTH	128		//���崮�ڽ��ջ��峤��

/**********************************************************/

#include	<reg51.h>

sfr AUXR1 = 0xA2;
sfr	AUXR = 0x8E;
sfr S2CON = 0x9A;	//12C5A60S2˫����ϵ��
sfr S2BUF = 0x9B;	//12C5A60S2˫����ϵ��
sfr IE2   = 0xAF;	//STC12C5A60S2ϵ��
sfr BRT   = 0x9C;

unsigned char 	uart1_wr;		//дָ��
unsigned char 	uart1_rd;		//��ָ��
unsigned char 	xdata RX1_Buffer[BUF_LENTH];
bit		B_TI;

unsigned char 	uart2_wr;		//дָ��
unsigned char 	uart2_rd;		//��ָ��
unsigned char 	xdata RX2_Buffer[BUF_LENTH];
bit		B_TI2;


/****************** �������Զ����ɣ��û������޸� ************************************/

#define T1_TimerReload	(256 - MAIN_Fosc / 192 / Baudrate1)			//Calculate the timer1 reload value	at 12T mode
#define BRT_Reload		(256 - MAIN_Fosc / 12 / 16 / Baudrate2)		//Calculate BRT reload value

#define	TimeOut1		(28800 / (unsigned long)Baudrate1 + 2)
#define	TimeOut2		(28800 / (unsigned long)Baudrate2 + 2)

#define	TI2				(S2CON & 0x02) != 0
#define	RI2				(S2CON & 0x01) != 0
#define	CLR_TI2()		S2CON &= ~0x02
#define	CLR_RI2()		S2CON &= ~0x01

/**********************************************************/

/******************** ���غ������� ***************/
void	uart1_init(void);
void	uart2_init(void);
void	UART1_TxByte(unsigned char dat);
void	UART2_TxByte(unsigned char dat);
void	PrintString1(unsigned char code *puts);
void	PrintString2(unsigned char code *puts);



void	main(void)
{
	uart1_rd = 0;
	uart1_wr = 0;
	uart2_rd = 0;
	uart2_wr = 0;

//	AUXR |=  0x01;		//����1ʹ�ö��������ʷ�����, �����ʸ�����2һ��
	AUXR1 |= (1<<4);	//��UART2��P1���л��� RXD2--P1.2�л���P4.2   TXD2---P1.3�л���P4.3
	
	uart1_init();
	uart2_init();
	
	PrintString1("����1���Գ���");
	PrintString2("����2���Գ���");
	
	while(1)
	{
		if(uart1_rd != uart1_wr)	//����0ת��
		{
			UART1_TxByte(RX1_Buffer[uart1_rd]);
			if(++uart1_rd >= BUF_LENTH)		uart1_rd = 0;
		}

		if(uart2_rd != uart2_wr)	//����2ת��
		{
			UART2_TxByte(RX2_Buffer[uart2_rd]);
			if(++uart2_rd >= BUF_LENTH)		uart2_rd = 0;
		}
	}
}

void	UART1_TxByte(unsigned char dat)
{
	B_TI = 0;
	SBUF = dat;
	while(!B_TI);
	B_TI = 0;
}

void	UART2_TxByte(unsigned char dat)
{
	B_TI2 = 0;
	S2BUF = dat;
	while(!B_TI2);
	B_TI2 = 0;
}

void PrintString1(unsigned char code *puts)		//����һ���ַ���
{
    for (; *puts != 0;	puts++)  UART1_TxByte(*puts); 	//����ֹͣ��0����
}

void PrintString2(unsigned char code *puts)		//����һ���ַ���
{
    for (; *puts != 0;	puts++)  UART2_TxByte(*puts); 	//����ֹͣ��0����
}



void	uart1_init(void)
{
	PCON |= 0x80;		//UART0 Double Rate Enable
	SCON = 0x50;		//UART0 set as 10bit , UART0 RX enable
	TMOD &= ~(1<<6);		//Timer1 Set as Timer, 12T
	TMOD = (TMOD & ~0x30) | 0x20;	//Timer1 set as 8 bits auto relaod
	TH1 = T1_TimerReload;		//Load the timer
	TR1  = 1;
	ES  = 1;
	EA = 1;
}



/**********************************************/
void UART0_RCV (void) interrupt 4
{
	if(RI)
	{
		RI = 0;
		RX1_Buffer[uart1_wr] = SBUF;
		if(++uart1_wr >= BUF_LENTH)	uart1_wr = 0;
	}

	if(TI)
	{
		TI = 0;
		B_TI = 1;
	}
}


/**********************************************/
void	uart2_init(void)
{
	AUXR |=  (1 << 3);		//����2�����ʼӱ�
	S2CON  = (S2CON & 0x3f) | (1<<6);	//����2ģʽ1��8λUART��(2^S2SMOD / 32) * BRT�����
	S2CON |= 1 << 4;		//����2����

	AUXR |=  1 << 4;	//baudrate use BRT
	BRT = BRT_Reload;

	IE2 |=  1;			//������2�ж�
}

/**********************************************/
void UART2_RCV (void) interrupt 8
{
	if(RI2)
	{
		CLR_RI2();
		RX2_Buffer[uart2_wr] = S2BUF;
		if(++uart2_wr >= BUF_LENTH)	uart2_wr = 0;
	}

	if(TI2)
	{
		CLR_TI2();
		B_TI2 = 1;
	}
}
