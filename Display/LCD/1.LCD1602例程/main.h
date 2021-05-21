#ifndef __MAIN_H__
#define __MAIN_H__


#include <reg52.h>

//LED IO设置
sbit LED1 = P2^0;
sbit LED2 = P2^1;

//函数或者变量声明
extern void Delay_ms(unsigned int n);


#endif