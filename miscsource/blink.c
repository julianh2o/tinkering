#include <htc.h>
#include <pic16f628a.h>

__CONFIG(0x3FF0);

void delay(void);

void main(void) {
    TRISB = 0x00;
    while (1) {
        PORTB = 0b00000001;
        delay();
        PORTB = 0b00000000;
        delay();
    }
}

void delay(void) {
    unsigned int i;
    for (i=0; i<10000; i ++);
}
