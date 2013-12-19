#include "config.h"
#include "constants.h"
#include <delays.h>
#include "serlcd.h"

#define STATUS_TRIS TRISCbits.TRISC0
#define STATUS_LED PORTCbits.RC0

void delay(void);

void main(void) {
    char i;

    STATUS_TRIS = OUTPUT;
    STATUS_LED = 0;

    //setup internal oscillator
    OSCCONbits.IRCF = 0b111; //sets internal osc to 111=16mhz, 110=8mhz
    OSCCONbits.SCS = 0b00;
    OSCTUNEbits.PLLEN = 0b0; //1=pllx4 enabled

    setupLCD();
    setBacklight(10);
    clear();
 
    for (i = 0; i<11; i++) {
        setPosition(0,i);
        sendLiteralBytes(" Hello");
        setPosition(1,i);
        sendLiteralBytes(" World");
        delay();
    }
    
    while(1) {
        clear();
        sendLiteralBytes("Hello");
        STATUS_LED = !STATUS_LED;
        delay();
        delay();
        delay();
        clear();
        sendLiteralBytes("World");
        delay();
        delay();
        delay();
    }
}

void delay(void) {
    Delay10KTCYx(100);
}