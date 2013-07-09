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

const int PW_period = 230;

void delay(int n);
void HSVtoRGB(char *r, char *g, char *b, short h);

void setHue(short h);
void setColor(int r, int g, int b);
void led_update();

const char hsbTable[1080] = {255, 0, 0, 255, 4, 0, 255, 8, 0, 255, 12, 0, 255, 16, 0, 255, 21, 0, 255, 25, 0, 255, 29, 0, 255, 33, 0, 255, 38, 0, 255, 42, 0, 255, 46, 0, 255, 50, 0, 255, 55, 0, 255, 59, 0, 255, 63, 0, 255, 67, 0, 255, 72, 0, 255, 76, 0, 255, 80, 0, 255, 84, 0, 255, 89, 0, 255, 93, 0, 255, 97, 0, 255, 102, 0, 255, 106, 0, 255, 110, 0, 255, 114, 0, 255, 119, 0, 255, 123, 0, 255, 127, 0, 255, 131, 0, 255, 136, 0, 255, 140, 0, 255, 144, 0, 255, 148, 0, 255, 153, 0, 255, 157, 0, 255, 161, 0, 255, 165, 0, 255, 170, 0, 255, 174, 0, 255, 178, 0, 255, 182, 0, 255, 187, 0, 255, 191, 0, 255, 195, 0, 255, 199, 0, 255, 204, 0, 255, 208, 0, 255, 212, 0, 255, 216, 0, 255, 221, 0, 255, 225, 0, 255, 229, 0, 255, 233, 0, 255, 238, 0, 255, 242, 0, 255, 246, 0, 255, 250, 0, 255, 255, 0, 250, 255, 0, 246, 255, 0, 242, 255, 0, 238, 255, 0, 233, 255, 0, 229, 255, 0, 225, 255, 0, 221, 255, 0, 216, 255, 0, 212, 255, 0, 208, 255, 0, 204, 255, 0, 199, 255, 0, 195, 255, 0, 191, 255, 0, 187, 255, 0, 182, 255, 0, 178, 255, 0, 174, 255, 0, 170, 255, 0, 165, 255, 0, 161, 255, 0, 157, 255, 0, 153, 255, 0, 148, 255, 0, 144, 255, 0, 140, 255, 0, 136, 255, 0, 131, 255, 0, 127, 255, 0, 123, 255, 0, 118, 255, 0, 114, 255, 0, 110, 255, 0, 106, 255, 0, 101, 255, 0, 97, 255, 0, 93, 255, 0, 89, 255, 0, 84, 255, 0, 80, 255, 0, 76, 255, 0, 72, 255, 0, 67, 255, 0, 63, 255, 0, 59, 255, 0, 55, 255, 0, 50, 255, 0, 46, 255, 0, 42, 255, 0, 38, 255, 0, 33, 255, 0, 29, 255, 0, 25, 255, 0, 21, 255, 0, 16, 255, 0, 12, 255, 0, 8, 255, 0, 4, 255, 0, 0, 255, 0, 0, 255, 4, 0, 255, 8, 0, 255, 12, 0, 255, 17, 0, 255, 21, 0, 255, 25, 0, 255, 29, 0, 255, 33, 0, 255, 38, 0, 255, 42, 0, 255, 46, 0, 255, 51, 0, 255, 55, 0, 255, 59, 0, 255, 63, 0, 255, 67, 0, 255, 72, 0, 255, 76, 0, 255, 80, 0, 255, 85, 0, 255, 89, 0, 255, 93, 0, 255, 97, 0, 255, 101, 0, 255, 106, 0, 255, 110, 0, 255, 114, 0, 255, 119, 0, 255, 123, 0, 255, 127, 0, 255, 131, 0, 255, 135, 0, 255, 140, 0, 255, 144, 0, 255, 148, 0, 255, 153, 0, 255, 157, 0, 255, 161, 0, 255, 165, 0, 255, 169, 0, 255, 174, 0, 255, 178, 0, 255, 182, 0, 255, 187, 0, 255, 191, 0, 255, 195, 0, 255, 199, 0, 255, 203, 0, 255, 208, 0, 255, 212, 0, 255, 216, 0, 255, 221, 0, 255, 225, 0, 255, 229, 0, 255, 233, 0, 255, 237, 0, 255, 242, 0, 255, 246, 0, 255, 250, 0, 255, 255, 0, 250, 255, 0, 246, 255, 0, 242, 255, 0, 237, 255, 0, 233, 255, 0, 229, 255, 0, 225, 255, 0, 221, 255, 0, 216, 255, 0, 212, 255, 0, 208, 255, 0, 203, 255, 0, 199, 255, 0, 195, 255, 0, 191, 255, 0, 187, 255, 0, 182, 255, 0, 178, 255, 0, 174, 255, 0, 169, 255, 0, 165, 255, 0, 161, 255, 0, 157, 255, 0, 153, 255, 0, 148, 255, 0, 144, 255, 0, 140, 255, 0, 135, 255, 0, 131, 255, 0, 127, 255, 0, 123, 255, 0, 119, 255, 0, 114, 255, 0, 110, 255, 0, 106, 255, 0, 101, 255, 0, 97, 255, 0, 93, 255, 0, 89, 255, 0, 85, 255, 0, 80, 255, 0, 76, 255, 0, 72, 255, 0, 67, 255, 0, 63, 255, 0, 59, 255, 0, 55, 255, 0, 51, 255, 0, 46, 255, 0, 42, 255, 0, 38, 255, 0, 33, 255, 0, 29, 255, 0, 25, 255, 0, 21, 255, 0, 17, 255, 0, 12, 255, 0, 8, 255, 0, 4, 255, 0, 0, 255, 4, 0, 255, 8, 0, 255, 12, 0, 255, 16, 0, 255, 21, 0, 255, 25, 0, 255, 29, 0, 255, 34, 0, 255, 38, 0, 255, 42, 0, 255, 46, 0, 255, 51, 0, 255, 55, 0, 255, 59, 0, 255, 63, 0, 255, 67, 0, 255, 72, 0, 255, 76, 0, 255, 80, 0, 255, 84, 0, 255, 89, 0, 255, 93, 0, 255, 97, 0, 255, 102, 0, 255, 106, 0, 255, 110, 0, 255, 114, 0, 255, 119, 0, 255, 123, 0, 255, 127, 0, 255, 131, 0, 255, 135, 0, 255, 140, 0, 255, 144, 0, 255, 148, 0, 255, 152, 0, 255, 157, 0, 255, 161, 0, 255, 165, 0, 255, 170, 0, 255, 174, 0, 255, 178, 0, 255, 182, 0, 255, 187, 0, 255, 191, 0, 255, 195, 0, 255, 199, 0, 255, 203, 0, 255, 208, 0, 255, 212, 0, 255, 216, 0, 255, 220, 0, 255, 225, 0, 255, 229, 0, 255, 233, 0, 255, 238, 0, 255, 242, 0, 255, 246, 0, 255, 250, 0, 255, 255, 0, 255, 255, 0, 250, 255, 0, 246, 255, 0, 242, 255, 0, 238, 255, 0, 233, 255, 0, 229, 255, 0, 225, 255, 0, 220, 255, 0, 216, 255, 0, 212, 255, 0, 208, 255, 0, 203, 255, 0, 199, 255, 0, 195, 255, 0, 191, 255, 0, 187, 255, 0, 182, 255, 0, 178, 255, 0, 174, 255, 0, 170, 255, 0, 165, 255, 0, 161, 255, 0, 157, 255, 0, 152, 255, 0, 148, 255, 0, 144, 255, 0, 140, 255, 0, 135, 255, 0, 131, 255, 0, 127, 255, 0, 123, 255, 0, 119, 255, 0, 114, 255, 0, 110, 255, 0, 106, 255, 0, 102, 255, 0, 97, 255, 0, 93, 255, 0, 89, 255, 0, 84, 255, 0, 80, 255, 0, 76, 255, 0, 72, 255, 0, 67, 255, 0, 63, 255, 0, 59, 255, 0, 55, 255, 0, 51, 255, 0, 46, 255, 0, 42, 255, 0, 38, 255, 0, 34, 255, 0, 29, 255, 0, 25, 255, 0, 21, 255, 0, 16, 255, 0, 12, 255, 0, 8, 255, 0, 4};

void main(void) {
    TRISB0 = 0; //blue
    TRISB1 = 0; //green
    TRISB2 = 0; //red

    //set up timer
    T0IF = 0;
    TMR0 = PW_period;
    T0CS = 0;
    PSA = 1;
    GIE = 1; //global interrupt flag
    T0IE = 1;

    TRISB3 = 1; //button

    short i = EEPROM_READ(0x0A);
    while (1) {
        if (i >= 360/60) i=0;

        EEPROM_WRITE(0x0A, i);

        setHue(60*i);

        //delay(1000);
        while(RB3 == 1); //debounce
        while(RB3 == 0);
        setColor(100,100,100);
        delay(1000);
        i++;
    }
}

void setHue(short h) {
    char r,g,b;
    HSVtoRGB(&r,&g,&b,h);
    setColor(r,g,b);
}

void HSVtoRGB(char *r, char *g, char *b, short h) {
    *r = hsbTable[3*h];
    *g = hsbTable[3*h+1];
    *b = hsbTable[3*h+2];
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

void delay(int n) {
    unsigned int i;
    for (i=0; i<n; i ++);
}


