
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

/************************** ISP/IAP *****************************
;STC12C/LE5201/PWM/AD	STC12C/LE5202/PWM/AD STC12C/LE5204/PWM/AD STC12C/LE5205/PWM/AD �������䣬512�ֽ�/����	(0000~03FF=1K)
;SECTOR1	SECTOR2
;0000~01FF	0200~03FF

; STC12C/LE5206PWM/AD ������Ӧ�ó����޸�Ӧ�ó���0000~17FFH,12������

;STC12C/LE5A60S2/AD/PWM	�������䣬512�ֽ�/��������0x0000��ʼ��
STC12C/LE5A08S2/AD/PWM   8K    16����  0x0000~0x1FFF
STC12C/LE5A16S2/AD/PWM   8K    16����  0x0000~0x1FFF
STC12C/LE5A20S2/AD/PWM   8K    16����  0x0000~0x1FFF
STC12C/LE5A32S2/AD/PWM   28K   56����  0x0000~0x6FFF
STC12C/LE5A40S2/AD/PWM   20K   40����  0x0000~0x4FFF
STC12C/LE5A48S2/AD/PWM   12K   24����  0x0000~0x2FFF
STC12C/LE5A52S2/AD/PWM   8K    16����  0x0000~0x1FFF
STC12C/LE5A56S2/AD/PWM   4K     8����  0x0000~0x0FFF
STC12C/LE5A60S2/AD/PWM   1K     2����  0x0000~0x03FF

;STC1F/L01E/02E/04E/05E  �������䣬512�ֽ�/����	(0000~03FF=1K)
;SECTOR1	SECTOR2
;0000~01FF	0200~03FF

;IAP11F/L06 ������Ӧ�ó����޸�Ӧ�ó���0000~17FFH,12������6K
;IAP11F/L62 ������Ӧ�ó����޸�Ӧ�ó���0000~17FFH,124������62K

STC11F/L01E    1K     2����  0x0000~0x03FF
STC11F/L02E    1K     2����  0x0000~0x03FF
STC11F/L03E    1K     2����  0x0000~0x03FF
STC11F/L04E    1K     2����  0x0000~0x03FF
STC11F/L05E    1K     2����  0x0000~0x03FF

STC11F/L08XE   8K    16����  0x0000~0x1FFF
STC11F/L16XE   8K    16����  0x0000~0x1FFF
STC11F/L20XE   8K    16����  0x0000~0x1FFF
STC11F/L32XE  28K    56����  0x0000~0x6FFF
STC11F/L40XE  20K    40����  0x0000~0x4FFF
STC11F/L48XE  12K    24����  0x0000~0x2FFF
STC11F/L52XE   8K    16����  0x0000~0x1FFF
STC11F/L56XE   4K     8����  0x0000~0x0FFF
STC11F/L60XE   1K     2����  0x0000~0x03FF

STC10F/L02XE   4K     8����  0x0000~0x0FFF
STC10F/L04XE   4K     8����  0x0000~0x0FFF
STC10F/L06XE   4K     8����  0x0000~0x0FFF
STC10F/L08XE   4K     8����  0x0000~0x0FFF
STC10F/L10XE   2K     4����  0x0000~0x07FF
STC10F/L12XE   1K     2����  0x0000~0x03FF

;IAP10F/L14X ������Ӧ�ó����޸�Ӧ�ó���0000~37FFH,28������14K

*/

#include "config.h"

sfr ISP_DATA  = 0xC2;
sfr ISP_ADDRH = 0xC3;
sfr ISP_ADDRL = 0xC4;
sfr ISP_CMD   = 0xC5;
sfr ISP_TRIG  = 0xC6;
sfr ISP_CONTR = 0xC7;

//sfr ISP_CMD   = 0xC5;
#define		ISP_STANDBY()	ISP_CMD = 0		//ISP���������ֹ��
#define		ISP_READ()		ISP_CMD = 1		//ISP��������
#define		ISP_WRITE()		ISP_CMD = 2		//ISPд������
#define		ISP_ERASE()		ISP_CMD = 3		//ISP��������

//sfr ISP_TRIG  = 0xC6;
#define 	ISP_TRIG()	ISP_TRIG = 0x5A,	ISP_TRIG = 0xA5		//ISP��������

