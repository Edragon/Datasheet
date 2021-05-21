#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>

//LED IO设置
sbit LED1 = P2^0;
sbit LED2 = P2^1;
#define LED_ON 0
#define LED_OFF 1
sbit Buzzer = P3^7;

//按键 IO设置
sbit KEY1 = P3^2;
sbit KEY2 = P3^3;
sbit KEY3 = P3^4;
sbit KEY4 = P3^5;



//函数或者变量声明
extern void Delay_ms(unsigned int n);
extern void Scan_Key();
extern void Display_RTCC();
extern void Display_Temp();

extern void Buzzer_Di();


extern unsigned char KEY_NUM;
extern bit Flag_KEY_Set;
extern unsigned char KEY2_Count;


#endif