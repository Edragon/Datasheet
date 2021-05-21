
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
	测试方法：

	根据您的主时钟和波特率修改下面的相关定义，编译后下载到MCU。

	通过串口助手向MCU发送数据，MCU收到后原样返回。
*/

#include	<reg51.h>

sfr	AUXR = 0x8E;

#define		BUF_LENTH	128		//定义串口接收缓冲长度
unsigned char 	RX0_Cnt;
unsigned char	RX0_TimerOut;
unsigned char 	xdata RX0_Buffer[BUF_LENTH];
bit		B_TI;

void	uart0_init(void);



/*************** 用户定义参数 *****************************/

#define MAIN_Fosc		18432000L	//define main clock
#define Baudrate0		9600		//define the baudrate,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ
									//1T  mode: 7200~1382400 for 22.1184MHZ, 3600~691200 for 11.0592MHZ

/**********************************************************/


/****************** 编译器自动生成，用户请勿修改 ************************************/

#define T1_TimerReload		(256 - MAIN_Fosc / 192 / Baudrate0)		//Calculate the timer1 reload value	at 12T mode
//#define T1_TimerReload	(256 - MAIN_Fosc / 16 / Baudrate0)		//Calculate the timer1 reload value ar 1T mode

#define	TimeOut				(28800 / (unsigned long)Baudrate0 + 2)
/**********************************************************/


void	main(void)
{
	unsigned char i;
	unsigned int  j;
	uart0_init();
	
	while(1)
	{
		for(j=0; j<1500; j++)	;	//14T per loop, delay 1ms
		if(RX0_TimerOut != 0)
		{
			if(--RX0_TimerOut == 0)
			{
				if(RX0_Cnt != 0)
				{
					for(i=0; i<RX0_Cnt; i++)
					{
						B_TI = 0;
						SBUF = RX0_Buffer[i];
						while(!B_TI)	;
						B_TI = 0;
					}
				}
				RX0_Cnt = 0;
			}
		}
	}
}



void	uart0_init(void)
{
	PCON |= 0x80;		//UART0 Double Rate Enable
	SCON = 0x50;		//UART0 set as 10bit , UART0 RX enable
	TMOD &= ~(1<<6);		//Timer1 Set as Timer, 12T
	TMOD = (TMOD & ~0x30) | 0x20;	//Timer1 set as 8 bits auto relaod
//	AUXR |=  (1<<6);		//Timer1 set as 1T mode
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
		if(RX0_Cnt >= BUF_LENTH)		RX0_Cnt = 0;
		RX0_Buffer[RX0_Cnt++] = SBUF;
		RX0_TimerOut = TimeOut;	//5ms time out for >= 4800
	}

	if(TI)
	{
		TI = 0;
		B_TI = 1;
	}
}
