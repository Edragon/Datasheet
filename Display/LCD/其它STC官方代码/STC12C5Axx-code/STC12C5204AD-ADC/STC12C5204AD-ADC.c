
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

P1.0 ��ADC���룬�Ӵ���������(ASCII)��9600,8,N,1.

�ɻ�ֻ��Ҫ���� MAIN_Fosc ����Ӧ�Լ���ϵͳ��

******************************************/


/*************	�û�ϵͳ����	**************/

#define MAIN_Fosc		22118400L	//������ʱ��, ģ�⴮�ںͺ���ʱ���Զ���Ӧ��5~35MHZ

/*************	���º궨���û������޸�	**************/
#include	"reg51.H"
#define	uchar	unsigned char
#define uint	unsigned int
/******************************************/

sfr P1ASF     = 0x9D;	//12C5204AD/S2ϵ��ģ������(AD��LVD)ѡ��
sfr ADC_CONTR = 0xBC;	//��ADϵ��
sfr ADC_RES   = 0xBD;	//��ADϵ��
sfr ADC_RESL  = 0xBE;	//��ADϵ��

//								7       6      5       4         3      2    1    0   Reset Value
//sfr ADC_CONTR = 0xBC;		ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000	//AD ת�����ƼĴ��� 
#define ADC_OFF()	ADC_CONTR = 0
#define ADC_ON		(1 << 7)
#define ADC_90T		(3 << 5)
#define ADC_180T	(2 << 5)
#define ADC_360T	(1 << 5)
#define ADC_540T	0
#define ADC_FLAG	(1 << 4)	//�����0
#define ADC_START	(1 << 3)	//�Զ���0
#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_CH4		4
#define ADC_CH5		5
#define ADC_CH6		6
#define ADC_CH7		7


/************* ���ر������� **************/
sbit	P_TXD1 = P3^1;


/************* ���غ������� **************/
void	Tx1Send(uchar dat);
void	PrintString(unsigned char code *puts);
void  	delay_ms(unsigned char ms);
uchar	GetAdc8(uchar channel);	//channel = 0~7


void TX_ADC(uchar chn, uchar adc)
{
	Tx1Send('A');
	Tx1Send('D');
	Tx1Send(chn+'0');
	Tx1Send('=');
	Tx1Send(adc / 100 + '0');
	Tx1Send(adc % 100 / 10 + '0');
	Tx1Send(adc % 10 + '0');
	Tx1Send(0x0d);
	Tx1Send(0x0a);
}

/********************* ������ *************************/
void main(void)
{
	uint	j;

	PrintString("****** STC12C5204ADϵ��ADC���� 2011-02-27 ******\r\n");	//�ϵ�󴮿ڷ���һ����ʾ��Ϣ

	P1ASF = 1 << ADC_CH0;			//12C5204ADϵ��ģ������(AD)ѡ��
	ADC_CONTR = ADC_360T | ADC_ON;

	while(1)
	{
		delay_ms(250);		//1��ת��1��
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		
		GetAdc8(0);			// P1.0 ADC, ����
		j = GetAdc8(0);		// P1.0 ADC
		TX_ADC(0,j);

		Tx1Send(0x0d);
		Tx1Send(0x0a);
	}
}

/********************* ��һ��ADCת�� *******************/
uchar	GetAdc8(uchar channel)	//channel = 0~7
{
	uchar	i;

	ADC_RES = 0;

	ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | channel; 

	for(i=0; i<250; i++)		//13T/loop, 40*13=520T=23.5us @ 22.1184M
	{
		if(ADC_CONTR & ADC_FLAG)
		{
			ADC_CONTR &= ~ADC_FLAG;
			return	ADC_RES;
		}
	}while(--i);
	return	0;	//����
}

//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��.
// ����: none.
// �汾: VER1.0
// ����: 2010-12-15
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
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
	EA = 0;
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
	EA = 1;
	BitTime();
	BitTime();
}

void PrintString(unsigned char code *puts)		//����һ���ַ���
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//����ֹͣ��0����
}
