//For the RX target board, the connection is Gray, White, Orange, Blue, and Black, Red, White, Yellow

#include "p18f25k80.h"
#include "constants.h"
#include "nRF2401.h"
#include "adc.h"
#include <timers.h>
#include <math.h>
#include <delays.h>

#define STRIP_DATA_TRIS TRISBbits.TRISB0
#define STRIP_DATA PORTBbits.RB0

#define STATUS_TRIS TRISCbits.TRISC0
#define STATUS_LED PORTCbits.RC0

#define BUTTON_TRIS TRISBbits.TRISB1
#define BUTTON PORTBbits.RB1

#define MODE_SELECT_TRIS TRISBbits.TRISB2
#define MODE_SELECT PORTBbits.RB2

//#define STRIP_LENGTH 125
#define STRIP_LENGTH 5
#define DATA_SIZE 375

#pragma idata large_idata
//char led_buffer[375] = {0,10,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,10,0,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,10,0,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,10,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,10,0,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,10,0,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,10,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,10,0,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,10,0,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,10,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,10,0,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,10,0,0,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,0,10,0,10,0,0,0,10,0,0,10,0,0,10,0,0,10};
char led_buffer[375] = {10,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
const char rainbow[375] = {0,15,0,0,15,0,1,15,0,2,15,0,3,15,0,3,15,0,4,15,0,5,15,0,6,15,0,6,15,0,7,15,0,8,15,0,9,15,0,9,15,0,10,15,0,11,15,0,12,15,0,13,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,12,0,15,11,0,15,11,0,15,10,0,15,9,0,15,8,0,15,8,0,15,7,0,15,6,0,15,5,0,15,5,0,15,4,0,15,3,0,15,2,0,15,2,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,1,15,0,2,15,0,3,15,0,4,15,0,4,15,0,5,15,0,6,15,0,7,15,0,7,15,0,8,15,0,9,15,0,10,15,0,10,15,0,11,15,0,12,15,0,13,15,0,14,15,0,14,15,0,15,15,0,15,14,0,15,14,0,15,13,0,15,12,0,15,11,0,15,10,0,15,10,0,15,9,0,15,8,0,15,7,0,15,7,0,15,6,0,15,5,0,15,4,0,15,4,0,15,3,0,15,2,0,15,1,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,2,15,0,2,15,0,3,15,0,4,15,0,5,15,0,5,15,0,6,15,0,7,15,0,8,15,0,8,15,0,9,15,0,10,15,0,11,15,0,11,15,0,12,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,13,0,15,12,0,15,11,0,15,10,0,15,9,0,15,9,0,15,8,0,15,7,0,15,6,0,15,6,0,15,5,0,15,4,0,15,3,0,15,3,0,15,2,0,15,1,0,15,0};
#pragma idata

unsigned char tx_buf[TX_PLOAD_WIDTH];
unsigned char rx_buf[TX_PLOAD_WIDTH];

void run(void);
void runStrip(void);
void runControl(void);

void displayStatus(char status);
void sendStrip();
void loadFrame(char frame);
void updateBuffer();

void doRainbow(void);
void delay(void);
extern void updateLEDs(void);

char runFlag=0;

//========== main =====================================================================
void main(void) {
    //Misc config
    STRIP_DATA_TRIS = OUTPUT;
    STATUS_TRIS = OUTPUT;
    BUTTON_TRIS = INPUT;
    MODE_SELECT_TRIS = INPUT;
    STATUS_LED = 0;

    //This is to toggle pins from digital to analog
    //unimp, RD3, RD2, RD1     RD1, AN10, AN9, AN8 (in order)
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
    //OSCTUNEbits.PLLEN = 0b0; //1=pllx4 enabled

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

    run();
    
    while(1);
}

void run(void) {
    if (MODE_SELECT) {
        runControl();
    } else {
        runStrip();
    }
}

void updateBuffer() {
    char loc = rx_buf[0];
    char i;

    //0,1,2,3,4 = (status info)
    //0 = multiplier
    //5-29 = data
    for (i = 0; i<25; i++) {
        led_buffer[25*loc+i] = rx_buf[5+i];
    }
}

void runStrip() {
    int i;
    char status;

    nrf_init();
    delay();

    initRX();
    delay();

    Delay10KTCYx(100);

    while(1) {
        setupLCD();
        status = getStatus();
        displayStatus(status);
        
        STATUS_LED = nrf_Recieve(&rx_buf);

//        updateBuffer();
//
//        updateLEDs();
        Delay10KTCYx(50);
    }
}

int readPotentiometer() {
    ConvertADC();
    while( BusyADC() );

    return ReadADC(); // (0,4096)
}

void loadFrame(char frame) {
    char i;
    for (i=0; i<25; i++) {
        tx_buf[i] = led_buffer[frame*25+i];
    }
}

void sendStrip() {
    char i;
    for (i=0; i<15; i++) {
        loadFrame(i);
        nrf_Send(&tx_buf, &rx_buf);
    }
}

void displayStatus(char status) {
    setPosition(0,0);
    sendLiteralBytes("stat:");
    sendBin(status);
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

void runControl() {
    char status;
    char config;
    char i;
    int value;

    //OpenADC(ADC_FOSC_64 & ADC_RIGHT_JUST & ADC_20_TAD, ADC_CH0 & ADC_INT_OFF, 0b0000);
    
    nrf_init();
    delay();

//    config = SPI_Read(CONFIG);
//    sendHex(config);
//    sendLiteralBytes("  ");

    initTX();
    delay();

//    config = SPI_Read(CONFIG);
//    sendHex(config);
//    sendLiteralBytes("  ");

    //while(1);
    

//    while(1) {
//        clear();
//        sendLiteralBytes("hello");
//        delay();
//    }
    while(1) {
        //STATUS_LED = !STATUS_LED;
        //value = readPotentiometer();
        //value = value >> 4;

//        tx_buf[0] = 0;
//        tx_buf[1] = 0;
//        tx_buf[2] = 0;
//        tx_buf[3] = 0;
//        tx_buf[4] = 0;
//
//        for (i=5; i<30; i++) {
//            tx_buf[i] = 5;
//        }
        setupLCD();
        status = getStatus();
        //config = SPI_Read(CONFIG);
        //clear();
        //sendHex(config);
        displayStatus(status);
        //STATUS_LED = !STATUS_LED;
        STATUS_LED = nrf_Send(&tx_buf, &rx_buf);

        //for (i=0; i<10; i++) {
            //led_buffer[i] = value;
        //}

        //sendStrip();
        Delay10KTCYx(50);
    }

    //CloseADC();
}

void doRainbow(void) {
    short i,offset,i_rainbow;

    offset = 0;
    while(1) {
        i_rainbow = offset;
        for (i=0; i<STRIP_LENGTH; i++) {
            led_buffer[i*3] = rainbow[i_rainbow*3];
            led_buffer[i*3+1] = rainbow[i_rainbow*3+1];
            led_buffer[i*3+2] = rainbow[i_rainbow*3+2];

            i_rainbow++;
            if (i_rainbow >= STRIP_LENGTH) i_rainbow = 0;
        }
        updateLEDs();
        Delay10KTCYx(50);
        offset++;
        if (offset >= STRIP_LENGTH) offset = 0;
    }
}

void delay(void) {
    Delay10KTCYx(254);
}

void updateLEDs() {
    _asm
            BSF INTCON, 2, ACCESS //Set the interrupt flag so that we pass through the initial wait loop without waiting

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
            BTFSS INTCON, 2, ACCESS //1, 2 or 3
            BRA timerWaitLoop2

            BSF PORTB, 0, ACCESS ///////////////////////////////////////////////// SET

            MOVLW 242
            MOVWF TMR0L, ACCESS

            //clear timer overflow (timer trips again in 20 cycles)
            BCF INTCON, 2, ACCESS //1

            //bit shift and set carry flag
            RLCF RXB1D7, 1, 0 //1
            BC transmitOne //1 or 2
            //NOP

        transmitZero:
            BCF PORTB, 0, ACCESS ///////////////////////////////////////////////// CLEAR

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

            BCF PORTB, 0, ACCESS ///////////////////////////////////////////////// CLEAR

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
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true

    _endasm
}
