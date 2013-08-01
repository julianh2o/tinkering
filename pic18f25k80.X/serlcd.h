#include "serlcd_config.h"

void setupLCD(void);
void sendByte(char byte);
void sendCommand(char byte);
void sendLiteralBytes(rom const char * bytes);
void sendBytes(char * bytes);
