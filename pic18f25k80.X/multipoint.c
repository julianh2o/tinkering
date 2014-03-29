//For the RX target board, the connection is Gray, White, Orange, Blue, and Black, Red, White, Yellow

#include "p18f25k80.h"
#include "constants.h"
#include "config.h"
#include "serlcd.h"
#include "nRF2401.h"
#include <timers.h>
#include <math.h>
#include <delays.h>

#define LED_RED_TRIS      TRISAbits.TRISA0
#define LED_RED           PORTAbits.RA0

#define LED_GREEN_TRIS    TRISAbits.TRISA1
#define LED_GREEN         PORTAbits.RA1

#define LED_YELLOW_TRIS   TRISAbits.TRISA2
#define LED_YELLOW        PORTAbits.RA2

#define EEPROM_CS_TRIS    TRISBbits.TRISB3
#define EEPROM_CS         PORTBbits.RB3

#define BUTTON_TRIS       TRISBbits.TRISB4
#define BUTTON            PORTBbits.RB4
#define BUTTON_DOWN       0
#define BUTTON_UP         1

#define LED_ON            0
#define LED_OFF           1

#define STRIP_TRIS        TRISCbits.TRISC2
#define STRIP             PORTCbits.RC2

#define STRIP_LENGTH 125
#define DATA_SIZE 375

