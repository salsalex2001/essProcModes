
#include <stdint.h>
#include "RPI_BASE.h"
#include "RPI2_GPIO.h"
#include "RPI2_AUX.h"

//------------------------------------------------------------------------------
// AUX peripheral that contains a UART and 2 SPI
//------------------------------------------------------------------------------

#define PBASE 0x3F000000

/*
extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );
extern void dummy ( unsigned int );
*/

// Private services ------------------------------------------------------------
void hexstrings (unsigned int d);
void binstrings (unsigned int d);


#define ARM_TIMER_CTL   (PBASE+0x0000B408)
#define ARM_TIMER_CNT   (PBASE+0x0000B420)

//#define GPFSEL1         (PBASE+0x00200004)
//#define GPSET0          (PBASE+0x0020001C)
//#define GPCLR0          (PBASE+0x00200028)
//#define GPPUD           (PBASE+0x00200094)
//#define GPPUDCLK0       (PBASE+0x00200098)


//GPIO14  TXD0 and TXD1
//GPIO15  RXD0 and RXD1

//------------------------------------------------------------------------------
/* Configuration
  Bitrate:   115200 bps
  Data bits: 8
  Stop bit:  1
  Parity:    none
  Flow ctrl: none
  
 */
void muInit ( void ) {
  unsigned int ra;
  
  PUT32 (AUX_ENABLES,1);
  PUT32 (AUX_MU_IER_REG,0);
  PUT32 (AUX_MU_CNTL_REG,0);
  PUT32 (AUX_MU_LCR_REG,3);
  PUT32 (AUX_MU_MCR_REG,0);
  PUT32 (AUX_MU_IER_REG,0);
  PUT32 (AUX_MU_IIR_REG,0xC6);
  PUT32 (AUX_MU_BAUD_REG,270);

  // GPIO settings
  gpioSetDir (MU_TXD, GPIO_FSEL_FCT5);
  gpioSetDir (MU_RXD, GPIO_FSEL_FCT5);

  // GPIO IF settings
  gpioSetIF (MU_RXD, GPIO_PUD_OFF);
  gpioSetIF (MU_TXD, GPIO_PUD_OFF);

  // 
  PUT32 (AUX_MU_CNTL_REG,3);
}


//------------------------------------------------------------------------------
//void hexstring ( unsigned int d ) {
void muLogInt2Hex (unsigned int d) {
  hexstrings(d);
  muSendByte(0x0D);
  muSendByte(0x0A);
}

//------------------------------------------------------------------------------
void muLogInt2Bin (unsigned int d) {
  binstrings(d);
  muSendByte(0x0D);
  muSendByte(0x0A);
}

//------------------------------------------------------------------------------
// c is the char to print
// md is the mode, 0 for char only, 1 with CR, 2 with CR and LF
//------------------------------------------------------------------------------
void muLogChar (char c, char md) {
  muSendByte(c);
  if (md > 0) {
    muSendByte(0x0D);
    if (md > 1) {
      muSendByte(0x0A);
    }
  }
}

//------------------------------------------------------------------------------
void muLogString (char * str) {
  uint16_t nbChar;
  nbChar = 0;
  
  do {
    muSendByte(*str);
  } while ( (*(++str) != 0) && (nbChar++ < 50) ); // String is ended by a 0
  muSendByte(0x0D);
  muSendByte(0x0A);
}

//------------------------------------------------------------------------------
/* Prints a string followed by an integer (hex coded)
 *
 * */
void muLogStrInt (char * str, uint32_t d) {
  uint16_t nbChar;
  nbChar = 0;
  
  do {
    muSendByte(*str);
  } while ( (*(++str) != 0) && (nbChar++ < 50) ); // String is ended by a 0
  hexstrings(d);
  muSendByte(0x0D);
  muSendByte(0x0A);
}


//------------------------------------------------------------------------------
// This function uses a in-string terminator, the '|' char
// ASM usage
//------------------------------------------------------------------------------
void muLogString2 (char * str) {
  uint16_t nbChar;
  nbChar = 0;
  
  do {
    muSendByte(*str);
  } while ( (*(++str) != '|') && (nbChar++ < 50) ); // If need to use a termination char, use '|'
  muSendByte(0x0D);
  muSendByte(0x0A);
}


