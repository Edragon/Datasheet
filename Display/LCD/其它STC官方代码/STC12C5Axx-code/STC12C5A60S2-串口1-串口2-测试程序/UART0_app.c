
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

	根据您的主时钟和波特率修改"用户定义参数"里的相关定义，编译后下载到MCU。

	通过串口助手向MCU发送数据，MCU收到后原样返回。
*/


/*************** 用户定义参数 *****************************/

#define MAIN_Fosc		22118400L	//define main clock

#define Baudrate1		9600		//define the baudrate, 如果使用BRT做波特率发生器,则波特率跟串口2一样
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ

#define Baudrate2		19200		//define the baudrate2,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ

#define		BUF_LENTH	128		//定义串口接收缓冲长度

/**********************************************************/

#include	<reg51.h>

sfr AUXR1 = 0xA2;
sfr	AUXR = 0x8E;
sfr S2CON = 0x9A;	//12C5A60S2双串口系列
sfr S2BUF = 0x9B;	//12C5A60S2双串口系列
sfr IE2   = 0xAF;	//STC12C5A60S2系列
sfr BRT   = 0x9C;

unsigned char 	uart1_wr;		//写指针
unsigned char 	uart1_rd;		//读指针
unsigned char 	xdata RX1_Buffer[BUF_LENTH];
bit		B_TI;

unsigned char 	uart2_wr;		//写指针
unsigned char 	uart2_rd;		//读指针
unsigned char 	xdata RX2_Buffer[BUF_LENTH];
bit		B_TI2;


/****************** 编译器自动生成，用户请勿修改 ************************************/

#define T1_TimerReload	(256 - MAIN_Fosc / 192 / Baudrate1)			//Calculate the timer1 reload value	at 12T mode
#define BRT_Reload		(256 - MAIN_Fosc / 12 / 16 / Baudrate2)		//Calculate BRT reload value

#define	TimeOut1		(28800 / (unsigned long)Baudrate1 + 2)
#define	TimeOut2		(28800 / (unsigned long)Baudrate2 + 2)

#define	TI2				(S2CON & 0x02) != 0
#define	RI2				(S2CON & 0x01) != 0
#define	CLR_TI2()		S2CON &= ~0x02
#define	CLR_RI2()		S2CON &= ~0x01

/**********************************************************/

/******************** 本地函数声明 ***************/
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

//	AUXR |=  0x01;		//串口1使用独立波特率发生器, 波特率跟串口2一样
	AUXR1 |= (1<<4);	//将UART2从P1口切换到 RXD2--P1.2切换到P4.2   TXD2---P1.3切换到P4.3
	
	uart1_init();
	uart2_init();
	
	PrintString1("串口1测试程序");
	PrintString2("串口2测试程序");
	
	while(1)
	{
		if(uart1_rd != uart1_wr)	//串口0转发
		{
			UART1_TxByte(RX1_Buffer[uart1_rd]);
			if(++uart1_rd >= BUF_LENTH)		uart1_rd = 0;
		}

		if(uart2_rd != uart2_wr)	//串口2转发
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

void PrintString1(unsigned char code *puts)		//发送一串字符串
{
    for (; *puts != 0;	puts++)  UART1_TxByte(*puts); 	//遇到停止符0结束
}

void PrintString2(unsigned char code *puts)		//发送一串字符串
{
    for (; *puts != 0;	puts++)  UART2_TxByte(*puts); 	//遇到停止符0结束
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
	AUXR |=  (1 << 3);		//串口2波特率加倍
	S2CON  = (S2CON & 0x3f) | (1<<6);	//串口2模式1，8位UART，(2^S2SMOD / 32) * BRT溢出率
	S2CON |= 1 << 4;		//允许串2接收

	AUXR |=  1 << 4;	//baudrate use BRT
	BRT = BRT_Reload;

	IE2 |=  1;			//允许串口2中断
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
