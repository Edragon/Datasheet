
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
sfr BRT   = 0x9C;

#define		BUF_LENTH	128		//定义串口接收缓冲长度
unsigned char 	uart0_wr;		//写指针
unsigned char 	uart0_rd;		//读指针
unsigned char 	xdata RX0_Buffer[BUF_LENTH];	//接收缓冲
bit		B_TI;

void	uart0_init(void);



/*************** 用户定义参数 *****************************/

#define MAIN_Fosc		22118400UL	//define main clock
#define Baudrate0		9600UL		//define the baudrate,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ
									//1T  mode: 7200~1382400 for 22.1184MHZ, 3600~691200 for 11.0592MHZ

/**********************************************************/


/****************** 编译器自动生成，用户请勿修改 ************************************/

#define BRT_Reload			(256 - MAIN_Fosc / 16 / Baudrate0)		//Calculate the timer1 reload value ar 1T mode


/**********************************************************/


void	main(void)
{
	uart0_init();
	
	while(1)
	{
		if(uart0_rd != uart0_wr)	//串口转发
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
	AUXR |=  0x01;		//UART0 使用BRT
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
