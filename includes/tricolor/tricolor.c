void init() {
    T0IF = 0;
    TMR0 = PW_period;
    T0CS = 0;
    PSA = 1;
    GIE = 1;
    T0IE = 1;
}

void setColor(int r, int g, int b) {
	LEDColor.r = r;
	LEDColor.g = g;
	LEDColor.b = b;
}

void LEDUpdate (void) {
    RED_LED = (LEDColor.index < LEDColor.r);
    GREEN_LED = (LEDColor.index < LEDColor.g);
    BLUE_LED = (LEDColor.index < LEDColor.b);

    LEDColor.index++;
    if (LEDColor.index >= 100) LEDColor.index = 0;
}

void interrupt tmr0_isr (void) {
    LEDUpdate();

    T0IF = 0;
    TMR0 = PW_period;
}