//							  7    6    5      4    3    2    1     0    Reset Value
//sfr IAP_CONTR = 0xC7;		IAPEN SWBS SWRST CFAIL  -   WT2  WT1   WT0   0000,x000	//IAP Control Register
#define ISP_EN			0x80
#define ISP_SWBS		0x40
#define ISP_SWRST		0x20
#define ISP_CMD_FAIL	0x10
#define ISP_WAIT_1MHZ	7
#define ISP_WAIT_2MHZ	6
#define ISP_WAIT_3MHZ	5
#define ISP_WAIT_6MHZ	4
#define ISP_WAIT_12MHZ	3
#define ISP_WAIT_20MHZ	2
#define ISP_WAIT_24MHZ	1
#define ISP_WAIT_30MHZ	0

#define	ISP_Fosc	MAIN_Fosc
#if (ISP_Fosc >= 24000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_30MHZ
#elif (ISP_Fosc >= 20000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_24MHZ
#elif (ISP_Fosc >= 12000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_20MHZ
#elif (ISP_Fosc >= 6000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_12MHZ
#elif (ISP_Fosc >= 3000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_6MHZ
#elif (ISP_Fosc >= 2000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_3MHZ
#elif (ISP_Fosc >= 1000000L)
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_2MHZ
#else
	#define		ISP_WAIT_FREQUENCY	ISP_WAIT_1MHZ
#endif

#define		ISP_ENABLE()	ISP_CONTR = (ISP_EN + ISP_WAIT_FREQUENCY)
#define		ISP_DISABLE()	ISP_CONTR = 0; ISP_CMD = 0; ISP_TRIG = 0; ISP_ADDRH = 0xff; ISP_ADDRL = 0xff

/*********************************************************************/

void DisableEEPROM(void)		//���������Բ��ã�ֻ�ǳ��ڰ�ȫ���Ƕ���
{
	ISP_CONTR = 0;				//��ֹISP/IAP����
	ISP_CMD   = 0;				//ȥ��ISP/IAP����
	ISP_TRIG  = 0;				//��ֹISP/IAP�����󴥷�
	ISP_ADDRH = 0xff;			//ָ���EEPROM������ֹ�����
	ISP_ADDRL = 0xff;			//ָ���EEPROM������ֹ�����
}


/******************** ��N���ֽں��� ���255�ֽ�һ�� *****************/
void EEPROM_read_n(uint EE_address,uchar *DataAddress,uchar lenth)
{
	EA = 0;		//��ֹ�ж�
	ISP_ENABLE();					//�����, ���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	ISP_READ();						//�����, ���ֽڶ���������ı�ʱ����������������
	do
	{
		ISP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		ISP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		ISP_TRIG();							//�����, ����5AH������A5H��ISP/IAP�����Ĵ�����ÿ�ζ���Ҫ���
		_nop_();
		*DataAddress = ISP_DATA;			//��������������
		EE_address++;
		DataAddress++;
	}while(--lenth);

	DisableEEPROM();
	EA = 1;		//���������ж�
}

/******************** ������������ *****************/
void EEPROM_SectorErase(uint EE_address)
{
	EA = 0;		//��ֹ�ж�
											//ֻ������������û���ֽڲ�����512�ֽ�/������
											//����������һ���ֽڵ�ַ����������ַ��
	ISP_ADDRH = EE_address / 256;			//��������ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
	ISP_ADDRL = EE_address % 256;			//��������ַ���ֽ�
	ISP_ENABLE();							//���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	ISP_ERASE();							//�����, ������������������ı�ʱ����������������
	ISP_TRIG();								//�����, ����5AH������A5H��ISP/IAP�����Ĵ�����ÿ�ζ���Ҫ���
	DisableEEPROM();
	EA = 1;		//���������ж�
}

/******************** дN���ֽں��� ���255�ֽ�һ�� *****************/
void EEPROM_write_n(uint EE_address,uchar *DataAddress,uchar lenth)
{
	EA = 0;		//��ֹ�ж�
	ISP_ENABLE();							//���õȴ�ʱ�䣬����ISP/IAP��������һ�ξ͹�
	ISP_WRITE();							//�����, ���ֽ�д��������ı�ʱ����������������
	do
	{
		ISP_ADDRH = EE_address / 256;		//�͵�ַ���ֽڣ���ַ��Ҫ�ı�ʱ���������͵�ַ��
		ISP_ADDRL = EE_address % 256;		//�͵�ַ���ֽ�
		ISP_DATA  = *DataAddress;			//�����ݵ�ISP_DATA��ֻ�����ݸı�ʱ����������
		ISP_TRIG();							//�����, ����5AH������A5H��ISP/IAP�����Ĵ�����ÿ�ζ���Ҫ���
		_nop_();
		EE_address++;						//��һ����ַ
		DataAddress++;						//��һ������
	}while(--lenth);						//ֱ������

	DisableEEPROM();
	EA = 1;		//���������ж�
}

