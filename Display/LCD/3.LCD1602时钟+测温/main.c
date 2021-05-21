//****************************************************
//****************************************************
//			Сǿ���������Сϵͳ����
//****************************************************
//****************************************************

#include "main.h"
#include "LCD1602.h"
#include "DS1302.h"
#include "DS18B20.h"

//�������
unsigned int Temp_Buffer = 0;
unsigned char KEY_NUM = 0;
bit Flag_KEY_Set = 0;
unsigned char KEY2_Count = 0;


//****************************************************
//������
//****************************************************
void main()
{
	LCD1602_Init();									//��ʼ��LCD1602
	DS1302_Init();									//��ʼ��DS1302

	LCD1602_write_com(0x80);
	LCD1602_write_word("Welcome to use!");

	Temp_Buffer = Get_temp();  					//��ȡDS18B20��ֵ
	Delay_ms(1000);	

	LCD1602_write_com(0x01);					//����

	while(1)
	{				
		DS1302_GetTime(&DS1302Buffer);							//��ȡ��ǰRTCCֵ

		if(Flag_Time_Refresh == 1 && Flag_KEY_Set == 0)			//���ݸ���ʱ��ˢ��LCD
		{
			Flag_Time_Refresh = 0;
			Display_RTCC();										//��ʾʵʱʱ��
			Display_Temp();										//��ȡ����ʾ�¶�ֵ
		}

		Scan_Key();											//����ɨ��

		switch( KEY_NUM )
		{
			case 1:						//����1ִ�г���
					KEY_NUM = 0;		//��հ�����־
					Flag_KEY_Set = ~Flag_KEY_Set;	//���ð�����־λ��ת
					if(Flag_KEY_Set)
					{						
						LCD1602_write_com(0x0d);	//LCD1602ָ����˸��
						LCD1602_write_com(0x80+ 3);	//ָ�����õ���һ�е�4��
						KEY2_Count = 0;				//����S2��������
					}
					else
					{
						LCD1602_write_com(0x0c);	//LCD1602ָ����˸��
					}
					break;
			case 2:						//����2ִ�г���
					KEY_NUM = 0;		//��հ�����־
					if(Flag_KEY_Set)			//������״̬ʱ����
					{
						KEY2_Count++;
						if(KEY2_Count == 7)
							KEY2_Count = 0;
						switch(KEY2_Count)
						{
							case 0:
									LCD1602_write_com(0x80+ 3);	//ָ��ָ���꣬��˸
									break;
							case 1:
									LCD1602_write_com(0x80+ 6);	//ָ��ָ���£���˸
									break;
							case 2:
									LCD1602_write_com(0x80+ 9);	//ָ��ָ���գ���˸
									break;
							case 3:
									LCD1602_write_com(0x80+ 15);	//ָ��ָ�����ڣ���˸
									break;
							case 4:
									LCD1602_write_com(0x80+ 0x40 + 1);	//ָ��ָ��Сʱ����˸
									break;
							case 5:
									LCD1602_write_com(0x80+ 0x40 + 4);	//ָ��ָ����ӣ���˸
									break;
							case 6:
									LCD1602_write_com(0x80+ 0x40 + 7);	//ָ��ָ�����ӣ���˸
									break;
							default : break;
						}	
					}					
					break;
			case 3:						//����3ִ�г���
					KEY_NUM = 0;		//��հ�����־
					if(Flag_KEY_Set)			//������״̬ʱ����
					{
						switch(KEY2_Count)
						{
							case 0:
									DS1302Buffer.Year++;
									if(DS1302Buffer.Year >= 100)
									{
										DS1302Buffer.Year = 0;
									}
									DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );

									Display_RTCC();
									
									LCD1602_write_com(0x80+ 3);	//ָ��ָ���꣬��˸
									break;
							case 1:
									DS1302Buffer.Month++;
									if(DS1302Buffer.Month >= 13)
									{
										DS1302Buffer.Month = 1;
									}
									DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 6);	//ָ��ָ���£���˸
									break;
							case 2:
									DS1302Buffer.Day++;
									if(DS1302Buffer.Day >= 32)
									{
										DS1302Buffer.Day = 1;
									}
									DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 9);	//ָ��ָ���գ���˸
									break;
							case 3:
									DS1302Buffer.Week++;
									if(DS1302Buffer.Week >= 8)
									{
										DS1302Buffer.Week = 1;
									}
									DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 15);	//ָ��ָ�����ڣ���˸
									break;
							case 4:
									DS1302Buffer.Hour++;
									if(DS1302Buffer.Hour >= 24)
									{
										DS1302Buffer.Hour = 0;
									}
									DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 1);	//ָ��ָ��Сʱ����˸
									break;
							case 5:
									DS1302Buffer.Minute++;
									if(DS1302Buffer.Minute >= 60)
									{
										DS1302Buffer.Minute = 0;
									}
									DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 4);	//ָ��ָ����ӣ���˸
									break;
							case 6:
									DS1302Buffer.Second++;
									if(DS1302Buffer.Second >= 60)
									{
										DS1302Buffer.Second = 0;
									}
									DS1302_SetTime( DS1302_SECOND , DS1302Buffer.Second);
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 7);	//ָ��ָ�����ӣ���˸
									break;
							default : break;
						}
					}
					break;
			case 4:						//����4ִ�г���
					KEY_NUM = 0;		//��հ�����־
					if(Flag_KEY_Set)			//������״̬ʱ����
					{
						switch(KEY2_Count)
						{
							case 0:
									DS1302Buffer.Year--;
									if(DS1302Buffer.Year == 255)
									{
										DS1302Buffer.Year = 99;
									}
									DS1302_SetTime( DS1302_YEAR , DS1302Buffer.Year );
									Display_RTCC();
									
									LCD1602_write_com(0x80+ 3);	//ָ��ָ���꣬��˸
									break;
							case 1:
									DS1302Buffer.Month--;
									if(DS1302Buffer.Month == 0)
									{
										DS1302Buffer.Month = 12;
									}
									DS1302_SetTime( DS1302_MONTH , DS1302Buffer.Month );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 6);	//ָ��ָ���£���˸
									break;
							case 2:
									DS1302Buffer.Day--;
									if(DS1302Buffer.Day == 0)
									{
										DS1302Buffer.Day = 31;
									}
									DS1302_SetTime( DS1302_DAY , DS1302Buffer.Day );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 9);	//ָ��ָ���գ���˸
									break;
							case 3:
									DS1302Buffer.Week--;
									if(DS1302Buffer.Week == 0)
									{
										DS1302Buffer.Week = 7;
									}
									DS1302_SetTime( DS1302_WEEK , DS1302Buffer.Week );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 15);	//ָ��ָ�����ڣ���˸
									break;
							case 4:
									DS1302Buffer.Hour--;
									if(DS1302Buffer.Hour == 255)
									{
										DS1302Buffer.Hour = 23;
									}
									DS1302_SetTime( DS1302_HOUR , DS1302Buffer.Hour );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 1);	//ָ��ָ��Сʱ����˸
									break;
							case 5:
									DS1302Buffer.Minute--;
									if(DS1302Buffer.Minute == 255)
									{
										DS1302Buffer.Minute = 59;
									}
									DS1302_SetTime( DS1302_MINUTE , DS1302Buffer.Minute );
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 4);	//ָ��ָ����ӣ���˸
									break;
							case 6:
									DS1302Buffer.Second--;
									if(DS1302Buffer.Second == 255)
									{
										DS1302Buffer.Second = 59;
									}
									DS1302_SetTime( DS1302_SECOND , DS1302Buffer.Second);
									Display_RTCC();
	
									LCD1602_write_com(0x80+ 0x40 + 7);	//ָ��ָ�����ӣ���˸
									break;
							default : break;
						}
					}
					break;
			default : break;
		}				
	}
}

