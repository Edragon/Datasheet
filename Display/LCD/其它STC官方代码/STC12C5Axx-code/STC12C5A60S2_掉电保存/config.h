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

//  ����: 2010-11-9

#ifndef		_config_h
#define		_config_h	1

/*********************************************************/

#define MAIN_Fosc		22118400L	//������ʱ��	����0������600~115200
//#define MAIN_Fosc		33177600L	//������ʱ��	����0������300~57600
//#define MAIN_Fosc		11059200L	//������ʱ��	����0������300~57600
//#define MAIN_Fosc		18432000L	//������ʱ��	����0������600~19200
//#define MAIN_Fosc		24000000L	//������ʱ��	����0������600~115200
//#define MAIN_Fosc		12000000L	//������ʱ��	����0������300~4800

#define D_TIMER0		1000		//ѡ��ʱ��0��׼��ʱʱ��
#define Baudrate0		9600		//����0������600~115200

#include "reg51.h"
#include <intrins.h>

/*********************************************************/


/**************************************************************************/

#define freq_base		(MAIN_Fosc / 1200)
#define D_UART0_CNT		(256 - MAIN_Fosc / 12 / 16 / Baudrate0)		//���㲨����
#define Timer0_Reload	(D_TIMER0 * freq_base / 10000)

/***********************************************************/

#define	uchar	unsigned char
#define uint	unsigned int
#define ulong	unsigned long


#endif
