#include "main.h"
#include "TFT.h"



void main()
{
	lcd_initial();
	bl=1;
	dsp_single_colour(WHITE);//°×É«
	Display_Desc();
	dsp_single_colour(BLACK);//ºÚÉ«	
	while(1)
	{
		Display_ASCII8X16_Color(10,10,"111",RED,BLACK);	
		Display_ASCII8X16_Color(10,10,"222",RED,BLACK);	
		Display_ASCII8X16_Color(10,10,"333",RED,BLACK);		
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

