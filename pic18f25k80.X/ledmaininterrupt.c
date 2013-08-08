#include "config.h"
#include "constants.h"
#include <delays.h>
#include "serlcd.h"

#define STRIP_DATA_TRIS TRISBbits.TRISB0
#define STRIP_DATA PORTBbits.RB0

#define STATUS_TRIS TRISCbits.TRISC0
#define STATUS_LED PORTCbits.RC0

void delay(void);

extern void setLEDs(void);
extern void sendBatch(char * ptr, char len);
extern void sendReset(void);

#define STRIP_LENGTH 125
#define DATA_SIZE 375

#define GREEN 0
#define RED 0
#define BLUE 20

#pragma idata large_idata
    char led_buffer[375] = {GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE,GREEN,RED,BLUE};
#pragma idata

#define ONE_HIGH	243
#define ONE_LOW     243
#define ZERO_HIGH	255
#define ZERO_LOW	237

void main(void) {
    char val;
    char i;

    //ledPointer = led_buffer;
    //rainbowPointer = rainbow;
    
    STRIP_DATA_TRIS = OUTPUT;
    STATUS_TRIS = OUTPUT;
    STATUS_LED = 0;

    //setup 16mhz
    //OSCCONbits.IRCF = 0b111; //sets internal osc to 16Mhz
    //OSCCONbits.SCS = 0b11;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    //OSCTUNEbits.PLLEN = 0b0;

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

    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    //INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;


    //setupLCD();
    //sendCommand(0x01);
    _asm
    _endasm

    while(1) {
        STATUS_LED = 1;

        delay();
        delay();
        delay();

        _asm
                MOVLW 72 //1
            loop:
                BSF PORTB, 0, ACCESS
                NOP
                NOP
                NOP
                BCF PORTB, 0, ACCESS
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP

                ADDLW -1 //1

                BNZ loop //1 if false, 2 if true
        _endasm

        sendReset();
        STATUS_LED = 0;

        delay();
        delay();
        delay();
    }

    while(1) {
        Nop();
    }

//Junk code
        //fill remaining bits in 3 byte chunk (with 0s)
        _asm
                MOVLW 16 //1
            loop:
                BSF PORTB, 0, ACCESS
                NOP
                NOP
                NOP
                BCF PORTB, 0, ACCESS
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP
                NOP

                ADDLW -1 //1

                BNZ loop //1 if false, 2 if true
        _endasm
}

void setLEDs() {
    //sendReset();
    //sendBatch(&led_buffer,STRIP_LENGTH);
    sendBatch(&led_buffer,STRIP_LENGTH);
    //sendReset();
}

void sendReset() {
    _asm
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

void sendBatch(char * ptr, char len) {
    _asm
            BSF INTCON, 2, ACCESS //Set the interrupt flag so that we pass through the initial wait loop without waiting

            //load the initial memory address and populate the shifting register
            LFSR 0,led_buffer //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1

        startStrip:
            //CURRENT LED
            MOVLW  10//1
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

            MOVLW 230
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

    _endasm

    sendReset();
}

#pragma code VECTOR_HIGH = 0x08
#pragma interrupt high_isr
void high_isr (void) {
    INTCONbits.GIE = 0;
    INTCONbits.TMR0IF = 0;

    
    STATUS_LED = 1;


    INTCONbits.GIE = 1;
}

void delay(void) {
    Delay10KTCYx(254);
}
