

#include <reg51.h>
#include <intrins.h>
#include <sys\sys.h>
#include <lcd\lcd.h>
#include <font\font.h>
/* 
程序默认IO连接方式：
sbit LCD_CS     =P0^0;     //片选	
sbit LCD_AO     =P0^2;	  //数据/命令切换
sbit LCD_SDA    =P1^5;	  //数据
sbit LCD_SCK    =P1^7;	  //时钟
sbit LCD_REST   =P0^1;	  //复位  
*/
main()
{ 
	Lcd_Init();   //tft初始化
	LCD_Clear(WHITE); //清屏
	BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 

	while(1)
	{		 
		 Lcd_Init();   //tft初始化
		 LCD_Clear(RED);
		 delayms(3000);
		 LCD_Clear(GREEN);
		 delayms(3000);
		 LCD_Clear(BLUE);
		 delayms(3000);
    }


}
