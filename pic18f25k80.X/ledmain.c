#include "config.h"
#include "constants.h"
#include <p18F25K80.h>
#include <delays.h>

#define STRIP_DATA_TRIS TRISBbits.TRISB0
#define STRIP_DATA PORTBbits.RB0

void delay(void);
void wait(void);
void highFor(char cycles);
void reset(void);
void one(void);
void zero(void);
void Delay1TCYx(char n);

//#define oneD() STRIP_DATA = SET; Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); STRIP_DATA = CLEAR; Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop(); Nop();
//#define zeroD() STRIP_DATA = SET; Nop();Nop();Nop();Nop();Nop();Nop();Nop(); STRIP_DATA = CLEAR; Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();Nop();
#define oneD() STRIP_DATA = SET; Nop();Nop();Nop();Nop(); STRIP_DATA = CLEAR; Nop();Nop();Nop();Nop();
#define zeroD() STRIP_DATA = SET; Nop();Nop(); STRIP_DATA = CLEAR; Nop();Nop();Nop();Nop();Nop();Nop();

int adjust = 0;
void main(void) {
    int i;
    STRIP_DATA_TRIS = OUTPUT;
    OSCCONbits.IRCF = 0b111; //sets internal osc to 16Mhz
    OSCCONbits.SCS = 0b11;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    OSCTUNEbits.PLLEN = 0b0;

    
    

    while(1) {
        _asm
            CALL asm_reset,1

            CALL asm_one,1 //2
            CALL asm_zero,1 //2
            CALL asm_zero,1 //2
            CALL asm_zero,1 //2
            CALL asm_zero,1 //2
            CALL asm_zero,1 //2
            CALL asm_zero,1 //2
            CALL asm_zero,1 //2

            CALL asm_one,1 //2
            CALL asm_zero,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2

            CALL asm_one,1 //2
            CALL asm_zero,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2
            CALL asm_one,1 //2

            GOTO skip

        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 66 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true

            RETURN 1 //2

        asm_zero:
            BSF PORTB, 0, ACCESS //1
            NOP
            BCF PORTB, 0, ACCESS //1
            NOP
            NOP
            NOP                        
            RETURN 1 //2

        asm_one:
            BSF PORTB, 0, ACCESS //1
            NOP
            NOP
            NOP
            NOP
            BCF PORTB, 0, ACCESS //1
            RETURN 1 //2

        skip:

        _endasm

        STRIP_DATA = SET;
        delay();
        STRIP_DATA = CLEAR;
        delay();
    }
}

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