#pragma idata large_idata
char led_buffer[375] = {10,0,0,0,10,0,0,0,10,10,10,10,0,0,10,0,10,0,10,0,0,10,10,10,0,10,0,10,0,0,0,0,10,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
char rainbow[DATA_SIZE] = {0,255,0,12,255,0,24,255,0,36,255,0,48,255,0,61,255,0,73,255,0,85,255,0,97,255,0,110,255,0,122,255,0,134,255,0,146,255,0,159,255,0,171,255,0,183,255,0,195,255,0,208,255,0,220,255,0,232,255,0,244,255,0,255,252,0,255,240,0,255,228,0,255,216,0,255,204,0,255,191,0,255,179,0,255,167,0,255,155,0,255,142,0,255,130,0,255,118,0,255,106,0,255,93,0,255,81,0,255,69,0,255,57,0,255,44,0,255,32,0,255,20,0,255,8,0,255,0,4,255,0,16,255,0,28,255,0,40,255,0,53,255,0,65,255,0,77,255,0,89,255,0,101,255,0,114,255,0,126,255,0,138,255,0,150,255,0,163,255,0,175,255,0,187,255,0,199,255,0,212,255,0,224,255,0,236,255,0,248,248,0,255,236,0,255,224,0,255,212,0,255,199,0,255,187,0,255,175,0,255,163,0,255,150,0,255,138,0,255,126,0,255,114,0,255,101,0,255,89,0,255,77,0,255,65,0,255,53,0,255,40,0,255,28,0,255,16,0,255,4,0,255,0,8,255,0,20,255,0,32,255,0,44,255,0,57,255,0,69,255,0,81,255,0,93,255,0,106,255,0,118,255,0,130,255,0,142,255,0,155,255,0,167,255,0,179,255,0,191,255,0,203,255,0,216,255,0,228,255,0,240,255,0,252,255,0,255,244,0,255,232,0,255,220,0,255,208,0,255,195,0,255,183,0,255,171,0,255,159,0,255,146,0,255,134,0,255,122,0,255,110,0,255,97,0,255,85,0,255,73,0,255,61,0,255,48,0,255,36,0,255,24,0,255,12};
char other[375] = {10,0,0,0,10,0,0,0,10,10,10,10,0,0,10,0,10,0,10,0,0,10,10,10,0,10,0,10,0,0,0,0,10,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
#pragma idata

unsigned char tx_buf[MAX_PAYLOAD];
unsigned char rx_buf[MAX_PAYLOAD];
char runFlag=0;
int timerCount = 0;
int value;

void setup(void);

////                            MasterCode                                 ////
void masterMain(void);
void masterInterrupt(void);

////                          SlaveCode                                 ////
void slaveMain(void);
void writeSource(char * source, short offset);
void slaveInterrupt(void);

////                            Shared Code                                 ////
void clearStrip(char r, char g, char b);
void setLED(unsigned char n, char r, char g, char b);
void displayStatus(char status);
void delay(void);

////                            System Code                                 ////
void run(void);
void main(void);
void INT_AT_HIGH_VECTOR(void);
void HIGH_ISR(void);

////                            LED Code                                    ////
void updateLEDs(void);

void setup(void) {
    //Misc config
    LED_RED_TRIS = OUTPUT;
    LED_GREEN_TRIS = OUTPUT;
    LED_YELLOW_TRIS = OUTPUT;
    EEPROM_CS_TRIS = OUTPUT;
    STRIP_TRIS = OUTPUT;
    BUTTON_TRIS = INPUT;


    //oscillator setup
    OSCCONbits.IRCF = 0b111; //sets internal osc to 111=16mhz, 110=8mhz
    OSCCONbits.SCS = 0b00;
    OSCTUNEbits.PLLEN = 0b1; //1=pllx4 enabled



    //Enable internal pullup resistor for port B
    INTCON2bits.RBPU = CLEAR;
    WPUB = 0b11111111;

    //This is to toggle pins from digital to analog
    //unimp, RD3, RD2, RD1     RD1, AN10, AN9, AN8 (in order)
    //ANCON0 = 0b00000000;
    //ANCON1 = 0b11111000;
    ANCON0 = 0x00;
    ANCON1 = 0x00;

    //NRF port configure (todo: move me)
    TRIS_CE = OUTPUT;
    TRIS_CSN = OUTPUT;
    TRIS_IRQ = INPUT;
    TRIS_SCK = OUTPUT;
    TRIS_MISO = INPUT;
    TRIS_MOSI = OUTPUT;

    //set up timer for LEDs
    T2CONbits.TMR2ON = 1; //enable timer 2
    T2CONbits.T2CKPS = 0b00; //prescaler 0b10=1/16
    PIE1bits.TMR2IE = 0;
    PR2 = 20;

    //set up timer for interrupt
    T0CONbits.TMR0ON = 1; //enable timer 0
    T0CONbits.T0CS = 0; //select clock (0=internal,1=t0pin)
    T0CONbits.PSA = 1; //disable's prescaler (1=disable, 0=enable)
    T0CONbits.T08BIT = 0; //set mode (1=8bit mode, 0=16bit mode)
    T0CONbits.T0SE = 1; //edge select (1=falling edge, 0=rising edge)
    T0CONbits.T0PS = 0b000; //configure prescaler 000=1:2

    //Set up timer0 interrupts
    INTCONbits.TMR0IE = 1;
    INTCONbits.TMR0IF = 0;
    INTCONbits.PEIE = 0;
    INTCONbits.GIE = 0;


    //set up USB serial port
    TRISCbits.TRISC6 = 1;
    RCSTA1bits.SPEN = 1;
    TXSTA1bits.TXEN = 1;

    TXSTA1bits.SYNC = 0;
    BAUDCON1bits.BRG16 = 0;
    TXSTA1bits.BRGH = 1;

    // 19.2kbaud = 001, 832
    SPBRG1 = 34;
    
    //9.6kbaud = 000, 103
    //SPBRG1 = 103;
    RCSTA1bits.CREN = SET;
}

///////////////////////////////////////////////////////////
////                                                                        ////
////                            Sender Code                                 ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////
void masterMain() {

}

void masterInterrupt(void) {

}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            Receiver Code                               ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////

void processPacket(char * buf) {
    int loc = buf[0];
    int len = buf[1];
    int i;
    int n;

    for (i = 0; i<len; i++) {
        n = loc*3+i;
        if (n > DATA_SIZE) continue;

        led_buffer[n] = buf[2+i];
    }
}

void colorFlash(char r, char g, char b) {
    clearStrip(r,g,b);
    updateLEDs();
    Delay10KTCYx(100);
    clearStrip(0,0,0);
    updateLEDs();
    Delay10KTCYx(25);
    clearStrip(r,g,b);
    updateLEDs();
    Delay10KTCYx(100);
    clearStrip(0,0,0);
    updateLEDs();
}

void confirmFlash() {
    colorFlash(0,100,0);
}

void alertFlash() {
    colorFlash(100,0,0);
}

char detectButton() {
    int i=0;
    int LONG_PRESS = 500;

    while(BUTTON == BUTTON_DOWN) {
        i++;
        if (i > LONG_PRESS) return 2;
        Delay10KTCYx(1);
    }
    if (i > 0) return 1;
    return 0;
}

char interpolateColor(unsigned char a, unsigned char b, int current, int total) {
    int ret;
    int ai;
    int bi;
    ai = 0+a;
    bi = 0+b;
//    sendLiteralBytes("a: ");
//    sendIntDec(ai);
//    sendLiteralBytes("\n");
//    sendLiteralBytes("b: ");
//    sendIntDec(bi);
//    sendLiteralBytes("\n");
//
//    sendLiteralBytes("aa: ");
//    sendDec(a);
//    sendLiteralBytes("\n");
//    sendLiteralBytes("bb: ");
//    sendDec(b);
//    sendLiteralBytes("\n");
    ret = bi-ai;
//    sendLiteralBytes("diff: ");
//    sendIntDec(ret);
//    sendLiteralBytes("\n");
    ret *= current;
//    sendLiteralBytes("muld: ");
//    sendIntDec(ret);
//    sendLiteralBytes("\n");
    ret /= total;
//    sendLiteralBytes("total: ");
//    sendIntDec(total);
//    sendLiteralBytes("\n");
//    sendLiteralBytes("divd: ");
//    sendIntDec(ret);
//    sendLiteralBytes("\n");
    ret += ((int)ai);
//    sendLiteralBytes("final: ");
//    sendIntDec(ret);
//    sendLiteralBytes("\n");
    return ret;
}

#define FIRE_LENGTH 12
//char fire[12] = {231,57,0,231,140,0,231,225,0,255,255,158};
char fire[12] = {231,57,0,231,140,0,231,225,0,231,57,0};
//char fire[12] = {254,0,0,0,0,254,0,254,0,254,254,254};
void doFire(char offset, char total) {
    int i1;
    int i2;
    unsigned char colors;
    unsigned char unitsPerColor;
    unsigned char currentSection;
    unsigned char currentCompletion;
    unsigned char r,g,b;

//    if (offset > 100) {
//        offset = 10;
//    } else {
//        offset = 0;
//    }

    colors = FIRE_LENGTH / 3; //4
    unitsPerColor = total/(colors-1);
    currentSection = offset/unitsPerColor;
    currentCompletion = offset % unitsPerColor;
    i1 = currentSection*3;
    i2 = (currentSection+1)*3;
//    sendLiteralBytes("NEW FRAME\n\n\n");
    r = interpolateColor(fire[i1],fire[i2], currentCompletion, unitsPerColor);
    g = interpolateColor(fire[i1+1],fire[i2+1], currentCompletion, unitsPerColor);
    b = interpolateColor(fire[i1+2],fire[i2+2], currentCompletion, unitsPerColor);

//    sendLiteralBytes("rgb: ");
//    sendIntDec(r);
//    sendLiteralBytes(" ");
//    sendIntDec(g);
//    sendLiteralBytes(" ");
//    sendIntDec(b);
//    sendLiteralBytes("\n");
//    r = fire[i2];
//    g = fire[i2+1];
//    b = fire[i2+2];
    //231,57,0
    //231,140,0
    //231,225,0
    //255,255,158
    clearStrip(r,g,b);
//    clearStrip(0,0,0);
//    setLED(currentCompletion,10,10,10);
//    setLED(2,r,g,b);
}

void slaveMain() {
    //slave
    int i;
    char offset;
    char speed;
    char status;
    char automatic = 0;
    int lastData;
    char button;

    nrf_init();
    delay();

    nrf_rxmode();
    delay();

    offset = 0;
    speed = 0;
    automatic = 6;
    sendLiteralBytes("start\n");
    while(1) {
        LED_RED = BUTTON;
        
        lastData++;
        LED_GREEN = LED_OFF;

        if (nrf_receive(&tx_buf,&rx_buf)) {
            processPacket(&rx_buf);
            updateLEDs();
            LED_GREEN = LED_ON;
            lastData = 0;
            automatic = 0;
            LED_YELLOW = 0;
        }

        if (lastData > 500 && automatic == 0) {
            automatic = 1;
            LED_YELLOW = 1;
        }

        if (automatic) {
            button = detectButton();
            if (button == 1) {
                automatic++;
                if (automatic > 6) automatic = 1;
                confirmFlash();
                delay();
            } else if (button == 2) {
                speed++;
                if (speed >= 3) speed = 0;
                colorFlash(0,0,100);
                delay();
            }

            offset++;
            if (offset >= 125) offset = 0;

            switch(automatic) {
                case 1:
                    i = offset;
                    i = i*3;

                    clearStrip(rainbow[i],rainbow[i+1],rainbow[i+2]);
                break;
                
                case 2:
                    clearStrip(10,10,10);
                break;

                case 3:
                    clearStrip(255,255,255);
                break;

                case 4:
                    i = offset - 63;
                    if (i < 0) i *= -1;
                    i *= 4;
                    clearStrip(i,i,i);
                break;

                case 5:
                    writeSource(&rainbow,offset);
                break;

                case 6:
                    doFire(offset,125);
                break;
            }
            updateLEDs();
            Delay10KTCYx(25*(speed+1));
        }
    }
}

void writeSource(char * source, short offset) {
    short i,i_source;

    i_source = offset;
    for (i=0; i<STRIP_LENGTH; i++) {
        led_buffer[i*3] = source[i_source*3];
        led_buffer[i*3+1] = source[i_source*3+1];
        led_buffer[i*3+2] = source[i_source*3+2];

        i_source++;
        if (i_source >= STRIP_LENGTH) i_source = 0;
    }
}


void slaveInterrupt() {

}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            Shared Code                                 ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////

void clearStrip(char r, char g, char b) {
    char i = 0;
    for (i=0; i<STRIP_LENGTH; i++) {
        setLED(i,r,g,b);
    }
}

void setLED(unsigned char n, char r, char g, char b) {
    int offset = ((int)n)*3;
    led_buffer[offset] = g;
    led_buffer[offset+1] = r;
    led_buffer[offset+2] = b;
}

void displayStatus(char status) {
    sendLiteralBytes("stat:");
    sendBinPad(status);
    sendLiteralBytes("\n");
    
    if (status & 0b1000000) sendLiteralBytes("DR ");
    if (status & 0b100000) sendLiteralBytes("DS ");
    if (status & 0b10000) sendLiteralBytes("RT ");
    if (status & 0b1) sendLiteralBytes("TXF ");
    sendLiteralBytes("\n");
}

void delay(void) {
    Delay10KTCYx(254);
}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            System Code                                 ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////

void run(void) {
    while(1) {
        //if (MODE_SELECT == MODE_SEND) {
        //    masterMain();
        //} else {
        slaveMain();
        //}
    }
}

void main(void) {
    setup();

    run();

    while(1);
}

#pragma code high_vector=0x08
void INT_AT_HIGH_VECTOR(void) {
    _asm GOTO HIGH_ISR _endasm
}
#pragma code

//====== high interrupt service routine =======================================
#pragma interrupt HIGH_ISR
void HIGH_ISR(void) {
    slaveInterrupt();

    INTCONbits.TMR0IF = CLEAR;
}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            LED Code                                    ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////

void updateLEDs() {
    char saveGIE = INTCONbits.GIE;
    INTCONbits.GIE = 0;
    _asm
            BSF PIR1, 1, ACCESS //Set the interrupt flag so that we pass through the initial wait loop without waiting

            //load the initial memory address and populate the shifting register
            LFSR 0,led_buffer //1
            MOVF POSTINC0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1
        startStrip:
            //CURRENT LED
            MOVLW  STRIP_LENGTH//1
            MOVWF RXB1D4, ACCESS //1

            //CURRENT COLOR COMPONENT
            MOVLW  3//1
            MOVWF RXB1D5, ACCESS //1

            //CURRENT BIT
            MOVLW  8//1
            MOVWF RXB1D6, ACCESS //1

        timerWaitLoop2:
            BTFSS PIR1, 1, ACCESS //1, 2 or 3
            BRA timerWaitLoop2

            BSF PORTC, 2, ACCESS ///////////////////////////////////////////////// SET

            //clear timer overflow (timer trips again in 20 cycles)
            BCF PIR1, 1, ACCESS //1

            //bit shift and set carry flag
            RLCF RXB1D7, 1, 0 //1
            BC transmitOne //1 or 2
            //BC transmitOne
            //NOP

        transmitZero:
            BCF PORTC, 2, ACCESS ///////////////////////////////////////////////// CLEAR

            //Decrement current bit, jump if nonzero
            DECF RXB1D6, 1, ACCESS //1
            BNZ timerWaitLoop2

            //load a new byte into memory
            MOVF POSTINC0, 0, ACCESS //1
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

            BCF PORTC, 2, ACCESS ///////////////////////////////////////////////// CLEAR

            //Decrement current bit, jump if nonzero
            DECF RXB1D6, 1, ACCESS //1
            BNZ timerWaitLoop2

            //load a new byte into memory
            MOVF POSTINC0, 0, ACCESS //1
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

        //################### ASM RESET ##############
        // sents a reset to the LED strip
        // a reset is a low for t > 50 microseconds
        asm_reset:
            BCF PORTC, 2, ACCESS /////////////////////////////////////////////////////

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true
    _endasm
    INTCONbits.GIE = saveGIE;
}
