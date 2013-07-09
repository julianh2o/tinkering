#include <htc.h>
#include <pic16f628a.h>
#include <math.h>

__CONFIG(FOSC_INTOSCIO &
WDTE_OFF &
PWRTE_ON &
MCLRE_ON &
BOREN_ON &
LVP_OFF &
CPD_OFF &
CP_OFF);

void delay(int n);

void main(void) {
    TRISB0 = 0; //blue
    TRISB1 = 0; //green
    TRISB2 = 0; //red

    TRISB3 = 1; //button

    char i = EEPROM_READ(0x0A);
    while (1) {
        if (i > 2) i=0;

        EEPROM_WRITE(0x0A, i);
        
        RB0 = (i==0);
        RB1 = (i==1);
        RB2 = (i==2);

        while(RB3 == 1); //debounce
        delay(1000);
        while(RB3 == 0);
        i++;
    }
}

void delay(int n) {
    unsigned int i;
    for (i=0; i<n; i++);
}