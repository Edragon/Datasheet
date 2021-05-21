
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


EEPROM 掉电保存测试程序。

波特率:  9600,8,N,1 .

MCU每隔1秒从串口输出一个变化的数(000~250). 掉电后保存,上电时读取.

适用于STC12C5Axxxx系列, 从P4.6接一个10K电阻到地, 接一个电阻R2到电源, 当R2为24K时大约在4.5V开始进入保存(本人建议此值),

当R2为22K时大约在4.25V开始进入保存, 当R2为20K时大约在4.0V开始进入保存.

由于制造误差,可能跟理论值有微小出入.

或者,从电源输入端检测掉电更为理想. 此时要注意计算R2的值.

电源的滤波电容根据电路电流大小来选取.

P4.6上电后默认为高阻.程序也可以重新设置一下.本例程不设置.


工程文件:
STC12C5A60S2_EEPROM.c
PowerDownSave.c


******************************************/


#include	"config.h"


/*************	本地常量声明	**************/

#define	EEP_address	0x0000


/*************	本地变量声明	**************/
sfr	AUXR = 0x8E;

sbit ELVD = IE^6;	//低压监测中断允许位
sbit PLVD = IP^6; 	//低压中断 优先级设定位

//						7   6     5    4     3      2    1     0     Reset Value
//sfr PCON   = 0x87;	-   -   LVDF  POF   GF1    GF0   PD   IDL    0001,0000	 //Power Control 
//LVDF: 低压检测标志，只要电压低于检测门槛电压，该位置1，软件清0。睡眠前禁止低压中断可降低功耗。允许则低压时中断唤醒MCU。

uchar	TestCnt;

sbit	P00 = P0^0;
sbit	P01 = P0^1;

/*************	本地函数声明	**************/
sbit	P_TXD1 = P3^1;
void	Tx1Send(unsigned char dat);
void	PrintString(unsigned char code *puts);
void  	delay_ms(unsigned char ms);


/*************  外部函数和变量声明 *****************/
void	EEPROM_read_n(uint EE_address,uchar *DataAddress,uchar lenth);
void 	EEPROM_SectorErase(uint EE_address);
void 	EEPROM_write_n(uint EE_address,uchar *DataAddress,uchar lenth);



/********************* 主函数 *************************/
void main(void)
{

	delay_ms(250);

	PCON = PCON & ~(1<<5);	//低压检测标志清0
	ELVD = 1;	//低压监测中断允许
	PLVD = 1; 	//低压中断 优先级高
	EA  = 1;

	EEPROM_read_n(EEP_address,&TestCnt,1);	//读出保存值
	if(TestCnt >= 251)	TestCnt = 0;
	EEPROM_SectorErase(EEP_address);		//当掉电后电压降落比较快时,在这里先擦除


while(1)
	{
		Tx1Send(TestCnt / 100 + '0');			//发给串口
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

	P00 = 0;
//	EEPROM_SectorErase(EEP_address);	//当掉电后电压降落很缓慢时,可以在这里擦除

	EEPROM_write_n(EEP_address,&TestCnt,1);	//掉电保存值

	while(PCON & (1<<5))			//检测是否仍然低电压
	{
		PCON = PCON & ~(1<<5);		//低压检测标志清0
		for(i=0; i<100; i++)	;	//延时一下
	}
	P00 = 1;
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
		i = MAIN_Fosc / 14000L;
		while(--i)	;
     }while(--ms);
}

/********************** 模拟串口相关函数************************/

void	BitTime(void)	//位时间函数 9600bps
{
	unsigned int i;
	i = ((MAIN_Fosc / 100) * 104) / 140000L - 1;
	while(--i)	;
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


