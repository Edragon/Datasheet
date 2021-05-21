#ifndef __TFT__H__
#define __TFT__H__

#include<reg52.h>
#include<absacc.h>
#include<intrins.h>
#include<string.h>

#define uchar unsigned char
#define uint unsigned int


//#define USE_LANDSCAPE//测试程序横竖屏切换，横屏请开启此宏，竖屏请屏蔽

//---------------------------液晶屏接线说明-------------------------------------//
sbit bl        =P2^4;//接模块BL引脚
sbit scl       =P2^5;//接模块CLK引脚
sbit sda       =P2^3;//接模块DIN/MOSI引脚
sbit rs        =P2^2;//接模块D/C引脚
sbit cs        =P2^0;//接模块CE引脚
sbit reset     =P2^1;//接模块RST引脚

//定义常用颜色
#define RED  		0xf800
#define GREEN		0x07e0
#define BLUE 		0x001f
#define WHITE		0xffff
#define BLACK		0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   
#define GRAY1   0x8410      	
#define GRAY2   0x4208  

void delay_ms(uint time);
void  SPI_WriteData(uchar Data);
void  Lcd_WriteIndex(uchar Data);
void  Lcd_WriteData(uchar Data);
void  LCD_WriteData_16Bit(unsigned int Data);
void Reset();
void lcd_initial();
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end);
void PutPixel(uint x_start,uint y_start,uint color);
void dsp_single_colour(int color);
//void Display_ASCII8X16(uint x0,uint y0,uchar *s);
void Display_ASCII8X16_Color(uint x0,uint y0,uchar *s,uint forecolor,uint backgroundcolor);


uchar code Zk_ASCII8X16[];


#endif