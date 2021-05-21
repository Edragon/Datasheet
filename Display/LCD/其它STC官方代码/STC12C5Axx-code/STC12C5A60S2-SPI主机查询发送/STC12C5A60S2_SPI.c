
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

每隔50ms，从SPI发送32个字节数据.

******************************************/


#include	<reg51.h>

#define		MAIN_Fosc	22118400L	//定义时钟

sbit	SPI_SCL		= P1^7;	//SPI卡同步时钟
sbit	SPI_MISO	= P1^6;	//SPI卡同步数据
sbit	SPI_MOSI	= P1^5;	//SPI卡同步数据
sbit	SPI_CS		= P1^4;	//SPI卡片选
sfr AUXR1 = 0xA2;

unsigned char 	xdata SPI_TxBuffer[32];
unsigned char 	xdata SPI_RxBuffer[32];


sfr SPSTAT = 0xCD;	//STC12C5A60S2系列
sfr SPCTL  = 0xCE;	//STC12C5A60S2系列
sfr SPDAT  = 0xCF;	//STC12C5A60S2系列

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
	SPCTL = (SSIG << 7) + (SPEN << 6) + (DORD << 5) + (MSTR << 4) + (CPOL << 3) + (CPHA << 2) + SPEED_16;
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
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void main(void)
{
	unsigned char i;
	
	delay_ms(100);		//延时一下，让外部SPI器件准备好
	
	AUXR1 |= (1<<5);	//SPI切换到P4口
	SPI_init();			//配置为主机
	
	for(i=0; i<32; i++)		SPI_TxBuffer[i] = i;	//装载要发送的数据
	
	while(1)
	{
		delay_ms(50);
		
		SPI_CS = 0;							//允许外部PSI器件
		for(i=0; i<32; i++)
		{
			SPSTAT = SPIF + WCOL;			//清0 SPIF和WCOL标志
			SPDAT = SPI_TxBuffer[i];		//发送一个字节
			while((SPSTAT & SPIF) == 0)	;	//等待发送完成
			SPI_RxBuffer[i] = SPDAT;		//接收返回的字节
		}
		SPSTAT = SPIF + WCOL;				//清0 SPIF和WCOL标志
		SPI_CS = 1;							//禁止外部PSI器件
	
	}

}