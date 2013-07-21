#include "config.h"
#include "constants.h"
#include <p18F25K80.h>
#include <delays.h>

#define STRIP_DATA_TRIS TRISBbits.TRISB0
#define STRIP_DATA PORTBbits.RB0

#define STATUS_TRIS TRISBbits.TRISB1
#define STATUS_LED PORTBbits.RB1

void delay(void);

void setLEDs();
void sendBatch(char * ptr, char len);
void sendReset();

#define DATA_SIZE_1 255

//RAINBOW
const char led_buffer[DATA_SIZE_1] = {0,15,0,0,15,0,1,15,0,2,15,0,3,15,0,3,15,0,4,15,0,5,15,0,6,15,0,6,15,0,7,15,0,8,15,0,9,15,0,9,15,0,10,15,0,11,15,0,12,15,0,13,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,12,0,15,11,0,15,11,0,15,10,0,15,9,0,15,8,0,15,8,0,15,7,0,15,6,0,15,5,0,15,5,0,15,4,0,15,3,0,15,2,0,15,2,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,1,15,0,2,15,0,3,15,0,4,15,0,4,15,0,5,15,0,6,15,0,7,15,0,7,15,0,8,15,0,9,15,0,10,15,0,10,15,0,11,15,0,12,15,0,13,15,0,14,15,0,14,15,0,15,15,0,15,14,0,15,14,0,15,13,0,15,12,0,15,11,0,15,10,0,15,10,0,15,9,0,15,8,0,15,7,0,15,7,0,15,6,0,15,5,0,15,4,0,15,4,0,15,3,0,15,2,0,15,1,0,15,1,0,15,0,0,15,0,0,15};
//const char led_buffer[DATA_SIZE_1+1] = {0,15,0,0,15,0,1,15,0,2,15,0,3,15,0,3,15,0,4,15,0,5,15,0,6,15,0,6,15,0,7,15,0,8,15,0,9,15,0,9,15,0,10,15,0,11,15,0,12,15,0,13,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,12,0,15,11,0,15,11,0,15,10,0,15,9,0,15,8,0,15,8,0,15,7,0,15,6,0,15,5,0,15,5,0,15,4,0,15,3,0,15,2,0,15,2,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,1,15,0,2,15,0,3,15,0,4,15,0,4,15,0,5,15,0,6,15,0,7,15,0,7,15,0,8,15,0,9,15,0,10,15,0,10,15,0,11,15,0,12,15,0,13,15,0,14,15,0,14,15,0,15,15,0,15,14,0,15,14,0,15,13,0,15,12,0,15,11,0,15,10,0,15,10,0,15,9,0,15,8,0,15,7,0,15,7,0,15,6,0,15,5,0,15,4,0,15,4,0,15,3,0,15,2,0,15,1,0,15,1,0,15,0,0,15,0,0,15,0};

//BLUE EVERY 10
//const char led_buffer[DATA_SIZE_1] = {0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0};



void main(void) {
    STRIP_DATA_TRIS = OUTPUT;
    STATUS_TRIS = OUTPUT;
    PORTBbits.RB2 = OUTPUT;

    //setup 16mhz
    //OSCCONbits.IRCF = 0b111; //sets internal osc to 16Mhz
    //OSCCONbits.SCS = 0b11;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    //OSCTUNEbits.PLLEN = 0b0;

    //setup 32mhz
    OSCCONbits.IRCF = 0b110; //sets internal osc to 16Mhz
    OSCCONbits.SCS = 0b00;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    OSCTUNEbits.PLLEN = 0b1;

    INTCON2bits.RBPU = 0b0;

    setLEDs();
}

void setLEDs() {
    sendReset();
    sendBatch(&led_buffer,DATA_SIZE_1);
    //sendBatch(&led_buffer2,DATA_SIZE_2);
    sendReset();
}

void sendReset() {
    _asm
        //################### ASM RESET ##############
        // sents a reset to the LED strip
        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true

    _endasm
}

void sendBatch(char * ptr, char len) {
        _asm

        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true



            //loop over elements
//            MOVLW  //1
//            MOVWF RXB1D4, ACCESS //1
            LFSR 0,led_buffer //1

        nextLED:
            //loop over colors
            MOVLW 255 //1
            MOVWF RXB1D5, ACCESS //1
            
            //one: high 5, low 5
            //zero: high 2, low 8
        loadNewData:
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1

        dataLoadedSendBits:
            MOVLW 7 //1
            MOVWF RXB1D6, ACCESS //1

        sendBitsLoop:
        //## SEND BIT##
            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
            BC carryBitSet //1 or 2
        carryBitClear:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1
            NOP
            NOP
            NOP
            NOP //extra nop
            DECF RXB1D6, 1, ACCESS //1
            BNZ sendBitsLoop //1 if false, 2 if true
            GOTO done //2

        carryBitSet:
            //Transmit a one (high 5, low 5)
            DECF RXB1D6, 1, ACCESS //1
            NOP
            NOP
            NOP //extra nop
            BCF PORTB, 0, ACCESS //1
            BNZ sendBitsLoop //1 if false, 2 if true
            GOTO done //2

        done:

            //## SEND BIT##
            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
            BC carryBitSetFinal //1 or 2
        carryBitClearFinal:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1
            BZ doneFinal
            INCF FSR0, 1, ACCESS
            DECFSZ RXB1D5, 1, ACCESS
            GOTO loadNewData //2
//            DECFSZ RXB1D4, 1, ACCESS
//            GOTO nextLED
            GOTO doneFinal

        carryBitSetFinal:
            //Transmit a one (high 5, low 5)
            //increment memory address
            INCF FSR0, 1, ACCESS
            //Load next memory location into a register
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1

            //OUTPUT LOW
            BCF PORTB, 0, ACCESS //1
            //restart counter at 7
            MOVLW 7 //1
            MOVWF RXB1D6, ACCESS //1

            DECFSZ RXB1D5, 1, ACCESS
            GOTO sendBitsLoop //2
//            DECFSZ RXB1D4, 1, ACCESS
//            GOTO nextLED
            GOTO doneFinal

        doneFinal:

        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true


        _endasm
}

void delay(void) {
    Delay10KTCYx(254);
}