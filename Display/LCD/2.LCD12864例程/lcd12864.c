#include "LCD12864.h"

//********************************************************************
//LCD12864  æ �źż��
//********************************************************************
void LCD12864_WaitIdle()

{
	unsigned char i;
	LCD12864_DA_PORT = 0xff;
	LCD12864_RS_PORT = 0;
	LCD12864_RW_PORT = 1;
	LCD12864_E_PORT = 1;
	while((LCD12864_DA_PORT&0x80)==1); /*�ȴ�BF ��Ϊ1*/
	LCD12864_E_PORT = 0;
	for(i=0;i<5;i++);
}


//********************************************************************
//���æ�ź�д��������	com_da Ϊ��д���������
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
//�����æ�ź�д��������		com_da Ϊ��д���������
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
//����д��		da  Ϊ��д���8λ����
//********************************************************************
void LCD12864_Data_Write(unsigned char da)

{
	LCD12864_WaitIdle(); /*���æ�ź�*/
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
//д�����ַ�����
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
//1MSΪ��λ����ʱ���򣬲�׼ȷ
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
//LCD12864��ʼ��
//********************************************************************
void LCD12864_Reset()
{
	lcd_delay_ms(100); /*�ʵ���ʱ��LCD�Զ���λ���*/
	LCD12864_NoWaitIdle_COM_Write(0x30); /*ʹ��8λ����ͨѶ*/
	lcd_delay_ms(10);
	LCD12864_NoWaitIdle_COM_Write(0x30); /*ʹ��8λ����ͨѶ*/
	lcd_delay_ms(10);
	LCD12864_NoWaitIdle_COM_Write(0x0c); /*��ʾ�����������*/
	lcd_delay_ms(10);
	LCD12864_NoWaitIdle_COM_Write(0x01); /*��ʾ����*/
	lcd_delay_ms(30);
	LCD12864_NoWaitIdle_COM_Write(0x06); /*DDRAM�ĵ�ַ��������AC����1*/
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
////ST7920 �����ַ�д��
////����˵��: 	xpos ��д���Xλ��
////ypos ��д���Yλ��
////daH ��д��ĺ��ֵĸ߰�λ daL��д��ĺ��ֵĵͰ�λ
//{
//	unsigned char xy_pos;
//	if((xpos>=8)||(ypos>=4) ) return; /*Xλ�ó�����ʾ��Χ�˳�*/
//	if(ypos==0)         xy_pos = 0x80 + xpos;
//	else if(ypos==1) xy_pos = 0x90 + xpos; /*����ת����ַ*/
//	else if(ypos==2) xy_pos = 0x88 + xpos;
//	else if(ypos==3) xy_pos = 0x98 + xpos;
//	LCD12864_COM_Write(xy_pos); /*д��ַ*/
//	lcd_delay_ms(1);
//	LCD12864_Data_Write(daH); /*д�߰�λ����*/
//	lcd_delay_ms(1);	
//	LCD12864_Data_Write(daL); /*д�Ͱ�λ����*/	
//	lcd_delay_ms(1);
//}
//
//
//void LCD12864_PHOTO_WRITE(unsigned char *img)
//{
//	unsigned char x,y,i,j;
//	unsigned int k=0;
//	y=0x80; /*������ʼ ��ͼ���� Y��ַ����*/
//	x=0x80; /*������ʼ ��ͼ���� X��ַ����*/
//	for(i=0;i<32;i++){  /*д�ϰ벿*/
//		LCD12864_COM_Write(y);
//		LCD12864_COM_Write(x);
//		for(j=0;j<16;j++){
//			LCD12864_Data_Write(img[k]);
//			k++;
//			}
//		y++;
//		}
//	
//	y=0x80; /*������ʼ ��ͼ���� Y��ַ����*/
//	x=0x88; /*������ʼ ��ͼ���� X��ַ����*/
//	for(i=0;i<32;i++){  /*д�°벿*/
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