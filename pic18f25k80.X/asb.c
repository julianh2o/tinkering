//For the RX target board, the connection is Gray, White, Orange, Blue, and Black, Red, White, Yellow

#include "p18f25k80.h"
#include "constants.h"
#include "nRF2401.h"
#include <timers.h>
#include <math.h>
#include <delays.h>

#define LED		PORTCbits.RC0
#define TRIS_LED	TRISCbits.TRISC0

void HIGH_ISR(void);

unsigned char tx_buf[TX_PLOAD_WIDTH];
volatile unsigned char rx_buf[32];


//========== main =====================================================================
void main(void) {
    nrf_init();

    TRIS_CE = output;
    TRIS_CSN = output;
    TRIS_IRQ = input;
    TRIS_SCK = output;
    TRIS_MISO = input;
    TRIS_MOSI = output;

    OSCCONbits.IRCF = 0b111; //sets internal osc to 111=16mhz, 110=8mhz
    OSCCONbits.SCS = 0b00;
    OSCTUNEbits.PLLEN = 0b1; //1=pllx4 enabled

    //set up timer
    T0CONbits.TMR0ON = 1; //enable timer 0
    T0CONbits.T0CS = 0; //select clock (0=internal,1=t0pin)
    T0CONbits.PSA = 1; //disable's prescaler (1=disable, 0=enable)
    T0CONbits.T08BIT = 1; //set mode (1=8bit mode, 0=16bit mode)
    T0CONbits.T0SE = 1; //edge select (1=falling edge, 0=rising edge)
    T0CONbits.T0PS = 0b000; //configure prescaler 000=1:2

    //Set up timer0 interrupts
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.PEIE = 1;

    //Misc port config
    TRIS_LED = output;

    initRX();
    Delay10KTCYx(20);

    while(1) {
        nrf_Recieve(&rx_buf);
        LED = rx_buf[0];
        Delay10KTCYx(20);
    }

}