//****************************************************
//��ʾ�¶�
//****************************************************
void Display_Temp()
{
	Temp_Buffer = Get_temp();  					//��ȡDS18B20��ֵ
	LCD1602_write_com(0x80+0x40+10);				//����LCD1602ָ�뵽�ڶ��е�11��
	if(flag_temper == 1)						//�����¶ȱ�־λ��ʾ�¶�����
	{
		LCD1602_write_data('-');	
	}
	if( Temp_Buffer/1000 != 0 )					//�����һλΪ0��������ʾ
	{
		LCD1602_write_data(Temp_Buffer/1000+0X30);	   //��ʾ�¶Ȱ�λֵ
	}
	LCD1602_write_data(Temp_Buffer%1000/100+0X30);	   //��ʾ�¶�ʮλֵ
	LCD1602_write_data(Temp_Buffer%100/10+0X30);	   //��ʾ�¶ȸ�λֵ
	LCD1602_write_data('.');						   //��ʾС����
	LCD1602_write_data(Temp_Buffer%10+0X30);		   //��ʾ�¶�ʮ��λֵ
	LCD1602_write_data('C');						   //��ʾ�ַ�C
		
}

//****************************************************
//��ʾʵʱʱ��
//****************************************************
void Display_RTCC()
{
	LCD1602_write_com(0x80);							//ָ�����õ���һ�е�1��
	
	LCD1602_write_word("20");
	LCD1602_write_data(DS1302Buffer.Year/10 + 0x30);	 //��ʾYear��λ
	LCD1602_write_data(DS1302Buffer.Year%10 + 0x30);	 //��ʾYear��λ
	LCD1602_write_data('-');
	LCD1602_write_data(DS1302Buffer.Month/10 + 0x30);	 //��ʾMonth��λ
	LCD1602_write_data(DS1302Buffer.Month%10 + 0x30);	 //��ʾMonth��λ
	LCD1602_write_data('-');
	LCD1602_write_data(DS1302Buffer.Day/10 + 0x30);		 //��ʾDay��λ
	LCD1602_write_data(DS1302Buffer.Day%10 + 0x30);		 //��ʾDay��λ

	LCD1602_write_com(0x80+13);							 //ָ�����õ���һ�е�14��
	switch(DS1302Buffer.Week)
	{
		case 1:
				LCD1602_write_word("Mon");				 //��ʾ����һ
				break;
		case 2:
				LCD1602_write_word("Tue");				 //��ʾ���ڶ�
				break;
		case 3:
				LCD1602_write_word("Wed");				 //��ʾ������
				break;
		case 4:
				LCD1602_write_word("Thu");				 //��ʾ������
				break;
		case 5:
				LCD1602_write_word("Fri");				 //��ʾ������
				break;
		case 6:
				LCD1602_write_word("Sat");				 //��ʾ������
				break;
		case 7:
				LCD1602_write_word("Sun");				 //��ʾ������
				break;
		default : break;						
	}		


	LCD1602_write_com(0x80 + 0x40);				 //ָ�����õ��ڶ��е�1��

	LCD1602_write_data(DS1302Buffer.Hour/10 + 0x30);	 //��ʾHour��λ
	LCD1602_write_data(DS1302Buffer.Hour%10 + 0x30);	 //��ʾHour��λ
	LCD1602_write_data(':');
	LCD1602_write_data(DS1302Buffer.Minute/10 + 0x30);	 //��ʾMinute��λ
	LCD1602_write_data(DS1302Buffer.Minute%10 + 0x30);	 //��ʾMinute��λ
	LCD1602_write_data(':');
	LCD1602_write_data(DS1302Buffer.Second/10 + 0x30);	 //��ʾSecond��λ
	LCD1602_write_data(DS1302Buffer.Second%10 + 0x30);	 //��ʾSecond��λ	
}

