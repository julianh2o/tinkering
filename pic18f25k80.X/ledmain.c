#include "config.h"
#include "constants.h"
#include <p18F25K80.h>
#include <delays.h>

#define STRIP_DATA_TRIS TRISBbits.TRISB0
#define STRIP_DATA PORTBbits.RB0

#define STATUS_TRIS TRISBbits.TRISB1
#define STATUS_LED PORTBbits.RB1

void delay(void);

void setLEDs();
void sendBatch(char * ptr, char len);
void sendReset();

#define STRIP_LENGTH 125
#define DATA_SIZE 375

//RAINBOW
#pragma idata large_idata
char rainbow[DATA_SIZE] = {0,15,0,0,15,0,1,15,0,2,15,0,3,15,0,3,15,0,4,15,0,5,15,0,6,15,0,6,15,0,7,15,0,8,15,0,9,15,0,9,15,0,10,15,0,11,15,0,12,15,0,13,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,12,0,15,11,0,15,11,0,15,10,0,15,9,0,15,8,0,15,8,0,15,7,0,15,6,0,15,5,0,15,5,0,15,4,0,15,3,0,15,2,0,15,2,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,1,15,0,2,15,0,3,15,0,4,15,0,4,15,0,5,15,0,6,15,0,7,15,0,7,15,0,8,15,0,9,15,0,10,15,0,10,15,0,11,15,0,12,15,0,13,15,0,14,15,0,14,15,0,15,15,0,15,14,0,15,14,0,15,13,0,15,12,0,15,11,0,15,10,0,15,10,0,15,9,0,15,8,0,15,7,0,15,7,0,15,6,0,15,5,0,15,4,0,15,4,0,15,3,0,15,2,0,15,1,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,2,15,0,2,15,0,3,15,0,4,15,0,5,15,0,5,15,0,6,15,0,7,15,0,8,15,0,8,15,0,9,15,0,10,15,0,11,15,0,11,15,0,12,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,13,0,15,12,0,15,11,0,15,10,0,15,9,0,15,9,0,15,8,0,15,7,0,15,6,0,15,6,0,15,5,0,15,4,0,15,3,0,15,3,0,15,2,0,15,1,0,15,0};
//char led_buffer[DATA_SIZE] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//char led_buffer[375] = {0,0,0,1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6,7,7,7,8,8,8,9,9,9,10,10,10,11,11,11,12,12,12,13,13,13,14,14,14,15,15,15,16,16,16,17,17,17,18,18,18,19,19,19,20,20,20,21,21,21,22,22,22,23,23,23,24,24,24,25,25,25,26,26,26,27,27,27,28,28,28,29,29,29,30,30,30,31,31,31,32,32,32,33,33,33,34,34,34,35,35,35,36,36,36,37,37,37,38,38,38,39,39,39,40,40,40,41,41,41,42,42,42,43,43,43,44,44,44,45,45,45,46,46,46,47,47,47,48,48,48,49,49,49,50,50,50,51,51,51,52,52,52,53,53,53,54,54,54,55,55,55,56,56,56,57,57,57,58,58,58,59,59,59,60,60,60,61,61,61,62,62,62,63,63,63,64,64,64,65,65,65,66,66,66,67,67,67,68,68,68,69,69,69,70,70,70,71,71,71,72,72,72,73,73,73,74,74,74,75,75,75,76,76,76,77,77,77,78,78,78,79,79,79,80,80,80,81,81,81,82,82,82,83,83,83,84,84,84,85,85,85,86,86,86,87,87,87,88,88,88,89,89,89,90,90,90,91,91,91,92,92,92,93,93,93,94,94,94,95,95,95,96,96,96,97,97,97,98,98,98,99,99,99,100,100,100,101,101,101,102,102,102,103,103,103,104,104,104,105,105,105,106,106,106,107,107,107,108,108,108,109,109,109,110,110,110,111,111,111,112,112,112,113,113,113,114,114,114,115,115,115,116,116,116,117,117,117,118,118,118,119,119,119,120,120,120,121,121,121,122,122,122,123,123,123,0,0,150};
//char led_buffer[375] = {10,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,10,0,0,0,0,0,0,0,0,0,0,0,0};
//char led_buffer[375] = {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0};
//char led_buffer[375] = {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0};
//char led_buffer[375] = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
char led_buffer[375] = {0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00,0b00};
#pragma idata

