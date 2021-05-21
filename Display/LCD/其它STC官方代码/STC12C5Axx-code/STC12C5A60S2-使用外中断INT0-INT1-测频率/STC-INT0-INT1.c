
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

外部中断INT0、INT1输入信号测频率，下降沿中断, PCA0做秒定时。

最大频率信号为主时钟的 1/300.  比如24MHZ主时钟,可以测量80KHZ.

结果从串口输出, 9600,8,N,1.  主频11.0592MHZ.


******************************************/



#include	"reg51.H"

#define	uchar	unsigned char
#define uint	unsigned int


//#define		STC12C5201AD				//如果注销，则PCA输出默认使用STC12C5A60S2系列

#define MAIN_Fosc		11059200L		//定义主时钟

#define		PCA_IDLE_DISABLE	0		//1: MCU在IDLE模式时禁止PCA工作。	0:  MCU在IDLE模式时允许PCA工作。
#define		PCA_SOURCE_SELECT	0		//选择PCA的基准时钟源。
										//0：系统时钟Fosc/12。
										//1：系统时钟Fosc/2。
										//2：定时器0的溢出。
										//3：ECI/P3.4脚的外部时钟输入（最大=Fosc/2）。
										//4：系统时钟Fosc。
										//5：系统时钟Fosc/4。
										//6：系统时钟Fosc/6。
										//7：系统时钟Fosc/8。
#define		PCA_ECF				0		//1: 允许PCA计数器溢出中断，0: 禁止

/**********************************************************************************************************/

#include	<reg51.h>

sfr IP2   = 0xB5;	//STC12C5A60S2系列
sfr IPH2  = 0xB6;	//STC12C5A60S2系列
sfr IPH   = 0xB7;

sfr CCON = 0xD8;	//STC12C5A60S2系列
sfr CMOD = 0xD9;	//STC12C5A60S2系列
sfr CCAPM0 = 0xDA;	//PCA模块0的工作模式寄存器。
sfr CCAPM1 = 0xDB;	//PCA模块1的工作模式寄存器。

sfr CL     = 0xE9;	//
sfr CCAP0L = 0xEA;	//PCA模块0的捕捉/比较寄存器低8位。
sfr CCAP1L = 0xEB;	//PCA模块1的捕捉/比较寄存器低8位。

sfr PCA_PWM0 = 0xF2;	//PCA模块0 PWM寄存器。
sfr PCA_PWM1 = 0xF3;	//PCA模块1 PWM寄存器。
sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;		//PCA模块0的捕捉/比较寄存器高8位。
sfr CCAP1H = 0xFB;		//PCA模块1的捕捉/比较寄存器高8位。

sbit PPCA  = IP^6;
sbit CCF0  = CCON^0;	//PCA 模块0中断标志，由硬件置位，必须由软件清0。
sbit CCF1  = CCON^1;	//PCA 模块1中断标志，由硬件置位，必须由软件清0。
sbit CR    = CCON^6;	//1: 允许PCA计数器计数，必须由软件清0。
sbit CF    = CCON^7;	//PCA计数器溢出（CH，CL由FFFFH变为0000H）标志。PCA计数器溢出后由硬件置位，必须由软件清0。

#ifdef	STC12C5201AD
	sbit  CCP0  = P3^7;	//STC12C5201AD/PWM
	sbit  CCP1  = P3^5;	//STC12C5201AD/PWM

#else
	sbit  CCP0  = P1^3;	//STC12C5A60S2
	sbit  CCP1  = P1^4;	//STC12C5A60S2
#endif


uint	CCAP0_tmp;						//比较匹配影射寄存器
uint	Freq_INT0,Freq_INT1;			//频率, 0~10KHZ
uint	Freq_INT0_cnt,Freq_INT1_cnt;	//频率计数, 0~10KHZ
uchar	cnt_1S;							//1秒定时
bit		B_FrequencyOk;					//测频OK标志


void	uart0_init(void);
void	TxByte(uchar dat);
void	PCA_init(void);


