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
// �ļ�: PWMn.c
// ����: �û����úꡣ
// �汾: VER1.0
// ����: 2009-12-30
// ��ע: 
//========================================================================

//#define		STC12C5201AD				//���ע������PCA���Ĭ��ʹ��STC12C5A60S2ϵ��

#define MAIN_Fosc		22118400L		//������ʱ��

#define		PCA_IDLE_DISABLE	0		//1: MCU��IDLEģʽʱ��ֹPCA������	0:  MCU��IDLEģʽʱ����PCA������
#define		PCA_SOURCE_SELECT	0		//ѡ��PCA�Ļ�׼ʱ��Դ��
										//0��ϵͳʱ��Fosc/12��
										//1��ϵͳʱ��Fosc/2��
										//2����ʱ��0�������
										//3��ECI/P3.4�ŵ��ⲿʱ�����루���=Fosc/2����
										//4��ϵͳʱ��Fosc��
										//5��ϵͳʱ��Fosc/4��
										//6��ϵͳʱ��Fosc/6��
										//7��ϵͳʱ��Fosc/8��
#define		PCA_ECF				1		//1: ����PCA����������жϣ�0: ��ֹ

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
sfr IP2   = 0xB5;	//STC12C5A60S2ϵ��
sfr IPH2  = 0xB6;	//STC12C5A60S2ϵ��
sfr IPH   = 0xB7;

sfr CCON = 0xD8;	//STC12C5A60S2ϵ��
sfr CMOD = 0xD9;	//STC12C5A60S2ϵ��
sfr CCAPM0 = 0xDA;	//PCAģ��0�Ĺ���ģʽ�Ĵ�����
sfr CCAPM1 = 0xDB;	//PCAģ��1�Ĺ���ģʽ�Ĵ�����

sfr CL     = 0xE9;	//
sfr CCAP0L = 0xEA;	//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1L = 0xEB;	//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��

sfr PCA_PWM0 = 0xF2;	//PCAģ��0 PWM�Ĵ�����
sfr PCA_PWM1 = 0xF3;	//PCAģ��1 PWM�Ĵ�����
sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;		//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1H = 0xFB;		//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��

sbit PPCA  = IP^6;
sbit CCF0  = CCON^0;	//PCA ģ��0�жϱ�־����Ӳ����λ�������������0��
sbit CCF1  = CCON^1;	//PCA ģ��1�жϱ�־����Ӳ����λ�������������0��
sbit CR    = CCON^6;	//1: ����PCA�����������������������0��
sbit CF    = CCON^7;	//PCA�����������CH��CL��FFFFH��Ϊ0000H����־��PCA�������������Ӳ����λ�������������0��

#ifdef	STC12C5201AD
	sbit  CCP0  = P3^7;	//STC12C5201AD/PWM
	sbit  CCP1  = P3^5;	//STC12C5201AD/PWM

#else
	sbit  CCP0  = P1^3;	//STC12C5A60S2
	sbit  CCP1  = P1^4;	//STC12C5A60S2
#endif

#endif
