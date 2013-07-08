#include <htc.h>
#include <pic16f628a.h>
#include <math.h>

__CONFIG(0x3FF0);

void delay(void);
void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v);

void setColor(int r, int g, int b);
void led_update();

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

    while(1) {
        setColor(10,0,0);
        delay();
        setColor(0,10,0);
        delay();
        setColor(0,0,10);
        delay();
    }
}

//Source: http://www.cs.rit.edu/~ncs/color/t_convert.html
void HSVtoRGB(float *r, float *g, float *b, float h, float s, float v) {
    int i;
    float f,p,q,t;

    if (s == 0) {
        *r = *g = *b = v;
        return;
    }

    h /= 60;
    i = (int)floor(h);
    f = h - i;
    p = v * (1-s);
    q = v * (1-s*f);
    t = v * (1-s*(1-f));

    switch (i) {
        case 0:
            *r = v;
            *g = t;
            *b = p;
            break;
        case 1:
            *r = q;
            *g = v;
            *b = p;
            break;
        case 2:
            *r = p;
            *g = v;
            *b = t;
            break;
        case 3:
            *r = p;
            *g = q;
            *b = v;
            break;
        case 4:
            *r = t;
            *g = p;
            *b = v;
            break;
        default:
            *r = v;
            *g = p;
            *b = q;
            break;
    }
}

/* Tricolor LED code */
int led_i = 0;
int led_r=0,led_g=0,led_b=0;

void setColor(int r, int g, int b) {
    led_r = r;
    led_g = g;
    led_b = b;
}

void led_update (void) {
    int t_rb0 = 1;
    int t_rb1 = 1;
    int t_rb2 = 1;

    if (led_i >= led_r) t_rb2 = 0;
    if (led_i >= led_g) t_rb1 = 0;
    if (led_i >= led_b) t_rb0 = 0;

    RB0 = t_rb0;
    RB1 = t_rb1;
    RB2 = t_rb2;

    led_i++;
    if (led_i > 100) led_i = 0;
}
/* Tricolor LED code */

void interrupt tmr0_isr (void) {
    led_update();

    T0IF = 0;
    TMR0 = 200;
}

void delay(void) {
    unsigned int i;
    for (i=0; i<10000; i ++);
}


