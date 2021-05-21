/*************************************************************/

#include<reg51.h>
#include<ziku.c>
//#include <stdio.h>
#include <absacc.h>
#include <intrins.h>
#define uchar unsigned char
#define uint unsigned int


/**************************************************************
*12864Һ���Ķ��壨T6963������								  *
***************************************************************/
sbit REST = P2^0;			    //Reset signal, active"L"
sbit C_D = P2^1;				 	//L:data   H:code
sbit C_E = P2^2;					//Chip enable signal, active"L"
sbit R_D = P2^3;					//read signal, active"L"
sbit W_R = P2^4;					//write signal, active"L"



#define width      				 15 //��ʾ�����
#define Graphic          	     1
#define TXT           	    	 0
#define LcmLengthDots            128
#define LcmWidthDots             64


/**************************12864Һ������غ���**********************/         								  
void delay_nms(uint i)
{
	while(i)
	i--;
}
void write_commond(uchar com)	   //��Һ��дһ��ָ��
{	 
	C_E = 0;	
	C_D = 1;
	R_D = 1;
	P0 = com;
	W_R = 0;	    	// write	  
	_nop_();					
	W_R = 1;		    // disable write
	C_E = 1;
	C_D = 0;

}
int sadsa;

void write_date(uchar dat)	  //��Һ��дһ������
{
	C_E = 0;
	C_D = 0;
	R_D = 1;
	P0 = dat;
	W_R = 0;
	_nop_();
	W_R = 1;
	C_E = 1;
	C_D = 1;
}
 void write_dc(uchar com,uchar dat)	 //дһ��ָ���һ������
{
	write_date(dat);
	write_commond(com);
}

//д�������ݺ�һ��ָ��
void write_ddc(uchar com,uchar dat1,uchar dat2)
{
	write_date(dat1);
	write_date(dat2);
	write_commond(com);
}

//LCD��ʼ������
void F12864_init(void)
{	
	REST = 0;
	delay_nms(2000);					
	REST = 1;
	write_ddc(0x40,0x00,0x00);			      		//�����ı���ʾ���׵�ַ
	write_ddc(0x41,128/8,0x00);  						//�����ı���ʾ�����
	write_ddc(0x42,0x00,0x08);			      		//����ͼ����ʾ���׵�ַ0x0800
	write_ddc(0x43,128/8,0x00);  						//����ͼ����ʾ�����
	write_commond(0xA0);                         //���ù����״ 8x8����
	write_commond(0x80);                         //��ʾ��ʽ���� �ı�andͼ��(���)
	write_commond(0x92);                         //���ù��
	write_commond(0x9F);                         //��ʾ�������� �ı���,ͼ�ο�,�����˸��							 	

}
//**************************//����ʾ�洢������

void F12864_clear(void)
{
	unsigned int i;
	write_ddc(0X24,0x00,0x00); 							//�õ�ַָ��Ϊ���㿪ʼ
   write_commond(0xb0); 									//�Զ�д
	for(i = 0;i < 128 * 64 ;i++)write_date(0x00); // ��һ��
	write_commond(0xb2); 									// �Զ�д����
	write_ddc(0x24,0x00,0x00); 							// ���õ�ַָ��
  
}

//�趨��ʾ�ĵ�ַ
void goto_xy(uchar x,uchar y,uchar mode)
{
     uint  temp;
     temp = 128 / 8 * y + x;
     if(mode)                                 //mode = 1ΪGraphic
     {                                        //���ͼ��ģʽҪ����ͼ�����׵�ַ0x0800
         temp = temp + 0x0100;
     }
     write_ddc(0x24,temp & 0xff,temp / 256);	  //��ַָ��λ��
}

//��ʾһ��ASCII�뺯��
void Putchar(uchar x,uchar y,uchar Charbyte)
{
     goto_xy(x,y,TXT);
     write_dc(0xC4,Charbyte-32);		      //����һ�ζ�д��ʽ //���ַ�rom
	 
}

void display_string(uchar x,uchar y,uchar *p)
{
	while(*p != 0)
	{
		if(x > 15 )                          //�Զ�����	 128*64
		{
			x = 0;
			y++;
		}
		Putchar(x,y,*p);
		++x;
		++p;
	}
}



//��ʾһ������,j = k + nΪ(nΪҪ��ʾ���ֵĸ���),kΪѡ����ĸ��ֿ�ʼ
void dprintf_hanzi_string_1(struct typFNT_GB16 code *GB_16,uint X_pos,uint Y_pos,uchar j,uchar k)
{  
	unsigned int address;
	unsigned char m,n;

	while(k < j)
	{
		
		m = 0;
		address = LcmLengthDots / 8 * Y_pos + X_pos + 0x0800;
		for(n = 0;n < 16;n++)                                     //����ֵ16
    	{
		   write_ddc(0x24,(uchar)(address),(uchar)(address>>8));  //������ʾ�洢����ַ
		   write_dc(0xc0,GB_16[k].Mask[m++]);                     //д�뺺����ģ��
		   write_dc(0xc0,GB_16[k].Mask[m++]);                     //д�뺺����ģ�Ҳ�
		  
		   address = address + 128/8;                           //�޸���ʾ�洢����ַ����ʾ��һ�У���16�У�
	    }

		X_pos += 2;
		k++;
	}
}

void main()
{

	F12864_init();
	F12864_clear();
	while(1)
	{
		dprintf_hanzi_string_1(GB_16,0,0,10,3);			 //���¶ȴ�������
		//dprintf_hanzi_string_1(GB_16,3,18,15,10);			 // ����ʾ����
		dprintf_hanzi_string_1(GB_16,1,18,18,15);			 //  �����

		display_string(7,3,":2008/5");								

	}
}