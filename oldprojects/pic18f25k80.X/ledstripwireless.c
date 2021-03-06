//For the RX target board, the connection is Gray, White, Orange, Blue, and Black, Red, White, Yellow

#include "p18f25k80.h"
#include "constants.h"
#include "config.h"
#include "nRF2401.h"
#include <timers.h>
#include <math.h>
#include <delays.h>

#define LED_GREEN_TRIS TRISBbits.TRISB4
#define LED_GREEN PORTBbits.RB4

#define LED_RED_TRIS TRISBbits.TRISB5
#define LED_RED PORTBbits.RB5

#define STRIP_DATA_TRIS TRISAbits.TRISA0
#define STRIP_DATA PORTAbits.RA0

#define PROBE_TRIS TRISAbits.TRISA1
#define PROBE PORTAbits.RA1

#define STATUS_TRIS TRISBbits.TRISB4
#define STATUS_LED PORTBbits.RB4

#define BUTTON_TRIS TRISBbits.TRISB1
#define BUTTON PORTBbits.RB1

#define MODE_SELECT_TRIS TRISBbits.TRISB0
#define MODE_SELECT PORTBbits.RB0
#define MODE_SEND 1

#define STRIP_LENGTH 125
#define DATA_SIZE 375

#pragma idata large_idata
char led_buffer[375] = {10,0,0,0,10,0,0,0,10,10,10,10,0,0,10,0,10,0,10,0,0,10,10,10,0,10,0,10,0,0,0,0,10,10,10,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//const char source[375] = {255,255,255,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const char source[375] = {0,15,0,0,15,0,1,15,0,2,15,0,3,15,0,3,15,0,4,15,0,5,15,0,6,15,0,6,15,0,7,15,0,8,15,0,9,15,0,9,15,0,10,15,0,11,15,0,12,15,0,13,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,12,0,15,11,0,15,11,0,15,10,0,15,9,0,15,8,0,15,8,0,15,7,0,15,6,0,15,5,0,15,5,0,15,4,0,15,3,0,15,2,0,15,2,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,1,15,0,2,15,0,3,15,0,4,15,0,4,15,0,5,15,0,6,15,0,7,15,0,7,15,0,8,15,0,9,15,0,10,15,0,10,15,0,11,15,0,12,15,0,13,15,0,14,15,0,14,15,0,15,15,0,15,14,0,15,14,0,15,13,0,15,12,0,15,11,0,15,10,0,15,10,0,15,9,0,15,8,0,15,7,0,15,7,0,15,6,0,15,5,0,15,4,0,15,4,0,15,3,0,15,2,0,15,1,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,2,15,0,2,15,0,3,15,0,4,15,0,5,15,0,5,15,0,6,15,0,7,15,0,8,15,0,8,15,0,9,15,0,10,15,0,11,15,0,11,15,0,12,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,13,0,15,12,0,15,11,0,15,10,0,15,9,0,15,9,0,15,8,0,15,7,0,15,6,0,15,6,0,15,5,0,15,4,0,15,3,0,15,3,0,15,2,0,15,1,0,15,0};
#pragma idata

unsigned char tx_buf[MAX_PAYLOAD];
unsigned char rx_buf[MAX_PAYLOAD];
char runFlag=0;
int timerCount = 0;
int value;

void setup(void);

////                            Sender Code                                 ////
void senderMain(void);
void senderInterrupt(void);
void writeSource(short offset);
void doCycle(void);
void doOscillate(void);
void loadFrame(char frame);
void loadPayload(char location, char length);
void sendStrip();

////                          Receiver Code                                 ////
void receiverMain(void);
void receiverInterrupt(void);
void updateBuffer();

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
extern void updateLEDs(void);

void setup(void) {
    //Misc config
    STRIP_DATA_TRIS = OUTPUT;
    STATUS_TRIS = OUTPUT;
    BUTTON_TRIS = INPUT;
    MODE_SELECT_TRIS = INPUT;
    STATUS_LED = 0;
    LED_GREEN_TRIS = OUTPUT;
    LED_RED_TRIS = OUTPUT;
    PROBE_TRIS = OUTPUT;

    //Enable internal pullup resistor for port B
    INTCON2bits.RBPU = CLEAR;
    WPUB = 0b1111;

    //This is to toggle pins from digital to analog
    //unimp, RD3, RD2, RD1     RD1, AN10, AN9, AN8 (in order)
    ANCON0 = 0b00000000;
    ANCON1 = 0b11111000;

    //NRF port configure (todo: move me)
    TRIS_CE = OUTPUT;
    TRIS_CSN = OUTPUT;
    TRIS_IRQ = INPUT;
    TRIS_SCK = OUTPUT;
    TRIS_MISO = INPUT;
    TRIS_MOSI = OUTPUT;

    //oscillator setup
    OSCCONbits.IRCF = 0b111; //sets internal osc to 111=16mhz, 110=8mhz
    OSCCONbits.SCS = 0b00;
    OSCTUNEbits.PLLEN = 0b1; //1=pllx4 enabled

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
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;


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

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            Sender Code                                 ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////
void senderMain() {
    int fixweirdbehavior;
    unsigned char status;
    short i;
    char mode;
    short offset;

    //OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_20_TAD, ADC_CH0 & ADC_INT_OFF, 0b0000);

    nrf_init();
    delay();

    nrf_txmode();
    delay();

    PIE1bits.RC1IE = SET; //enables peripheral interrupts for serial

    INTCONbits.TMR0IE = 0;
    INTCONbits.TMR0IF = 0;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;

    LED_RED = 0;
    LED_GREEN = 0;
//    mode = 0;
//    while(1) {
//        //while (!PIR1bits.RC1IF);
//        //LED_RED = 1;
//        //readSerialIntoBuffer(led_buffer);
//        //LED_RED = 0;
//        //delay();
//
//        mode = !mode;
//        led_buffer[3] = mode ? 25 : 0;
//        led_buffer[4] = mode ? 25 : 0;
//        led_buffer[5] = mode ? 25 : 0;
//        sendStrip();
//        //STATUS_LED = 0;
//    }

    mode = 0;

    offset = 0;
    //LED_GREEN = 0;
    while(1) {
        //LED_RED++;

        offset ++;
        if (offset >= 125) offset = 0;

        //value = readPotentiometer();
        //value = value >> 4;
        //value = value >> 1;
        //if (value > 124) value = 124;

        //offset = value;
        //if (value > offset) offset++;
        //if (value < offset) offset--;

//        if (mode) {
//            clearStrip(0,0,0);
//            setLED(offset,10,10,10);
        //} else {
        for (i = 0; i<125; i++) {
            led_buffer[3*i] =   10*((i+offset) % 3 == 0);
            led_buffer[3*i+1] = 10*((i+offset) % 3 == 1);
            led_buffer[3*i+2] = 10*((i+offset) % 3 == 2);
        }
        //ŚloadPayload
        //writeSource(offset);
        //}

        //Delay10KTCYx(50);

        sendStrip();
        
        //sendStrip();
    }

    //CloseADC();
}

void updateSenderLCD() {
    setupLCD();
    clear();
    sendIntDec(value);
}

char write_index = -1;
char bytes_remaining = -1;
void senderInterrupt(void) {
    char byte = RCREG1;

    if (bytes_remaining == -1) {
        bytes_remaining = byte;
        return;
    }
    bytes_remaining--;
    if (write_index == -1) {
        write_index = 3*byte;
        return;
    }

    led_buffer[write_index++] = byte;
    
    if (bytes_remaining == 0) {
        bytes_remaining = -1;
        write_index = -1;
    }
}

void writeSource(short offset) {
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

void doCycle(void) {
    short offset;

    offset = 0;
    while(1) {
        writeSource(offset);
        updateLEDs();
        Delay10KTCYx(50);
        offset++;
        if (offset >= STRIP_LENGTH) offset = 0;
    }
}

void doOscillate(void) {
    short offset;
    char dir = 1;

    offset = 0;
    while(1) {
        writeSource(offset);
        updateLEDs();
        offset+= dir;
        if (offset > STRIP_LENGTH) {
            offset = STRIP_LENGTH;
            dir = -1;
        } else if (offset < 0) {
            offset = 0;
            dir = 1;
        }
    }
}

int readPotentiometer() {
    ConvertADC();
    while( BusyADC() );

    return ReadADC(); // (0,4096)
}

#define FRAME_SIZE 9
void loadFrame(char frame) {
    loadPayload(frame*FRAME_SIZE,FRAME_SIZE*3);
}

void loadPayload(char location, char length) {
    short i;
    int n;
    tx_buf[0] = location;
    tx_buf[1] = length;
    tx_buf[2] = 0;
    tx_buf[3] = 0;
    tx_buf[4] = 0;
    for (i=0; i<length; i++) {
        n = location*3+i;

        tx_buf[5+i] = led_buffer[n];
    }
}

void sendStrip() {
    short i;
    for (i=5; i<10; i++) {
        loadFrame(i);

        STATUS_LED = nrf_send(&tx_buf, &rx_buf);
    }
}

////////////////////////////////////////////////////////////////////////////////
////                                                                        ////
////                            Receiver Code                               ////
////                                                                        ////
////////////////////////////////////////////////////////////////////////////////

void receiverMain() {
    int i;
    char offset;
    char status;

    //doCycle();
    //doOscillate();

    nrf_init();
    delay();

    nrf_rxmode();
    delay();

    Delay10KTCYx(100);

    offset = 0;
    STATUS_LED = 0;
    while(1) {
        //STATUS_LED = 
        nrf_receive(&rx_buf);

        updateBuffer();
        updateLEDs();
    }
}

void receiverInterrupt() {
//    if (timerCount++ > 100) {
//        STATUS_LED = !STATUS_LED;
//        timerCount = 0;
//    }
}

void updateBuffer() {
    int loc = rx_buf[0];
    int len = rx_buf[1];
    int i;
    int n;

    //0,1,2,3,4 = (status info)
    //0 = multiplier
    //5-29 = data
    for (i = 0; i<len; i++) {
        n = loc*3+i;
        if (n > DATA_SIZE) continue;

        led_buffer[n] = rx_buf[5+i];
    }
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
    setPosition(0,0);
    sendLiteralBytes("stat:");
    sendBinPad(status);
    fillLine();
    
    if (runFlag == 0) {
        setPosition(0,15);
        sendLiteralBytes("_");
    }
    runFlag = !runFlag;
    
    setPosition(1,0);
    if (status & 0b1000000) sendLiteralBytes("DR ");
    if (status & 0b100000) sendLiteralBytes("DS ");
    if (status & 0b10000) sendLiteralBytes("RT ");
    if (status & 0b1) sendLiteralBytes("TXF ");
    fill();
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
        if (MODE_SELECT == MODE_SEND) {
            senderMain();
        } else {
            receiverMain();
        }
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
    if (MODE_SELECT == MODE_SEND) {
        senderInterrupt();
    } else {
        receiverInterrupt();
    }

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

            BSF PORTA, 0, ACCESS ///////////////////////////////////////////////// SET

            //clear timer overflow (timer trips again in 20 cycles)
            BCF PIR1, 1, ACCESS //1

            //bit shift and set carry flag
            RLCF RXB1D7, 1, 0 //1
            BC transmitOne //1 or 2
            //NOP

        transmitZero:
            BCF PORTA, 0, ACCESS ///////////////////////////////////////////////// CLEAR

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

            BCF PORTA, 0, ACCESS ///////////////////////////////////////////////// CLEAR

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
            BCF PORTA, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true
    _endasm
    INTCONbits.GIE = saveGIE;
}
