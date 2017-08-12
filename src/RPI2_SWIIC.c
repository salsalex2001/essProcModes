
#include "RPI2_GPIO.h"
#include "RPI2_SYSTIMER.h"
#include "RPI2_SWIIC.h"

// GPIO register structure to enable fast macros (SDA_H/L and SCL_H/L)
volatile rpi_gpio_t * iicgpio;

/*
  Sets the SCL and SDA lines as ouput and at high level.
*/
void swiicInit (void) {
  
  iicgpio = RPI_getGpio();

  /* Seems to be unecessary as 1st, this is the default state and 2nd
     the PI has pull-ups already mounted (R23 & R24, 1,8 KOhm)

  // Setup the pull-ups
  iicgpio->GPPUD = 0x02;     // Set the control signal for pull-up enabling
  RPI_waitMicroSeconds(150); // Wait 150 "cycles"
  iicgpio->GPPUDCLK0 = ( 1 << SDA1_GPIO_BIT + 1 << SCL1_GPIO_BIT ); // Enable the clock control
  RPI_waitMicroSeconds(150); // Wait for another 150 "cycles"
  iicgpio->GPPUD = 0x00;     // Disable the control signal
  iicgpio->GPPUDCLK0 = 0;    // Release the clock signal
  */
  
  // Setup the pin GPIO function (output)
  gpioSetDir (SCL1_GPIO_BIT, GPIO_OUTPUT);
  gpioSetDir (SDA1_GPIO_BIT, GPIO_OUTPUT);

  SDA1_H;
  SCL1_H;
}

/*
  PRE:
  swiicInit called before and both lines in HIGH state
  DOES:
  Generates a START condition.
  POST:
  Both lines LOW
      __     
  SDA   \____
      ____   
  SCL     \__
      | | | |
*/
void swiicStart (void) {
  SDA1_L;
  RPI_waitMicroSeconds(1);
  SCL1_L;
}

/*
  PRE:
  swiicInit called before and both lines in LOW state
  DOES:
  Generate a STOP condition
  POST:
  Both lines HIGH, ready for new transfer cycle
      __      __
  SDA __\____/ 
      __    ____
  SCL __\__/   
      | |  | | |  
*/
void swiicStop (void) {
  // Confirm initial conditions...
  SCL1_L; // Should be alreay low, but to be sure
  SDA1_L; // Should be alreay low, but to be sure
  // .. then proceed
  RPI_waitMicroSeconds(1);
  SCL1_H;
  RPI_waitMicroSeconds(1);
  SDA1_H;
}

/*
  PRE:
  swiicInit called before and both lines in HIGH state
  DOES:
  Generates a START condition.
  POST:
  Both lines LOW
      __________
  SDA ____/     \____
      __     _____
  SCL __\___/     \__
      | | | |   | |  
*/
void swiicRestart (void) {
  // make sure both lines are high 
  RPI_waitMicroSeconds(1);
  SCL1_L; // make sure SCL is low before transitionning SDA (may create a STA or STO otherwise)
  RPI_waitMicroSeconds(1);
  SDA1_H; // Set SDA high when SCL is LOW (no effect)
  RPI_waitMicroSeconds(1);
  SCL1_H; // Then set SCL high for init conditions
  RPI_waitMicroSeconds(2);

  SDA1_L;
  RPI_waitMicroSeconds(1);
  SCL1_L;
}

/*
  PRE:
  swiicInit invoked before and both lines in LOW state. A start condition has been sent first.
  DOES:
  Send a single bit on the bus.
  POST:
  SCL line LOW, SDA unchanged, ready for next bit transfer
 */
void swiicSendBit (char bit) {
  if (bit == 0) {
    SDA1_L;
  }
  else {
    SDA1_H;
  }
  RPI_waitMicroSeconds(1);
  SCL1_H;
  RPI_waitMicroSeconds(1);
  SCL1_L;
}

/*
  PRE:
  Start condition has been already sent.
  DO:
  Sends 8 bytes of the b argument
  POST:
  Both lines will be in LOW state. Next is to get the acknowledge.
 */
void swiicSendByte (char b) {
  char i;
  for (i=0; i<8 ; i++) {
    swiicSendBit(b & 0x80);
    b *= 2;
  }
}

/*
  PRE:
  Start condition has been sent - both lines are low
  DO:
  Sends 'nb' bytes of the buffer pointed by 'bs'. Each bytes is
  confirmed by the reception of an ACK (not verified here).
  POST:
  Lines in the state after swiicGetAck() - both LOW (output).
*/
void swiicSendBytes (unsigned char * bs, unsigned char nb) {
  unsigned char i;

  for (i=0 ; i<nb ; i++) {
    swiicSendByte(bs[i]);
    swiicGetAck();
  }
}

