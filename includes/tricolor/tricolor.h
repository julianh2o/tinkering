const int PW_period = 230;

//Uses timer 0

void init(void); //call me to set up interrupt
void setColor(int r, int g, int b);

struct LEDColor {
	char index; //state of the pwm
	char r;
	char g;
	char b;
};

