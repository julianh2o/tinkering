#include "p18f25k80.h"
#include "constants.h"
#include "nRF2401.h"
#include <timers.h>
#include <math.h>
#include <delays.h>

#pragma config RETEN = OFF      // VREG Sleep Enable bit (Ultra low-power regulator is Disabled (Controlled by REGSLP bit))
#pragma config INTOSCSEL = HIGH // LF-INTOSC Low-power Enable bit (LF-INTOSC in High-power mode during Sleep)
#pragma config SOSCSEL = HIGH   // SOSC Power Selection and mode Configuration bits (High Power SOSC circuit selected)
#pragma config XINST = OFF       // Extended Instruction Set
#pragma config FOSC = INTIO2    // Oscillator (Internal RC oscillator)
#pragma config PLLCFG = OFF     // PLL x4 Enable bit (Disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor (Disabled)
#pragma config IESO = OFF       // Internal External Oscillator Switch Over Mode (Disabled)
#pragma config PWRTEN = OFF     // Power Up Timer (Disabled)
#pragma config BOREN = SBORDIS  // Brown Out Detect (Enabled in hardware, SBOREN disabled)
#pragma config BORV = 3         // Brown-out Reset Voltage bits (1.8V)
#pragma config BORPWR = ZPBORMV // BORMV Power level (ZPBORMV instead of BORMV is selected)
#pragma config WDTEN = SWDTDIS  // Watchdog Timer (WDT enabled in hardware; SWDTEN bit disabled)
#pragma config WDTPS = 1048576  // Watchdog Postscaler (1:1048576)
#pragma config CANMX = PORTB    // ECAN Mux bit (ECAN TX and RX pins are located on RB2 and RB3, respectively)
#pragma config MSSPMSK = MSK7   // MSSP address masking (7 Bit address masking mode)
#pragma config MCLRE = ON       // Master Clear Enable (MCLR Enabled, RE3 Disabled)
#pragma config STVREN = ON      // Stack Overflow Reset (Enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size (2K word Boot Block size)
#pragma config CP0 = OFF        // Code Protect 00800-01FFF (Disabled)
#pragma config CP1 = OFF        // Code Protect 02000-03FFF (Disabled)
#pragma config CP2 = OFF        // Code Protect 04000-05FFF (Disabled)
#pragma config CP3 = OFF        // Code Protect 06000-07FFF (Disabled)
#pragma config CPB = OFF        // Code Protect Boot (Disabled)
#pragma config CPD = OFF        // Data EE Read Protect (Disabled)
#pragma config WRT0 = OFF       // Table Write Protect 00800-03FFF (Disabled)
#pragma config WRT1 = OFF       // Table Write Protect 04000-07FFF (Disabled)
#pragma config WRT2 = OFF       // Table Write Protect 08000-0BFFF (Disabled)
#pragma config WRT3 = OFF       // Table Write Protect 0C000-0FFFF (Disabled)
#pragma config WRTC = OFF       // Config. Write Protect (Disabled)
#pragma config WRTB = OFF       // Table Write Protect Boot (Disabled)
#pragma config WRTD = OFF       // Data EE Write Protect (Disabled)
#pragma config EBTR0 = OFF      // Table Read Protect 00800-03FFF (Disabled)
#pragma config EBTR1 = OFF      // Table Read Protect 04000-07FFF (Disabled)
#pragma config EBTR2 = OFF      // Table Read Protect 08000-0BFFF (Disabled)
#pragma config EBTR3 = OFF      // Table Read Protect 0C000-0FFFF (Disabled)
#pragma config EBTRB = OFF      // Table Read Protect Boot (Disabled)

unsigned char ACK_buf[2] = {0x12,0x34};
volatile unsigned char rx_buf[32];


//======== prototypes =================
void TXService(void);

//========== main =====================================================================
void main(void) {
	int temp1;
	int T1;
	int T2;
	int i;

        INTCON2bits.RBPU = set;
	OSCCON = 0b01100011; //internal oscilator
	OSCCON2 = 0b00000000;
	RCONbits.IPEN = clear;
	ODCON = 0b00000000;
	T1CONbits.SOSCEN = clear;
	T3CONbits.SOSCEN = clear;
	ANCON1= 0b00000000;

	TRIS_CE = output;
	TRIS_CSN = output;
	TRIS_IRQ = input;	

	ANCON0 = 0b00000001;

	TRIS_SCK = output;
	TRIS_MISO = input;
	TRIS_MOSI = output;



	Delay10KTCYx(50);

	//===configure SPI for nordic RF module
	SSPSTAT = 0b00000000;	//SPI, clock on idle to active clk trans
	SSPSTATbits.CKE = 1; 	//clock on idle to active clk trans
	SSPCON1 = 0b00100001;	//SPI setup. clk at 1/16; idle low.
	SSPCON1bits.CKP = 0;	//clock polarity, idle low
	SSPCON1bits.SSPEN = set;	//enable SPI module

	
        initTX();

	Delay10KTCYx(20);

        TXService();

	i=0;
	while(1) {
            i++;
            Delay10KTCYx(20);
	}

}

//====== TXService =======================================
void TXService(void) {
	unsigned char status = 0;
	unsigned char tx_buf[TX_PLOAD_WIDTH];
	unsigned char config_reg;

	// ----- check nRF config and correct if needed ---------
	config_reg = SPI_Read(CONFIG);
	if(config_reg != 0x0E) {
            initTX();
	}

	//---- get status for IRQ service -------
	status=SPI_Read(STATUS);
	
	// ---- service IRQ flags ---------
	if(status&MAX_RT) {
            SPI_RW_Reg(WRITE_REG + STATUS, MAX_RT);				//clear MAX_RT flag
	}
	
        SPI_RW_Reg(FLUSH_TX,0);

        tx_buf[0] = 1;
        tx_buf[1] = 0;
        tx_buf[2] = 0;
        tx_buf[3] = 0;
        SPI_Write_Buf(WR_TX_PLOAD,tx_buf,TX_PLOAD_WIDTH);
        CE = set;											//begin transmit
        Delay1KTCYx(2);
        CE = clear;

	if(status&RX_DR) {
            SPI_RW_Reg(WRITE_REG + STATUS, RX_DR);
            SPI_Read_Buf(RD_RX_PLOAD,rx_buf,2);
            SPI_RW_Reg(FLUSH_RX,0);
	}
	//---- END service IRQ flags ---------
}


