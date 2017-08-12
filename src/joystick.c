
#include "joystick.h"


void joyInit (void) {
  sgpioEvtType evt;
  evt.re = 0;
  evt.fe = 0;
  evt.hl = 1;
  evt.ll = 0;
  evt.are = 0;
  evt.afe = 0;

  gpioSetDir (JOY_INT_PORT, GPIO_INPUT);
  gpioSetEvent (JOY_INT_PORT, &evt);
  /*intEnable (JOY_INT_GRP, JOY_INT_FLAG);
  */
}

//unsigned char joyGetState (void) {
//  unsigned char state;
//  
//
//}
