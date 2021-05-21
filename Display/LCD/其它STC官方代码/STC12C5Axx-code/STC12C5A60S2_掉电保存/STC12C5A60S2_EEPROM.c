
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
;STC12C/LE5201/PWM/AD	STC12C/LE5202/PWM/AD STC12C/LE5204/PWM/AD STC12C/LE5205/PWM/AD 扇区分配，512字节/扇区	(0000~03FF=1K)
;SECTOR1	SECTOR2
;0000~01FF	0200~03FF

; STC12C/LE5206PWM/AD 可以在应用程序修改应用程序。0000~17FFH,12个扇区

;STC12C/LE5A60S2/AD/PWM	扇区分配，512字节/扇区，从0x0000开始。
STC12C/LE5A08S2/AD/PWM   8K    16扇区  0x0000~0x1FFF
STC12C/LE5A16S2/AD/PWM   8K    16扇区  0x0000~0x1FFF
STC12C/LE5A20S2/AD/PWM   8K    16扇区  0x0000~0x1FFF
STC12C/LE5A32S2/AD/PWM   28K   56扇区  0x0000~0x6FFF
STC12C/LE5A40S2/AD/PWM   20K   40扇区  0x0000~0x4FFF
STC12C/LE5A48S2/AD/PWM   12K   24扇区  0x0000~0x2FFF
STC12C/LE5A52S2/AD/PWM   8K    16扇区  0x0000~0x1FFF
STC12C/LE5A56S2/AD/PWM   4K     8扇区  0x0000~0x0FFF
STC12C/LE5A60S2/AD/PWM   1K     2扇区  0x0000~0x03FF

;STC1F/L01E/02E/04E/05E  扇区分配，512字节/扇区	(0000~03FF=1K)
;SECTOR1	SECTOR2
;0000~01FF	0200~03FF

;IAP11F/L06 可以在应用程序修改应用程序。0000~17FFH,12个扇区6K
;IAP11F/L62 可以在应用程序修改应用程序。0000~17FFH,124个扇区62K

STC11F/L01E    1K     2扇区  0x0000~0x03FF
STC11F/L02E    1K     2扇区  0x0000~0x03FF
STC11F/L03E    1K     2扇区  0x0000~0x03FF
STC11F/L04E    1K     2扇区  0x0000~0x03FF
STC11F/L05E    1K     2扇区  0x0000~0x03FF

STC11F/L08XE   8K    16扇区  0x0000~0x1FFF
STC11F/L16XE   8K    16扇区  0x0000~0x1FFF
STC11F/L20XE   8K    16扇区  0x0000~0x1FFF
STC11F/L32XE  28K    56扇区  0x0000~0x6FFF
STC11F/L40XE  20K    40扇区  0x0000~0x4FFF
STC11F/L48XE  12K    24扇区  0x0000~0x2FFF
STC11F/L52XE   8K    16扇区  0x0000~0x1FFF
STC11F/L56XE   4K     8扇区  0x0000~0x0FFF
STC11F/L60XE   1K     2扇区  0x0000~0x03FF

STC10F/L02XE   4K     8扇区  0x0000~0x0FFF
STC10F/L04XE   4K     8扇区  0x0000~0x0FFF
STC10F/L06XE   4K     8扇区  0x0000~0x0FFF
STC10F/L08XE   4K     8扇区  0x0000~0x0FFF
STC10F/L10XE   2K     4扇区  0x0000~0x07FF
STC10F/L12XE   1K     2扇区  0x0000~0x03FF

;IAP10F/L14X 可以在应用程序修改应用程序。0000~37FFH,28个扇区14K

*/

#include "config.h"

sfr ISP_DATA  = 0xC2;
sfr ISP_ADDRH = 0xC3;
sfr ISP_ADDRL = 0xC4;
sfr ISP_CMD   = 0xC5;
sfr ISP_TRIG  = 0xC6;
sfr ISP_CONTR = 0xC7;

