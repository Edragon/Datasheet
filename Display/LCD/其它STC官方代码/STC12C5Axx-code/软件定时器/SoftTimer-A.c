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

/*************	����˵��	**************

��Timer0����׼10ms��ʱ��16������ʱ�����ʱ����

���ʹ�ҵ����������PLC�������ʱ����

ÿ����ʱ����ֵ��0��ʼͬ����ʱ������0ֹͣ��

��������Ķ�ʱ����8λ�ģ���ʱֵΪ1~255����Ӧ0.01~2.55��.

��ʱ��0~7����P1�ϵ�8��LED����ʱ��8Ϊ��ɨ�趨ʱ������P2�����룬��Ӧ8��LED��

��ʱ��9~15û��ʹ��.

******************************************/



/*************	�û�ϵͳ����	**************/

#define MAIN_Fosc		24000000L	//������ʱ��

#define D_TIMER0		10000		//ѡ��ʱ��ʱ��, us


/*************	���º궨���û������޸�	**************/
#include	"reg51.H"
#define	uchar	unsigned char
#define uint	unsigned int

#define freq_base			(MAIN_Fosc / 1200)
#define Timer0_Reload		(65536 - (D_TIMER0 * freq_base / 10000))

uchar	SoftTimer[16];		//16�������ʱ��

uchar	SoftTimerEnable;	//��ʱ��������ƣ���ӦλΪ1�������Ӧ�Ķ�ʱ����Ϊ0����ֹ��
uchar	KeyState;			//��״̬


uchar	code T_BitTable[8] = {1,2,4,8,16,32,64,128};

/**************** Timer��ʼ������ ******************************/
void InitTimer(void)
{
	TMOD = 1;
	TH0 = Timer0_Reload / 256;
	TL0 = Timer0_Reload % 256;
	ET0 = 1;
	TR0 = 1;
	EA  = 1;
}

/********************* ������ *************************/
void main(void)
{
	uchar	i;

	InitTimer();		//��ʼ��Timer
	
	while(1)
	{
		if(SoftTimer[8] == 0)	//SoftTimer[8]����ʱ��0, ��ɨ��
		{
			SoftTimer[8] = 5;	//50ms ���ɨ��
			i = KeyState;		//������һ�μ�ֵ
			KeyState = ~P2;		//����
			i = (i ^ KeyState) & KeyState;	//���㰴�µļ�
			SoftTimerEnable ^= i;	//���µļ�����Ӧ��λȡ��
		}
		
		for(i=0; i<8; i++)		//���8����ʱ���Ƿ񵹼�ʱ��0
		{
			if(SoftTimerEnable & T_BitTable[i])		//����Ӧ�Ķ�ʱ��������
			{
				if(SoftTimer[i] == 0)		//��ʱ������ʱ��0
				{
					P1 ^= T_BitTable[i];	//ȡ����Ӧ��LED
					SoftTimer[i] = 25;		//��װ��Ӧ�Ķ�ʱֵ��2HZ��˸
				}
			}
			else		//��ʱ������ֹ
			{
				SoftTimer[i] = 0;		// ֹͣ��Ӧ�Ķ�ʱ��
				P1 |= T_BitTable[i];	// �رն�Ӧ��LED
			}
		}
	}
}

/********************** Timer0�жϺ���************************/
void timer0 (void) interrupt 1
{
	uchar	i;

	TR0 = 0;
	TH0 = Timer0_Reload / 256;
	TL0 = Timer0_Reload % 256;
	TR0 = 1;

	for(i=0; i<16; i++)
	{
		if(SoftTimer[i] != 0)	SoftTimer[i]--;		//����ʱ��ʱ��
	}
}
