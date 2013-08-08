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

#define GREEN 255
#define RED 255
#define BLUE 255

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
        _asm
                BSF INTCON, 2, ACCESS //Set the interrupt flag so that we pass through the initial wait loop without waiting

                //load the initial memory address and populate the shifting register
                LFSR 0,led_buffer //1
                MOVF INDF0, 0, ACCESS //1
                MOVWF RXB1D7, ACCESS //1


                //load a new byte into memory
                //INCF FSR0, 1, ACCESS //1
                //MOVF INDF0, 0, ACCESS //1

                MOVLW 255
                MOVWF RXB1D7, ACCESS //1


            startStrip:
                //CURRENT BYTE
                MOVLW  3//1
                MOVWF RXB1D4, ACCESS //1

            //newLED:
                //CURRENT COLOR COMPONENT
                //MOVLW  3//1
                //MOVWF RXB1D5, ACCESS //1

            newByte:
                //CURRENT BIT
                MOVLW  8//1
                MOVWF RXB1D6, ACCESS //1

            loop:

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

                BRA transmitOne //1 or 2

            transmitZero:
                BCF PORTB, 0, ACCESS ///////////////////////////////////////////////// CLEAR

                //do stuff
                // we have about 10 cycles free here
                //do stuff

                //Decrement current bit, jump if nonzero
                DECF RXB1D6, 1, ACCESS //1
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

                //Decrement current byte, jump if nonzero
                //DECF RXB1D4, 1, ACCESS //1
                //BNZ newByte

            done:

        _endasm

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

        sendReset();
        STATUS_LED = 1;

        delay();
        delay();
        delay();
        delay();
        delay();

        _asm
                MOVLW 24 //1
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
        delay();
        delay();
    }

    while(1) {
        Nop();
    }
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
            //loop over elements

            //send 3 leds worth of data
            MOVLW  47//1
            MOVWF RXB1D4, ACCESS //1
            LFSR 0,led_buffer //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1

            //set up color iteration
            MOVLW 3 //1
            MOVWF RXB1D5, ACCESS //1
            
        restartLoopOneNop:
            NOP

        dataLoadedSendBits:
            MOVLW 6 //1
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

        carryBitClearContinue:
        
            //decrement bit counter
            DECF RXB1D6, 1, ACCESS //1
            BZ done //1 if false, 2 otherwise
            BRA sendBitsLoop //2
            //BRA done //2

        carryBitSet:
            //Transmit a one (high 5, low 5)

            //decrement bit counter
            DECF RXB1D6, 1, ACCESS //1
            NOP
            BCF PORTB, 0, ACCESS //1
            NOP
            BNZ sendBitsLoop //1 if false, 2 if true

        done:
            NOP
        
            //## SEND PENULTIMATE BIT##
            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
            BC carryBitSetPenultimate //1 or 2
        carryBitClearPenultimate:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1  (start bit7p2)

            //set overflow bit accordingly
            RLCF RXB1D7, 1, 0 //1

            //we have 3 spare cycles here, lets use these to load the next byte into memory
            INCF FSR0, 1, ACCESS //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1
            
            //restart counter at 6
            MOVLW 6 //1
            MOVWF RXB1D6, ACCESS //1

            NOP
        
            BSF PORTB, 0, ACCESS //1 (start bit8p1)
            BC carryBitSetFinal //1 or 2
        
            //copied and pasted from below
            BCF PORTB, 0, ACCESS //1 (start bit8p1)

            //decrement color index and branch if not zero
            DECF RXB1D5, 1, ACCESS
            BNZ restartLoopOneNop //1 or 2 (we dont actually need to load new data.. we did already, we need to burn cycles)
            //BNZ dataLoadedSendBits //2

            //Decrement the current LED
            DECF RXB1D4, 1, ACCESS //1
            BZ doneFinal //1 or 2

            //reset color index
            MOVLW 3 //1
            MOVWF RXB1D5, ACCESS //1

            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1 (start bit0p1)

            BC carryBitSet //1 or 2

            BCF PORTB, 0, ACCESS //1 (start bit0p2)
            BRA carryBitClearContinue
            //copied and pasted from below



        carryBitSetPenultimate:
            //Transmit a one (high 5, low 5)
        
            //restart counter at 6
            MOVLW 6 //1
            MOVWF RXB1D6, ACCESS //1

            BCF PORTB, 0, ACCESS //1

            //we have 3 spare cycles here, lets use these to load the next byte into memory
            INCF FSR0, 1, ACCESS //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1
            
        donePenultimate:
        
            //## SEND FINAL BIT ##
            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
            BC carryBitSetFinal //1 or 2
        
        // THIS SECTION IS EXECUTED IF THE LAST BIT IN THE SET IS 0
        carryBitClearFinal:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1

            //decrement color index and branch if not zero
            DECF RXB1D5, 1, ACCESS
            BNZ restartLoopOneNop //1 or 2 (we dont actually need to load new data.. we did already, we need to burn cycles)
            //BNZ dataLoadedSendBits //2

            //Decrement the current LED
            DECF RXB1D4, 1, ACCESS //1
            BZ doneFinal //1 or 2

            //reset color index
            MOVLW 3 //1
            MOVWF RXB1D5, ACCESS //1

            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
        
            BC carryBitSet //1 or 2
        carryBitClearFinalNext:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1

            //reset counter to 5 so we can skip decrementing it
            MOVLW 5 //1
            MOVWF RXB1D6, ACCESS //1

            NOP
            NOP

            BRA sendBitsLoop //2


        // THIS SECTION IS EXECUTED IF THE LAST BIT IN THE SET IS 1
        carryBitSetFinal:
            //Transmit a one (high 5, low 5)

            NOP
            //NOP

            //decrement color index and branch if not zero (this is straddling the BCF to set the output low)
            DECF RXB1D5, 1, ACCESS
        
            //OUTPUT LOW
            BCF PORTB, 0, ACCESS //1
            BNZ sendBitsLoop //1 or 2 (we dont actually need to load new data.. we did already, we need to burn cycles)

            //Decrement the current LED
            DECF RXB1D4, 1, ACCESS //1
            BZ doneFinal //1 or 2

            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1

            BC carryBitSet //1 or 2
        carryClearFinalNext2:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1
            BRA carryBitClearContinue

        doneFinal:

        

        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true

        _endasm
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
