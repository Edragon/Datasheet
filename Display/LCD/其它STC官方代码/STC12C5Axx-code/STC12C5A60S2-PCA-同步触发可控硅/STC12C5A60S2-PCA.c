

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

PCA�жϲ��Գ���.
ʱ��ʹ��11.0592MHZ.
CCAP0�����ʱ20ms�жϣ�		��P1.0���һ������( 50HZ at 11.0592MHZ).

INT1(P3.3)��������ź�, P1.3��������źŸ��ɿع�,����ʱ��Ϊ50us. 
CCAP1�������ഥ��.
����������50HZ��Ƶ,�������60HZ,�������޸�,������Ƴ��Զ���Ӧ��.
����ʹ��P1.0�����50HZ�źŴ��湤Ƶ������.

PCA����������жϣ�	��P1.1���һ������( 7HZ at 11.0592MHZ).


******************************************/

#include	"PCA.h"

unsigned int PhaseShift;	//����ʱ��,us
sbit	P_TRIG = P1^3;		//�������
unsigned char TrigIndex;	//��������
bit		B_SYNC;				//ͬ����־, �ײ�ʹ��,�û����ɼ�.
bit		B_TrigEnable;		//������ֹ�������

sbit	P10 = P1^0;
sbit	P11 = P1^1;

unsigned int CCAP0_tmp,CCAP1_tmp;
unsigned int PCA_Timer0,PCA_Timer1;

void	PWMn_init(void);


/******************** ������ **************************/
//========================================================================
// ����: void main(void)
// ����: �����ʱֵ����ʾ����
// ����: �ޡ�
// ����: �ޡ�
// �汾: VER1.0
// ����: 2009-12-30
// ��ע: 
//========================================================================
void main(void)
{
	B_TrigEnable = 1;	//1--����,  0--��ֹ
	PhaseShift = 2765;	//����3000us,��������ʾ,�û����Ը��ݵ�λ������,һ��Ϊ46~9170, ��Ӧ50~9950us.
	
	PCA_Timer0 = 9216;		//CCAP0�����ʱ20ms�жϣ���P1.0���һ������( 50HZ at 11.0592MHZ).
	PWMn_init();	//��ʼ��PCA

	IT1 = 1;	// �ⲿ�ж�1�½����ж�
	EX1 = 1;	// �ⲿ�ж�1����

	while(1)
	{
	
	}

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
void	PWMn_init(void)
{
	P1M1 &= ~0x07,	P1M0 |=  0x07;		//P1.0 P1.1 P1.2 ʹ��PUSH-PULL���ģʽ
	CCON = 0;					//���CF��CR��CCF0��CCF1
	IPH |= 0x80;				//PCA�ж�ʹ��������ȼ�
	PPCA = 1;
	CMOD = (PCA_IDLE_DISABLE << 7) | (PCA_SOURCE_SELECT << 1) | PCA_ECF;	//��ʼ��PCAģʽ�Ĵ�����
	CCAPM0 = 0x49;				//16λ�����ʱ��������Ƚ�ƥ���ж�(ECCF0=1)��
	CCAPM1 = 0x49;				//16λ�����ʱ��������Ƚ�ƥ���ж�(ECCF1=1)��
	CL = 0;						//���PCA������������
	CH = 0;
	CCAP0_tmp = 100;
	CCAP1_tmp = 100;
	EA = 1;						//�������ж�
	CR = 1;						//����PCA��
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

#define		TRIG_ON		1
#define		TRIG_OFF	0

void PCA_interrupt (void) interrupt 7
{
	if(CCF0 == 1)		//PCAģ��0�ж�
	{
		P10 = ~P10;
		CCF0 = 0;		//��PCAģ��0�жϱ�־
		CCAP0_tmp += PCA_Timer0;
		CCAP0L = (unsigned char)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP0H = (unsigned char)(CCAP0_tmp >> 8);	//��дCCAP0H
	}

	if(CCF1 == 1)	//PCAģ��1�ж�
	{
		CCF1 = 0;			//��PCAģ��1�жϱ�־
		if(B_TrigEnable && B_SYNC)	//������, �����յ���0ͬ���ź�
		{
			if(TrigIndex == 0)		//����0,��1���������忪ʼ
			{
				P_TRIG = TRIG_ON;
				CCAP1_tmp += 46;	//����ʱ��Ϊ50us
				TrigIndex = 1;
			}
			else if(TrigIndex == 1)	//����1, ��1�������������
			{
				P_TRIG = TRIG_OFF;
				CCAP1_tmp += 9170;	//���ʱ��Ϊ10000-50 = 9950us
				TrigIndex = 2;
			}
			else if(TrigIndex == 2)	//����2,��2���������忪ʼ
			{
				P_TRIG = TRIG_ON;
				CCAP1_tmp += 46;	//����ʱ��Ϊ50us
				TrigIndex = 3;
			}
			else if(TrigIndex == 3)	//����1, ��1�������������
			{
				P_TRIG = TRIG_OFF;
				CCAP1_tmp += 9170;	//���ʱ��û������,���ǲ���ȡ��
				B_SYNC = 0;			//һ��ͬ����������
			}
			CCAP1L = (unsigned char)CCAP1_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
			CCAP1H = (unsigned char)(CCAP1_tmp >> 8);	//��дCCAP0H
		}
		else	P_TRIG = TRIG_OFF;
	}

	if(CF == 1)	//PCA����ж�
	{
		P11 = ~P11;		//7HZ at 11.0592MHZ
		CF = 0;			//��PCA����жϱ�־
	}
}

//========================================================================
// ����: void INT1_int (void) interrupt 2
// ����: INT1�жϷ������
// ����: �ޡ�
// ����: �ޡ�
// ����: Coody
// �汾: VER1.0
// ����: 2010-7-31
// ��ע: 
//========================================================================
void INT1_int (void) interrupt 2
{
	unsigned char	i,j;

	i = CH;
	j = CL;
	if(i != CH)	i++;	//��ʱ�պ÷�����λ
	CCAP1_tmp = i;
	CCAP1_tmp = (CCAP1_tmp << 8) + j;
	CCAP1_tmp += PhaseShift;	//����
	CCAP1L = (unsigned char)CCAP1_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
	CCAP1H = (unsigned char)(CCAP1_tmp >> 8);	//��дCCAP0H
	TrigIndex = 0;		//��������ָʾ
	B_SYNC = 1;			//��־ͬ��
}

