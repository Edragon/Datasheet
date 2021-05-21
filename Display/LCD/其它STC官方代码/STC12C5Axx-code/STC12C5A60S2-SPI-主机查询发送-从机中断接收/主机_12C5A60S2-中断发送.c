

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
	测试方法：
	根据您的主时钟和波特率修改下面的相关定义，编译后下载到MCU。
	通过串口助手向MCU发送数据，MCU收到后通过SPI发给从机。串口波特率 115200,8,n,1 at 22.1184MHZ
*/

/*************** 用户定义参数 *****************************/

#define MAIN_Fosc		22118400L	//define main clock
#define Baudrate0		115200L		//define the baudrate, 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ

/**********************************************************/


#include	<reg51.h>

sfr	AUXR  = 0x8E;

#define		BUF_LENTH	64		//定义串口接收缓冲长度
unsigned char 	uart0_rx_cnt;	//写指针
unsigned char 	RX0_TimeOut;
unsigned char 	SPI_tx_cnt;		//读指针
unsigned char 	SPI_tx_number;
unsigned char 	idata RX0_Buffer[BUF_LENTH];
unsigned char 	idata SPI_Buffer[BUF_LENTH];

sbit	SPI_SCL		= P1^7;	//SPI卡同步时钟
sbit	SPI_MISO	= P1^6;	//SPI卡同步数据
sbit	SPI_MOSI	= P1^5;	//SPI卡同步数据
sbit	SPI_CS		= P1^4;	//SPI卡片选
sfr 	AUXR1 = 0xA2;
sfr 	IE2   = 0xAF;

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


void	uart0_init(void);
void	delay_ms(unsigned char ms);
void	SPI_init(void);


/****************** 编译器自动生成，用户请勿修改 ************************************/

#define T1_TimerReload	(256 - MAIN_Fosc / 192 / Baudrate0)			//Calculate the timer1 reload value	at 12T mode

/**********************************************************/


void	main(void)
{
	unsigned char	i;

	uart0_init();
	SPI_init();
	
	while(1)
	{
		delay_ms(1);
		if(RX0_TimeOut > 0)
		{
			if(--RX0_TimeOut == 0)
			{
				if(uart0_rx_cnt > 0)	//串口0转发
				{
					for(i=0; i<uart0_rx_cnt; i++)	SPI_Buffer[i] = RX0_Buffer[i];
					SPI_CS = 0;						//允许外部PSI器件
					SPSTAT = SPIF + WCOL;			//清0 SPIF和WCOL标志
					SPDAT = SPI_Buffer[0];			//发送一个字节
					SPI_tx_cnt = 1;					//下一个字节
					SPI_tx_number = uart0_rx_cnt;	//数据长度
					uart0_rx_cnt = 0;
				}
			}
		}
	}
}

//========================================================================
// 函数: void  delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数.
// 返回: none.
// 版本: VER1.0
// 日期: 2010-12-15
// 备注: 
//========================================================================
void  delay_ms(unsigned char ms)
{
     unsigned int i;
	 do
	 {
		i = MAIN_Fosc / 14000L;
		while(--i)	;
     }while(--ms);
}

/**********************************************/
void	SPI_init(void)
{
	SPCTL = (SSIG << 7) + (SPEN << 6) + (DORD << 5) + (MSTR << 4) + (CPOL << 3) + (CPHA << 2) + SPEED_64;
	IE2 |= 0x02;	//允许SPI中断
}


/**********************************************/
void SPI_Transivion (void) interrupt 9
{
//	SPI_Buffer[SPI_RxWr] = SPDAT;
//	SPSTAT = SPIF + WCOL;	//清0 SPIF和WCOL标志
//	if(++SPI_RxWr >= SPI_BUF_LENTH)		SPI_RxWr = 0;

	SPSTAT = SPIF + WCOL;			//清0 SPIF和WCOL标志
	if(--SPI_tx_number > 0)			//仍有数据要发
	{
		SPDAT = SPI_Buffer[SPI_tx_cnt++];	//发送一个字节
	}
	else	SPI_CS = 1;						//禁止外部PSI器件
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
		if(uart0_rx_cnt >= BUF_LENTH)	uart0_rx_cnt = 0;
		RX0_Buffer[uart0_rx_cnt++] = SBUF;
		RX0_TimeOut = 5;
	}

	if(TI)
	{
		TI = 0;
	}
}



