
;/*------------------------------------------------------------------*/
;/* --- STC MCU International Limited -------------------------------*/
;/* --- STC 1T Series MCU Programme Demo ----------------------------*/
;/* --- Fax: 86-755-82944243 ----------------------------------------*/
;/* --- Tel: 86-755-82948412 ----------------------------------------*/
;/* --- Web: www.STCMCU.com -----------------------------------------*/
;/* If you want to use the program or the program referenced in the  */
;/* article, please specify in which data and procedures from STC    */
;/*------------------------------------------------------------------*/

;/*      �����򾭹�������ȫ����, ���ṩ�绰����֧��, �粻�����, �����в�����ػ���.  */


;*************	��������˵��	**************

;	���Է�����
;	����������ʱ�ӺͲ������޸��������ض��壬��������ص�MCU��
;	ͨ������������MCU�������ݣ�MCU�յ���ԭ�����ء�
;	����0Ϊ9600bps������2Ϊ19200bps��ʱ��Ϊ22.1184MHZ.


AUXR	EQU	08EH
S2CON	EQU	09AH
S2BUF	EQU 09BH
IE2		EQU 0AFH
BRT		EQU 09CH

BUF_LENTH	EQU 128		;���崮�ڽ��ջ��峤��
uart0_wr	EQU 30H		;дָ��
uart0_rd	EQU	31H		;��ָ��
RX0_Buffer	EQU	0000H	;���忪ʼ��ַ
B_TI		BIT	20H.0	;������ɱ�־

uart2_wr	EQU 32H		;дָ��
uart2_rd	EQU	33H		;��ָ��
RX2_Buffer	EQU	0x080	;���忪ʼ��ַ
B_TI2		BIT	20H.1	;������ɱ�־

Baudrate0	EQU	(256-12)	;9600bps at 22.1184MHZ

Baudrate2	EQU	(256-6)		;19200bps at 22.1184MHZ

/**********************************************************/


/****************** �������Զ����ɣ��û������޸� ************************************/


/**********************************************************/

	ORG		00000H
	LJMP	F_main

	ORG		00023H
	LJMP	F_UART0_RCV

	ORG		00043H
	LJMP	F_UART2_RCV


;****************** ������ ****************************/
F_main:
	MOV		SP,#080H

	LCALL	F_uart0_init	;����0��ʼ��
	LCALL	F_uart2_init	;����1��ʼ��
L_MainLoop:
; 		
	MOV  	A,uart0_rd	;if(uart0_rd != uart0_wr)	//�ж϶�дָ���Ƿ���ȣ������������ݣ�����0ԭ������
	XRL  	A,uart0_wr
	JZ   	L_CheckUART2

	CLR  	B_TI		;�������������־
	MOV  	A,#LOW (RX0_Buffer)	;ȡ�����ַ
	ADD  	A,uart0_rd
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,#HIGH (RX0_Buffer)
	MOV  	DPH,A
	MOVX 	A,@DPTR		;����������
	MOV  	SBUF,A		;���ڷ���
	JNB  	B_TI,$		;�ȴ��������
	CLR  	B_TI		;�������������־
	
	INC  	uart0_rd	;ָ��+1
	MOV  	A,uart0_rd
	CLR  	C
	SUBB 	A,#080H		;�ж�ָ���Ƿ�Խ��
	JC   	L_CheckUART2
	MOV  	uart0_rd,#0	;ָ��Խ�磬��0

L_CheckUART2:
	MOV  	A,uart2_rd	;if(uart2_rd != uart2_wr)	//�ж϶�дָ���Ƿ���ȣ������������ݣ�����2ԭ������
	XRL  	A,uart2_wr
	JZ   	L_MainLoop	;ָ����ȣ�������Ҫ��

	CLR  	B_TI2		;�������������־
	MOV  	A,#LOW (RX2_Buffer)	;ȡ�����ַ
	ADD  	A,uart2_rd
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,#HIGH (RX2_Buffer)
	MOV  	DPH,A
	MOVX 	A,@DPTR		;����������
	MOV  	S2BUF,A	;���ڷ���
	JNB  	B_TI2,$	;�ȴ��������
	CLR  	B_TI2	;�������������־

	INC  	uart2_rd	;ָ��+1
	MOV  	A,uart2_rd
	CLR  	C
	SUBB 	A,#080H		;�ж�ָ���Ƿ�Խ��
	JC   	L_MainLoop
	MOV  	uart2_rd,#0	;ָ��Խ�磬��0
	SJMP 	L_MainLoop

