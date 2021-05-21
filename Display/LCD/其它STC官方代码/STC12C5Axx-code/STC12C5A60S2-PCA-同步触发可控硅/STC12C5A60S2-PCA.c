

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

PCA中断测试程序.
时钟使用11.0592MHZ.
CCAP0软件定时20ms中断，		从P1.0输出一个方波( 50HZ at 11.0592MHZ).

INT1(P3.3)输入过零信号, P1.3输出触发信号给可控硅,触发时间为50us. 
CCAP1管理移相触发.
本例程适用50HZ工频,如果用在60HZ,则自行修改,或者设计成自动适应的.
可以使用P1.0输出的50HZ信号代替工频来测试.

PCA计数器溢出中断，	从P1.1输出一个方波( 7HZ at 11.0592MHZ).


******************************************/

#include	"PCA.h"

unsigned int PhaseShift;	//相移时间,us
sbit	P_TRIG = P1^3;		//触发输出
unsigned char TrigIndex;	//触发序列
bit		B_SYNC;				//同步标志, 底层使用,用户不可见.
bit		B_TrigEnable;		//允许或禁止触发输出

sbit	P10 = P1^0;
sbit	P11 = P1^1;

unsigned int CCAP0_tmp,CCAP1_tmp;
unsigned int PCA_Timer0,PCA_Timer1;

void	PWMn_init(void);


/******************** 主函数 **************************/
//========================================================================
// 函数: void main(void)
// 描述: 软件定时值设置示例。
// 参数: 无。
// 返回: 无。
// 版本: VER1.0
// 日期: 2009-12-30
// 备注: 
//========================================================================
void main(void)
{
	B_TrigEnable = 1;	//1--允许,  0--禁止
	PhaseShift = 2765;	//相移3000us,仅仅是演示,用户可以根据档位来设置,一般为46~9170, 对应50~9950us.
	
	PCA_Timer0 = 9216;		//CCAP0软件定时20ms中断，从P1.0输出一个方波( 50HZ at 11.0592MHZ).
	PWMn_init();	//初始化PCA

	IT1 = 1;	// 外部中断1下降沿中断
	EX1 = 1;	// 外部中断1允许

	while(1)
	{
	
	}

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
void	PWMn_init(void)
{
	P1M1 &= ~0x07,	P1M0 |=  0x07;		//P1.0 P1.1 P1.2 使用PUSH-PULL输出模式
	CCON = 0;					//清除CF、CR、CCF0、CCF1
	IPH |= 0x80;				//PCA中断使用最高优先级
	PPCA = 1;
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1) | PCA_ECF;	//初始化PCA模式寄存器。
	CCAPM0 = 0x49;				//16位软件定时器，允许比较匹配中断(ECCF0=1)。
	CCAPM1 = 0x49;				//16位软件定时器，允许比较匹配中断(ECCF1=1)。
	CL = 0;						//清空PCA基本计数器。
	CH = 0;
	CCAP0_tmp = 100;
	CCAP1_tmp = 100;
	EA = 1;						//允许总中断
	CR = 1;						//启动PCA。
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

#define		TRIG_ON		1
#define		TRIG_OFF	0

void PCA_interrupt (void) interrupt 7
{
	if(CCF0 == 1)		//PCA模块0中断
	{
		P10 = ~P10;
		CCF0 = 0;		//清PCA模块0中断标志
		CCAP0_tmp += PCA_Timer0;
		CCAP0L = (unsigned char)CCAP0_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
		CCAP0H = (unsigned char)(CCAP0_tmp >> 8);	//后写CCAP0H
	}

	if(CCF1 == 1)	//PCA模块1中断
	{
		CCF1 = 0;			//清PCA模块1中断标志
		if(B_TrigEnable && B_SYNC)	//允许触发, 并且收到过0同步信号
		{
			if(TrigIndex == 0)		//序列0,第1个触发脉冲开始
			{
				P_TRIG = TRIG_ON;
				CCAP1_tmp += 46;	//触发时间为50us
				TrigIndex = 1;
			}
			else if(TrigIndex == 1)	//序列1, 第1个触发脉冲结束
			{
				P_TRIG = TRIG_OFF;
				CCAP1_tmp += 9170;	//间隔时间为10000-50 = 9950us
				TrigIndex = 2;
			}
			else if(TrigIndex == 2)	//序列2,第2个触发脉冲开始
			{
				P_TRIG = TRIG_ON;
				CCAP1_tmp += 46;	//触发时间为50us
				TrigIndex = 3;
			}
			else if(TrigIndex == 3)	//序列1, 第1个触发脉冲结束
			{
				P_TRIG = TRIG_OFF;
				CCAP1_tmp += 9170;	//这个时间没有意义,但是不能取消
				B_SYNC = 0;			//一次同步触发结束
			}
			CCAP1L = (unsigned char)CCAP1_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
			CCAP1H = (unsigned char)(CCAP1_tmp >> 8);	//后写CCAP0H
		}
		else	P_TRIG = TRIG_OFF;
	}

	if(CF == 1)	//PCA溢出中断
	{
		P11 = ~P11;		//7HZ at 11.0592MHZ
		CF = 0;			//清PCA溢出中断标志
	}
}

//========================================================================
// 函数: void INT1_int (void) interrupt 2
// 描述: INT1中断服务程序。
// 参数: 无。
// 返回: 无。
// 作者: Coody
// 版本: VER1.0
// 日期: 2010-7-31
// 备注: 
//========================================================================
void INT1_int (void) interrupt 2
{
	unsigned char	i,j;

	i = CH;
	j = CL;
	if(i != CH)	i++;	//读时刚好发生进位
	CCAP1_tmp = i;
	CCAP1_tmp = (CCAP1_tmp << 8) + j;
	CCAP1_tmp += PhaseShift;	//相移
	CCAP1L = (unsigned char)CCAP1_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
	CCAP1H = (unsigned char)(CCAP1_tmp >> 8);	//后写CCAP0H
	TrigIndex = 0;		//触发序列指示
	B_SYNC = 1;			//标志同步
}

