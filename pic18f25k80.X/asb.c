//For the RX target board, the connection is Gray, White, Orange, Blue, and Black, Red, White, Yellow

#include "p18f25k80.h"
#include "config.h"
#include "constants.h"
#include "nRF2401.h"
#include <timers.h>
#include <math.h>
#include <delays.h>
//#include "lcdcontroller.h"
#include <adc.h>

//#pragma config OSC = INTIO67, LVP = OFF, PBADEN = ON, WDT = OFF

//===============================================================================


#define MODE		1
#define TRIS_MODE	TRISAbits.TRISA2

#define LED			PORTBbits.RB0
#define TRIS_LED	TRISBbits.TRISB0

#define GREEN		PORTAbits.RA5
#define TRIS_GREEN	TRISAbits.TRISA5
#define Button		PORTAbits.RA3
#define TRIS_Button	TRISAbits.TRISA3

#define	M1			PORTBbits.RB2
#define	TRIS_M1		TRISBbits.TRISB2
#define	M2			PORTBbits.RB3
#define	TRIS_M2		TRISBbits.TRISB3
#define	M3			PORTBbits.RB4
#define	TRIS_M3		TRISBbits.TRISB4
#define	M4			PORTBbits.RB5
#define	TRIS_M4		TRISBbits.TRISB5

unsigned char ACK_buf[2] = {0x12,0x34};
volatile unsigned char rx_buf[32];



//======== constants =================
//const int THERM_TEMP[22] = {15,25,30,35,40,45,50,55,60,65,70,75,80,85,90,95,100,105,110,115,120,125};
//const int THERM_MEAS[22] = {62,93,113,136,162,191,222,257,293,332,372,412,453,494,533,572,609,644,677,708,737,762};
//const int THERM_A[22] = {327,251,218,193,173,158,145,136,130,125,123,123,123,126,129,136,142,152,162,175,194,214};




//======== prototypes =================

void TXService(void);
void RXService(void);
void HIGH_ISR(void);

//======== global variables =================
unsigned char CT1, CT2;
unsigned char Rxed;
unsigned char NoDat;

//========== main =====================================================================
void main(void)
{
	int temp1;
	int T1;
	int T2;
	int i;

    INTCON2bits.RBPU = set;
	OSCCON = 0b01100011; //internal oscilator
	OSCCON2 = 0b00000000;
	RCONbits.IPEN = clear;
	ODCON = 0b00000000;
	T1CONbits.SOSCEN = clear;
	T3CONbits.SOSCEN = clear;
	ANCON1= 0b00000000;

	TRIS_CE = output;
	TRIS_CSN = output;
	TRIS_IRQ = input;

	TRIS_LED = output;
	TRIS_MODE = input;
	TRIS_Button = input;
	TRIS_GREEN = output;

	M1 = clear;
	M2 = clear;
	M3 = clear;
	M4 = clear;

	ANCON0 = 0b00000001;

	TRIS_SCK = output;
	TRIS_MISO = input;
	TRIS_MOSI = output;

	TRISAbits.TRISA0 = input;

	INTCONbits.PEIE = clear;
	INTCONbits.GIE = clear;
	T0CON = 0b10001000; //bit3: prescale disable, bits 0-2 prescale.  Timer Period.
	if (MODE == clear) {
		T0CON = 0b11000110; //bit3: prescale disable, bits 0-2 prescale.  Timer Period.
	}
	INTCONbits.TMR0IE = set;
	INTCONbits.TMR0IF = clear;
	//T2CON = 0b00000100; //prescale 1, postscale 1
	PIE1bits.TMR2IE = clear;
	IPR1bits.TMR2IP = clear;
	PIR1bits.TMR2IF = clear;

	Delay10KTCYx(50);

	//===configure SPI for nordic RF module
	SSPSTAT = 0b00000000;	//SPI, clock on idle to active clk trans
	SSPSTATbits.CKE = 1; 	//clock on idle to active clk trans
	SSPCON1 = 0b00100001;	//SPI setup. clk at 1/16; idle low.
	SSPCON1bits.CKP = 0;	//clock polarity, idle low
	SSPCON1bits.SSPEN = set;	//enable SPI module
	CE = clear;
	CSN = set;

	LED = set;
	Delay10KTCYx(20);
	LED = clear;
	Delay10KTCYx(20);
	LED = set;
	Delay10KTCYx(20);
	LED = clear;

        initTX();
        TRIS_M1 = input;
        TRIS_M2 = input;
        TRIS_M3 = input;
        TRIS_M4 = input;


	Delay10KTCYx(20);
        TRISAbits.TRISA7 = output;
        PORTAbits.RA7 = 1;


	LED = set;
	INTCONbits.GIE = set;
	GREEN = set;

	Rxed = clear;
	NoDat = clear;

	//LCD_putch('O'); LCD_putch('K'); LCD_putch('!');
	i=0;
	while(1) {
		i++;
		//LCD_cmd(LINE_2);
		//writenum(Rxed,4,0,0);
		Delay10KTCYx(20);
	}

}