/********************* 主函数 *************************/
void main(void)
{
	IT0 = 1;	// 外部中断0下降沿中断
	EX0 = 1;	// 外部中断0允许
	IT1 = 1;	// 外部中断1下降沿中断
	EX1 = 1;	// 外部中断1允许
	
	PX1	 = 1;	//外部中断1优先级设定位
	PX0	 = 1;	//外部中断0优先级设定位
	
	PCA_init();	//PCA初始化
	uart0_init();

	EA  = 1;	// 中断总允许

	while(1)
	{
		if(B_FrequencyOk)
		{
			B_FrequencyOk = 0;
			TxByte('F');
			TxByte('0');
			TxByte('=');
			TxByte(Freq_INT0/10000+'0');
			TxByte(Freq_INT0%10000/1000+'0');
			TxByte(Freq_INT0%1000/100+'0');
			TxByte(Freq_INT0%100/10+'0');
			TxByte(Freq_INT0%10+'0');
			TxByte('H');
			TxByte('Z');
			TxByte(0x0d);
			TxByte(0x0a);

			TxByte('F');
			TxByte('1');
			TxByte('=');
			TxByte(Freq_INT1/10000+'0');
			TxByte(Freq_INT1%10000/1000+'0');
			TxByte(Freq_INT1%1000/100+'0');
			TxByte(Freq_INT1%100/10+'0');
			TxByte(Freq_INT1%10+'0');
			TxByte('H');
			TxByte('Z');
			TxByte(0x0d);
			TxByte(0x0a);
		}
	}
}


void	TxByte(uchar dat)
{
	TI = 0;
	SBUF = dat;
	while(!TI);
	TI = 0;
}

void	uart0_init(void)
{
	PCON |= 0x80;		//UART0 Double Rate Enable
	SCON = 0x50;		//UART0 set as 10bit , UART0 RX enable

	TMOD &= ~(1<<6);		//Timer1 Set as Timer, defalul 12T
	TMOD = (TMOD & ~0x30) | 0x20;	//Timer1 set as 8 bits auto relaod
	TH1 = 250;		//Load the timer
	TR1  = 1;
}




//========================================================================
// 函数: void	PWMn_init(void)
// 描述: 初始化程序。
// 参数: 无。
// 返回: 无。
// 版本: VER1.0
// 日期: 2009-12-30
// 备注: 
//========================================================================
void	PCA_init(void)
{
	CCON = 0;					//清除CF、CR、CCF0、CCF1
	PPCA = 1;
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1) | PCA_ECF;	//初始化PCA模式寄存器。
	CCAPM0 = 0x49;				//16位软件定时器，允许比较匹配中断(ECCF0=1)。
	CL = 0;						//清空PCA基本计数器。
	CH = 0;
	CCAP0_tmp = 18432;		//18432*12/11059.200 = 20ms
	CCAP0L = (uchar)CCAP0_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
	CCAP0H = (uchar)(CCAP0_tmp >> 8);	//后写CCAP0H
	CR = 1;						//启动PCA。
}

//========================================================================
// 函数: void PCA_interrupt (void) interrupt 7
// 描述: PCA中断服务程序。
// 参数: 无。
// 返回: 无。
// 作者: Coody
// 版本: VER1.0
// 日期: 2010-7-31
// 备注: 
//========================================================================
void PCA_interrupt (void) interrupt 7
{
	if(CCF0 == 1)		//PCA模块0中断
	{
		if(++cnt_1S >= 50)				//50*20=1000ms
		{
			EA = 0;
			Freq_INT0 = Freq_INT0_cnt;	//频率1
			Freq_INT0_cnt = 0;			//清除频率计数
			Freq_INT1 = Freq_INT1_cnt;	//频率2
			Freq_INT1_cnt = 0;			//清除频率计数
			EA = 1;
			B_FrequencyOk = 1;			//标志频率OK
			cnt_1S = 0;
		}

		CCF0 = 0;				//清PCA模块0中断标志
		CCAP0_tmp += 18432;		//18432*12/11059.200 = 20ms
		CCAP0L = (uchar)CCAP0_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
		CCAP0H = (uchar)(CCAP0_tmp >> 8);	//后写CCAP0H
	}

	CCF1 = 0;		//清PCA模块1中断标志
	CF = 0;			//清PCA溢出中断标志
}



/********************* INT0中断函数 *************************/
void INT0_int (void) interrupt 0
{
	Freq_INT0_cnt++;	//频率计数+1
}

/********************* INT1中断函数 *************************/
void INT1_int (void) interrupt 2
{
	Freq_INT1_cnt++;	//频率计数+1
}
