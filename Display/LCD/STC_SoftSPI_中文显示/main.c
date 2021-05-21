#include "main.h"
#include "TFT.h"

void main()
{
	lcd_initial();
	bl=1;
//	dsp_single_colour(WHITE);//��ɫ
//	Display_Desc();
//	dsp_single_colour(BLACK);//��ɫ	
//	Display_ASCII8X16_Color(10,10,"111",RED,BLACK);	
//	Display_ASCII8X16_Color(10,10,"222",RED,BLACK);	
//	Display_ASCII8X16_Color(10,10,"333",RED,BLACK);	
	while(1)
	{
		Font_Test();			
	}	
}



void Display_Desc()
{ 
    Display_ASCII8X16_Color(10,10,"Welcome",RED,WHITE);
    Display_ASCII8X16_Color(10,26,"ILoveMCU",GREEN,WHITE);
    Display_ASCII8X16_Color(10,42,"Dots:128*128",BLUE,WHITE);
    Display_ASCII8X16_Color(10,58,"IC: ST7735",BLACK,WHITE);
    Display_ASCII8X16_Color(10,74,"VA:6 o'clock",YELLOW,WHITE);
    Display_ASCII8X16_Color(10,90,"2016.05.08",RED,WHITE);
}

void Font_Test(void)
{
	dsp_single_colour(WHITE);
	Fast_DrawFont_GBK16(16,10,BLUE,YELLOW,	"������ʾ����");
	Fast_DrawFont_GBK16(16,30,RED,WHITE,	"��ѧ����������");
	Fast_DrawFont_GBK16(40,50,BLUE,WHITE,	"��ӭ��");
	Fast_DrawFont_GBK16(32,70,WHITE,BLUE, 	"��Ϊרע");
	Fast_DrawFont_GBK16(32,90,WHITE,BLUE,	"����רҵ");

	delay_ms(1800);	
}