//====== high interrupt service address =======================================
#pragma code high_vector=0x08
void INT_AT_HIGH_VECTOR(void) {
	_asm GOTO HIGH_ISR _endasm
}
#pragma code

//====== high interrupt service routine =======================================
#pragma interrupt HIGH_ISR
void HIGH_ISR(void) {
	LED = set;
        //PORTAbits.RA7 = !PORTAbits.RA7;
	if (MODE == set) {
		TXService();
	} else {
		RXService();
	}
	INTCONbits.TMR0IF = clear;
	LED = clear;
}

//====== TXService =======================================
void TXService(void) {
	unsigned char status = 0;
	unsigned char tx_buf[TX_PLOAD_WIDTH];
	unsigned char config_reg;

	//Delay10KTCYx(100);

	// ----- check nRF config and correct if needed ---------
	config_reg = SPI_Read(CONFIG);
	//LCD_cmd(LINE_2);
	//writenum(config_reg,4,0,0);
	//LCD_cmd(LINE_3+13);
	//writenum(SPI_Read(FEATURE),4,0,0);  //read and display FEATURE
        
	if(config_reg != 0x0E) {
		//LCD_cmd(LINE_1);
		//LCD_putch('x'); LCD_putch('x');
		initTX();
		//Delay10KTCYx(2);
		//LCD_cmd(LINE_1);
		//LCD_putch('O'); LCD_putch('K'); LCD_putch('!');
	}

	//---- get status for IRQ service -------
	//SPI_RW_Reg(WRITE_REG + STATUS, MAX_RT);
	status=SPI_Read(STATUS);
	//LCD_cmd(LINE_1+8);
	//LCD_putch('S'); LCD_putch(':');
	//writenum(status,4,0,0);
	//if(LED==0) LCD_putch('|'); else LCD_putch('-');
	//LCD_cmd(LINE_2+2);
	//writenum(SPI_Read(FIFO_STATUS),4,0,0);  //read and display FIFO status
	//writenum(SPI_Read(FEATURE),4,0,0);  //read and display FEATURE

	//status=SPI_Read(STATUS);
	// ---- service IRQ flags ---------
	if(status&MAX_RT) {
		//Delay10KTCYx(10);
		//SPI_RW_Reg(FLUSH_TX,0);
		SPI_RW_Reg(WRITE_REG + STATUS, MAX_RT);				//clear MAX_RT flag
		//SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);	//reload payload
		//CE = set;											//begin transmit
		//Delay1KTCYx(2);
		//CE = clear;
	}

        SPI_RW_Reg(FLUSH_TX,0);
        tx_buf[0] = M1;
        tx_buf[1] = 1;
        tx_buf[2] = 1;
        tx_buf[3] = 1;
        SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);
        CE = set;											//begin transmit
        Delay1KTCYx(2);
        CE = clear;


        PORTAbits.RA7 = status & RX_DR;
	if(status&RX_DR) {
		SPI_RW_Reg(WRITE_REG + STATUS, RX_DR);
		SPI_Read_Buf(RD_RX_PLOAD,rx_buf,2);
		SPI_RW_Reg(FLUSH_RX,0);
		//writenum(SPI_Read(R_RX_PL_WID),1,0,0);
	}
	//---- END service IRQ flags ---------
}

