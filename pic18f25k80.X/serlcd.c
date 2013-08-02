#include <p18F25K80.h>
#include "serlcd.h"

//const int SPBRG_value = ((FOSC/LCD_BAUD)/16)-1;
const int SPBRG_value = ((FOSC/LCD_BAUD)/64)-1;

void setupLCD(void) {
    TRISCbits.TRISC6 = 1;
    RCSTA1bits.SPEN = 1;
    TXSTA1bits.TXEN = 1;

    TXSTA1bits.SYNC = 0;
    BAUDCON1bits.BRG16 = 0;
    TXSTA1bits.BRGH = 0;
    
    SPBRG1 = SPBRG_value;
}

void sendSpecialCommand(char byte) {
    sendByte(0x7C); //control character
    sendByte(byte);
}

void sendCommand(char byte) {
    sendByte(0xFE); //control character
    sendByte(byte);
}

void setPosition(char row, char column) {
    sendCommand(0x80 + 64*row + column);
}

void setBacklight(char brightness) {
    sendSpecialCommand(128+brightness);
}

void clear() {
    sendCommand(0x01);
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