
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

	通过串口助手向MCU发送数据，MCU收到后通过SPI发给从机。串口波特率 115200,8,n,1 at 22.1184MHZ
*/


#include	<reg51.h>

sfr	AUXR  = 0x8E;

#define		BUF_LENTH	64		//定义串口接收缓冲长度
unsigned char 	uart0_wr;		//写指针
unsigned char 	uart0_rd;		//读指针
unsigned char 	idata RX0_Buffer[BUF_LENTH];
bit		B_TI;

sbit	SPI_SCL		= P1^7;	//SPI卡同步时钟
sbit	SPI_MISO	= P1^6;	//SPI卡同步数据
sbit	SPI_MOSI	= P1^5;	//SPI卡同步数据
sbit	SPI_CS		= P1^4;	//SPI卡片选
sfr AUXR1 = 0xA2;

sfr SPSTAT = 0xCD;
sfr SPCTL  = 0xCE;
sfr SPDAT  = 0xCF;

//sfr SPCTL   = 0x85;	SPI控制寄存器
//   7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00
#define	SSIG		1	//1: 忽略SS脚，由MSTR位决定主机还是从机		0: SS脚用于决定主从机。
#define	SPEN		1	//1: 允许SPI，								0：禁止SPI，所有SPI管脚均为普通IO
#define	DORD		0	//1：LSB先发，								0：MSB先发
#define	MSTR		1	//1：设为主机								0：设为从机
#define	CPOL		1	//1: 空闲时SCLK为高电平，					0：空闲时SCLK为低电平
#define	CPHA		1	//
#define	SPR1		0	//SPR1,SPR0   00: fosc/4,     01: fosc/16
#define	SPR0		0	//            10: fosc/64,    11: fosc/128
#define	SPEED_4		0	// fosc/4
#define	SPEED_16	1	// fosc/16
#define	SPEED_64	2	// fosc/64
#define	SPEED_128	3	// fosc/128

//sfr SPSTAT  = 0xCD;	//SPI状态寄存器
//   7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define	SPIF	0x80		//SPI传输完成标志。写入1清0。
#define	WCOL	0x40		//SPI写冲突标志。写入1清0。


/**********************************************/
void	SPI_init(void)	//配置SPI
{
	SPCTL = (SSIG << 7) + (SPEN << 6) + (DORD << 5) + (MSTR << 4) + (CPOL << 3) + (CPHA << 2) + SPEED_64;
}

void	uart0_init(void);

/*************** 用户定义参数 *****************************/

#define MAIN_Fosc		22118400L	//define main clock
#define Baudrate0		115200L		//define the baudrate,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ
									//1T  mode: 7200~1382400 for 22.1184MHZ, 3600~691200 for 11.0592MHZ

/**********************************************************/


/****************** 编译器自动生成，用户请勿修改 ************************************/

#define T1_TimerReload	(256 - MAIN_Fosc / 192 / Baudrate0)			//Calculate the timer1 reload value	at 12T mode

/**********************************************************/


void	main(void)
{
	uart0_init();
	SPI_init();
	
	while(1)
	{
		if(uart0_rd != uart0_wr)	//串口0转发
		{
			SPI_CS = 0;							//允许外部PSI器件
			SPSTAT = SPIF + WCOL;			//清0 SPIF和WCOL标志
			SPDAT = RX0_Buffer[uart0_rd];	//发送一个字节
			while((SPSTAT & SPIF) == 0)	;	//等待发送完成
			SPSTAT = SPIF + WCOL;				//清0 SPIF和WCOL标志
			SPI_CS = 1;							//禁止外部PSI器件

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
