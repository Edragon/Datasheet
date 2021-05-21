
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

����xdata��д���ʲ�Ӱ��IO(WR,RD,P0,P2,ALE)��

*****************************************/

#include	<reg51.h>

#define	uchar unsigned char
#define	uint  unsigned int

sfr BUS_SPEED = 0xA1;
sfr	AUXR = 0x8E;

#define		BUS_ALE_1T()	BUS_SPEED &= 0x0f
#define		BUS_ALE_2T()	BUS_SPEED = (BUS_SPEED & 0x0f) | 0x10
#define		BUS_ALE_3T()	BUS_SPEED = (BUS_SPEED & 0x0f) | 0x20	//default
#define		BUS_ALE_4T()	BUS_SPEED = (BUS_SPEED & 0x0f) | 0x30
#define		BUS_RW_1T()		BUS_SPEED &= 0xf0
#define		BUS_RW_2T()		BUS_SPEED = (BUS_SPEED & 0xf0) | 0x01
#define		BUS_RW_3T()		BUS_SPEED = (BUS_SPEED & 0xf0) | 0x02
#define		BUS_RW_4T()		BUS_SPEED = (BUS_SPEED & 0xf0) | 0x03	//default
#define		BUS_RW_5T()		BUS_SPEED = (BUS_SPEED & 0xf0) | 0x04
#define		BUS_RW_6T()		BUS_SPEED = (BUS_SPEED & 0xf0) | 0x05
#define		BUS_RW_7T()		BUS_SPEED = (BUS_SPEED & 0xf0) | 0x06
#define		BUS_RW_8T()		BUS_SPEED = (BUS_SPEED & 0xf0) | 0x07

#define 	EXTRAM_enable()			AUXR |=  0x02			//�����ⲿXRAM����ֹʹ���ڲ�1024RAM
#define 	INTRAM_enable()			AUXR &= ~0x02			//��ֹ�ⲿXRAM������ʹ���ڲ�1024RAM

uchar	xdata	*ptr;
uchar	ReadData;
sbit	P10 = P1^0;
sbit	P11 = P1^1;

#define	START_XDATA		0		//��ʼ��ַ
#define	XDATA_LENTH		1024	//����


/******************** task A **************************/
void main(void)
{
	uchar	i;

//	BUS_ALE_1T();		//ƥ��ALE�ٶ�
//	BUS_RW_2T();		//ƥ���д�ٶ�
//	EXTRAM_enable();	//ʹ���ⲿ64K XDATA
	INTRAM_enable();	//ʹ���ڲ�1024�ֽ�XDATA

	while (1)
	{
		P10 = 0;	//дָʾ
		i = 0;
		for(ptr=START_XDATA; ptr<XDATA_LENTH; ptr++)	//д
		{
			*ptr = i;
			i++;
		}
		P10 = 1;

		P11 = 0;	//��ָʾ
		i = 0;
		for(ptr=START_XDATA; ptr<XDATA_LENTH; ptr++)	//��
		{
			ReadData = *ptr;
		}
		P11 = 1;
	}
}