//****************************************************
//����������
//****************************************************
void Buzzer_Di()
{
	Buzzer = 0;
	Delay_ms(3);
	Buzzer = 1;
	Delay_ms(3);
}
//****************************************************
//����ɨ�����
//****************************************************
void Scan_Key()
{
	if( KEY1 == 0 )	 					//����1ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY1 == 0 )
		{
			Buzzer_Di();
			while(KEY1 == 0);			//�ȴ�����
			KEY_NUM = 1;
		}
	}

	if( KEY2 == 0 )						//����2ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY2 == 0 )
		{
			Buzzer_Di();
			while(KEY2 == 0);			//�ȴ�����
			KEY_NUM = 2;
		}
	}

	if( KEY3 == 0 )						//����2ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY3 == 0 )
		{
			Buzzer_Di();
			while(KEY3 == 0);			//�ȴ�����
			KEY_NUM = 3;
		}
	}

	if( KEY4 == 0 )						//����2ɨ��
	{
		Delay_ms(10);					//��ʱȥ��
		if( KEY4 == 0 )
		{
			Buzzer_Di();
			while(KEY4 == 0);			//�ȴ�����
			KEY_NUM = 4;
		}
	}
}

//****************************************************
//MS��ʱ����(12M�����²���)
//****************************************************
void Delay_ms(unsigned int n)
{
	unsigned int  i,j;
	for(i=0;i<n;i++)
		for(j=0;j<123;j++);
}