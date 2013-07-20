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

char led_buffer[6] = {150,255,255,0,255,255};

int adjust = 0;
void main(void) {
    int i;
    int a;
    STRIP_DATA_TRIS = OUTPUT;

    //setup 16mhz
    //OSCCONbits.IRCF = 0b111; //sets internal osc to 16Mhz
    //OSCCONbits.SCS = 0b11;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    //OSCTUNEbits.PLLEN = 0b0;

    //setup 32mhz
    OSCCONbits.IRCF = 0b110; //sets internal osc to 16Mhz
    OSCCONbits.SCS = 0b00;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    OSCTUNEbits.PLLEN = 0b1;
    
    
    //while(1) {

//        i = led_buffer[0];
//        a = 5;
//        a = a+i;
//        i = a;
        //i++;
        _asm
            //presetup
    
            CALL asm_reset,1
            GOTO skipClear

            
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

            GOTO done

        skipClear:

            




            //setup
            MOVLW 0
            MOVWF PLUSW2, ACCESS //1

            //loop counter
            MOVLW 24 //1

            

            //move led_buffer to W
            //MOVF led_buffer, 0, ACCESS //1
            
            //one: high 5, low 5
            //zero: high 2, low 8
        fill_loop:
            RLCF PLUSW2, 1, 0 //1
            //DECF PLUSW2, 1, 0 //1
            //BZ done //1 or 2



            //start
            BSF PORTB, 0, ACCESS //1
            BC skipToOne //1 or 2
            BCF PORTB, 0, ACCESS //1
            NOP
            NOP
            NOP
            ADDLW -1 //1
            BNZ fill_loop //1 if false, 2 if true
            GOTO done //2

        skipToOne:
            ADDLW -1 //1
            NOP
            BCF PORTB, 0, ACCESS //1
            NOP
            BNZ fill_loop //1 if false, 2 if true
            GOTO done //2

        done:

            CALL asm_reset,1

            GOTO skip

        asm_send:


        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
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