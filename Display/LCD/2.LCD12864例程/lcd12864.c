#include "LCD12864.h"

//********************************************************************
//LCD12864  忙 信号检测
//********************************************************************
void LCD12864_WaitIdle()

{
	unsigned char i;
	LCD12864_DA_PORT = 0xff;
	LCD12864_RS_PORT = 0;
	LCD12864_RW_PORT = 1;
	LCD12864_E_PORT = 1;
	while((LCD12864_DA_PORT&0x80)==1); /*等待BF 不为1*/
	LCD12864_E_PORT = 0;
	for(i=0;i<5;i++);
}


//********************************************************************
//检测忙信号写入命令字	com_da 为待写入的命令字
//********************************************************************
void LCD12864_COM_Write( unsigned char com_da)	
{
	LCD12864_WaitIdle();
	LCD12864_RS_PORT = 0;
	LCD12864_RW_PORT = 0;
	LCD12864_DA_PORT = com_da;
	LCD12864_E_PORT = 1;
	_nop_();
	_nop_();	
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD12864_E_PORT = 0;	
}

//********************************************************************
//不检测忙信号写入命令字		com_da 为待写入的命令字
//********************************************************************
void LCD12864_NoWaitIdle_COM_Write(unsigned char com_da)	
{
	LCD12864_RS_PORT = 0;
	LCD12864_RW_PORT = 0;
	LCD12864_DA_PORT = com_da;
	LCD12864_E_PORT = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD12864_E_PORT = 0;	
}

//********************************************************************
//数据写入		da  为待写入的8位数据
//********************************************************************
void LCD12864_Data_Write(unsigned char da)

{
	LCD12864_WaitIdle(); /*检测忙信号*/
	LCD12864_RS_PORT = 1;
	LCD12864_RW_PORT = 0;
	LCD12864_DA_PORT = da;
	LCD12864_E_PORT = 1;
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	_nop_();
	LCD12864_E_PORT = 0;	
}


//*************************************************************************************
//写连续字符函数
//*************************************************************************************
void LCD12864_write_word(unsigned char *s)
{
	while(*s>0)
	{
		LCD12864_Data_Write(*s);
		s++;
	}
}

//********************************************************************
//1MS为单位的延时程序，不准确
//********************************************************************
void lcd_delay_ms(unsigned char x)
{
    unsigned char j;
    while(x--){
        for(j=0;j<125;j++)
            {;}
        }   
}


//********************************************************************
//LCD12864初始化
//********************************************************************
void LCD12864_Reset()
{
	lcd_delay_ms(100); /*适当延时待LCD自动复位完成*/
	LCD12864_NoWaitIdle_COM_Write(0x30); /*使用8位并口通讯*/
	lcd_delay_ms(10);
	LCD12864_NoWaitIdle_COM_Write(0x30); /*使用8位并口通讯*/
	lcd_delay_ms(10);
	LCD12864_NoWaitIdle_COM_Write(0x0c); /*显示开及光标设置*/
	lcd_delay_ms(10);
	LCD12864_NoWaitIdle_COM_Write(0x01); /*显示清屏*/
	lcd_delay_ms(30);
	LCD12864_NoWaitIdle_COM_Write(0x06); /*DDRAM的地址计数器（AC）加1*/
	lcd_delay_ms(30);	
}


//void LCD12864_PHOTO_SET()
//{
//	LCD12864_COM_Write(0x36);
//	lcd_delay_ms(10);
//	LCD12864_COM_Write(0x36);
//	lcd_delay_ms(10);
//}
//
void LCD12864_HAIZI_SET()
{
	LCD12864_COM_Write(0x30);
	lcd_delay_ms(10);
	LCD12864_COM_Write(0x30);
	lcd_delay_ms(10);
}
//
//
//void LCD12864_HAIZI_WRITE(unsigned char xpos,unsigned char ypos,unsigned char daH,unsigned char daL)
////ST7920 汉字字符写入
////参数说明: 	xpos 待写入的X位置
////ypos 待写入的Y位置
////daH 待写入的汉字的高八位 daL待写入的汉字的低八位
//{
//	unsigned char xy_pos;
//	if((xpos>=8)||(ypos>=4) ) return; /*X位置超出显示范围退出*/
//	if(ypos==0)         xy_pos = 0x80 + xpos;
//	else if(ypos==1) xy_pos = 0x90 + xpos; /*计算转换地址*/
//	else if(ypos==2) xy_pos = 0x88 + xpos;
//	else if(ypos==3) xy_pos = 0x98 + xpos;
//	LCD12864_COM_Write(xy_pos); /*写地址*/
//	lcd_delay_ms(1);
//	LCD12864_Data_Write(daH); /*写高八位数据*/
//	lcd_delay_ms(1);	
//	LCD12864_Data_Write(daL); /*写低八位数据*/	
//	lcd_delay_ms(1);
//}
//
//
//void LCD12864_PHOTO_WRITE(unsigned char *img)
//{
//	unsigned char x,y,i,j;
//	unsigned int k=0;
//	y=0x80; /*设置起始 绘图区的 Y地址坐标*/
//	x=0x80; /*设置起始 绘图区的 X地址坐标*/
//	for(i=0;i<32;i++){  /*写上半部*/
//		LCD12864_COM_Write(y);
//		LCD12864_COM_Write(x);
//		for(j=0;j<16;j++){
//			LCD12864_Data_Write(img[k]);
//			k++;
//			}
//		y++;
//		}
//	
//	y=0x80; /*设置起始 绘图区的 Y地址坐标*/
//	x=0x88; /*设置起始 绘图区的 X地址坐标*/
//	for(i=0;i<32;i++){  /*写下半部*/
//		LCD12864_COM_Write(y);
//		LCD12864_COM_Write(x);
//		for(j=0;j<16;j++){
//			LCD12864_Data_Write(img[k]);
//			k++;
//			}
//		y++;
//		}
//	
//}