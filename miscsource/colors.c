#include <htc.h>
#include <pic16f628a.h>


__CONFIG(0x3FF0);

void delay(void);

void main(void) {
    TRISB0 = 0; //blue
    TRISB1 = 0; //green
    TRISB2 = 0; //red

    int r=0,g=0,b=0;

    while (1) {
        unsigned int a;
        for (a=0; a<10; a++) {
            RB0 = 1;
            RB1 = 1;
            RB2 = 1;

            unsigned int i;
            for (i=0; i<100; i++) {
                if (i > r) RB2 = 0;
                if (i > g) RB1 = 0;
                if (i > b) RB0 = 0;
            }
        }
        
        r += 1;
        g += 2;
        b += 3;
        if (r > 100) r = 0;
        if (g > 100) g = 0;
        if (b > 100) b = 0;
    }
}

void delay(void) {
    unsigned int i;
    for (i=0; i<100; i ++);
}