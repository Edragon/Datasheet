
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

适用IC型号:  STC12C/LE5A60S2系列MCU.

P1.0 P1.1 P1.2 做ADC输入，从串口输出结果(ASCII)，9600,8,N,1.

用户只需要更改 MAIN_Fosc 来适应自己的系统。

******************************************/


/*************	用户系统配置	**************/

#define MAIN_Fosc		22118400L	//定义主时钟, 模拟串口和和延时会自动适应。5~35MHZ

/*************	以下宏定义用户请勿修改	**************/
#include	"reg51.H"
#define	uchar	unsigned char
#define uint	unsigned int
/******************************************/

sfr P1ASF     = 0x9D;	//12C5A60AD/S2系列模拟输入(AD或LVD)选择
sfr ADC_CONTR = 0xBC;	//带AD系列
sfr ADC_RES   = 0xBD;	//带AD系列
sfr ADC_RESL  = 0xBE;	//带AD系列

//								7       6      5       4         3      2    1    0   Reset Value
//sfr ADC_CONTR = 0xBC;		ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000	//AD 转换控制寄存器 
#define ADC_OFF()	ADC_CONTR = 0
#define ADC_ON		(1 << 7)
#define ADC_90T		(3 << 5)
#define ADC_180T	(2 << 5)
#define ADC_360T	(1 << 5)
#define ADC_540T	0
#define ADC_FLAG	(1 << 4)	//软件清0
#define ADC_START	(1 << 3)	//自动清0
#define ADC_CH0		0
#define ADC_CH1		1
#define ADC_CH2		2
#define ADC_CH3		3
#define ADC_CH4		4
#define ADC_CH5		5
#define ADC_CH6		6
#define ADC_CH7		7


/************* 本地变量声明 **************/
sbit	P_TXD1 = P3^1;


/************* 本地函数声明 **************/
void	Tx1Send(uchar dat);
void	PrintString(unsigned char code *puts);
void  	delay_ms(unsigned char ms);
uint	adc10_start(uchar channel);	//channel = 0~7




/********************* 主函数 *************************/
void main(void)
{
	uchar	i;
	uint	j;

	PrintString("****** STC12C5A60S2系列ADC程序 2011-02-27 ******\r\n");	//上电后串口发送一条提示信息

	P1ASF = 0x07;			//12C5A60AD/S2系列模拟输入(AD)选择
	ADC_CONTR = ADC_360T | ADC_ON;

	while(1)
	{
		
		for(i=0; i<3; i++)
		{
			delay_ms(250);
			delay_ms(250);

			j = adc10_start(i);
			Tx1Send('A');
			Tx1Send('D');
			Tx1Send(i+'0');
			Tx1Send('=');
			Tx1Send(j/1000 + '0');
			Tx1Send(j%1000/100 + '0');
			Tx1Send(j%100/10 + '0');
			Tx1Send(j%10 + '0');
			Tx1Send(0x0d);
			Tx1Send(0x0a);
		}
	}
}

/********************* 做一次ADC转换 *******************/
uint	adc10_start(uchar channel)	//channel = 0~7
{
	uint	adc;
	uchar	i;

	ADC_RES = 0;
	ADC_RESL = 0;

	ADC_CONTR = (ADC_CONTR & 0xe0) | ADC_START | channel; 

//	for(i=0; i<250; i++)		//13T/loop, 40*13=520T=23.5us @ 22.1184M
	i = 250;
	do{
		if(ADC_CONTR & ADC_FLAG)
		{
			ADC_CONTR &= ~ADC_FLAG;
		//	adc = 0;
		//	adc = (ADC_RES << 8) | ADC_RESL;	//ADRJ_enable()
			adc = (uint)ADC_RES;
			adc = (adc << 2) | (ADC_RESL & 3);
			return	adc;
		}
	}while(--i);
	return	1024;
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
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}


/********************** 模拟串口相关函数************************/

void	BitTime(void)	//位时间函数
{
	uint i;
	i = ((MAIN_Fosc / 100) * 104) / 140000L - 1;		//根据主时钟来计算位时间
	while(--i);
}

//模拟串口发送
void	Tx1Send(uchar dat)		//9600，N，8，1		发送一个字节
{
	uchar	i;
	EA = 0;
	P_TXD1 = 0;
	BitTime();
	for(i=0; i<8; i++)
	{
		if(dat & 1)		P_TXD1 = 1;
		else			P_TXD1 = 0;
		dat >>= 1;
		BitTime();
	}
	P_TXD1 = 1;
	EA = 1;
	BitTime();
	BitTime();
}

void PrintString(unsigned char code *puts)		//发送一串字符串
{
    for (; *puts != 0;	puts++)  Tx1Send(*puts); 	//遇到停止符0结束
}
