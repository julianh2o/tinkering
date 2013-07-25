//For the RX target board, the connection is Gray, White, Orange, Blue, and Black, Red, White, Yellow

#include "p18f25k80.h"
#include "constants.h"
#include "nRF2401.h"
#include <timers.h>
#include <math.h>
#include <delays.h>

#define LED		PORTBbits.RB2
#define TRIS_LED	TRISBbits.TRISB2

void HIGH_ISR(void);

unsigned char tx_buf[TX_PLOAD_WIDTH];
volatile unsigned char rx_buf[32];


//========== main =====================================================================
void main(void) {
    nrf_init();

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

    ANCON0 = 0b00000001;

    TRIS_SCK = output;
    TRIS_MISO = input;
    TRIS_MOSI = output;


//    INTCONbits.PEIE = clear;
//    INTCONbits.GIE = clear;
//    T0CON = 0b10001000; //bit3: prescale disable, bits 0-2 prescale.  Timer Period.
//    T0CON = 0b11000110; //bit3: prescale disable, bits 0-2 prescale.  Timer Period.
//    INTCONbits.TMR0IE = set;
//    INTCONbits.TMR0IF = clear;
    //T2CON = 0b00000100; //prescale 1, postscale 1
//
//    PIE1bits.TMR2IE = clear;
//    IPR1bits.TMR2IP = clear;
//    PIR1bits.TMR2IF = clear;

    Delay10KTCYx(50);

    initRX();
    Delay10KTCYx(20);

    //INTCONbits.GIE = set;

    while(1) {
        nrf_Recieve(&rx_buf);
        LED = rx_buf[0];
        Delay10KTCYx(20);
    }

}
//
////====== high interrupt service address =======================================
//#pragma code high_vector=0x08
//void INT_AT_HIGH_VECTOR(void) {
//    _asm GOTO HIGH_ISR _endasm
//}
//#pragma code
//
////====== high interrupt service routine =======================================
//#pragma interrupt HIGH_ISR
//void HIGH_ISR(void) {
//    nrf_Recieve(&rx_buf);
//    LED = 1;
//    //LED = ~rx_buf[0];
//
//    INTCONbits.TMR0IF = clear;
//}
//
//
