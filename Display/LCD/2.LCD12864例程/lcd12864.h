
#ifndef __LCD12864_H__
#define __LCD12864_H__

#include <reg52.h>
#include <intrins.h>

//引脚定义
sbit LCD12864_RS_PORT  = P2^5;  
sbit LCD12864_RW_PORT = P2^6;
sbit LCD12864_E_PORT    = P2^7;
#define LCD12864_DA_PORT P0


//函数或者变量声明
extern void LCD12864_WaitIdle();
extern void LCD12864_COM_Write( unsigned char com_da);
extern void LCD12864_NoWaitIdle_COM_Write(unsigned char com_da);
extern void LCD12864_Data_Write(unsigned char da);
extern void lcd_delay_ms(unsigned char x);
extern void LCD12864_Reset();
extern void LCD12864_write_word(unsigned char *s);
//extern void LCD12864_PHOTO_SET();
extern void LCD12864_HAIZI_SET();
//extern void LCD12864_HAIZI_WRITE(unsigned char xpos,unsigned char ypos,unsigned char daH,unsigned char daL);
//extern void LCD12864_PHOTO_WRITE(unsigned char *img);


#endif


