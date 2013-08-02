#include "config.h"
#include "constants.h"
#include <delays.h>
#include "serlcd.h"

#define STATUS_TRIS TRISCbits.TRISC0
#define STATUS_LED PORTCbits.RC0

void delay(void);

void main(void) {
    char val;
    char i;

    STATUS_TRIS = OUTPUT;
    STATUS_LED = 0;

    //setup internal oscillator
    OSCCONbits.IRCF = 0b111; //sets internal osc to 111=16mhz, 110=8mhz
    OSCCONbits.SCS = 0b00;
    OSCTUNEbits.PLLEN = 0b0; //1=pllx4 enabled

//    setupLCD();
//    sendCommand(0x01);
//    sendBytes("Hello World!");
    
    while(1) {
        STATUS_LED = !STATUS_LED;
        delay();
    }
}

void delay(void) {
    Delay10KTCYx(254);
}