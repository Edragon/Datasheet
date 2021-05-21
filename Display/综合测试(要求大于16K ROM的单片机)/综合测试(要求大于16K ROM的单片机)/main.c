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
void xianshi()//显示信息
{   
	BACK_COLOR=WHITE;
	POINT_COLOR=RED;	
	showhanzi(10,0,0);  //晶
	showhanzi(45,0,1);  //耀
    LCD_ShowString(10,30,"2.2 inch TFT 240*320");
}
void showimage() //显示40*40图片
{
  	int i,j,k;
	xianshi(); //显示信息
	for(k=2;k<4;k++)
	{
	   	for(j=0;j<3;j++)
		{	
			Address_set(40*j,40*k,40*j+39,40*k+39);		//坐标设置
		    for(i=0;i<1600;i++)
			 {				
			  	 LCD_WR_DATA8(image[i*2+1]);	 //发送颜色数据
				 LCD_WR_DATA8(image[i*2]);					
			 }	
		 }
	}
		
}
main()
{ 
	Lcd_Init();   //tft初始化
	LCD_Clear(WHITE); //清屏
	BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 
    showimage(); //显示40*40图片

	while(1)
	{
	 
//	Lcd_Init();   //tft初始化
//	LCD_Clear(WHITE); //清屏
//	BACK_COLOR=BLACK;;POINT_COLOR=WHITE; 
 //  showimage(); //显示40*40图片
//	delayms(2000);
    }


}
