#include "p18f25k80.h"
//Configuration for p18f25k80

// Pin Definitions
#define CE              PORTCbits.RC1
#define TRIS_CE		TRISCbits.TRISC1
#define CSN		PORTCbits.RC2
#define TRIS_CSN	TRISCbits.TRISC2
#define IRQ		PORTCbits.RC6
#define TRIS_IRQ	TRISCbits.TRISC6

#define TRIS_SCK	TRISCbits.TRISC3
#define TRIS_MISO	TRISCbits.TRISC4
#define TRIS_MOSI	TRISCbits.TRISC5

#define SPI_BUFFER_FULL_STAT    SSPSTATbits.BF
#define SPI_BUFFER              SSPBUF

#define SPI_STATUS              SSPSTAT
#define SPI_CLK_EDGE          	SSPSTATbits.CKE //clock edge
#define SPI_CLK_POL     	SSPCON1bits.CKP //clock polarity
#define SPI_CONFIG_1		SSPCON1
#define SPI_ENABLE		SSPCON1bits.SSPEN
