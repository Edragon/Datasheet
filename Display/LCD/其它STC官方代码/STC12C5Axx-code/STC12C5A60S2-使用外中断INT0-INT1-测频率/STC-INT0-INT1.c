
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

�ⲿ�ж�INT0��INT1�����źŲ�Ƶ�ʣ��½����ж�, PCA0���붨ʱ��

���Ƶ���ź�Ϊ��ʱ�ӵ� 1/300.  ����24MHZ��ʱ��,���Բ���80KHZ.

����Ӵ������, 9600,8,N,1.  ��Ƶ11.0592MHZ.


******************************************/



#include	"reg51.H"

#define	uchar	unsigned char
#define uint	unsigned int


//#define		STC12C5201AD				//���ע������PCA���Ĭ��ʹ��STC12C5A60S2ϵ��

#define MAIN_Fosc		11059200L		//������ʱ��

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
#define		PCA_ECF				0		//1: ����PCA����������жϣ�0: ��ֹ

/**********************************************************************************************************/

#include	<reg51.h>

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


uint	CCAP0_tmp;						//�Ƚ�ƥ��Ӱ��Ĵ���
uint	Freq_INT0,Freq_INT1;			//Ƶ��, 0~10KHZ
uint	Freq_INT0_cnt,Freq_INT1_cnt;	//Ƶ�ʼ���, 0~10KHZ
uchar	cnt_1S;							//1�붨ʱ
bit		B_FrequencyOk;					//��ƵOK��־


void	uart0_init(void);
void	TxByte(uchar dat);
void	PCA_init(void);


/********************* ������ *************************/
void main(void)
{
	IT0 = 1;	// �ⲿ�ж�0�½����ж�
	EX0 = 1;	// �ⲿ�ж�0����
	IT1 = 1;	// �ⲿ�ж�1�½����ж�
	EX1 = 1;	// �ⲿ�ж�1����
	
	PX1	 = 1;	//�ⲿ�ж�1���ȼ��趨λ
	PX0	 = 1;	//�ⲿ�ж�0���ȼ��趨λ
	
	PCA_init();	//PCA��ʼ��
	uart0_init();

	EA  = 1;	// �ж�������

	while(1)
	{
		if(B_FrequencyOk)
		{
			B_FrequencyOk = 0;
			TxByte('F');
			TxByte('0');
			TxByte('=');
			TxByte(Freq_INT0/10000+'0');
			TxByte(Freq_INT0%10000/1000+'0');
			TxByte(Freq_INT0%1000/100+'0');
			TxByte(Freq_INT0%100/10+'0');
			TxByte(Freq_INT0%10+'0');
			TxByte('H');
			TxByte('Z');
			TxByte(0x0d);
			TxByte(0x0a);

			TxByte('F');
			TxByte('1');
			TxByte('=');
			TxByte(Freq_INT1/10000+'0');
			TxByte(Freq_INT1%10000/1000+'0');
			TxByte(Freq_INT1%1000/100+'0');
			TxByte(Freq_INT1%100/10+'0');
			TxByte(Freq_INT1%10+'0');
			TxByte('H');
			TxByte('Z');
			TxByte(0x0d);
			TxByte(0x0a);
		}
	}
}


void	TxByte(uchar dat)
{
	TI = 0;
	SBUF = dat;
	while(!TI);
	TI = 0;
}

void	uart0_init(void)
{
	PCON |= 0x80;		//UART0 Double Rate Enable
	SCON = 0x50;		//UART0 set as 10bit , UART0 RX enable

	TMOD &= ~(1<<6);		//Timer1 Set as Timer, defalul 12T
	TMOD = (TMOD & ~0x30) | 0x20;	//Timer1 set as 8 bits auto relaod
	TH1 = 250;		//Load the timer
	TR1  = 1;
}




//========================================================================
// ����: void	PWMn_init(void)
// ����: ��ʼ������
// ����: �ޡ�
// ����: �ޡ�
// �汾: VER1.0
// ����: 2009-12-30
// ��ע: 
//========================================================================
void	PCA_init(void)
{
	CCON = 0;					//���CF��CR��CCF0��CCF1
	PPCA = 1;
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1) | PCA_ECF;	//��ʼ��PCAģʽ�Ĵ�����
	CCAPM0 = 0x49;				//16λ�����ʱ��������Ƚ�ƥ���ж�(ECCF0=1)��
	CL = 0;						//���PCA������������
	CH = 0;
	CCAP0_tmp = 18432;		//18432*12/11059.200 = 20ms
	CCAP0L = (uchar)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
	CCAP0H = (uchar)(CCAP0_tmp >> 8);	//��дCCAP0H
	CR = 1;						//����PCA��
}

//========================================================================
// ����: void PCA_interrupt (void) interrupt 7
// ����: PCA�жϷ������
// ����: �ޡ�
// ����: �ޡ�
// ����: Coody
// �汾: VER1.0
// ����: 2010-7-31
// ��ע: 
//========================================================================
void PCA_interrupt (void) interrupt 7
{
	if(CCF0 == 1)		//PCAģ��0�ж�
	{
		if(++cnt_1S >= 50)				//50*20=1000ms
		{
			EA = 0;
			Freq_INT0 = Freq_INT0_cnt;	//Ƶ��1
			Freq_INT0_cnt = 0;			//���Ƶ�ʼ���
			Freq_INT1 = Freq_INT1_cnt;	//Ƶ��2
			Freq_INT1_cnt = 0;			//���Ƶ�ʼ���
			EA = 1;
			B_FrequencyOk = 1;			//��־Ƶ��OK
			cnt_1S = 0;
		}

		CCF0 = 0;				//��PCAģ��0�жϱ�־
		CCAP0_tmp += 18432;		//18432*12/11059.200 = 20ms
		CCAP0L = (uchar)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP0H = (uchar)(CCAP0_tmp >> 8);	//��дCCAP0H
	}

	CCF1 = 0;		//��PCAģ��1�жϱ�־
	CF = 0;			//��PCA����жϱ�־
}



/********************* INT0�жϺ��� *************************/
void INT0_int (void) interrupt 0
{
	Freq_INT0_cnt++;	//Ƶ�ʼ���+1
}

/********************* INT1�жϺ��� *************************/
void INT1_int (void) interrupt 2
{
	Freq_INT1_cnt++;	//Ƶ�ʼ���+1
}
