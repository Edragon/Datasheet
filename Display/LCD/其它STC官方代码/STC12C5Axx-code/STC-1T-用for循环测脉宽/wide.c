
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

��forѭ����������.  ͨ�ø���1T��MCU�͡�

******************************************/

#define		MCU_MODE	1	//1: 1T,   6: 6T,   12: 12T

#include	<reg51.h>

//#define		MAIN_Fosc	 5529600L	//����ʱ��
//#define		MAIN_Fosc	 6000000L	//����ʱ��
//#define		MAIN_Fosc	11059200L	//����ʱ��
//#define		MAIN_Fosc	12000000L	//����ʱ��
//#define		MAIN_Fosc	16000000L	//����ʱ��
//#define		MAIN_Fosc	16384000L	//����ʱ��
//#define		MAIN_Fosc	18432000L	//����ʱ��
//#define		MAIN_Fosc	20000000L	//����ʱ��
#define		MAIN_Fosc	22118400L	//����ʱ��
//#define		MAIN_Fosc	24000000L	//����ʱ��
//#define		MAIN_Fosc	27000000L	//����ʱ��
//#define		MAIN_Fosc	30000000L	//����ʱ��
//#define		MAIN_Fosc	32768000L	//����ʱ��
//#define		MAIN_Fosc	33000000L	//����ʱ��
//#define		MAIN_Fosc	33177600L	//����ʱ��

sbit	P_PULSE = P1^0;	//��������
sbit	P_TXD1  = P3^1;	//ģ�⴮�ڷ���

void	Tx1Send(unsigned char dat);
void	PrintString(unsigned char code *puts);
void  	delay_ms(unsigned char ms);

unsigned int wide_H;	//������ʱ��
unsigned int wide_L;	//������ʱ��


void main(void)
{
	unsigned int j;

	PrintString("ģ�⴮�ڷ��ͣ���ʱ����.  ͨ�ø���IC��Ƶ�ʡ� 2011-2-2\r\n");

	while(1)
	{
		delay_ms(250);	//��ʱ0.5��
		delay_ms(250);
		
		for(wide_H=0; wide_H<=65000; wide_H++)	//�ȴ�һ���͵�ƽ, ���źų�ʱ�˳�
		{
			if(!P_PULSE)	break;
		}
		
		for(wide_H=0; wide_H<=65000; wide_H++)	//�ȴ�һ���ߵ�ƽ, ���źų�ʱ�˳�
		{
			if(P_PULSE)	break;
		}
		
		for(wide_H=0; wide_H<=65000; wide_H++)	//��һ���ߵ�ƽ
		{
			if(!P_PULSE)	break;
		}
		
		for(wide_L=0; wide_L<=65000; wide_L++)	//��һ���͵�ƽ
		{
			if(P_PULSE)	break;
		}
		
		if(wide_H > 65000)	PrintString("�����ߵ�ƽ��ʱ!\r\n");
		else
		{
			PrintString("�������ߵ�ƽ���� = ");
			j = wide_H;
			Tx1Send(j / 10000 + '0');
			Tx1Send(j % 10000 / 1000 + '0');
			Tx1Send(j % 1000 / 100 + '0');
			Tx1Send(j % 100 / 10 + '0');
			Tx1Send(j % 10 + '0');
			Tx1Send(0x0d);
			Tx1Send(0x0a);
		}

		if(wide_L > 65000)	PrintString("�����͵�ƽ��ʱ!\r\n");
		else
		{
			PrintString("�������͵�ƽ���� = ");
			j = wide_L;
			Tx1Send(j / 10000 + '0');
			Tx1Send(j % 10000 / 1000 + '0');
			Tx1Send(j % 1000 / 100 + '0');
			Tx1Send(j % 100 / 10 + '0');
			Tx1Send(j % 10 + '0');
			Tx1Send(0x0d);
			Tx1Send(0x0a);
		}
			Tx1Send(0x0d);
			Tx1Send(0x0a);
		
	}
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
		i = MAIN_Fosc / 14000L;	//1T
		while(--i)	;   //8T per loop at 12T or 6T mode, 14T at 1T mode
     }while(--ms);
}

/********************** ģ�⴮����غ���************************/

void	BitTime(void)	//λʱ�亯�� 9600bps
{
	unsigned int i;

	i = ((MAIN_Fosc / 100) * 104) / 140000L - 1;	//������ʱ��������λʱ�� 1T
	while(--i)	;   //8T per loop at 12T or 6T mode, 14T at 1T mode
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

void PrintString(unsigned char code *puts)		//����һ���ַ���
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//����ֹͣ��0����
}
