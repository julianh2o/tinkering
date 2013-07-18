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

void main(void) {
    int i;
    STRIP_DATA_TRIS = OUTPUT;

    while(1) {
        reset();

        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();

        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();

        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();

        /////////////////////////////
        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();

        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();

        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();

        /////////////////////////////
        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();

        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();

        one();
        zero();
        zero();
        one();
        one();
        one();
        one();
        one();
    }

    while(1) {
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
    Delay100TCYx(32);
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