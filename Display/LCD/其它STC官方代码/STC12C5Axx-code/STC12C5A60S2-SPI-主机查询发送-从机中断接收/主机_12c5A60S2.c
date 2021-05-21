
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

	����������ʱ�ӺͲ������޸��������ض��壬��������ص�MCU��

	ͨ������������MCU�������ݣ�MCU�յ���ͨ��SPI�����ӻ������ڲ����� 115200,8,n,1 at 22.1184MHZ
*/


#include	<reg51.h>

sfr	AUXR  = 0x8E;

#define		BUF_LENTH	64		//���崮�ڽ��ջ��峤��
unsigned char 	uart0_wr;		//дָ��
unsigned char 	uart0_rd;		//��ָ��
unsigned char 	idata RX0_Buffer[BUF_LENTH];
bit		B_TI;

sbit	SPI_SCL		= P1^7;	//SPI��ͬ��ʱ��
sbit	SPI_MISO	= P1^6;	//SPI��ͬ������
sbit	SPI_MOSI	= P1^5;	//SPI��ͬ������
sbit	SPI_CS		= P1^4;	//SPI��Ƭѡ
sfr AUXR1 = 0xA2;

sfr SPSTAT = 0xCD;
sfr SPCTL  = 0xCE;
sfr SPDAT  = 0xCF;

//sfr SPCTL   = 0x85;	SPI���ƼĴ���
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00
#define	SSIG		1	//1: ����SS�ţ���MSTRλ�����������Ǵӻ�		0: SS�����ھ������ӻ���
#define	SPEN		1	//1: ����SPI��								0����ֹSPI������SPI�ܽž�Ϊ��ͨIO
#define	DORD		0	//1��LSB�ȷ���								0��MSB�ȷ�
#define	MSTR		1	//1����Ϊ����								0����Ϊ�ӻ�
#define	CPOL		1	//1: ����ʱSCLKΪ�ߵ�ƽ��					0������ʱSCLKΪ�͵�ƽ
#define	CPHA		1	//
#define	SPR1		0	//SPR1,SPR0   00: fosc/4,     01: fosc/16
#define	SPR0		0	//            10: fosc/64,    11: fosc/128
#define	SPEED_4		0	// fosc/4
#define	SPEED_16	1	// fosc/16
#define	SPEED_64	2	// fosc/64
#define	SPEED_128	3	// fosc/128

//sfr SPSTAT  = 0xCD;	//SPI״̬�Ĵ���
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define	SPIF	0x80		//SPI������ɱ�־��д��1��0��
#define	WCOL	0x40		//SPIд��ͻ��־��д��1��0��


/**********************************************/
void	SPI_init(void)	//����SPI
{
	SPCTL = (SSIG << 7) + (SPEN << 6) + (DORD << 5) + (MSTR << 4) + (CPOL << 3) + (CPHA << 2) + SPEED_64;
}

void	uart0_init(void);

/*************** �û�������� *****************************/

#define MAIN_Fosc		22118400L	//define main clock
#define Baudrate0		115200L		//define the baudrate,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ
									//1T  mode: 7200~1382400 for 22.1184MHZ, 3600~691200 for 11.0592MHZ

/**********************************************************/


/****************** �������Զ����ɣ��û������޸� ************************************/

#define T1_TimerReload	(256 - MAIN_Fosc / 192 / Baudrate0)			//Calculate the timer1 reload value	at 12T mode

/**********************************************************/


void	main(void)
{
	uart0_init();
	SPI_init();
	
	while(1)
	{
		if(uart0_rd != uart0_wr)	//����0ת��
		{
			SPI_CS = 0;							//�����ⲿPSI����
			SPSTAT = SPIF + WCOL;			//��0 SPIF��WCOL��־
			SPDAT = RX0_Buffer[uart0_rd];	//����һ���ֽ�
			while((SPSTAT & SPIF) == 0)	;	//�ȴ��������
			SPSTAT = SPIF + WCOL;				//��0 SPIF��WCOL��־
			SPI_CS = 1;							//��ֹ�ⲿPSI����

			if(++uart0_rd >= BUF_LENTH)		uart0_rd = 0;
		}
	}
}


void	uart0_init(void)
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
		RX0_Buffer[uart0_wr] = SBUF;
		if(++uart0_wr >= BUF_LENTH)	uart0_wr = 0;
	}

	if(TI)
	{
		TI = 0;
		B_TI = 1;
	}
}
