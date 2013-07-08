#include <htc.h>
#include <pic16f628a.h>
//#include <math.h>


__CONFIG(0x3FF0);

int pwm_i = 0;
int r=0,g=0,b=0;

void delay(void);

void main(void) {
    TRISB0 = 0; //blue
    TRISB1 = 0; //green
    TRISB2 = 0; //red

    //set up timer
    T0IF = 0;
    TMR0 = 158; //interrupt at 0.1ms (no prescaler)
    T0CS = 0;
    PSA = 1;
    //OPTION_REG |= 0x070;
    GIE = 1;
    T0IE = 1;

    RB0 = 0;
    RB1 = 0;
    RB2 = 0;

    int i=0;
    int up = 1;
    while (1) {
        if (up) {
            i++;
            if (i >= 100) up = 0;
        } else {
            i--;
            if (i <= 0) up = 1;
        }

        r = 0;
        g = 100 - i;
        b = i;
        delay();
    }
    
}

void pwm_update (void) {

}

void interrupt tmr0_isr (void) {
    int t_rb0 = 1;
    int t_rb1 = 1;
    int t_rb2 = 1;

    if (pwm_i >= r) t_rb2 = 0;
    if (pwm_i >= g) t_rb1 = 0;
    if (pwm_i >= b) t_rb0 = 0;

    RB0 = t_rb0;
    RB1 = t_rb1;
    RB2 = t_rb2;

    pwm_i++;
    if (pwm_i > 100) pwm_i = 0;

    T0IF = 0;
    TMR0 = 200;
}

void delay(void) {
    unsigned int i;
    for (i=0; i<1000; i ++);
}