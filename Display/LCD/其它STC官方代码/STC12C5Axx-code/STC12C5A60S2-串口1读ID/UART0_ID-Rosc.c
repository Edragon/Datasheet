
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
	发单个字符'i'，将返回唯一ID号。
	发单个字符'r'，将返回内部震荡频率。
*/


/*************** 用户定义参数 *****************************/

#define MAIN_Fosc		22118400L	//define main clock
#define Baudrate0		9600		//define the baudrate,
									//12T mode: 600~115200 for 22.1184MHZ, 300~57600 for 11.0592MHZ
									//1T  mode: 7200~1382400 for 22.1184MHZ, 3600~691200 for 11.0592MHZ

/**********************************************************/


#include	<reg51.h>

sfr	AUXR = 0x8E;

bit		B_TI;

unsigned char id[7];
unsigned long Rosc,RoscZ;
unsigned char Command;

void	uart0_init(void);


/****************** 编译器自动生成，用户请勿修改 ************************************/

#define T1_TimerReload		(256 - MAIN_Fosc / 192 / Baudrate0)		//Calculate the timer1 reload value	at 12T mode
//#define T1_TimerReload	(256 - MAIN_Fosc / 16 / Baudrate0)		//Calculate the timer1 reload value ar 1T mode

/**********************************************************/

void 	TxByte(unsigned char dat);
unsigned char HEX2DEC(unsigned char dat);
void	PrintStrings(unsigned char code *sts);


void	main(void)
{
	unsigned char i;
	unsigned int  j;
	unsigned char idata *p;
	
	p = 0xf1;
	for(i=0; i<7; i++)
	{
		id[i] = *p;
		p++;
	}
	
	Rosc = 0;
	for(p=0xf8; p<=0xfb; p++)	Rosc = (Rosc << 8) + *p;
	RoscZ = 0;
	for(p=0xfc; p>0; p++)	RoscZ = (RoscZ << 8) + *p;
	
	uart0_init();
	PrintStrings("读ID和内部震荡频率\r\n");
	PrintStrings("发单个字符'i'，将返回唯一ID号。\r\n");
	PrintStrings("发单个字符'r'，将返回内部震荡频率。\r\n");
	
	
	while(1)
	{
		for(j=0; j<1500; j++)	;	//14T per loop, delay 1ms
		if(Command != 0)		//收到命令
		{
			if(Command == 'i')	//读取ID命令
			{
				PrintStrings("本IC的ID号为：");
				for(i=0; i<7; i++)
				{
					TxByte(HEX2DEC(id[i] >> 4));
					TxByte(HEX2DEC(id[i]& 0x0f));
					TxByte(' ');
				}
				TxByte(0x0d);
				TxByte(0x0a);
			}
			if(Command == 'r')	//读取内部频率命令
			{
				PrintStrings("内部震荡频率为：");
				TxByte(Rosc / 10000000L + '0');
				TxByte(Rosc % 10000000L / 1000000L + '0');
				TxByte('.');
				TxByte(Rosc % 1000000L / 100000L + '0');
				TxByte(Rosc % 100000L / 10000 + '0');
				TxByte(Rosc % 10000 / 1000 + '0');
				TxByte(Rosc % 1000 / 100 + '0');
				TxByte(Rosc % 100 / 10 + '0');
				TxByte(Rosc % 10 + '0');
				TxByte('M');
				TxByte('H');
				TxByte('Z');
				TxByte(0x0d);
				TxByte(0x0a);
			}
			Command = 0;
		}
	}
}


unsigned char HEX2DEC(unsigned char dat)
{
	dat &= 0x0f;
	if(dat <= 9)	return (dat+'0');
	else			return (dat+55);
}

void TxByte(unsigned char dat)
{
	B_TI = 0;
	SBUF = dat;
	while(!B_TI)	;
	B_TI = 0;
}

/**********************************************/
void	PrintStrings(unsigned char code *sts)
{
    for (; *sts != 0; sts++)	TxByte(*sts);
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
		Command = SBUF;
	}

	if(TI)
	{
		TI = 0;
		B_TI = 1;
	}
}