//====== RXService =======================================
void RXService( void ) {
	unsigned char i;
	unsigned char k = 0;
	unsigned char status;
	unsigned char ffstat, config_reg;

	LED = set;

	// ----- check nRF config and correct if needed ---------
	config_reg = SPI_Read(CONFIG);
	//LCD_cmd(LINE_2+13);
	//writenum(config_reg,4,0,0);
	//LCD_cmd(LINE_3+13);
	//writenum(SPI_Read(FEATURE),4,0,0);  //read and display FEATURE
	if(config_reg != 0x0F) {
		//LCD_cmd(LINE_1);
		//LCD_putch('x'); LCD_putch('x');
		initRX();
		Delay10KTCYx(50);
		//LCD_cmd(LINE_1);
		//LCD_putch('O'); LCD_putch('K'); LCD_putch('!');
	}

	//LCD_cmd(LINE_1+3);
	//LCD_putch('R'); LCD_putch('X');

	//------ load ACK payload data -------------
	ACK_buf[1] = 0;
	//LCD_cmd(LINE_3+6);
	//LCD_putch('A'); LCD_putch(':');
	//writeHEXbyte(ACK_buf[1]);
	SPI_RW_Reg(FLUSH_TX,0);
	SPI_Write_Buf(W_ACK_PAYLOAD,ACK_buf,2);

	// ----- get status for IRQ service ---------
	status = SPI_Read(STATUS);
	ffstat = SPI_Read(FIFO_STATUS);


	//LCD_cmd(LINE_2);
	//LCD_putch('S'); LCD_putch('t'); LCD_putch('a'); LCD_putch('t');
	//writenum(status,4,0,0);
	//if(LED) LCD_putch('|'); else LCD_putch('-');
	//writenum(ffstat,4,0,0);  //display fifo status
	//LED++;

	NoDat++;
	//------- IRQ service --------
	if(((status & RX_DR))||(!(ffstat & 0x01))) {
		SPI_RW_Reg(WRITE_REG + STATUS, RX_DR);	//clear RX flag
		//LCD_cmd(LINE_3);
		//LCD_putch('R'); LCD_putch('X'); LCD_putch('!'); LCD_putch('!'); LCD_putch('!');
		while((ffstat & 0x01) == 0) {
			//read entire buffer---------
			Rxed++;
			NoDat = clear;
			SPI_Read_Buf(RD_RX_PLOAD,rx_buf,32);
			//read buffer done, now display it-----------
			//LCD_cmd(LINE_4);
			//LCD_putch('R'); LCD_putch('X'); LCD_putch('>');
			//for(i=0;i<=4;i++) {
			//	writeHEXbyte(rx_buf[i]);
			//}
			//LCD_putch('-');
			//writeHEXbyte(rx_buf[31]);
			//display buffer done
			ffstat = SPI_Read(FIFO_STATUS);
		}
	} else {
		//Delay10KTCYx(20);
		//LCD_cmd(LINE_3);
		//LCD_putch('R'); LCD_putch('X'); LCD_putch('.'); LCD_putch('.'); LCD_putch('.');
	}
	//------- END IRQ service -------


	if(NoDat < 10) {
		if (rx_buf[0] == clear) {
 			M1 = clear;
			M2 = set;
			M3 = set;
			M4 = clear;
		} else if (rx_buf[1] == clear) {
 			M1 = set;
			M2 = clear;
			M3 = set;
			M4 = clear;
		} else if (rx_buf[2] == clear) {
 			M1 = clear;
			M2 = set;
			M3 = clear;
			M4 = set;
		} else if (rx_buf[3] == clear) {
 			M1 = set;
			M2 = clear;
			M3 = clear;
			M4 = set;
		}
	} else {
		NoDat = 10;
		M1 = clear;
		M2 = clear;
		M3 = clear;
		M4 = clear;
	}

	LED = clear;
}





