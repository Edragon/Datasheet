

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
CCAP0软件定时10ms中断，		从P1.0输出一个方波( 50HZ at 22.1184MHZ).
CCAP1软件定时 5ms中断，		从P1.1输出一个方波(100HZ at 22.1184MHZ).
PCA计数器溢出35.556ms中断，	从P1.2输出一个方波( 14HZ at 22.1184MHZ).


******************************************/

#include	"PCA.h"


sbit	P10 = P1^0;
sbit	P11 = P1^1;
sbit	P12 = P1^2;

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
	PCA_Timer0 = 18432;		//CCAP0软件定时10ms中断，从P1.0输出一个方波( 50HZ at 22.1184MHZ).
	PCA_Timer1 = 9216;		//CCAP1软件定时 5ms中断，从P1.1输出一个方波(100HZ at 22.1184MHZ).
	PWMn_init();	//初始化PCA
	
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
		P11  = ~P11;
		CCF1 = 0;		//清PCA模块1中断标志
		CCAP1_tmp += PCA_Timer1;
		CCAP1L = (unsigned char)CCAP1_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
		CCAP1H = (unsigned char)(CCAP1_tmp >> 8);	//后写CCAP0H
	}

	if(CF == 1)	//PCA溢出中断
	{
		CF = 0;			//清PCA溢出中断标志
		P12 = ~P12;		//168.75HZ at 22.1184MHZ
	}
}