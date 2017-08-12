

#include "RPI2_SYSTIMER.h"
#include "RPI2_SWIIC.h"
#include "RPI_HAT.h"





void hatClear (void) {
  unsigned char table[200];
  unsigned char row;

  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write
  table[1] = 0;
  
  for ( row=0 ; row<192 ; row++ ) {
    table[2+row] = 0;  // Address of the first pixel 
  }
  
  swiicStart();
  swiicSendBytes (table, 194);
  swiicStop();
  //sysWait(1); // 1ms
  RPI_waitMicroSeconds(10); // 10us
  
}

void hatClearPrev (void) {
  unsigned char table[16];
  unsigned char row, col;

  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write
  table[1] = 0;  // Address of the first pixel 
  table[2] = 0;  // Data...
  table[3] = 0;  // Data...
  table[4] = 0;  // Data...
  table[5] = 0;  // Data...
  table[6] = 0;  // Data...
  table[7] = 0;  // Data...
  table[8] = 0;  // Data...
  table[9] = 0;  // Data...
  
  for ( row=0 ; row<192 ; row = row+8 ) {
    table[1] = row;
    swiicStart();
    swiicSendBytes (table, 10);
    swiicStop();
    //sysWait(1); // 1ms
    RPI_waitMicroSeconds(10); // 10us
  }

}

/* Shows the char b in the selected row, in binary.

   Convention for this function. Byte is displayed in a colum, with
   the LSB at the bottom and the MSB at the top of the column.

   rgb is either 0, 8 or 16 to select R, G or B display color.

  Board orientation is connectors turned to uppoer side, SD card
  reader being at the bottom side.
*/
void hatShowByte (unsigned char b, unsigned char col, char rgb) {
  unsigned char table[16];

  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write
  table[1] = (col*24)+rgb;
  
  // First clear the row
  table[2] = 0;  // Data...
  table[3] = 0;  // Data...
  table[4] = 0;  // Data...
  table[5] = 0;  // Data...
  table[6] = 0;  // Data...
  table[7] = 0;  // Data...
  table[8] = 0;  // Data...
  table[9] = 0;  // Data...

  swiicStart();
  swiicSendBytes (table, 10);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us

  // Then put the desired data
  table[2] = (b & 0x01)?20:0;  // Data...
  table[3] = (b & 0x02)?20:0;  // Data...
  table[4] = (b & 0x04)?20:0;  // Data...
  table[5] = (b & 0x08)?20:0;  // Data...
  table[6] = (b & 0x10)?20:0;  // Data...
  table[7] = (b & 0x20)?20:0;  // Data...
  table[8] = (b & 0x40)?20:0;  // Data...
  table[9] = (b & 0x80)?20:0;  // Data...
  
  swiicStart();
  swiicSendBytes (table, 10);
  swiicStop();
  RPI_waitMicroSeconds(10); // 10us

}

