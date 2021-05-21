
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

	ͨ������������MCU�������ݣ�MCU�յ���ԭ�����ء�
*/

#include	<reg51.h>

sfr	AUXR = 0x8E;
sfr BRT   = 0x9C;

#define		BUF_LENTH	128		//���崮�ڽ��ջ��峤��
unsigned char 	uart0_wr;		//дָ��
unsigned char 	uart0_rd;		//��ָ��
unsigned char 	xdata RX0_Buffer[BUF_LENTH];	//���ջ���
bit		B_TI;

void	uart0_init(void);



/*************** �û�������� *****************************/

#define MAIN_Fosc		22118400UL	//define main clock
#define Baudrate0		9600UL		//define the baudrate,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ
									//1T  mode: 7200~1382400 for 22.1184MHZ, 3600~691200 for 11.0592MHZ

/**********************************************************/


/****************** �������Զ����ɣ��û������޸� ************************************/

#define BRT_Reload			(256 - MAIN_Fosc / 16 / Baudrate0)		//Calculate the timer1 reload value ar 1T mode


/**********************************************************/


void	main(void)
{
	uart0_init();
	
	while(1)
	{
		if(uart0_rd != uart0_wr)	//����ת��
		{
			B_TI = 0;
			SBUF = RX0_Buffer[uart0_rd];
			while(!B_TI)	;
			B_TI = 0;
			if(++uart0_rd >= BUF_LENTH)		uart0_rd = 0;
		}
	}
}



void	uart0_init(void)
{
	PCON |= 0x80;		//UART0 Double Rate Enable
	SCON = 0x50;		//UART0 set as 10bit , UART0 RX enable
	AUXR |=  0x01;		//UART0 ʹ��BRT
	AUXR |=  0x04;		//BRT set as 1T mode
	BRT = BRT_Reload;
	AUXR |=  0x10;		//start BRT	

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
