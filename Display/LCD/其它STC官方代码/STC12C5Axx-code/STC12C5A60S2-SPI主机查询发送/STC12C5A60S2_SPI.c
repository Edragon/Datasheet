
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

ÿ��50ms����SPI����32���ֽ�����.

******************************************/


#include	<reg51.h>

#define		MAIN_Fosc	22118400L	//����ʱ��

sbit	SPI_SCL		= P1^7;	//SPI��ͬ��ʱ��
sbit	SPI_MISO	= P1^6;	//SPI��ͬ������
sbit	SPI_MOSI	= P1^5;	//SPI��ͬ������
sbit	SPI_CS		= P1^4;	//SPI��Ƭѡ
sfr AUXR1 = 0xA2;

unsigned char 	xdata SPI_TxBuffer[32];
unsigned char 	xdata SPI_RxBuffer[32];


sfr SPSTAT = 0xCD;	//STC12C5A60S2ϵ��
sfr SPCTL  = 0xCE;	//STC12C5A60S2ϵ��
sfr SPDAT  = 0xCF;	//STC12C5A60S2ϵ��

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
	SPCTL = (SSIG << 7) + (SPEN << 6) + (DORD << 5) + (MSTR << 4) + (CPOL << 3) + (CPHA << 2) + SPEED_16;
}

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
	unsigned char i;
	
	delay_ms(100);		//��ʱһ�£����ⲿSPI����׼����
	
	AUXR1 |= (1<<5);	//SPI�л���P4��
	SPI_init();			//����Ϊ����
	
	for(i=0; i<32; i++)		SPI_TxBuffer[i] = i;	//װ��Ҫ���͵�����
	
	while(1)
	{
		delay_ms(50);
		
		SPI_CS = 0;							//�����ⲿPSI����
		for(i=0; i<32; i++)
		{
			SPSTAT = SPIF + WCOL;			//��0 SPIF��WCOL��־
			SPDAT = SPI_TxBuffer[i];		//����һ���ֽ�
			while((SPSTAT & SPIF) == 0)	;	//�ȴ��������
			SPI_RxBuffer[i] = SPDAT;		//���շ��ص��ֽ�
		}
		SPSTAT = SPIF + WCOL;				//��0 SPIF��WCOL��־
		SPI_CS = 1;							//��ֹ�ⲿPSI����
	
	}

}