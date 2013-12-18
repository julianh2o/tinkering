#include "config.h"
#include "constants.h"
#include <delays.h>
#include "serlcd.h"

#define STRIP_DATA_TRIS TRISBbits.TRISB0
#define STRIP_DATA PORTBbits.RB0

#define STATUS_TRIS TRISCbits.TRISC0
#define STATUS_LED PORTCbits.RC0

void delay(void);

extern void updateLEDs(void);

#define STRIP_LENGTH 125
#define DATA_SIZE 375

#define GREEN 0
#define RED 20
#define BLUE 20

#pragma idata large_idata
char led_buffer[375] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#pragma idata

void main(void) {
    char val;
    char i,offset,i_rainbow,i_strip;
    char g,r,b;
    
    STRIP_DATA_TRIS = OUTPUT;
    STATUS_TRIS = OUTPUT;
    STATUS_LED = 0;

    //setup internal oscillator
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
//    while(1) {
//    STATUS_LED = 1;
//    _asm
//            MOVLW  72
//            MOVWF RXB1D4, ACCESS
//
//        sendLoop:
//            BSF PORTB, 0, ACCESS ///////////////////////////////////////////////// SET
//            NOP
//            NOP
//            NOP
//            BCF PORTB, 0, ACCESS ///////////////////////////////////////////////// CLEAR
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//
//            DECF RXB1D4, 1, ACCESS //1
//            BNZ sendLoop
//
//
//
//            BCF PORTB, 0, ACCESS //1
//
//   //         MOVLW 135 //1
//   //     resetloop:
//   //         ADDLW -1 //1
//   //         BNZ resetloop //1 if false, 2 if true
//
//
//    _endasm
//    STATUS_LED = 0;
//    delay();
//    delay();
//
//
//    }

    offset = 0;
    while(1) {
        i_rainbow = offset;
        for (i_strip=0; i_strip<STRIP_LENGTH; i_strip++) {
            led_buffer[i_strip*3] = rainbow[i_rainbow*3];
            led_buffer[i_strip*3+1] = rainbow[i_rainbow*3+1];
            led_buffer[i_strip*3+2] = rainbow[i_rainbow*3+2];
            i_rainbow++;
            if (i_rainbow > STRIP_LENGTH) i_rainbow=0;

        }
        updateLEDs();
        offset ++;
        if (offset > STRIP_LENGTH) {
            offset = 0;
        }
    }

//
//    while(1) {
//        i=0;
//        while (i<42) {
//            g = 0;
//            r = 0;
//            b = 10;
//
//            led_buffer[i*3] = 0;//g
//            led_buffer[i*3+1] = 0;//r
//            led_buffer[i*3+2] = 10;//b
//
//            i++;
//        }
//        led_buffer[i*3] = 0b1111;//g
//        led_buffer[i*3+1] = 0;//r
//        led_buffer[i*3+2] = 0;//b
//        i++;
//        led_buffer[i*3] = 0;//g
//        led_buffer[i*3+1] = 0b111;//r
//        led_buffer[i*3+2] = 0;//b
//        //i = 3;
//        //led_buffer[i*3] = 0;//g
//        //led_buffer[i*3+1] = 0;//r
//        //led_buffer[i*3+2] = 10;//b
//        //led_buffer[2] = 10;
//        STATUS_LED = 1;
//        updateLEDs();
//        STATUS_LED = 0;
//
//        delay();
//        delay();
//        delay();

//        for (i=0; i<125; i++) {
//            g = r = b = 0;
//            if (i%2 == 1) {
//                r = 10;
//            } else {
//                b = 10;
//            }
//
//            led_buffer[i*3] = g;//g
//            led_buffer[i*3+1] = r;//r
//            led_buffer[i*3+2] = b;//b
//        }
//
//        updateLEDs();
//
//        delay();
//        delay();
//        delay();
    }
}

