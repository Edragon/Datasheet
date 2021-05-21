#include "DS18B20.h"
#include<intrins.h>

//�������
unsigned char flag_temper = 0;

//****************************************************
//DS18B20��ʱ����
//****************************************************
void DS18B20_Delay( unsigned int n )
{
	unsigned int i;
	for(i = 0 ; i < n ; i++ );
}
//****************************************************
//DS18B20д1�ֽ�
//****************************************************
void DS18B20_Write_Byte( unsigned char dat)
{
	unsigned char i;
	for( i = 0 ; i < 8 ; i++ )
	{
		DS18B20_DQ = 0;
		_nop_();			//��ʱ>1us
		_nop_();
		
		DS18B20_DQ = dat&0x01;	 	//��д��λ
		dat >>= 1;

		DS18B20_Delay(6);	//��ʱ60~120us
		
		DS18B20_DQ = 1;		//�ͷ�����
		_nop_();			//��ʱ>1us
		_nop_();	
	}		
}

//****************************************************
//DS18B20��1�ֽ�
//****************************************************
unsigned char DS18B20_Read_Byte( )
{
	unsigned char dat,i;
	for( i = 0 ; i < 8 ; i++ )
	{
		DS18B20_DQ = 0;
		_nop_();			//��ʱ>1us
		_nop_();
				
		DS18B20_DQ = 1;		//�ͷ�����
		_nop_();			//��ʱ>1us
		_nop_();

		dat >>= 1;
		if( DS18B20_DQ == 1)
		{
			dat |= 0X80;
		}
		else
		{
			dat &= 0x7f;
		}

		DS18B20_Delay(6);	//��ʱ60~120us			
	}
	return dat;		
}

//****************************************************
//DS18B20��ʼ��
//****************************************************
bit DS18B20_Init()
{
	bit Flag_exist = 1;
	DS18B20_DQ = 1;			//�ͷ�����
	_nop_();				//��ʱ>1us
	_nop_();

	DS18B20_DQ = 0;	
	DS18B20_Delay(50);		//��ʱ480~960us	

	DS18B20_DQ = 1;			//�ͷ�����
	DS18B20_Delay(3);		//��ʱ15~60us

	Flag_exist = DS18B20_DQ;
	DS18B20_Delay(10);		//��ʱ60~240us

	DS18B20_DQ = 1;			//�ͷ�����
	return Flag_exist;			
}

//**********************************************************
//��ȡ�¶Ⱥ����������¶ȵľ���ֵ������עflag_temper��flag_temper=1��ʾ����flag_temper=0��ʾ��
//**********************************************************
unsigned int Get_temp(void)         //��ȡ�¶�ֵ 
{  
	float tt;
	unsigned char a,b;
	unsigned int temp;
	if( DS18B20_Init() == 0 )                //��ʼ��
	{
		DS18B20_Write_Byte(0xcc);          //����ROMָ��
		DS18B20_Write_Byte(0x44);          //�¶�ת��ָ��
	
	//	_delay_ms(750);				//PROTEUS������Ҫ��
	
		if( DS18B20_Init() == 0 )                 //��ʼ��
		{
			DS18B20_Write_Byte(0xcc);          //����ROMָ��
			DS18B20_Write_Byte(0xbe);          //���ݴ���ָ��
			a = DS18B20_Read_Byte();           //��ȡ���ĵ�һ���ֽ�Ϊ�¶�LSB
			b = DS18B20_Read_Byte();           //��ȡ���ĵ�һ���ֽ�Ϊ�¶�MSB
			temp = b;                      //�ȰѸ߰�λ��Ч���ݸ���temp
			temp <<= 8;                    //������8λ���ݴ�temp�Ͱ�λ�Ƶ��߰�λ
			temp = temp|a;                //���ֽںϳ�һ�����ͱ���
			
			if(temp>0xfff)
			{
				flag_temper=1;				//�¶�Ϊ����
				temp=(~temp)+1;
			}
			else
			{																			   
				flag_temper=0;				//�¶�Ϊ������0
			}
			
			tt = temp*0.0625;              //�õ���ʵʮ�����¶�ֵ
			                                //��ΪDS18B20���Ծ�ȷ��0.0625��
			                                //���Զ������ݵ����λ�������0.0625��
			temp = tt*10+0.5;               //�Ŵ�ʮ��
			                                //��������Ŀ�Ľ�С������һλҲת��Ϊ����ʾ����
		                              	    //ͬʱ����һ���������������
		}
	}
	return temp;
}


