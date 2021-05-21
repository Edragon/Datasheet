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

#ifndef		PWMn_H
#define		PWMn_H


//========================================================================
// 文件: PWMn.c
// 功能: 用户配置宏。
// 版本: VER1.0
// 日期: 2009-12-30
// 备注: 
//========================================================================

//#define		STC12C5201AD				//如果注销，则PCA输出默认使用STC12C5A60S2系列

#define MAIN_Fosc		22118400L		//定义主时钟

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
#define		PCA_ECF				1		//1: 允许PCA计数器溢出中断，0: 禁止

/**********************************************************************************************************/

#include	<reg51.h>

sfr P1M1 = 0x91;	//P1M1.n,P1M0.n 	=00--->Standard,	01--->push-pull
sfr P1M0 = 0x92;	//					=10--->pure input,	11--->open drain
sfr P0M1 = 0x93;	//P0M1.n,P0M0.n 	=00--->Standard,	01--->push-pull
sfr P0M0 = 0x94;	//					=10--->pure input,	11--->open drain
sfr P2M1 = 0x95;	//P2M1.n,P2M0.n 	=00--->Standard,	01--->push-pull
sfr P2M0 = 0x96;	//					=10--->pure input,	11--->open drain
sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain
sfr P4M1  = 0xB3;	//P4M1.n,P4M0.n 	=00--->Standard,	01--->push-pull
sfr P4M0  = 0xB4;	//					=10--->pure input,	11--->open drain
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

#endif
