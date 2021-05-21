
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

演示用掉电唤醒定时器将MCU唤醒。睡眠大约2秒唤醒一次, 工作2秒后再睡眠. 

定时器的基准在5V时大约为0.56ms, 降低电压可能在0.5~1ms之间,以实测为准.

用户测量MCU的电流来确认.睡眠时电流大约2~3uA.

用户测试时,请保证MCU睡眠后其外围电路不耗电,才能测出MCU的睡眠电流.

为了测量MCU本身的睡眠电流,建议MCU的IO口全部悬空,复位脚要处理好,MCU有尽量靠近的0.1uF+10~100uF退耦电容.

用户可以通过测量P1.0 P1.1的低电平时间来确认工作和睡眠时间.

用户根据实际的工作频率修改 MAIN_Fosc 的值,其它程序请勿改动.

******************************************/


/*************	用户系统配置	**************/

#define MAIN_Fosc		5080000UL	//定义主时钟, 延时会自动适应。5~35MHZ



/*************	以下宏定义用户请勿修改	**************/
#include	"reg51.H"
#include <intrins.h>

#define	uchar	unsigned char
#define uint	unsigned int

/******************************************/


sfr WKTCL = 0xAA;	//STC11F系列 唤醒定时器低字节
sfr WKTCH = 0xAB;	//STC11F系列 唤醒定时器高字节
//	B7		B6	B5	B4	B3	B2	B1	B0		B7	B6	B5	B4	B3	B2	B1	B0
//	WKTEN				S11	S10	S9	S8		S7	S6	S5	S4	S3	S2	S1	S0	n * 560us
#define		WakeTimerDisable()		WKTCH &= 0x7f	//WKTEN = 0		禁止睡眠唤醒定时器
#define		WakeTimerSet(scale)		WKTCL = (scale) % 256,WKTCH = ((scale) / 256 & 0x0f) | 0x80		//WKTEN = 1	允许睡眠唤醒定时器



void  	delay_ms(unsigned char ms);

sbit	P10 = P1^0;
sbit	P11 = P1^1;

void main(void)
{

	uchar	i;
	
	while(1)
	{
		P10 = 0;		//P1.0低电平是工作时间
		for(i=0; i<8; i++)	delay_ms(250);	//延时2秒(工作2秒)
		P10 = 1;
		
		P11 = 0;		//P1.1低电平是睡眠时间
		WakeTimerSet(3571);		//睡眠唤醒定时器初值  3571*0.56 = 2000 ms
		
		PCON |= 2;	//Sleep
		_nop_();
		_nop_();
		P11 = 1;
	}

}


//========================================================================
// 函数: void  delay_ms(unsigned char ms)
// 描述: 延时函数。
// 参数: ms,要延时的ms数.
// 返回: none.
// 版本: VER1.0
// 日期: 2010-12-15
// 备注: 
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

