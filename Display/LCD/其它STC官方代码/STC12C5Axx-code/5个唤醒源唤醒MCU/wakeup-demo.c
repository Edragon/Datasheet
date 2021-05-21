
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

��ʾ5������Դ��MCU���ѡ�

�Ӵ���������(ASCII)��9600,8,N,1.

�û��޸���ʱ�ӿ���ƥ���Լ���ϵͳʱ��.

******************************************/


/*************	�û�ϵͳ����	**************/

#define MAIN_Fosc		22118400L	//������ʱ��, ģ�⴮�ںͺ���ʱ���Զ���Ӧ��5~35MHZ



/*************	���º궨���û������޸�	**************/
#include	<reg51.H>
#include 	<intrins.h>

#define	uchar	unsigned char
#define uint	unsigned int

/******************************************/

sfr WAKE_CLKO = 0x8F;


sbit	P_TXD1 = P3^1;
void	Tx1Send(unsigned char dat);
void	PrintString(unsigned char code *puts);
void  	delay_ms(unsigned char ms);

uchar	WakeUpSource;
uchar	WakeUpCnt;

void main(void)
{
	delay_ms(2);	//delay 2ms

	PrintString("*** STC11Fxxϵ��5������Դ���Գ��� ***\r\n");
	
	while(1)
	{
		T0 = 1;
		T1 = 1;
		INT0 = 1;
		INT1 = 1;
		RXD  = 1;

		WAKE_CLKO |=  (1<<4);	//���� T0 ��/P3.4 �½����� T0 �жϱ�־��Ҳ�ܻ��� powerdown
		WAKE_CLKO |=  (1<<5);	//���� T1 ��/P3.5 �½����� T1 �жϱ�־��Ҳ�ܻ��� powerdown
		WAKE_CLKO |=  (1<<6);	//���� P3.0(RXD) �½����� RI��Ҳ��ʹ RXD ���� powerdown
		IT0 = 1;	// �ⲿ�ж�0�½����жϡ�����
		EX0 = 1;	// �ⲿ�ж�0����
		IT1 = 1;	// �ⲿ�ж�1�½����жϡ�����
		EX1 = 1;	// �ⲿ�ж�1����
		ET0 = 1;	// T0�ж�����
		ET1 = 1;	// T1�ж�����
		ES  = 1;	// RXD�ж�����
		EA  = 1;		//Enable all interrupt
		
		while(!T0);
		while(!T1);
		while(!INT0);
		while(!INT1);
		while(!RXD);
		

		TF1  = 0;	//��ʱ��1����жϱ�־λ
		TF0  = 0;	//��ʱ��0����жϱ�־λ
		IE1  = 0;	//���ж�1��־λ
		IE0  = 0;	//���ж�0��־λ
		TI = 0;
		RI = 0;
		delay_ms(50);	//delay 50ms

		WakeUpSource = 0;

		_nop_();
		_nop_();
		_nop_();
		PCON |= 2;	//Sleep
		_nop_();
		_nop_();
		_nop_();
		
		if(WakeUpSource == 1)	PrintString("���ж�INT0����\r\n");
		if(WakeUpSource == 2)	PrintString("���ж�INT1����\r\n");
		if(WakeUpSource == 3)	PrintString("�ⲿT0����\r\n");
		if(WakeUpSource == 4)	PrintString("�ⲿT1����\r\n");
		if(WakeUpSource == 5)	PrintString("RXD����\r\n");
		WakeUpSource = 0;
		
		WakeUpCnt++;
		Tx1Send(WakeUpCnt/100+'0');
		Tx1Send(WakeUpCnt%100/10+'0');
		Tx1Send(WakeUpCnt%10+'0');
		PrintString("�λ���\r\n");
	}

}


void INT0_int (void) interrupt 0
{
	WakeUpSource = 1;	//���INT0����
}
void Timer0_int (void) interrupt 1
{
	WakeUpSource = 3;		//���Tiner0����
}
void INT1_int (void) interrupt 2
{
	WakeUpSource = 2;	//���INT1����
}
void Timer1_int (void) interrupt 3
{
	WakeUpSource = 4;		//���Tiner1����
}

void UART0_RCV (void) interrupt 4
{
	RI = 0;
	TI = 0;
	WakeUpSource = 5;	//���RXD����
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