//const char led_buffer[DATA_SIZE_1+1] = {0,15,0,0,15,0,1,15,0,2,15,0,3,15,0,3,15,0,4,15,0,5,15,0,6,15,0,6,15,0,7,15,0,8,15,0,9,15,0,9,15,0,10,15,0,11,15,0,12,15,0,13,15,0,13,15,0,14,15,0,15,15,0,15,15,0,15,15,0,15,14,0,15,13,0,15,12,0,15,11,0,15,11,0,15,10,0,15,9,0,15,8,0,15,8,0,15,7,0,15,6,0,15,5,0,15,5,0,15,4,0,15,3,0,15,2,0,15,2,0,15,1,0,15,0,0,15,0,0,15,0,1,15,0,1,15,0,2,15,0,3,15,0,4,15,0,4,15,0,5,15,0,6,15,0,7,15,0,7,15,0,8,15,0,9,15,0,10,15,0,10,15,0,11,15,0,12,15,0,13,15,0,14,15,0,14,15,0,15,15,0,15,14,0,15,14,0,15,13,0,15,12,0,15,11,0,15,10,0,15,10,0,15,9,0,15,8,0,15,7,0,15,7,0,15,6,0,15,5,0,15,4,0,15,4,0,15,3,0,15,2,0,15,1,0,15,1,0,15,0,0,15,0,0,15,0};

//BLUE EVERY 10
//const char led_buffer[DATA_SIZE_1] = {0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,10,0,0,0,0,0,0,0,0,0,0,0,0};

//char * ledPointer;
//char * rainbowPointer;

void main(void) {
    short offset,i_strip,i_rainbow;

    //ledPointer = led_buffer;
    //rainbowPointer = rainbow;
    
    STRIP_DATA_TRIS = OUTPUT;
    STATUS_TRIS = OUTPUT;
    PORTBbits.RB2 = OUTPUT;

    //setup 16mhz
    //OSCCONbits.IRCF = 0b111; //sets internal osc to 16Mhz
    //OSCCONbits.SCS = 0b11;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    //OSCTUNEbits.PLLEN = 0b0;

    //setup 32mhz
    OSCCONbits.IRCF = 0b110; //sets internal osc to 16Mhz
    OSCCONbits.SCS = 0b00;  //select internal osc as main source (This may or may not be redundant, based on your config bits.  It's not clear to me.)
    OSCTUNEbits.PLLEN = 0b1;

    INTCON2bits.RBPU = 0b0;

//    _asm
//       // ############### CLEAR 10 ################
//        // clears the first 10 LEDs to off
//        clear10:
//            CALL asm_reset,1
//
//            //CLEAR BITS
//            MOVLW 240 //1
//        loop:
//            BSF PORTB, 0, ACCESS //1
//            NOP
//            BCF PORTB, 0, ACCESS //1
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//            NOP
//
//            ADDLW -1 //1
//            BNZ loop //1 if false, 2 if true
//
//            CALL asm_reset,1
//
//            RETURN 1
//
//        _endasm

    while(1) {
        STATUS_LED = 1;
        setLEDs();
        STATUS_LED = 0;
    }

    offset = 0;
    while(1) {
        i_rainbow = offset;
        for (i_strip=0; i_strip<STRIP_LENGTH; i_strip++) {
            led_buffer[i_strip*3] = rainbow[i_rainbow*3];
            led_buffer[i_strip*3+1] = rainbow[i_rainbow*3+1];
            led_buffer[i_strip*3+2] = rainbow[i_rainbow*3+2];
            i_rainbow++;
            if (i_rainbow > STRIP_LENGTH) i_rainbow=0;

        }
        setLEDs();
        offset ++;
        if (offset > STRIP_LENGTH) {
            offset = 0;
        }
    }
}

void setLEDs() {
    //sendReset();
    //sendBatch(&led_buffer,STRIP_LENGTH);
    sendBatch(&led_buffer,STRIP_LENGTH);
    //sendReset();
}

void sendReset() {
    _asm
        //################### ASM RESET ##############
        // sents a reset to the LED strip
        // a reset is a low for t > 50 microseconds
        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true

    _endasm
}