void updateLEDs() {
    _asm

            BSF INTCON, 2, ACCESS //Set the interrupt flag so that we pass through the initial wait loop without waiting

            //load the initial memory address and populate the shifting register
            LFSR 0,led_buffer //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1

        startStrip:
            //CURRENT LED
            MOVLW  STRIP_LENGTH//1
            //MOVLW  40//1
            //MOVF len, 0, ACCESS
            MOVWF RXB1D4, ACCESS //1

            //CURRENT COLOR COMPONENT
            MOVLW  3//1
            MOVWF RXB1D5, ACCESS //1

            //CURRENT BIT
            MOVLW  8//1
            MOVWF RXB1D6, ACCESS //1

        timerWaitLoop2:
            BTFSS INTCON, 2, ACCESS //1, 2 or 3
            BRA timerWaitLoop2

            BSF PORTB, 0, ACCESS ///////////////////////////////////////////////// SET

            MOVLW 242
            MOVWF TMR0L, ACCESS

            //clear timer overflow (timer trips again in 20 cycles)
            BCF INTCON, 2, ACCESS //1

            //bit shift and set overflow flag
            RLCF RXB1D7, 1, 0 //1

            BC transmitOne //1 or 2

        transmitZero:
            BCF PORTB, 0, ACCESS ///////////////////////////////////////////////// CLEAR

            //do stuff
            // we have about 10 cycles free here
            //do stuff

            //Decrement current bit, jump if nonzero
            DECF RXB1D6, 1, ACCESS //1
            BNZ timerWaitLoop2

            //load a new byte into memory
            INCF FSR0, 1, ACCESS //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1

            //CURRENT BIT
            MOVLW  8//1
            MOVWF RXB1D6, ACCESS //1

            //Decrement component count
            DECF RXB1D5, 1, ACCESS //1
            BNZ timerWaitLoop2

            MOVLW  3//1
            MOVWF RXB1D5, ACCESS //1

            //Decrement LED count
            DECF RXB1D4, 1, ACCESS //1
            BNZ timerWaitLoop2

            BRA done
        transmitOne:
            //we have a maximum of 5 cycles here
            //count NOPs
            NOP
            NOP
            NOP
            NOP
            NOP

            BCF PORTB, 0, ACCESS ///////////////////////////////////////////////// CLEAR

            // 8 cycles before timerWaitLoop

            //Decrement current bit, jump if nonzero
            DECF RXB1D6, 1, ACCESS //1
            BNZ timerWaitLoop2

            //load a new byte into memory
            INCF FSR0, 1, ACCESS //1
            BNOV skipIncrement
            INCF BSR, 1, ACCESS //TODO work on me
        skipIncrement:
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1

            //CURRENT BIT
            MOVLW  8//1
            MOVWF RXB1D6, ACCESS //1

            //decrement component count
            DECF RXB1D5, 1, ACCESS //1
            BNZ timerWaitLoop2

            MOVLW  3//1
            MOVWF RXB1D5, ACCESS //1

            //decrement LED count
            DECF RXB1D4, 1, ACCESS //1
            BNZ timerWaitLoop2

        done:






//            BSF INTCON, 2, ACCESS //Set the interrupt flag so that we pass through the initial wait loop without waiting
//
//            //load the initial memory address and populate the shifting register
//            LFSR 0,led_buffer //1
//            MOVF INDF0, 0, ACCESS //1
//            MOVWF RXB1D7, ACCESS //1
//
//        startStripB:
//            //CURRENT LED
//            MOVLW  STRIP_LENGTH//1
//            MOVLW  40//1
//            //MOVF len, 0, ACCESS
//            MOVWF RXB1D4, ACCESS //1
//
//            //CURRENT COLOR COMPONENT
//            MOVLW  3//1
//            MOVWF RXB1D5, ACCESS //1
//
//            //CURRENT BIT
//            MOVLW  8//1
//            MOVWF RXB1D6, ACCESS //1
//
//        timerWaitLoop2B:
//            BTFSS INTCON, 2, ACCESS //1, 2 or 3
//            BRA timerWaitLoop2B
//
//            BSF PORTB, 0, ACCESS ///////////////////////////////////////////////// SET
//
//            MOVLW 242
//            MOVWF TMR0L, ACCESS
//
//            //clear timer overflow (timer trips again in 20 cycles)
//            BCF INTCON, 2, ACCESS //1
//
//            //bit shift and set overflow flag
//            RLCF RXB1D7, 1, 0 //1
//
//            BC transmitOneB //1 or 2
//
//        transmitZeroB:
//            BCF PORTB, 0, ACCESS ///////////////////////////////////////////////// CLEAR
//
//            //do stuff
//            // we have about 10 cycles free here
//            //do stuff
//
//            //Decrement current bit, jump if nonzero
//            DECF RXB1D6, 1, ACCESS //1
//            BNZ timerWaitLoop2B
//
//            //load a new byte into memory
//            INCF FSR0, 1, ACCESS //1
//            MOVF INDF0, 0, ACCESS //1
//            MOVWF RXB1D7, ACCESS //1
//
//            //CURRENT BIT
//            MOVLW  8//1
//            MOVWF RXB1D6, ACCESS //1
//
//            //Decrement component count
//            DECF RXB1D5, 1, ACCESS //1
//            BNZ timerWaitLoop2B
//
//            MOVLW  3//1
//            MOVWF RXB1D5, ACCESS //1
//
//            //Decrement LED count
//            DECF RXB1D4, 1, ACCESS //1
//            BNZ timerWaitLoop2B
//
//            BRA doneB
//        transmitOneB:
//            //we have a maximum of 5 cycles here
//            //count NOPs
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//
//            BCF PORTB, 0, ACCESS ///////////////////////////////////////////////// CLEAR
//
//            // 8 cycles before timerWaitLoop
//
//            //Decrement current bit, jump if nonzero
//            DECF RXB1D6, 1, ACCESS //1
//            BNZ timerWaitLoop2B
//
//            //load a new byte into memory
//            INCF FSR0, 1, ACCESS //1
//            MOVF INDF0, 0, ACCESS //1
//            MOVWF RXB1D7, ACCESS //1
//
//            //CURRENT BIT
//            MOVLW  8//1
//            MOVWF RXB1D6, ACCESS //1
//
//            //decrement component count
//            DECF RXB1D5, 1, ACCESS //1
//            BNZ timerWaitLoop2B
//
//            MOVLW  3//1
//            MOVWF RXB1D5, ACCESS //1
//
//            //decrement LED count
//            DECF RXB1D4, 1, ACCESS //1
//            BNZ timerWaitLoop2B
//
//        doneB:







        
        //################### ASM RESET ##############
        // sents a reset to the LED strip
        // a reset is a low for t > 50 microseconds
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
