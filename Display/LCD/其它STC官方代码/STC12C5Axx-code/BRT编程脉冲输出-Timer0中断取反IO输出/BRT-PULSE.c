
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

测试Timer0中断取反P1.1输出大约100HZ方波, 从P1.2输出秒脉冲. 

测试BRT输出脉冲大约10KHZ，并受5ms信号调制。


******************************************/


/*************	用户系统配置	**************/

#define MAIN_Fosc		22118400L	//定义主时钟, 延时会自动适应。5~35MHZ
#define D_TIMER0		5000		//	选择定时器时间us，注意不要溢出，溢出时编译会报错。

/******************************************/

#define Timer0_Reload_s			((MAIN_Fosc / 1000) * D_TIMER0 / 12000)
#if (Timer0_Reload_s < 65536)
	#define	Timer0_Reload		(65536 - Timer0_Reload_s)
#endif


#include	"reg51.H"

sfr	AUXR = 0x8E;
sfr WAKE_CLKO = 0x8F;
sfr BRT   = 0x9C;

/*************	本地变量声明	**************/
sbit	P10 = P1^0;
sbit	P11 = P1^1;
sbit	P12 = P1^2;
unsigned char ms_cnt;

/*************	本地函数声明	**************/
void  	delay_ms(unsigned char ms);


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
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}





void main(void)
{
	WAKE_CLKO |=  (1<<2);	//Enable BRT output frequency from P1.0
	BRT = 256 - 92;			//10KHZ @ 22.118400HZ 12T mode
	AUXR |=  (1<<4);		//Enable BRT
	
	TMOD |= 0x01;			//Timer0 set as 16bit timer
	TH0  = Timer0_Reload / 256;
	TL0  = Timer0_Reload % 256;
	ET0 = 1;
	TR0 = 1;
	EA = 1;
	
	while(1)
	{
		AUXR |=  (1<<4);	//Enable BRT
		delay_ms(5);
		AUXR &=  ~(1<<4);	//Disable BRT
		P10 = 0;			//强制输出0电平
		delay_ms(5);
	
	}
}


/**********************************************/
void timer0 (void) interrupt 1
{
	TR0 = 0;
	TH0  = Timer0_Reload / 256;
	TL0  = Timer0_Reload % 256;
	TR0 = 1;
	
	P11 = ~P11;		//Timer0中断取反P1.1输出 100HZ
	if(++ms_cnt >= 100)
	{
		ms_cnt = 0;
		P12 = ~P12;		//P1.2输出秒脉冲
	}
}
	
	
	
	