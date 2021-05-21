
;/*------------------------------------------------------------------*/
;/* --- STC MCU International Limited -------------------------------*/
;/* --- STC 1T Series MCU Programme Demo ----------------------------*/
;/* --- Fax: 86-755-82944243 ----------------------------------------*/
;/* --- Tel: 86-755-82948412 ----------------------------------------*/
;/* --- Web: www.STCMCU.com -----------------------------------------*/
;/* If you want to use the program or the program referenced in the  */
;/* article, please specify in which data and procedures from STC    */
;/*------------------------------------------------------------------*/

;/*      本程序经过测试完全正常, 不提供电话技术支持, 如不能理解, 请自行补充相关基础.  */


;*************	本程序功能说明	**************

;	测试方法：
;	根据您的主时钟和波特率修改下面的相关定义，编译后下载到MCU。
;	通过串口助手向MCU发送数据，MCU收到后原样返回。
;	串口0为9600bps，串口2为19200bps，时钟为22.1184MHZ.


AUXR	EQU	08EH
S2CON	EQU	09AH
S2BUF	EQU 09BH
IE2		EQU 0AFH
BRT		EQU 09CH

BUF_LENTH	EQU 128		;定义串口接收缓冲长度
uart0_wr	EQU 30H		;写指针
uart0_rd	EQU	31H		;读指针
RX0_Buffer	EQU	0000H	;缓冲开始地址
B_TI		BIT	20H.0	;发送完成标志

uart2_wr	EQU 32H		;写指针
uart2_rd	EQU	33H		;读指针
RX2_Buffer	EQU	0x080	;缓冲开始地址
B_TI2		BIT	20H.1	;发送完成标志

Baudrate0	EQU	(256-12)	;9600bps at 22.1184MHZ

Baudrate2	EQU	(256-6)		;19200bps at 22.1184MHZ

/**********************************************************/


/****************** 编译器自动生成，用户请勿修改 ************************************/


/**********************************************************/

	ORG		00000H
	LJMP	F_main

	ORG		00023H
	LJMP	F_UART0_RCV

	ORG		00043H
	LJMP	F_UART2_RCV


;****************** 主程序 ****************************/
F_main:
	MOV		SP,#080H

	LCALL	F_uart0_init	;串口0初始化
	LCALL	F_uart2_init	;串口1初始化
L_MainLoop:
; 		
	MOV  	A,uart0_rd	;if(uart0_rd != uart0_wr)	//判断读写指针是否相等，不等则有数据，串口0原样返回
	XRL  	A,uart0_wr
	JZ   	L_CheckUART2

	CLR  	B_TI		;清除发送完成软标志
	MOV  	A,#LOW (RX0_Buffer)	;取缓冲地址
	ADD  	A,uart0_rd
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,#HIGH (RX0_Buffer)
	MOV  	DPH,A
	MOVX 	A,@DPTR		;读缓冲数据
	MOV  	SBUF,A		;串口返回
	JNB  	B_TI,$		;等待发送完成
	CLR  	B_TI		;清除发送完成软标志
	
	INC  	uart0_rd	;指针+1
	MOV  	A,uart0_rd
	CLR  	C
	SUBB 	A,#080H		;判断指针是否越界
	JC   	L_CheckUART2
	MOV  	uart0_rd,#0	;指针越界，清0

L_CheckUART2:
	MOV  	A,uart2_rd	;if(uart2_rd != uart2_wr)	//判断读写指针是否相等，不等则有数据，串口2原样返回
	XRL  	A,uart2_wr
	JZ   	L_MainLoop	;指针相等，无数据要发

	CLR  	B_TI2		;清除发送完成软标志
	MOV  	A,#LOW (RX2_Buffer)	;取缓冲地址
	ADD  	A,uart2_rd
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,#HIGH (RX2_Buffer)
	MOV  	DPH,A
	MOVX 	A,@DPTR		;读缓冲数据
	MOV  	S2BUF,A	;串口返回
	JNB  	B_TI2,$	;等待发送完成
	CLR  	B_TI2	;清除发送完成软标志

	INC  	uart2_rd	;指针+1
	MOV  	A,uart2_rd
	CLR  	C
	SUBB 	A,#080H		;判断指针是否越界
	JC   	L_MainLoop
	MOV  	uart2_rd,#0	;指针越界，清0
	SJMP 	L_MainLoop

;****************** 串口0初始化 ****************************/
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


;***************** 串口0中断函数 *****************************/
F_UART0_RCV:
	PUSH 	ACC		;现场保护
	PUSH 	DPH
	PUSH 	DPL
	PUSH 	PSW

	JNB  	RI,L_UART0_TX		;检测是否串口0接收中断
	CLR  	RI					;清除串口0中断标志
	MOV  	A,#LOW (RX0_Buffer)	;取缓冲地址放DPTR
	ADD  	A,uart0_wr
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,#HIGH (RX0_Buffer)
	MOV  	DPH,A
	MOV  	A,SBUF				;读接收缓冲
	MOVX 	@DPTR,A
	INC  	uart0_wr			;下一个地址
	MOV  	A,uart0_wr			;检测指针是否越界
	CLR  	C
	SUBB 	A,#080H
	JC   	L_UART0_TX
	MOV  	uart0_wr,#00H		;指针越界清0

L_UART0_TX:
	JNB  	TI,L_UART0_END		;检测是否发送中断
	CLR  	TI					;清除发送标志
	SETB 	B_TI				;发送完成软标志

L_UART0_END:
	POP  	PSW		;恢复现场
	POP  	DPL
	POP  	DPH
	POP  	ACC
	RETI 	


;****************** 串口2初始化 ****************************/
F_uart2_init:
	ORL  	AUXR,#08H		;串口2波特率加倍
	MOV  	A,S2CON
	ANL  	A,#03FH			;清除高两位
	ORL  	A,#(1 SHL 6)	;串口2模式1，8位UART，(2^S2SMOD / 32) * BRT溢出率
	MOV  	S2CON,A
	ORL  	S2CON,#(1 SHL 4);允许串2接收
	ORL  	AUXR,#(1 SHL 4)	;baudrate use BRT
	MOV  	BRT,#Baudrate2		;波特率
	ORL  	IE2,#01H		;允许串口2中断
	RET  	

;******************** 串口2中断函数 **************************
F_UART2_RCV:

	PUSH 	ACC		;现场保护
	PUSH 	DPH
	PUSH 	DPL
	PUSH 	PSW
	
	MOV  	A,S2CON
	JNB  	ACC.0,L_UART2_TX	;不是接收中断，转
	ANL  	S2CON,#NOT 1; 0FEH	;清除中断标志
	MOV  	A,#LOW (RX2_Buffer)	;取缓冲地址放DPTR
	ADD  	A,uart2_wr
	MOV  	DPL,A
	CLR  	A
	ADDC 	A,#HIGH (RX2_Buffer)
	MOV  	DPH,A
	MOV  	A,S2BUF			;读接收缓冲
	MOVX 	@DPTR,A
	INC  	uart2_wr		;下一个地址
	MOV  	A,uart2_wr		;检测指针是否越界
	CLR  	C
	SUBB 	A,#080H
	JC   	L_UART2_TX
	MOV  	uart2_wr,#00H	;指针越界清0

L_UART2_TX:
	MOV  	A,S2CON
	JNB  	ACC.1,L_UART2_END	;检测是否发送中断
	ANL  	S2CON,#0FDH			;清除发送标志
	SETB 	B_TI2				;发送完成软标志

L_UART2_END:
	POP  	PSW		;恢复现场
	POP  	DPL
	POP  	DPH
	POP  	ACC
	RETI 	

	END


