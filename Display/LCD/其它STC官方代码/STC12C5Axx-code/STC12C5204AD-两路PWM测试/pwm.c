
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU Programme Demo ----------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*      �����򾭹�������ȫ����, ���ṩ�绰����֧��, �粻�����, �����в�����ػ���.  */


/*************	��������˵��	**************

����Ӳ��PWM. ����STC12C5204ADϵ�С�

******************************************/


#include	<reg51.h>
#define	uchar	unsigned char
#define uint	unsigned int

#define		PCA_IDLE_DISABLE	0		//1: MCU��IDLEģʽʱ��ֹPCA������	0:  MCU��IDLEģʽʱ����PCA������
#define		PCA_SOURCE_SELECT	4		//ѡ��PCA�Ļ�׼ʱ��Դ��
										//0��ϵͳʱ��Fosc/12��
										//1��ϵͳʱ��Fosc/2��
										//2����ʱ��0�������
										//3��ECI/P3.4�ŵ��ⲿʱ�����루���=Fosc/2����
										//4��ϵͳʱ��Fosc��
										//5��ϵͳʱ��Fosc/4��
										//6��ϵͳʱ��Fosc/6��
										//7��ϵͳʱ��Fosc/8��
#define		PCA_ECF				1		//1: ����PCA����������жϣ�0: ��ֹ



sfr	AUXR = 0x8E;
sfr CCON = 0xD8;
sfr CMOD = 0xD9;
sfr CCAPM0 = 0xDA;	//PCA module 0 work mode
sfr CCAPM1 = 0xDB;	//PCA module 1 work mode
sfr CL     = 0xE9;	//PCA counter
sfr CCAP0L = 0xEA;	//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1L = 0xEB;	//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��
sfr PCA_PWM0 = 0xF2;	//PCAģ��0 PWM�Ĵ�����
sfr PCA_PWM1 = 0xF3;	//PCAģ��1 PWM�Ĵ�����
sfr CH     = 0xF9;
sfr CCAP0H = 0xFA;		//PCAģ��0�Ĳ�׽/�ȽϼĴ�����8λ��
sfr CCAP1H = 0xFB;		//PCAģ��1�Ĳ�׽/�ȽϼĴ�����8λ��

sbit CCF0  = CCON^0;	//PCA ģ��0�жϱ�־����Ӳ����λ�������������0��
sbit CCF1  = CCON^1;	//PCA ģ��1�жϱ�־����Ӳ����λ�������������0��
sbit CR    = CCON^6;	//1: ����PCA�����������������������0��
sbit CF    = CCON^7;	//PCA�����������CH��CL��FFFFH��Ϊ0000H����־��PCA�������������Ӳ����λ�������������0��


#define		PWM0_NORMAL()	PCA_PWM0 = 0	//PWM0�������(Ĭ��)
#define		PWM0_OUT_0()	PCA_PWM0 = 3	//PWM0һֱ���0
#define		PWM1_NORMAL()	PCA_PWM1 = 0	//PWM1�������(Ĭ��)
#define		PWM1_OUT_0()	PCA_PWM1 = 3	//PWM1һֱ���0


/*************	���غ�������	**************/

void	en_PCA(void);
void	en_PWM(uchar channel,uchar start_value);


uchar	pwm0,pwm1;

void	main(void)
{
	uint	i;
	
	pwm0 = 128;			// PWM0 ��ʼֵ
	pwm1 = 64;			// PWM1 ��ʼֵ

	CCAP0H = pwm0;		//set PWM wide
	CCAPM0 = 0x42;		//0x42 Setup PCA module 0 in PWM mode

	CCAP1H = pwm1;		//set PWM wide
	CCAPM1 = 0x42;		//0x42 Setup PCA module 1 in PWM mode

	en_PCA();			//enable PCA

	while (1)
	{
		for(i=0; i<30000; i++)	;	//��ʱһ��
		pwm0++;
		pwm1++;
		CCAP0H = pwm0;		//set PWM wide
		CCAP1H = pwm1;		//set PWM wide
	}
}




//Note: CCAP0H and CCAP1H load the PWM output low time.

void	en_PCA(void)
{
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1);	//��ʼ��PCAģʽ�Ĵ�����
	CL = 0x00;		//clear PCA counter
	CH = 0x00;
	CR = 1;			//Start PCA counter
}

