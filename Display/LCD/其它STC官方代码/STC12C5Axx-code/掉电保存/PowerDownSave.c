
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

EEPROM ���籣����Գ���

������:  9600,8,N,1. �û�������config.h���޸���Ƶ MAIN_Fosc ����Ӧ�Լ���ϵͳ.

MCUÿ��1��Ӵ������һ���仯����(000~250). ����󱣴�,�ϵ�ʱ��ȡ.

������STC11Fxxxxϵ��, ��������ʱ����ѡ��λ��ѹΪ3.7V, ����ѹ����4.1V��ʼ�����ѹ����жϲ�����.

��Դ���˲����ݸ��ݵ�·������С��ѡȡ, ��֤���㹻�ı���ʱ��.


�����ļ�:
STC12C5A60S2_EEPROM.c
PowerDownSave.c

******************************************/


#include	"config.h"


/*************	���س�������	**************/

#define	EEP_address	0x0000


/*************	���ر�������	**************/
sfr	AUXR = 0x8E;

sbit ELVD = IE^6;	//��ѹ����ж�����λ
sbit PLVD = IP^6; 	//��ѹ�ж� ���ȼ��趨λ

//						7   6     5    4     3      2    1     0     Reset Value
//sfr PCON   = 0x87;	-   -   LVDF  POF   GF1    GF0   PD   IDL    0001,0000	 //Power Control 
//LVDF: ��ѹ����־��ֻҪ��ѹ���ڼ���ż���ѹ����λ��1�������0��˯��ǰ��ֹ��ѹ�жϿɽ��͹��ġ��������ѹʱ�жϻ���MCU��

uchar	TestCnt;

sbit	P00 = P0^0;
sbit	P01 = P0^1;
sbit	P_TXD1 = P3^1;

/*************	���غ�������	**************/
void	Tx1Send(unsigned char dat);
void	PrintString(unsigned char code *puts);
void  	delay_ms(unsigned char ms);


/*************  �ⲿ�����ͱ������� *****************/
void	EEPROM_read_n(uint EE_address,uchar *DataAddress,uchar lenth);
void 	EEPROM_SectorErase(uint EE_address);
void 	EEPROM_write_n(uint EE_address,uchar *DataAddress,uchar lenth);



/********************* ������ *************************/
void main(void)
{

	delay_ms(250);

	PCON = PCON & ~(1<<5);	//��ѹ����־��0
	ELVD = 1;	//��ѹ����ж�����
	PLVD = 1; 	//��ѹ�ж� ���ȼ���
	EA  = 1;

	EEPROM_read_n(EEP_address,&TestCnt,1);	//��������ֵ
	if(TestCnt >= 251)	TestCnt = 0;
	EEPROM_SectorErase(EEP_address);		//��������ѹ����ȽϿ�ʱ,�������Ȳ���
	
	P01 = 0;	//��������ָʾΪ�͵�ƽ,�����ڸ�λ״̬ʱ,��IO����ߵ�ƽ.

while(1)
	{
		Tx1Send(TestCnt / 100 + '0');			//��������
		Tx1Send(TestCnt % 100 / 10 + '0');
		Tx1Send(TestCnt % 10 + '0');
		Tx1Send(0x0d);
		Tx1Send(0x0a);
		
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		delay_ms(250);
		if(++TestCnt >= 251)	TestCnt = 0;
	}
} 
/**********************************************/


void	LVD_Routine(void) interrupt 6
{
	uchar	i;

	P00 = 0;	//����LVD�ж�ָʾ,�͵�ƽ
//	EEPROM_SectorErase(EEP_address);	//��������ѹ����ܻ���ʱ,�������������

	EEPROM_write_n(EEP_address,&TestCnt,1);	//���籣��ֵ

	while(PCON & (1<<5))			//����Ƿ���Ȼ�͵�ѹ
	{
		PCON = PCON & ~(1<<5);		//��ѹ����־��0
		for(i=0; i<100; i++)	;	//��ʱһ��
	}
	P00 = 1;	//�˳�LVD�ж�,�ߵ�ƽ
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
	 do
	 {
		i = MAIN_Fosc / 14000L;
		while(--i)	;
     }while(--ms);
}

/********************** ģ�⴮����غ���************************/

void	BitTime(void)	//λʱ�亯�� 9600bps
{
	unsigned int i;
	i = ((MAIN_Fosc / 100) * 104) / 140000L - 1;
	while(--i)	;
}

//ģ�⴮�ڷ���
void	Tx1Send(unsigned char dat)		//9600��N��8��1		����һ���ֽ�
{
	unsigned char	i;
	EA = 0;
	P_TXD1 = 0;
	BitTime();
	i = 8;
	do
	{
		dat >>= 1;
		P_TXD1 = CY;
		BitTime();
	}while(--i);
	P_TXD1 = 1;
	EA = 1;
	BitTime();	//stop bit
	BitTime();	//stop bit
}


