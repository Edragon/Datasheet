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

//  日期: 2010-11-9

#ifndef		_config_h
#define		_config_h	1

/*********************************************************/

#define MAIN_Fosc		22118400L	//定义主时钟	串口0波特率600~115200
//#define MAIN_Fosc		33177600L	//定义主时钟	串口0波特率300~57600
//#define MAIN_Fosc		11059200L	//定义主时钟	串口0波特率300~57600
//#define MAIN_Fosc		18432000L	//定义主时钟	串口0波特率600~19200
//#define MAIN_Fosc		24000000L	//定义主时钟	串口0波特率600~115200
//#define MAIN_Fosc		12000000L	//定义主时钟	串口0波特率300~4800

#define D_TIMER0		1000		//选择定时器0基准定时时间
#define Baudrate0		9600		//串口0波特率600~115200

#include "reg51.h"
#include <intrins.h>

/*********************************************************/


/**************************************************************************/

#define freq_base		(MAIN_Fosc / 1200)
#define D_UART0_CNT		(256 - MAIN_Fosc / 12 / 16 / Baudrate0)		//计算波特率
#define Timer0_Reload	(D_TIMER0 * freq_base / 10000)

/***********************************************************/

#define	uchar	unsigned char
#define uint	unsigned int
#define ulong	unsigned long


#endif
