#include "config.h"
#include "constants.h"
#include <p18F25K80.h>
#include <delays.h>

#define STRIP_DATA_TRIS TRISBbits.TRISB0
#define STRIP_DATA PORTBbits.RB0

#define STATUS_TRIS TRISBbits.TRISB1
#define STATUS_LED PORTBbits.RB1

void delay(void);
void wait(void);
void highFor(char cycles);
void reset(void);
void one(void);
void zero(void);
void Delay1TCYx(char n);

char led_buffer[6] = {10,255,255,0,255,255};

int adjust = 0;
void main(void) {
    int i = 0;
    int a;
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

        _asm
            CALL asm_reset,1

            //loop counter
            MOVLW 8 //1
            MOVWF RXB1D6, ACCESS //1
            
            //one: high 5, low 5
            //zero: high 2, low 8
        load_new_data:
            LFSR 0,led_buffer //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1
        fill_loop:
            //BCF STATUS, 0, ACCESS //1
            RLCF RXB1D7, 1, 0 //1
        
            //start
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
            BNZ fill_loop //1 if false, 2 if true
            GOTO done //2

        carryBitSet:
            //Transmit a one (high 5, low 5)
            DECF RXB1D6, 1, ACCESS //1
            NOP
            NOP
            NOP //extra nop
            BCF PORTB, 0, ACCESS //1
            BNZ fill_loop //1 if false, 2 if true
            GOTO done //2

        done:

            MOVLW 16
        sendRemainingBits:
            BSF PORTB, 0, ACCESS //1
            NOP
            BCF PORTB, 0, ACCESS //1
            NOP
            NOP
            NOP
            NOP
            NOP
            ADDLW -1 //1
            BNZ sendRemainingBits //1 if false, 2 if true

            CALL asm_reset,1

            


        
        GOTO skipSubroutines
        //################### ASM RESET ##############
        // sents a reset to the LED strip
        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true

            RETURN 1 //2


        // ############### CLEAR 10 ################
        // clears the first 10 LEDs to off
        clear10:
            CALL asm_reset,1

            //CLEAR BITS
            MOVLW 240 //1
        loop:
            BSF PORTB, 0, ACCESS //1
            NOP
            BCF PORTB, 0, ACCESS //1
            NOP
            NOP
            NOP
            NOP
            NOP
            NOP

            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true

            CALL asm_reset,1

            RETURN 1



        skipSubroutines:

        _endasm
//
//        STRIP_DATA = SET;
//        delay();
//        STRIP_DATA = CLEAR;
//        delay();
    //}
}




//            CALL asm_reset,1
//
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//
//            CALL asm_one,1 //2
//            CALL asm_one,1 //2
//            CALL asm_one,1 //2
//            CALL asm_one,1 //2
//            CALL asm_one,1 //2
//            CALL asm_one,1 //2
//            CALL asm_one,1 //2
//            CALL asm_one,1 //2
//
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2
//            CALL asm_zero,1 //2

void delay(void) {
    Delay10KTCYx(254);
}

void Delay1TCYx(char n) {
    while(--n>0) {
        Delay1TCY();
    }
}

void reset(void) {
    STRIP_DATA = CLEAR;
    Delay100TCYx(2);
}

void highFor(char cycles) {
    STRIP_DATA = SET;
    Delay1TCYx(cycles);
    STRIP_DATA = CLEAR;
    Delay1TCYx(80-cycles);

}

// (.6 / 10**6) / (1/64000000.0) = 38
void one(void) {
    highFor(38);
}

// (.25 / 10**6) / (1/64000000.0) = 16
void zero(void) {
    highFor(16);
}