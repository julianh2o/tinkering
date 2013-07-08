#include <htc.h>
#include <pic16f628a.h>
#include <math.h>
//0x3FF0
__CONFIG(FOSC_INTOSCIO &
WDTE_OFF &
PWRTE_ON &
MCLRE_ON &
BOREN_ON &
LVP_OFF &
CPD_OFF &
CP_OFF);

const int PW_period = 230;

void delay(void);
void HSVtoRGB(char *r, char *g, char *b, short h);

void setColor(int r, int g, int b);
void led_update();

void main(void) {
    TRISB0 = 0; //blue
    TRISB1 = 0; //green
    TRISB2 = 0; //red

    //set up timer
    T0IF = 0;
    TMR0 = PW_period; //interrupt at 0.1ms (no prescaler)
    T0CS = 0;
    PSA = 1;
    //OPTION_REG |= 0x070;
    GIE = 1; //off
    T0IE = 1;

    RB0 = 0;
    RB1 = 0;
    RB2 = 0;

    
    short hue = 0;
    while(1) {
        if (hue >= 360) hue = 0;
        char r,g,b;
        HSVtoRGB(&r,&g,&b,hue);
        setColor(r,g,b);
        delay();
        hue += 30;
        
        //setColor(hue,100-hue,0);
        //if (hue > 100) hue = 0;
    }
}

//Source: http://www.cs.rit.edu/~ncs/color/t_convert.html
void HSVtoRGB(char *r, char *g, char *b, short ch) {
    char i;
    float f,q;

    float h = ch / 60.0;
    i = floor(h);

    f = h - i;
    //q = 1 - f;

    char cf = f * 100;
    char cq = 100 - cf; //q * 100;

    
    switch (i) {
        case 0:
            *r = 100;
            *g = cf;
            *b = 0;
            break;
        case 1:
            *r = cq;
            *g = 100;
            *b = 0;
            break;
        case 2:
            *r = 0;
            *g = 100;
            *b = cf;
            break;
        case 3:
            *r = 0;
            *g = cq;
            *b = 100;
            break;
        case 4:
            *r = cf;
            *g = 0;
            *b = 100;
            break;
        default:
            *r = 100;
            *g = 0;
            *b = cq;
            break;
    }
    
}

/* Tricolor LED code */
char led_i = 0;
char led_r=0,led_g=0,led_b=0;

void setColor(int r, int g, int b) {
    led_r = r;
    led_g = g;
    led_b = b;
}

void led_update (void) {
    char t_rb0 = 1;
    char t_rb1 = 1;
    char t_rb2 = 1;

    if (led_i >= led_r) t_rb2 = 0;
    if (led_i >= led_g) t_rb1 = 0;
    if (led_i >= led_b) t_rb0 = 0;

    RB0 = t_rb0;
    RB1 = t_rb1;
    RB2 = t_rb2;

    led_i++;
    if (led_i >= 100) led_i = 0;
}
/* Tricolor LED code */

void interrupt tmr0_isr (void) {
    led_update();

    T0IF = 0;
    TMR0 = PW_period;
}

void delay(void) {
    unsigned int i;
    for (i=0; i<10000; i ++);
}


