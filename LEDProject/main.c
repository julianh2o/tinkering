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

#ifndef __TSMTYPES_H__
#define __TSMTYPES_H__

typedef unsigned char	Uint8;
typedef unsigned int	Uint16;
typedef unsigned long	Uint32;

typedef char		Sint8;
typedef int		Sint16;
typedef long		Sint32;

#endif/*__TSMTYPES_H__*/

/** This EXACT SEQUENCE of instructions is needed, any deviation will
 *  cause the write to FAIL!  The PIC even counts the number of
 *  instructions to check if you did this right!  This means we need
 *  inline ASM, C isn't going to get it Just Right(tm).
 */
#define EEPROM_WRITE()	do {				\
	EECON2=0x00;	/* Get in right bank */		\
	asm	MOVLW	0x55		endasm; 	\
	asm	MOVWF	EECON2		endasm; 	\
	asm	MOVLW	0xaa		endasm; 	\
        asm	MOVWF	EECON2		endasm; 	\
        asm	BSF	EECON1,1;	endasm;	\
	} while(0)
	} while(0)
	} while(0)
	} while(0)
	} while(0)
	} while(0)
	} while(0)
	} while(0)

/** C doesn't have this either, so we make it a macro. */
#define sleep() __asm sleep __endasm

// String containing file name and date of compilation.
//static const char *str=#__FILE__ "@" __DATE__;
// Until SDCC supports proper strings again, this message will be more terse.
static const char str[]={'S', 'D', 'C', 'C', '\0'};

void main(void)
{
	static Uint8 pos;

	PORTA=0x00;	// Output all zero on PORTA.

#ifdef  __16f628a       // Only compile this section for PIC16f628a
	CMCON = 0x07;	/** Disable comparators.  NEEDED FOR NORMAL PORTA
			 *  BEHAVIOR ON PIC16f628a!
			 */
#endif
	TRISA=0x00;	// PORTA all outputs.

	pos=0;		// Start at position 0 in the string and EEPROM.
	do
	{
		EEADR=pos;	// EEPROM position same as string position.
		EEDATA=str[pos];// Set the data to write.
		WREN=1;		// Enable writes.
		EEPROM_WRITE();	// Magic Instruction Sequence Go!
            while(WR);	// Wait for write to finish.
		WREN=0;		// Disable writes.

	} while(str[pos++]);	// Loop while str[pos] != 0x00

	PORTA|=0x01;	// Set A0 high to show the program's done.

	while(1)	// Loop forever:
		sleep();	// Go into sleep mode.
}