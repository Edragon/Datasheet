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

/*************	功能说明	**************

用Timer0做基准10ms定时，16个倒计时软件定时器。

合适工业控制中类似PLC的软件定时器。

每个定时器的值非0开始同步定时，减到0停止。

范例程序的定时器是8位的，定时值为1~255，对应0.01~2.55秒.

定时器0~7控制P1上的8个LED，定时器8为键扫描定时。键从P2口输入，对应8个LED。

定时器9~15没有使用.

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

uchar	SoftTimer[16];		//16个软件定时器

uchar	SoftTimerEnable;	//定时器允许控制，对应位为1，允许对应的定时器，为0，禁止。
uchar	KeyState;			//键状态


uchar	code T_BitTable[8] = {1,2,4,8,16,32,64,128};

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
	uchar	i;

	InitTimer();		//初始化Timer
	
	while(1)
	{
		if(SoftTimer[8] == 0)	//SoftTimer[8]倒计时到0, 键扫描
		{
			SoftTimer[8] = 5;	//50ms 间隔扫描
			i = KeyState;		//保存上一次键值
			KeyState = ~P2;		//读键
			i = (i ^ KeyState) & KeyState;	//计算按下的键
			SoftTimerEnable ^= i;	//按下的键将对应的位取反
		}
		
		for(i=0; i<8; i++)		//检测8个定时器是否倒计时到0
		{
			if(SoftTimerEnable & T_BitTable[i])		//检测对应的定时器被允许
			{
				if(SoftTimer[i] == 0)		//定时器倒计时到0
				{
					P1 ^= T_BitTable[i];	//取反对应的LED
					SoftTimer[i] = 25;		//重装对应的定时值，2HZ闪烁
				}
			}
			else		//定时器被禁止
			{
				SoftTimer[i] = 0;		// 停止对应的定时器
				P1 |= T_BitTable[i];	// 关闭对应的LED
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
