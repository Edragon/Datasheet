#ifndef __MAIN_H__
#define __MAIN_H__


#include <reg52.h>

//LED IO����
sbit LED1 = P2^0;
sbit LED2 = P2^1;

//�������߱�������
extern void Delay_ms(unsigned int n);


#endif