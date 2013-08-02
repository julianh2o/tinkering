#include <p18F25K80.h>
//#include "serlcd.h"

#define FOSC 16000000
//#define FOSC 64000000
#define LCD_BAUD 9600
void setupLCD(void);
void sendByte(char byte);
void sendCommand(char byte);
void sendLiteralBytes(rom const char * bytes);
void sendBytes(char * bytes);

void sendDigit(unsigned char digit);
void sendCharAsBase(unsigned char num, unsigned char base);

void sendDec(unsigned char num);
void sendHex(unsigned char num);
void sendBin(unsigned char num);
////////////////////////////////////

const int SPBRG_value = ((FOSC/LCD_BAUD)/16)-1;
//const int SPBRG_value = ((FOSC/LCD_BAUD)/64)-1;

void setupLCD(void) {
    //setup lcd
    //turn the ports on
    TRISCbits.TRISC6 = 1;
    RCSTA1bits.SPEN = 1;
    TXSTA1bits.TXEN = 1;

    TXSTA1bits.SYNC = 0;
    BAUDCON1bits.BRG16 = 0;
    TXSTA1bits.BRGH = 1 ;
    //TXSTA1bits.BRGH = 0;
    
    SPBRG1 = SPBRG_value;
    //SPBRG1 = 104;
    //setup lcd
}

void sendCommand(char byte) {
    sendByte(0xFE); //control character
    sendByte(byte); //control character
}

void sendByte(char byte) {
    TXREG1 = byte;
    while(!TXSTA1bits.TRMT) Nop();
}

void sendLiteralBytes(rom const char * bytes) {
    while(*bytes) {
        sendByte(*bytes++);
    }
}

void sendDigit(unsigned char digit) {
    if (digit >= 10)
        sendByte(digit + 65);
    else
        sendByte(digit + 48);
}

void sendCharAsBase(unsigned char num, unsigned char base) {
    char quotient;
    char remainder;
    char i = 0;
    char digits[8];
    
    while(1) {
        quotient = num / base;
        remainder = num - quotient*base;
        num = quotient;

        digits[i++] = remainder;

        if (quotient == 0) break;
    }

    while (i != 0) {
        sendDigit(digits[--i]);
    }
}

void sendDec(unsigned char num) {
    sendCharAsBase(num,10);
}

void sendHex(unsigned char num) {
    sendLiteralBytes("0x");
    sendCharAsBase(num,16);
}

void sendBin(unsigned char num) {
    sendLiteralBytes("0b");
    sendCharAsBase(num,2);
}