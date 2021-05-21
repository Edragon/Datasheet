
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

测试硬件PWM. 适用STC12C5204AD系列。

******************************************/


#include	<reg51.h>
#define	uchar	unsigned char
#define uint	unsigned int

#define		PCA_IDLE_DISABLE	0		//1: MCU在IDLE模式时禁止PCA工作。	0:  MCU在IDLE模式时允许PCA工作。
#define		PCA_SOURCE_SELECT	4		//选择PCA的基准时钟源。
										//0：系统时钟Fosc/12。
										//1：系统时钟Fosc/2。
										//2：定时器0的溢出。
										//3：ECI/P3.4脚的外部时钟输入（最大=Fosc/2）。
										//4：系统时钟Fosc。
										//5：系统时钟Fosc/4。
										//6：系统时钟Fosc/6。
										//7：系统时钟Fosc/8。
#define		PCA_ECF				1		//1: 允许PCA计数器溢出中断，0: 禁止



sfr	AUXR = 0x8E;
sfr CCON = 0xD8;
sfr CMOD = 0xD9;
sfr CCAPM0 = 0xDA;	//PCA module 0 work mode
sfr CCAPM1 = 0xDB;	//PCA module 1 work mode
sfr CL     = 0xE9;	//PCA counter
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


#define		PWM0_NORMAL()	PCA_PWM0 = 0	//PWM0正常输出(默认)
#define		PWM0_OUT_0()	PCA_PWM0 = 3	//PWM0一直输出0
#define		PWM1_NORMAL()	PCA_PWM1 = 0	//PWM1正常输出(默认)
#define		PWM1_OUT_0()	PCA_PWM1 = 3	//PWM1一直输出0


/*************	本地函数声明	**************/

void	en_PCA(void);
void	en_PWM(uchar channel,uchar start_value);


uchar	pwm0,pwm1;

void	main(void)
{
	uint	i;
	
	pwm0 = 128;			// PWM0 初始值
	pwm1 = 64;			// PWM1 初始值

	CCAP0H = pwm0;		//set PWM wide
	CCAPM0 = 0x42;		//0x42 Setup PCA module 0 in PWM mode

	CCAP1H = pwm1;		//set PWM wide
	CCAPM1 = 0x42;		//0x42 Setup PCA module 1 in PWM mode

	en_PCA();			//enable PCA

	while (1)
	{
		for(i=0; i<30000; i++)	;	//延时一下
		pwm0++;
		pwm1++;
		CCAP0H = pwm0;		//set PWM wide
		CCAP1H = pwm1;		//set PWM wide
	}
}




//Note: CCAP0H and CCAP1H load the PWM output low time.

void	en_PCA(void)
{
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1);	//初始化PCA模式寄存器。
	CL = 0x00;		//clear PCA counter
	CH = 0x00;
	CR = 1;			//Start PCA counter
}