//------------------------------------------------------------------------
char muRxFifoNotEmpty (void) {
  return( GET32(AUX_MU_LSR_REG) & AUX_MU_LSR_DATARDY);
}

//------------------------------------------------------------------------
// This is to be called when the RxFiFo at least contains a byte...
//------------------------------------------------------------------------
unsigned int muGetRxByte ( void ) {
  //while (1) {
  //  if (GET32(AUX_MU_LSR_REG)&0x01) break;
  //}
  return ( GET32(AUX_MU_IO_REG) & AUX_MU_IO_MASK );
}

//------------------------------------------------------------------------
unsigned int muWaitReceiveByte ( void ) {
  while (1) {
    if (GET32(AUX_MU_LSR_REG)&0x01) break;
  }
  return ( GET32(AUX_MU_IO_REG) & AUX_MU_IO_MASK );
}

//------------------------------------------------------------------------
void muSendByte ( unsigned int c ) {
  while (1) {
    if ( GET32(AUX_MU_LSR_REG) & 0x20 )
      break;
  }
  PUT32 (AUX_MU_IO_REG, c);
}





//------------------------------------------------------------------------
unsigned int uart_lcr ( void ) {
  return (GET32(AUX_MU_LSR_REG));
}

//------------------------------------------------------------------------
unsigned int uart_check ( void ) {
  if (GET32(AUX_MU_LSR_REG) & 0x01)
    return (1);
  return (0);
}


//------------------------------------------------------------------------------
void uart_flush ( void ) {
  while (1) {
    if ( (GET32(AUX_MU_LSR_REG)&0x100) == 0 )
      break;
  }
}

//------------------------------------------------------------------------------
// Prints a 32 bits number in hexadecimal
void hexstrings (unsigned int d) {
  unsigned int ra;
  unsigned int rb;
  
  ra = 32;
  while (1) {
    ra -= 4;
    rb = (d>>ra) & 0xF;
    if (rb > 9)
      rb += 0x37;
    else
      rb += 0x30;
    muSendByte (rb);
    if (ra == 16)
      muSendByte (32);
    if (ra == 0)
      break;
  }
  muSendByte(0x20);
}

//------------------------------------------------------------------------------
// Prints a number in binary, 32 bits.
void binstrings (unsigned int d) {
  //unsigned int ra;
  unsigned int rb;
  unsigned int rc;
  
  rb = 32;
  while (1) {
    rb -= 1;
    rc = (d>>rb) & 0x1;
    rc += 0x30;
    muSendByte (rc);
    if ( (rb & 0x7) == 0 ) // modulo 8
      muSendByte (32);
    if (rb == 0)
      break;
  }
  muSendByte(0x20);
}



/*
//------------------------------------------------------------------------
void  timer_init ( void ) {
  //0xF9+1 = 250
  //250MHz/250 = 1MHz
  PUT32(ARM_TIMER_CTL,0x00F90000);
  PUT32(ARM_TIMER_CTL,0x00F90200);
}

//-------------------------------------------------------------------------
unsigned int timer_tick ( void ) {
  return (GET32(ARM_TIMER_CNT));
}

//-------------------------------------------------------------------------
void leds_off ( void ) {
#define GPFSEL3 0x3F20000C
#define GPFSEL4 0x3F200010
#define GPSET1  0x3F200020
#define GPCLR1  0x3F20002C

  unsigned int ra;
  
  ra=GET32(GPFSEL4);
  ra&=~(7<<21);
  ra|=1<<21;
  PUT32(GPFSEL4,ra);
  
  ra=GET32(GPFSEL3);
  ra&=~(7<<15);
  ra|=1<<15;
  PUT32(GPFSEL3,ra);
  
  PUT32(GPCLR1,1<<(47-32));
  PUT32(GPCLR1,1<<(35-32));
  }*/

//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
