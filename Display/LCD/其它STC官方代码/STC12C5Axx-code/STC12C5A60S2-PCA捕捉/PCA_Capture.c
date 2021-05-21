
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

STC12C5A60S2系列 从P1.3 P1.4 做捕捉输入，从串口输出结果(ASCII)，9600,8,N,1.
STC12C5204AD系列 从P3.7 P3.5 做捕捉输入，从串口输出结果(ASCII)，9600,8,N,1.

******************************************/


/*************	用户系统配置	**************/

#define MAIN_Fosc		22118400L	//定义主时钟, 模拟串口和和延时会自动适应。5~35MHZ

#define		PCA_IDLE_DISABLE	0			//1: MCU在IDLE模式时禁止PCA工作。	0:  MCU在IDLE模式时允许PCA工作。
#define		PCA_SOURCE_SELECT	0			//选择PCA的基准时钟源。
											//0：系统时钟Fosc/12。
											//1：系统时钟Fosc/2。
											//2：定时器0的溢出。
											//3：ECI/P3.4脚的外部时钟输入（最大=Fosc/2）。
											//4：系统时钟Fosc。
											//5：系统时钟Fosc/4。
											//6：系统时钟Fosc/6。
											//7：系统时钟Fosc/8。
#define		PCA_ECF				1		//1: 允许PCA计数器溢出中断，0: 禁止


/*************	以下宏定义用户请勿修改	**************/
#include	"reg51.H"
#define	uchar	unsigned char
#define uint	unsigned int
/******************************************/

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

sbit CCF0  = CCON^0;	//PCA 模块0中断标志，由硬件置位，必须由软件清0。
sbit CCF1  = CCON^1;	//PCA 模块1中断标志，由硬件置位，必须由软件清0。
sbit CR    = CCON^6;	//1: 允许PCA计数器计数，必须由软件清0。
sbit CF    = CCON^7;	//PCA计数器溢出（CH，CL由FFFFH变为0000H）标志。PCA计数器溢出后由硬件置位，必须由软件清0。

/************* 本地变量声明 **************/
uint	CCAP0_tmp,CCAP1_tmp;
uint	CCAP0_Last,CCAP1_Last;
bit		B_Cap0,B_Cap1;
sbit	P_TXD1 = P3^1;

sbit	P10 = P1^0;

/************* 本地函数声明 **************/
void	Tx1Send(uchar dat);
void	PrintString(unsigned char code *puts);


void tx(uchar id,uint dat)
{
	Tx1Send('P');
	Tx1Send('C');
	Tx1Send('A');
	Tx1Send(id + '0');
	Tx1Send('=');
	Tx1Send(dat/10000 + '0');
	Tx1Send(dat%10000/1000 + '0');
	Tx1Send(dat%1000/100 + '0');
	Tx1Send(dat%100/10 + '0');
	Tx1Send(dat%10 + '0');
	Tx1Send(0x0d);
	Tx1Send(0x0a);
}



/********************* 主函数 *************************/
void main(void)
{
	CCAPM0 = 0x11;	//CCAP0下降沿捕捉，允许中断		下降沿捕捉: 0x11,  上升沿捕捉: 0x21, 上升下降沿捕捉: 0x31
	CCAPM1 = 0x11;	//CCAP1下降沿捕捉，允许中断		下降沿捕捉: 0x11,  上升沿捕捉: 0x21, 上升下降沿捕捉: 0x31
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1) | PCA_ECF;	//初始化PCA模式寄存器。
	CL = 0;					//清空PCA基本计数器。
	CH = 0;
	CR = 1;					//Start CR
	EA = 1;

	PrintString("****** STC12C5A60S2系列MCU捕捉程序 2011-02-25 ******\r\n");	//上电后串口发送一条提示信息

	while(1)
	{
		if(B_Cap0)
		{
			B_Cap0 = 0;
			tx(0, CCAP0_tmp - CCAP0_Last);
			CCAP0_Last = CCAP0_tmp;
		}
		if(B_Cap1)
		{
			B_Cap1 = 0;
			tx(1, CCAP1_tmp - CCAP1_Last);
			CCAP1_Last = CCAP1_tmp;
		}
	}
}



//========================================================================
// 函数: void PCA_interrupt (void) interrupt 7
// 描述: PCA中断服务程序。
// 参数: 无。
// 返回: 无。
// 版本: VER1.0
// 日期: 2009-12-30
// 备注: 
//========================================================================
void PCA_interrupt (void) interrupt 7
{
	if(CCF0 == 1)		//PCA模块0中断
	{
		CCF0 = 0;		//清PCA模块0中断标志
		CCAP0_tmp = CCAP0H;	//读CCAP0H
		CCAP0_tmp = (CCAP0_tmp << 8) + CCAP0L;
		B_Cap0 = 1;
	}

	if(CCF1 == 1)	//PCA模块1中断
	{
		CCF1 = 0;		//清PCA模块1中断标志
		CCAP1_tmp = CCAP1H;	//读CCAP0H
		CCAP1_tmp = (CCAP1_tmp << 8) + CCAP1L;
		B_Cap1 = 1;
	}

	if(CF == 1)	//PCA溢出中断
	{
		CF = 0;			//清PCA溢出中断标志
		P10 = ~P10;		//14HZ at 22.1184MHZ
	}
}

/********************** 模拟串口相关函数************************/

void	BitTime(void)	//位时间函数
{
	uint i;
	i = ((MAIN_Fosc / 100) * 104) / 140000L - 1;		//根据主时钟来计算位时间
	while(--i);
}

//模拟串口发送
void	Tx1Send(uchar dat)		//9600，N，8，1		发送一个字节
{
	uchar	i;
	P_TXD1 = 0;
	BitTime();
	for(i=0; i<8; i++)
	{
		if(dat & 1)		P_TXD1 = 1;
		else			P_TXD1 = 0;
		dat >>= 1;
		BitTime();
	}
	P_TXD1 = 1;
	BitTime();
	BitTime();
}

void PrintString(unsigned char code *puts)		//发送一串字符串
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//遇到停止符0结束
}
