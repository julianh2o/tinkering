#include "serlcd_config.h"

void setupLCD(void);
void sendByte(char byte);
void sendVisibleByte(char byte);

void sendCommand(char byte);
void sendSpecialCommand(char byte);
void setPosition(char row, char column);
void setBacklight(char brightness); //between 0 and 29
void clear(void);

void sendLiteralBytes(rom const char * bytes);
void sendBytes(char * bytes);

void sendDigit(unsigned char digit);
void sendCharAsBase(unsigned char num, unsigned char base);

void sendDec(unsigned char num);
void sendHex(unsigned char num);
void sendBin(unsigned char num);

void fill(void);