void sendBatch(char * ptr, char len) {
        _asm
            //loop over elements
            MOVLW  len//1
            MOVWF RXB1D4, ACCESS //1
            LFSR 0,led_buffer //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1

            //set up color iteration
            MOVLW 3 //1
            MOVWF RXB1D5, ACCESS //1
            
        restartLoopOneNop:
            NOP

        dataLoadedSendBits:
            MOVLW 6 //1
            MOVWF RXB1D6, ACCESS //1

        sendBitsLoop:
        //## SEND BIT##
            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
            BC carryBitSet //1 or 2
        carryBitClear:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1
            NOP
            NOP

        carryBitClearContinue:
        
            //decrement bit counter
            DECF RXB1D6, 1, ACCESS //1
            BZ done //1 if false, 2 otherwise
            BRA sendBitsLoop //2
            //BRA done //2

        carryBitSet:
            //Transmit a one (high 5, low 5)

            //decrement bit counter
            DECF RXB1D6, 1, ACCESS //1
            NOP
            BCF PORTB, 0, ACCESS //1
            NOP
            BNZ sendBitsLoop //1 if false, 2 if true

        done:
            NOP
        
            //## SEND PENULTIMATE BIT##
            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
            BC carryBitSetPenultimate //1 or 2
        carryBitClearPenultimate:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1

            //we have 3 spare cycles here, lets use these to load the next byte into memory
            INCF FSR0, 1, ACCESS //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1
            
            //restart counter at 6
            MOVLW 6 //1
            MOVWF RXB1D6, ACCESS //1

            NOP
        
            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
            BC carryBitSetFinal //1 or 2
        
            //copied and pasted from below
            BCF PORTB, 0, ACCESS //1

            //decrement color index and branch if not zero
            DECF RXB1D5, 1, ACCESS
            BNZ restartLoopOneNop //1 or 2 (we dont actually need to load new data.. we did already, we need to burn cycles)
            //BNZ dataLoadedSendBits //2

            //Decrement the current LED
            DECF RXB1D4, 1, ACCESS //1
            BZ doneFinal //1 or 2

            //reset color index
            MOVLW 3 //1
            MOVWF RXB1D5, ACCESS //1

            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1

            BC carryBitSet //1 or 2

            BCF PORTB, 0, ACCESS //1
            BRA carryBitClearContinue
            //copied and pasted from below



        carryBitSetPenultimate:
            //Transmit a one (high 5, low 5)
        
            //restart counter at 6
            MOVLW 6 //1
            MOVWF RXB1D6, ACCESS //1

            BCF PORTB, 0, ACCESS //1

            //we have 3 spare cycles here, lets use these to load the next byte into memory
            INCF FSR0, 1, ACCESS //1
            MOVF INDF0, 0, ACCESS //1
            MOVWF RXB1D7, ACCESS //1
            
        donePenultimate:
        
            //## SEND FINAL BIT ##
            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
            BC carryBitSetFinal //1 or 2
        
        // THIS SECTION IS EXECUTED IF THE LAST BIT IN THE SET IS 0
        carryBitClearFinal:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1

            //decrement color index and branch if not zero
            DECF RXB1D5, 1, ACCESS
            BNZ restartLoopOneNop //1 or 2 (we dont actually need to load new data.. we did already, we need to burn cycles)
            //BNZ dataLoadedSendBits //2

            //Decrement the current LED
            DECF RXB1D4, 1, ACCESS //1
            BZ doneFinal //1 or 2

            //reset color index
            MOVLW 3 //1
            MOVWF RXB1D5, ACCESS //1

            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1
        
            BC carryBitSet //1 or 2
        carryBitClearFinalNext:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1

            //reset counter to 5 so we can skip decrementing it
            MOVLW 5 //1
            MOVWF RXB1D6, ACCESS //1

            NOP
            NOP

            BRA sendBitsLoop //2


        // THIS SECTION IS EXECUTED IF THE LAST BIT IN THE SET IS 1
        carryBitSetFinal:
            //Transmit a one (high 5, low 5)

            NOP
            //NOP

            //decrement color index and branch if not zero (this is straddling the BCF to set the output low)
            DECF RXB1D5, 1, ACCESS
        
            //OUTPUT LOW
            BCF PORTB, 0, ACCESS //1
            BNZ sendBitsLoop //1 or 2 (we dont actually need to load new data.. we did already, we need to burn cycles)

            //Decrement the current LED
            DECF RXB1D4, 1, ACCESS //1
            BZ doneFinal //1 or 2

            RLCF RXB1D7, 1, 0 //1
            BSF PORTB, 0, ACCESS //1

            BC carryBitSet //1 or 2
        carryClearFinalNext2:
            //Transmit a zero (high 2, low 8)
            BCF PORTB, 0, ACCESS //1
            BRA carryBitClearContinue

        doneFinal:

        

        asm_reset:
            BCF PORTB, 0, ACCESS //1

            MOVLW 135 //1
        loop:
            ADDLW -1 //1
            BNZ loop //1 if false, 2 if true

        _endasm
}

void delay(void) {
    Delay10KTCYx(254);
}