/* Shows the 32 bits integer b in the selected columns, from col, in binary.

   Convention for this function:
   The word is displayed in a column, with the LSB at the bottom and
   the MSB at the top of the column. MSB byte is on the left, LSB on
   the right.

  Board orientation is connectors turned to upper side, SD card
  reader being at the bottom side.

  col is from 0 to 7 (left to right)
  rgb is either 0, 8 or 16 to select the color plane R, G or B.
*/
// Shows the 32bits integer b in the selected col, in binary.
void hatShowInt (uint32_t b, unsigned char col, char rgb) {
  unsigned char table[10];

  col +=3;
  
  // Byte 0, first column
  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write
  table[1] = (col*24)+rgb;
  
  // Store the 1st byte
  table[2] = (b & 0x00000001)?20:0;  // Data...
  table[3] = (b & 0x00000002)?20:0;  // Data...
  table[4] = (b & 0x00000004)?20:0;  // Data...
  table[5] = (b & 0x00000008)?20:0;  // Data...
  table[6] = (b & 0x00000010)?20:0;  // Data...
  table[7] = (b & 0x00000020)?20:0;  // Data...
  table[8] = (b & 0x00000040)?20:0;  // Data...
  table[9] = (b & 0x00000080)?20:0;  // Data...

  swiicStart();
  swiicSendBytes (table, 10);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us

  // Byte 1
  col--; // next column...
  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write
  table[1] = (col*24)+rgb;

  // Store the 2nd byte
  table[2] = (b & 0x00000100)?20:0;  // Data...
  table[3] = (b & 0x00000200)?20:0;  // Data...
  table[4] = (b & 0x00000400)?20:0;  // Data...
  table[5] = (b & 0x00000800)?20:0;  // Data...
  table[6] = (b & 0x00001000)?20:0;  // Data...
  table[7] = (b & 0x00002000)?20:0;  // Data...
  table[8] = (b & 0x00004000)?20:0;  // Data...
  table[9] = (b & 0x00008000)?20:0;  // Data...
  
  swiicStart();
  swiicSendBytes (table, 10);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us

  // Byte 2
  col--; // next column...
  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write
  table[1] = (col*24)+rgb;
  
  // Store the 3rd byte
  table[2] = (b & 0x00010000)?20:0;  // Data...
  table[3] = (b & 0x00020000)?20:0;  // Data...
  table[4] = (b & 0x00040000)?20:0;  // Data...
  table[5] = (b & 0x00080000)?20:0;  // Data...
  table[6] = (b & 0x00100000)?20:0;  // Data...
  table[7] = (b & 0x00200000)?20:0;  // Data...
  table[8] = (b & 0x00400000)?20:0;  // Data...
  table[9] = (b & 0x00800000)?20:0;  // Data...
  
  swiicStart();
  swiicSendBytes (table, 10);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us
  
  // Byte 3
  col--; // next column...
  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write
  table[1] = (col*24)+rgb;
  
  // Store the 4th byte
  table[2] = (b & 0x01000000)?20:0;  // Data...
  table[3] = (b & 0x02000000)?20:0;  // Data...
  table[4] = (b & 0x04000000)?20:0;  // Data...
  table[5] = (b & 0x08000000)?20:0;  // Data...
  table[6] = (b & 0x10000000)?20:0;  // Data...
  table[7] = (b & 0x20000000)?20:0;  // Data...
  table[8] = (b & 0x40000000)?20:0;  // Data...
  table[9] = (b & 0x80000000)?20:0;  // Data...
  
  swiicStart();
  swiicSendBytes (table, 10);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us

}

/*
  Setup a led color, given the led coordinates x and y and the color components, r, g, b.

  Convention for coordinates in this function: the SD card reader is
  turned to the bottom and the joystick is in the top right corner.

  x and y are in [0 , 7]
  r, g and b color components are integers on 6 bits only (0..63)
  
*/
void hatSetLed (unsigned char x, unsigned char y, char r, char g, char b) {
  unsigned char table[5];

  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write

  table[1] = x*24+y+0;    // Address of the first pixel R part
  table[2] = (r & 0x3F);         // Data...
  
  swiicStart();
  swiicSendBytes (table, 3);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us
  //sysWait(1);

  table[1] = x*24+y+8;  // Address of the first pixel G part
  table[2] = (g & 0x3F);         // Data...
  
  swiicStart();
  swiicSendBytes (table, 3);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us
  //sysWait(1);

  table[1] = x*24+y+16; // Address of the first pixel B part
  table[2] = (b & 0x3F);         // Data...
  
  swiicStart();
  swiicSendBytes (table, 3);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us
  //sysWait(1);

}

/*
  Fill in the hat led matrix with one single color.
 * */
void hatFull (char rgb) {
  hatShowInt (0xffffffff, 0, rgb);
  hatShowInt (0xffffffff, 4, rgb);
}

/*
  Send first a W with the I2C read address then read a byte

  This call also clears the KEY_INT pin of the HAT, ie GPIO23.

 */
unsigned char hatReadJoy () {
  unsigned char table[5];
  unsigned char dir;

  table[0] = (HAT_I2C_ADDRESS*2) + I2C_WRITE; // Slave address / Write
  table[1] = HAT_JOY_SUBADDRESS;   // Address of the joystick 'register'
  //table[2] = r;  // Data...
  
  swiicStart();
  swiicSendBytes (table, 2);
  //swiicStop();
  //sysWait (1);

  table[0] = (HAT_I2C_ADDRESS*2) + I2C_READ; // Slave address / Read
  //swiicStart();
  swiicRestart(); // Try a restart instead (same as start w/o stop)
  swiicReadData (table, 2);
  swiicStop();
  RPI_waitMicroSeconds(100); // 100us

  return (table[1]);
}
