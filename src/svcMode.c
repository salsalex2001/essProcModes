
#include <string.h>
#include <stdlib.h>

#include "RPI_BASE.h"
#include "RPI2_GPIO.h"
#include "RPI2_ARMTIMER.h"
#include "RPI2_SYSTIMER.h"
#include "RPI2_INTERRUPTS.h"
#include "RPI2_SWIIC.h"
#include "RPI2_AUX.h"

#include "svcMode.h"

#include "led.h"
#include "time.h"
#include "RPI_HAT.h"
#include "joystick.h"


unsigned char move;
unsigned int undefCount;
unsigned char nextMode;
unsigned char modeChange;

extern void moveLed (unsigned char * xled, unsigned char * yled, uint32_t * colorled,  unsigned char move);
extern void matrixCall (unsigned char x, unsigned char y, unsigned char move);






void supervisor_main (void) {
  unsigned int ctr;
  char i;
  unsigned int rxb;
  //unsigned int * fct;
  unsigned int res;
  //uint32_t c;
  unsigned char xled, yled, col;
  uint32_t colorled;
  sgpioEvtType gpioevt;
  
  ledsInit();  // Init leds and set both OFF
  swiicInit(); // Init software I2C driver
  joyInit();   // Init the joystick interface
  muInit();    // Init the Mini UART for logs on serial terminal
  hatClear();  // Switch the hat leds matrix off
  
  xled = 0;
  yled = 0;
  col = 0;
  colorled = 0x000A0000;
  undefCount = 0;
  nextMode = 1; //!!!
  modeChange = 0;

  //hatShowByte (r0, 1, 8);
  //hatShowByte (r1, 2, 8);
  //hatShowByte (r2, 3, 8);
  
  //blink(5);
  //sysWait (1000);

  // Enable periperals interrupts
  intEnable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
  intEnable (INT_GRP_IRQ2, EN_IRQ2_GPIO0 + EN_IRQ2_GPIO1 + EN_IRQ2_GPIO2 + EN_IRQ2_GPIO3);
  
  /* Setup the system timer interrupt */
  /* Timer frequency = Clk/256 * 0x400 */
  armTimerLoad(0x800);  
  
  // Get the processor mode from the CPSR register
  // Seems we start here in HYPervisor mode  
  res = _getCPSR();
  //res &= 0x000001F;
  //hatShowByte(res, 4, 16);
  hatShowInt(res, 4, 16);
  
  //sysWait (1000);

  /* Setup and start the ARM Timer */
  armTimerInit();
  armTimerStart();
  
  //hatSetLed (0,0, 0,0,40);
  hatSetLed (xled, yled, 0,0,20);  // OK
  led1OFF();
  led2ON();

  /* ARM level Enable interrupts */
  _enable_interrupts();
    
  /* Never exit as there is no OS to exit to! */
  //standaloneSOS();

  gpioevt.re = 1;
  gpioevt.fe = 0;
  gpioevt.hl = 0;
  gpioevt.ll = 0;
  gpioevt.are = 0;
  gpioevt.afe = 0;
  gpioSetEvent(JOY_INT_PORT, &gpioevt);

  //hatShowByte (0x87, 02); // Ok
  muLogString ("Salut SVC !!!");
  //muLogInt2Hex(_getSCR());
  muLogInt2Hex(_getSP());
  muLogInt2Hex(_getCPSR());
  muLogInt2Hex(_getVBAR());
  
  while (1) {
    //muLogString ("SVC loop!");
    //muLogInt2Hex(_getCPSR());
    if (move != 0) {
      //hatShowByte (move, col%8, 8*((col/8)%3));
      col++;
      moveLed (&xled, &yled, &colorled, move);
      matrixCall (xled, yled, move);
      move = 0;
    }

    if (modeChange != 0) {
      muLogString ("Mode changed");
      hatClear();
      res = _getCPSR();
      muLogInt2Hex(res);
      //res &= 0x000001F;
      //hatShowByte(res, 5, 16);
      hatShowInt(res, 3, 16);
      modeChange = 0;
    }
    
    if (muRxFifoNotEmpty()) {
      rxb = muGetRxByte();
      // Echo the received char
      muLogInt2Hex(rxb);
      switch (rxb) {
      case 0x30:
	muLogString("Enable Interrupts");
        _enable_interrupts();
	break;
      case 0x31:
	muLogString("CPSR = ");
	muLogInt2Hex(_getCPSR());
	break;
      }
    }
  }
  

}

