#ifndef __MAIN_H__
#define __MAIN_H__

#include <reg52.h>

//LED IO����
sbit LED1 = P1^3;
sbit LED2 = P1^4;
#define LED_ON 0
#define LED_OFF 1


//�������߱�������
extern void Delay_ms(unsigned int n);




#endif