#include "config.h"
#include "constants.h"
#include <p18F25K80.h>
#include <delays.h>

#define STATUS_TRIS TRISBbits.TRISB1
#define STATUS_LED PORTBbits.RB1



void delay(void);
void sendByte(char byte);
void sendCommand(char byte);
void sendLiteralBytes(rom const char * bytes);
void sendBytes(char * bytes);


void main(void) {
    char bar[] = "bar";
    //setup processor speed
    //setup 32mhz
    OSCCONbits.IRCF = 0b110; //sets internal osc to 16Mhz
    OSCCONbits.SCS = 0b00;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    OSCTUNEbits.PLLEN = 0b1;

    INTCON2bits.RBPU = 0b0;




    STATUS_TRIS = OUTPUT;
    STATUS_LED = CLEAR;

    while(1) {
        Delay10KTCYx(254);
        STATUS_LED = SET;
        sendCommand(0x01);
        sendCommand(128);
        sendLiteralBytes("Hello World!");
    }
}



void delay(void) {
    Delay10KTCYx(254);
}
