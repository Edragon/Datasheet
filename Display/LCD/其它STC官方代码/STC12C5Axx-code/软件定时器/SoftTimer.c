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

用Timer0做基准10ms定时，16个倒计时软件定时器。

合适工业控制中类似PLC的软件定时器。

每个定时器的值非0开始同步定时，减到0停止。

范例程序的定时器是8位的，定时值为1~255，对应0.01~2.55秒.

每个定时器点亮一个LED, 不同闪烁时间。

******************************************/



/*************	用户系统配置	**************/

#define MAIN_Fosc		24000000L	//定义主时钟

#define D_TIMER0		10000		//选择定时器时间, us


/*************	以下宏定义用户请勿修改	**************/
#include	"reg51.H"
#define	uchar	unsigned char
#define uint	unsigned int

#define freq_base			(MAIN_Fosc / 1200)
#define Timer0_Reload		(65536 - (D_TIMER0 * freq_base / 10000))

uchar	SoftTimer[16];

uchar	code T_BitTable[8] = {1,2,4,8,16,32,64,128};
uchar	code T_ReloadTimerL[16] = {10, 20, 30, 40, 50, 60, 70, 80, 90,100,110,120,130,140,150,160};
uchar	code T_ReloadTimerH[16] = {30, 40, 50, 60, 70, 80, 90,100,110,120,130,140,150,160,170,180};

/**************** Timer初始化函数 ******************************/
void InitTimer(void)
{
	TMOD = 1;
	TH0 = Timer0_Reload / 256;
	TL0 = Timer0_Reload % 256;
	ET0 = 1;
	TR0 = 1;
	EA  = 1;
}

/********************* 主函数 *************************/
void main(void)
{
	uchar	i,j;

	InitTimer();		//初始化Timer
	
	while(1)
	{
		for(i=0; i<16; i++)			//检测16个定时器是否倒计时到0
		{
			if(SoftTimer[i] == 0)	//倒计时到0
			{
				if(i < 8)			//0~7 对应P1.0~P1.7
				{
					P1 ^= T_BitTable[i];	//取反对应的LED
					j = P1 & T_BitTable[i];	//取当前输出状态
				}
				else				//8~15对应P2.0~P2.7
				{
					P2 ^= T_BitTable[i-8];		//取反对应的LED
					j = P2 & T_BitTable[i-8];	//取当前输出状态
				}
				if(j == 0)	SoftTimer[i] = T_ReloadTimerL[i];	//输出低电平的重装定时值
				else		SoftTimer[i] = T_ReloadTimerH[i];	//输出高电平的重装定时值
			}
		}
	}
}

/********************** Timer0中断函数************************/
void timer0 (void) interrupt 1
{
	uchar	i;

	TR0 = 0;
	TH0 = Timer0_Reload / 256;
	TL0 = Timer0_Reload % 256;
	TR0 = 1;

	for(i=0; i<16; i++)
	{
		if(SoftTimer[i] != 0)	SoftTimer[i]--;		//倒计时定时器
	}
}
