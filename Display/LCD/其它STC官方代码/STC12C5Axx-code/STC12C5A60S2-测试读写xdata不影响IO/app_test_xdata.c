
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU Programme Demo ----------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*      本程序经过测试完全正常, 不提供电话技术支持, 如不能理解, 请自行补充相关基础.  */


/*************	本程序功能说明	**************

测试xdata读写访问不影响IO(WR,RD,P0,P2,ALE)。

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

#define 	EXTRAM_enable()			AUXR |=  0x02			//允许外部XRAM，禁止使用内部1024RAM
#define 	INTRAM_enable()			AUXR &= ~0x02			//禁止外部XRAM，允许使用内部1024RAM

uchar	xdata	*ptr;
uchar	ReadData;
sbit	P10 = P1^0;
sbit	P11 = P1^1;

#define	START_XDATA		0		//开始地址
#define	XDATA_LENTH		1024	//长度


/******************** task A **************************/
void main(void)
{
	uchar	i;

//	BUS_ALE_1T();		//匹配ALE速度
//	BUS_RW_2T();		//匹配读写速度
//	EXTRAM_enable();	//使用外部64K XDATA
	INTRAM_enable();	//使用内部1024字节XDATA

	while (1)
	{
		P10 = 0;	//写指示
		i = 0;
		for(ptr=START_XDATA; ptr<XDATA_LENTH; ptr++)	//写
		{
			*ptr = i;
			i++;
		}
		P10 = 1;

		P11 = 0;	//读指示
		i = 0;
		for(ptr=START_XDATA; ptr<XDATA_LENTH; ptr++)	//读
		{
			ReadData = *ptr;
		}
		P11 = 1;
	}
}
