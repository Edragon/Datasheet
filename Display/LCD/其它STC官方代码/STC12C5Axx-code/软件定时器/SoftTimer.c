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

��Timer0����׼10ms��ʱ��16������ʱ�����ʱ����

���ʹ�ҵ����������PLC�������ʱ����

ÿ����ʱ����ֵ��0��ʼͬ����ʱ������0ֹͣ��

��������Ķ�ʱ����8λ�ģ���ʱֵΪ1~255����Ӧ0.01~2.55��.

ÿ����ʱ������һ��LED, ��ͬ��˸ʱ�䡣

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

uchar	SoftTimer[16];

uchar	code T_BitTable[8] = {1,2,4,8,16,32,64,128};
uchar	code T_ReloadTimerL[16] = {10, 20, 30, 40, 50, 60, 70, 80, 90,100,110,120,130,140,150,160};
uchar	code T_ReloadTimerH[16] = {30, 40, 50, 60, 70, 80, 90,100,110,120,130,140,150,160,170,180};

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
	uchar	i,j;

	InitTimer();		//��ʼ��Timer
	
	while(1)
	{
		for(i=0; i<16; i++)			//���16����ʱ���Ƿ񵹼�ʱ��0
		{
			if(SoftTimer[i] == 0)	//����ʱ��0
			{
				if(i < 8)			//0~7 ��ӦP1.0~P1.7
				{
					P1 ^= T_BitTable[i];	//ȡ����Ӧ��LED
					j = P1 & T_BitTable[i];	//ȡ��ǰ���״̬
				}
				else				//8~15��ӦP2.0~P2.7
				{
					P2 ^= T_BitTable[i-8];		//ȡ����Ӧ��LED
					j = P2 & T_BitTable[i-8];	//ȡ��ǰ���״̬
				}
				if(j == 0)	SoftTimer[i] = T_ReloadTimerL[i];	//����͵�ƽ����װ��ʱֵ
				else		SoftTimer[i] = T_ReloadTimerH[i];	//����ߵ�ƽ����װ��ʱֵ
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