//sfr ISP_CMD   = 0xC5;
#define		ISP_STANDBY()	ISP_CMD = 0		//ISP空闲命令（禁止）
#define		ISP_READ()		ISP_CMD = 1		//ISP读出命令
#define		ISP_WRITE()		ISP_CMD = 2		//ISP写入命令
#define		ISP_ERASE()		ISP_CMD = 3		//ISP擦除命令

//sfr ISP_TRIG  = 0xC6;
#define 	ISP_TRIG()	ISP_TRIG = 0x5A,	ISP_TRIG = 0xA5		//ISP触发命令

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

void DisableEEPROM(void)		//以下语句可以不用，只是出于安全考虑而已
{
	ISP_CONTR = 0;				//禁止ISP/IAP操作
	ISP_CMD   = 0;				//去除ISP/IAP命令
	ISP_TRIG  = 0;				//防止ISP/IAP命令误触发
	ISP_ADDRH = 0xff;			//指向非EEPROM区，防止误操作
	ISP_ADDRL = 0xff;			//指向非EEPROM区，防止误操作
}


/******************** 读N个字节函数 最多255字节一次 *****************/
void EEPROM_read_n(uint EE_address,uchar *DataAddress,uchar lenth)
{
	EA = 0;		//禁止中断
	ISP_ENABLE();					//宏调用, 设置等待时间，允许ISP/IAP操作，送一次就够
	ISP_READ();						//宏调用, 送字节读命令，命令不需改变时，不需重新送命令
	do
	{
		ISP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
		ISP_ADDRL = EE_address % 256;		//送地址低字节
		ISP_TRIG();							//宏调用, 先送5AH，再送A5H到ISP/IAP触发寄存器，每次都需要如此
		_nop_();
		*DataAddress = ISP_DATA;			//读出的数据送往
		EE_address++;
		DataAddress++;
	}while(--lenth);

	DisableEEPROM();
	EA = 1;		//重新允许中断
}

/******************** 扇区擦除函数 *****************/
void EEPROM_SectorErase(uint EE_address)
{
	EA = 0;		//禁止中断
											//只有扇区擦除，没有字节擦除，512字节/扇区。
											//扇区中任意一个字节地址都是扇区地址。
	ISP_ADDRH = EE_address / 256;			//送扇区地址高字节（地址需要改变时才需重新送地址）
	ISP_ADDRL = EE_address % 256;			//送扇区地址低字节
	ISP_ENABLE();							//设置等待时间，允许ISP/IAP操作，送一次就够
	ISP_ERASE();							//宏调用, 送扇区擦除命令，命令不需改变时，不需重新送命令
	ISP_TRIG();								//宏调用, 先送5AH，再送A5H到ISP/IAP触发寄存器，每次都需要如此
	DisableEEPROM();
	EA = 1;		//重新允许中断
}

/******************** 写N个字节函数 最多255字节一次 *****************/
void EEPROM_write_n(uint EE_address,uchar *DataAddress,uchar lenth)
{
	EA = 0;		//禁止中断
	ISP_ENABLE();							//设置等待时间，允许ISP/IAP操作，送一次就够
	ISP_WRITE();							//宏调用, 送字节写命令，命令不需改变时，不需重新送命令
	do
	{
		ISP_ADDRH = EE_address / 256;		//送地址高字节（地址需要改变时才需重新送地址）
		ISP_ADDRL = EE_address % 256;		//送地址低字节
		ISP_DATA  = *DataAddress;			//送数据到ISP_DATA，只有数据改变时才需重新送
		ISP_TRIG();							//宏调用, 先送5AH，再送A5H到ISP/IAP触发寄存器，每次都需要如此
		_nop_();
		EE_address++;						//下一个地址
		DataAddress++;						//下一个数据
	}while(--lenth);						//直到结束

	DisableEEPROM();
	EA = 1;		//重新允许中断
}

