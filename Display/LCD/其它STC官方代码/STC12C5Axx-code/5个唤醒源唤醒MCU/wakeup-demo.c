
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

演示5个唤醒源对MCU唤醒。

从串口输出结果(ASCII)，9600,8,N,1.

用户修改主时钟可以匹配自己的系统时钟.

******************************************/


/*************	用户系统配置	**************/

#define MAIN_Fosc		22118400L	//定义主时钟, 模拟串口和和延时会自动适应。5~35MHZ



/*************	以下宏定义用户请勿修改	**************/
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

	PrintString("*** STC11Fxx系列5个唤醒源测试程序 ***\r\n");
	
	while(1)
	{
		T0 = 1;
		T1 = 1;
		INT0 = 1;
		INT1 = 1;
		RXD  = 1;

		WAKE_CLKO |=  (1<<4);	//允许 T0 脚/P3.4 下降沿置 T0 中断标志，也能唤醒 powerdown
		WAKE_CLKO |=  (1<<5);	//允许 T1 脚/P3.5 下降沿置 T1 中断标志，也能唤醒 powerdown
		WAKE_CLKO |=  (1<<6);	//允许 P3.0(RXD) 下降沿置 RI，也能使 RXD 唤醒 powerdown
		IT0 = 1;	// 外部中断0下降沿中断、唤醒
		EX0 = 1;	// 外部中断0允许
		IT1 = 1;	// 外部中断1下降沿中断、唤醒
		EX1 = 1;	// 外部中断1允许
		ET0 = 1;	// T0中断允许
		ET1 = 1;	// T1中断允许
		ES  = 1;	// RXD中断允许
		EA  = 1;		//Enable all interrupt
		
		while(!T0);
		while(!T1);
		while(!INT0);
		while(!INT1);
		while(!RXD);
		

		TF1  = 0;	//定时器1溢出中断标志位
		TF0  = 0;	//定时器0溢出中断标志位
		IE1  = 0;	//外中断1标志位
		IE0  = 0;	//外中断0标志位
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
		
		if(WakeUpSource == 1)	PrintString("外中断INT0唤醒\r\n");
		if(WakeUpSource == 2)	PrintString("外中断INT1唤醒\r\n");
		if(WakeUpSource == 3)	PrintString("外部T0唤醒\r\n");
		if(WakeUpSource == 4)	PrintString("外部T1唤醒\r\n");
		if(WakeUpSource == 5)	PrintString("RXD唤醒\r\n");
		WakeUpSource = 0;
		
		WakeUpCnt++;
		Tx1Send(WakeUpCnt/100+'0');
		Tx1Send(WakeUpCnt%100/10+'0');
		Tx1Send(WakeUpCnt%10+'0');
		PrintString("次唤醒\r\n");
	}

}


void INT0_int (void) interrupt 0
{
	WakeUpSource = 1;	//标记INT0唤醒
}
void Timer0_int (void) interrupt 1
{
	WakeUpSource = 3;		//标记Tiner0唤醒
}
void INT1_int (void) interrupt 2
{
	WakeUpSource = 2;	//标记INT1唤醒
}
void Timer1_int (void) interrupt 3
{
	WakeUpSource = 4;		//标记Tiner1唤醒
}

void UART0_RCV (void) interrupt 4
{
	RI = 0;
	TI = 0;
	WakeUpSource = 5;	//标记RXD唤醒
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

/********************** 模拟串口相关函数************************/

void	BitTime(void)	//位时间函数 9600bps
{
	unsigned int i;
	i = ((MAIN_Fosc / 100) * 104) / 140000L - 1;	//根据主时钟来计算位时间 1T
	while(--i)	;   //8T per loop at 12T or 6T mode, 14T at 1T mode
}

//模拟串口发送
void	Tx1Send(unsigned char dat)		//9600，N，8，1		发送一个字节
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

void PrintString(unsigned char code *puts)		//发送一串字符串
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//遇到停止符0结束
}