;****************** ����0��ʼ�� ****************************/
F_uart0_init:

	ORL  	PCON,#080H		;UART0 Double Rate Enable
	MOV  	SCON,#050H		;UART0 set as 10bit , UART0 RX enable
	ANL  	TMOD,#NOT (1 SHL 6);0BFH	Timer1 Set as Timer, 12T
	MOV  	A,TMOD
	ANL  	A,#NOT (3 SHL 4)	;0CFH	
	ORL  	A,#020H		;Timer1 set as 8 bits auto relaod
	MOV  	TMOD,A
	MOV  	TH1,#Baudrate0	;Load the baudrate
	SETB 	TR1			;Enable Timer1
	SETB 	ES			;Enable UART0 interrupt
	SETB 	EA			;Enable grobal interrupt
	RET  	


;***************** ����0�жϺ��� *****************************/
F_UART0_RCV:
	PUSH 	ACC		;�ֳ�����
	PUSH 	DPH
	PUSH 	DPL
	PUSH 	PSW

	JNB  	RI,L_UART0_TX		;����Ƿ񴮿�0�����ж�
	CLR  	RI					;�������0�жϱ�־
	MOV  	A,#LOW (RX0_Buffer)	;ȡ�����ַ��DPTR
	ADD  	A,uart0_wr
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,#HIGH (RX0_Buffer)
	MOV  	DPH,A
	MOV  	A,SBUF				;�����ջ���
	MOVX 	@DPTR,A
	INC  	uart0_wr			;��һ����ַ
	MOV  	A,uart0_wr			;���ָ���Ƿ�Խ��
	CLR  	C
	SUBB 	A,#080H
	JC   	L_UART0_TX
	MOV  	uart0_wr,#00H		;ָ��Խ����0

L_UART0_TX:
	JNB  	TI,L_UART0_END		;����Ƿ����ж�
	CLR  	TI					;������ͱ�־
	SETB 	B_TI				;����������־

L_UART0_END:
	POP  	PSW		;�ָ��ֳ�
	POP  	DPL
	POP  	DPH
	POP  	ACC
	RETI 	


;****************** ����2��ʼ�� ****************************/
F_uart2_init:
	ORL  	AUXR,#08H		;����2�����ʼӱ�
	MOV  	A,S2CON
	ANL  	A,#03FH			;�������λ
	ORL  	A,#(1 SHL 6)	;����2ģʽ1��8λUART��(2^S2SMOD / 32) * BRT�����
	MOV  	S2CON,A
	ORL  	S2CON,#(1 SHL 4);����2����
	ORL  	AUXR,#(1 SHL 4)	;baudrate use BRT
	MOV  	BRT,#Baudrate2		;������
	ORL  	IE2,#01H		;������2�ж�
	RET  	

;******************** ����2�жϺ��� **************************
F_UART2_RCV:

	PUSH 	ACC		;�ֳ�����
	PUSH 	DPH
	PUSH 	DPL
	PUSH 	PSW
	
	MOV  	A,S2CON
	JNB  	ACC.0,L_UART2_TX	;���ǽ����жϣ�ת
	ANL  	S2CON,#NOT 1; 0FEH	;����жϱ�־
	MOV  	A,#LOW (RX2_Buffer)	;ȡ�����ַ��DPTR
	ADD  	A,uart2_wr
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,#HIGH (RX2_Buffer)
	MOV  	DPH,A
	MOV  	A,S2BUF			;�����ջ���
	MOVX 	@DPTR,A
	INC  	uart2_wr		;��һ����ַ
	MOV  	A,uart2_wr		;���ָ���Ƿ�Խ��
	CLR  	C
	SUBB 	A,#080H
	JC   	L_UART2_TX
	MOV  	uart2_wr,#00H	;ָ��Խ����0

L_UART2_TX:
	MOV  	A,S2CON
	JNB  	ACC.1,L_UART2_END	;����Ƿ����ж�
	ANL  	S2CON,#0FDH			;������ͱ�־
	SETB 	B_TI2				;����������־

L_UART2_END:
	POP  	PSW		;�ָ��ֳ�
	POP  	DPL
	POP  	DPH
	POP  	ACC
	RETI 	

	END


