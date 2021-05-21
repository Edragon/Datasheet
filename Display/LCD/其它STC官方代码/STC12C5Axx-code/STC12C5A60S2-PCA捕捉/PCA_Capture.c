
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

STC12C5A60S2ϵ�� ��P1.3 P1.4 ����׽���룬�Ӵ���������(ASCII)��9600,8,N,1.
STC12C5204ADϵ�� ��P3.7 P3.5 ����׽���룬�Ӵ���������(ASCII)��9600,8,N,1.

******************************************/


/*************	�û�ϵͳ����	**************/

#define MAIN_Fosc		22118400L	//������ʱ��, ģ�⴮�ںͺ���ʱ���Զ���Ӧ��5~35MHZ

#define		PCA_IDLE_DISABLE	0			//1: MCU��IDLEģʽʱ��ֹPCA������	0:  MCU��IDLEģʽʱ����PCA������
#define		PCA_SOURCE_SELECT	0			//ѡ��PCA�Ļ�׼ʱ��Դ��
											//0��ϵͳʱ��Fosc/12��
											//1��ϵͳʱ��Fosc/2��
											//2����ʱ��0�������
											//3��ECI/P3.4�ŵ��ⲿʱ�����루���=Fosc/2����
											//4��ϵͳʱ��Fosc��
											//5��ϵͳʱ��Fosc/4��
											//6��ϵͳʱ��Fosc/6��
											//7��ϵͳʱ��Fosc/8��
#define		PCA_ECF				1		//1: ����PCA����������жϣ�0: ��ֹ


/*************	���º궨���û������޸�	**************/
#include	"reg51.H"
#define	uchar	unsigned char
#define uint	unsigned int
/******************************************/

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

sbit CCF0  = CCON^0;	//PCA ģ��0�жϱ�־����Ӳ����λ�������������0��
sbit CCF1  = CCON^1;	//PCA ģ��1�жϱ�־����Ӳ����λ�������������0��
sbit CR    = CCON^6;	//1: ����PCA�����������������������0��
sbit CF    = CCON^7;	//PCA�����������CH��CL��FFFFH��Ϊ0000H����־��PCA�������������Ӳ����λ�������������0��

/************* ���ر������� **************/
uint	CCAP0_tmp,CCAP1_tmp;
uint	CCAP0_Last,CCAP1_Last;
bit		B_Cap0,B_Cap1;
sbit	P_TXD1 = P3^1;

sbit	P10 = P1^0;

/************* ���غ������� **************/
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



/********************* ������ *************************/
void main(void)
{
	CCAPM0 = 0x11;	//CCAP0�½��ز�׽�������ж�		�½��ز�׽: 0x11,  �����ز�׽: 0x21, �����½��ز�׽: 0x31
	CCAPM1 = 0x11;	//CCAP1�½��ز�׽�������ж�		�½��ز�׽: 0x11,  �����ز�׽: 0x21, �����½��ز�׽: 0x31
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1) | PCA_ECF;	//��ʼ��PCAģʽ�Ĵ�����
	CL = 0;					//���PCA������������
	CH = 0;
	CR = 1;					//Start CR
	EA = 1;

	PrintString("****** STC12C5A60S2ϵ��MCU��׽���� 2011-02-25 ******\r\n");	//�ϵ�󴮿ڷ���һ����ʾ��Ϣ

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
// ����: void PCA_interrupt (void) interrupt 7
// ����: PCA�жϷ������
// ����: �ޡ�
// ����: �ޡ�
// �汾: VER1.0
// ����: 2009-12-30
// ��ע: 
//========================================================================
void PCA_interrupt (void) interrupt 7
{
	if(CCF0 == 1)		//PCAģ��0�ж�
	{
		CCF0 = 0;		//��PCAģ��0�жϱ�־
		CCAP0_tmp = CCAP0H;	//��CCAP0H
		CCAP0_tmp = (CCAP0_tmp << 8) + CCAP0L;
		B_Cap0 = 1;
	}

	if(CCF1 == 1)	//PCAģ��1�ж�
	{
		CCF1 = 0;		//��PCAģ��1�жϱ�־
		CCAP1_tmp = CCAP1H;	//��CCAP0H
		CCAP1_tmp = (CCAP1_tmp << 8) + CCAP1L;
		B_Cap1 = 1;
	}

	if(CF == 1)	//PCA����ж�
	{
		CF = 0;			//��PCA����жϱ�־
		P10 = ~P10;		//14HZ at 22.1184MHZ
	}
}

/********************** ģ�⴮����غ���************************/

void	BitTime(void)	//λʱ�亯��
{
	uint i;
	i = ((MAIN_Fosc / 100) * 104) / 140000L - 1;		//������ʱ��������λʱ��
	while(--i);
}

//ģ�⴮�ڷ���
void	Tx1Send(uchar dat)		//9600��N��8��1		����һ���ֽ�
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

void PrintString(unsigned char code *puts)		//����һ���ַ���
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//����ֹͣ��0����
}