/*
  PRE:
  SCL line is in LOW state, SDA is anything
  DOES:
  Sets the SDA line in input mode, then generates SCL rising edge,
  samples the SDA daata then asserts SCL LOW. Reset SDA in output mode.
  POST:
  Both lines in LOW state, set as outputs
      ___________    
  SDA __/   |ACK|___
            ____    
  SCL _____/    \___
      | |  | |  |  |
 */
char swiicGetAck (void) {
  char ack;
  unsigned int temp;

  // Make sure SDA is High before (maybe useless with the pull-up)
  SDA1_H;

  // Setup the SDA1 GPIO as an input
  gpioSetDir (SDA1_GPIO_BIT, GPIO_INPUT);

  SCL1_H;
  RPI_waitMicroSeconds(1+1);
  // Read the data on SDA input...
  //if ( (iicgpio->GPLEV0 & (1 << SDA1_GPIO_BIT) ) == 0) {
  if (gpioGet(SDA1_GPIO_BIT) == 0) {
    // Level is LOW, this ia an ACK
    ack = 1;
  }
  else {
    // Level is HIGH, no ACK...
    ack = 0;
  }
  SCL1_L;
  
  RPI_waitMicroSeconds(1);

  // Keep SDA LOW after ACK is received
  SDA1_L;

  // Restore the SDA1 GPIO as an ouput
  gpioSetDir (SDA1_GPIO_BIT, GPIO_OUTPUT);

  return ack;
}

/*
  PRE:
  Start condition issued, both lines in low state
  DOES:
  Set SDA port in input mode then egenerate 8 clocks cycles on SCL and reset SDA in ouput mode
  POST:
  Both lines in outpute mode, SDA in high state, SCL in low state.
      ___________ _____ _    _ __
  SDA __/  |_____X_____X_    _X__\__ 
            _____    __      _     
  SCL _____/     \__/  \_     \_____
      | |  [ |   |  |  |  ... ]  | | 
*/
void swiicReadByte (unsigned char * buf) {
  unsigned char b;
  char i;
  
  b = 0;
  
  SDA1_H; // set high just before turnin to IN, maybe no need...
  gpioSetDir (SDA1_GPIO_BIT, GPIO_INPUT);
  RPI_waitMicroSeconds(1);
  
  for (i=0; i<8; i++) {
    SCL1_H;
    RPI_waitMicroSeconds(1);
    b <<= 1;
    b += gpioGet (SDA1_GPIO_BIT);
    SCL1_L;
    RPI_waitMicroSeconds(1);
  }
  
  // Set SDA L that will be the positive following ACK
  SDA1_L;
  gpioSetDir (SDA1_GPIO_BIT, GPIO_OUTPUT);
  RPI_waitMicroSeconds(1);
  
  *buf = b;
}

/*
  PRE:
  Start condition issued, tranmit started (Read) and 8 bits received. 
  SDA and SCL lines are low
  DOES:
  Issues a positive or negative ACK to the remote sender (slave).
  When type is 0 ack is negative, when type is not zero, ack is positive.
  POST:
  Both SDA and SCL lines in low state, output.
                 
  SDA __________
          ___  
  SCL ___/   \__
      |  |   | | 
*/
void swiicSendAck (char type) {
  if (type == 0) {
    SDA1_H; // Negative ACK
  }
  else {
    SDA1_L; // Positive ACK
  }
  RPI_waitMicroSeconds(2);
  SCL1_H;
  RPI_waitMicroSeconds(2);
  SCL1_L;
  RPI_waitMicroSeconds(1);
}

/*
  PRE:
  Start condition issued
  DOES:
  Read a buffer from I2C bus.
  For that, first transmit the slave address with R bit set.
  Then, reads the desired number of bytes, nb
  POST:
 */
void swiicReadData (unsigned char * buf, unsigned char nb) {

  // First sends the slave address and R bit (already in the buffer)
  swiicSendBytes (buf, 1);

  // nb currently ignored...
  swiicReadByte (&buf[1]);
  swiicSendAck (0); // Sends negative ack to signal end of the transfer to the slave

}


void swiicResetBus () {
  char i;
  
  SCL1_L;
  RPI_waitMicroSeconds(10);
  
  for (i=0 ; i<9 ; i++) {
    SCL1_H;
    RPI_waitMicroSeconds(1);
    SCL1_L;
    RPI_waitMicroSeconds(1);
  }
}
