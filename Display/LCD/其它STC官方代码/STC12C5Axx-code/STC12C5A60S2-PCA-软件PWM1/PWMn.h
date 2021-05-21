
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


#ifndef		PWMn_H
#define		PWMn_H


//========================================================================
// 文件: PWMn.c
// 功能: 用户配置宏。选择是STC12C5201AD系列还是STC12C5A60S2系列。
// 版本: VER1.0
// 日期: 2009-12-30
// 备注: 
//========================================================================

//#define		STC12C5201AD					//如果注销，则PCA输出默认使用STC12C5A60S2系列

#define		MAIN_Fosc	24576000L	//定义时钟

#define		PCA_IDLE_DISABLE	0			//1: MCU在IDLE模式时禁止PCA工作。	0:  MCU在IDLE模式时允许PCA工作。
#define		PCA_SOURCE_SELECT	4			//选择PCA的基准时钟源。
											//0：系统时钟Fosc/12。
											//1：系统时钟Fosc/2。
											//2：定时器0的溢出。
											//3：ECI/P3.4脚的外部时钟输入（最大=Fosc/2）。
											//4：系统时钟Fosc。
											//5：系统时钟Fosc/4。
											//6：系统时钟Fosc/6。
											//7：系统时钟Fosc/8。
#define		PWM_DUTY		4096			//定义PWM的周期，数值为PCA所选择的时钟脉冲个数。


/**********************************************************************************************************/

#include	"STC12C5A60S2.h"

#ifdef	STC12C5201AD
	sbit  CCP0  = P3^7;	//STC12C5201AD/PWM
	sbit  CCP1  = P3^5;	//STC12C5201AD/PWM

#else
	sbit  CCP0  = P1^3;	//STC12C5A60S2
	sbit  CCP1  = P1^4;	//STC12C5A60S2
#endif

#define		PWM_HIGH_MAX	PWM_DUTY-32		//限制PWM输出的最大占空比。
#define		PWM_HIGH_MIN		32			//限制PWM输出的最小占空比。

#endif
