
/*------------------------------------------------------------------*/
/* --- STC MCU International Limited -------------------------------*/
/* --- STC 1T Series MCU Programme Demo ----------------------------*/
/* --- Fax: 86-755-82944243 ----------------------------------------*/
/* --- Tel: 86-755-82948412 ----------------------------------------*/
/* --- Web: www.STCMCU.com -----------------------------------------*/
/* If you want to use the program or the program referenced in the  */
/* article, please specify in which data and procedures from STC    */
/*------------------------------------------------------------------*/

/*      �����򾭹�������ȫ����, ���ṩ�绰����֧��, �粻�����, �����в�����ػ���.  */


/*************	��������˵��	**************

����P0��P1��P2��P3��IO����PUSH-PULLģʽ���100HZ�ķ����ź�.

�ʺ�STC12C5204ADϵ�С�STC11Fxx��STC10Fxx��STC12C5A60S2ϵ�С�

******************************************/


/*************	�û�ϵͳ����	**************/

#define MAIN_Fosc		22118400L	//������ʱ��, ģ�⴮�ںͺ���ʱ���Զ���Ӧ��5~35MHZ


#include	<reg51.h>

sfr P1M1 = 0x91;	//P1M1.n,P1M0.n 	=00--->Standard,	01--->push-pull
sfr P1M0 = 0x92;	//					=10--->pure input,	11--->open drain
sfr P0M1 = 0x93;	//P0M1.n,P0M0.n 	=00--->Standard,	01--->push-pull
sfr P0M0 = 0x94;	//					=10--->pure input,	11--->open drain
sfr P2M1 = 0x95;	//P2M1.n,P2M0.n 	=00--->Standard,	01--->push-pull
sfr P2M0 = 0x96;	//					=10--->pure input,	11--->open drain
sfr P3M1  = 0xB1;	//P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
sfr P3M0  = 0xB2;	//					=10--->pure input,	11--->open drain



//========================================================================
// ����: void  delay_ms(unsigned char ms)
// ����: ��ʱ������
// ����: ms,Ҫ��ʱ��ms��.
// ����: none.
// �汾: VER1.0
// ����: 2010-12-15
// ��ע: 
//========================================================================
void  delay_ms(unsigned char ms)
{
     unsigned int i;
	 do{
	      i = MAIN_Fosc / 14000;
		  while(--i)	;   //14T per loop
     }while(--ms);
}

void	main(void)
{
	P0M1 &= ~0xff,	P0M0 |=  0xff;	//P0 set as push-pull output mode
	P1M1 &= ~0xff,	P1M0 |=  0xff;	//P0 set as push-pull output mode
	P2M1 &= ~0xff,	P2M0 |=  0xff;	//P0 set as push-pull output mode
	P3M1 &= ~0xff,	P3M0 |=  0xff;	//P0 set as push-pull output mode
	
	while(1)
	{
		delay_ms(5);
		P0 = ~P0;
		P1 = ~P1;
		P2 = ~P2;
		P3 = ~P3;
	}
}