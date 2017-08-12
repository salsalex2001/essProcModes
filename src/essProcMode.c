//

// Set senseHat led individually

/*

  This progran uses te senseHat for interaction.
  It enables ARM timer interrupts as well as GPIO23 (joystick event).
  The timer interrupts just blinks the green led (led 2).
  The GPIO interrupt is used to acquire the move on the joystick.

 */

#include <string.h>
#include <stdlib.h>

#include "RPI_BASE.h"
#include "RPI2_GPIO.h"
#include "RPI2_ARMTIMER.h"
#include "RPI2_SYSTIMER.h"
#include "RPI2_INTERRUPTS.h"
#include "RPI2_SWIIC.h"
#include "RPI2_AUX.h"

#include "syscst.h"
#include "svcMode.h"

#include "convert.h"
#include "printf.h"
#include "keyboard.h"

#include "led.h"
#include "time.h"
#include "RPI_HAT.h"
#include "joystick.h"

//volatile unsigned int* gpio;
//volatile rpi_gpio_t * gpio;



unsigned char move;
unsigned int undefCount;
unsigned char nextMode;
unsigned char modeChange;

void moveLed (unsigned char * xled, unsigned char * yled, uint32_t * colorled,  unsigned char move);
void matrixCall (unsigned char x, unsigned char y, unsigned char move);


/* Updated main for the bootloader match */
void hyp_main (unsigned int r0, unsigned int r1, unsigned int r2) {
  unsigned int ctr;
  char i;
  //unsigned int * src;
  //unsigned int * fct;
  uint32_t rxb;
  unsigned int res;
  uint32_t c;
  unsigned char xled, yled, col;
  uint32_t colorled;
  sgpioEvtType gpioevt;
  
  ledsInit();  // Init leds and set both OFF
  swiicInit(); // Init software I2C driver
  joyInit();   // Init the joystick interface
  kbdInit();   // Init the keyboard
  muInit();    // Init the Mini UART for logs on serial terminal
  hatClear();  // Switch the hat leds matrix off
  
  xled = 0;
  yled = 0;
  col = 0;
  colorled = 0x000A0000;
  undefCount = 0;
  nextMode = 1; //!!!
  modeChange = 0;

  hatShowByte (r0, 1, 8);
  hatShowByte (r1, 2, 8);
  hatShowByte (r2, 3, 8);
  
  //blink(5);
  //sysWait (1000);

  // Enable peripherals interrupts
  intEnable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
  intEnable (INT_GRP_IRQ2, EN_IRQ2_GPIO0 + EN_IRQ2_GPIO1 + EN_IRQ2_GPIO2 + EN_IRQ2_GPIO3);
  
  /* Setup the system timer interrupt */
  /* Timer frequency = Clk/256 * 0x400 */
  armTimerLoad(0x800);  
  
  // Get the processor mode from the CPSR register
  // Seems we start here in HYPervisor mode  
  res = _getCPSR();
  //res &= 0x000001F;
  hatShowByte(res, 4, 8);
  
  //sysWait (1000);

  /* Setup and start the ARM Timer */
  armTimerInit();
  armTimerStart();
  
  //hatSetLed (0,0, 0,0,40);
  hatSetLed (xled, yled, 10,0,0);  // OK
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
  muLogString ("Salut ALeX !!!");
  
  while (1) {
    if (move != 0) {
      //hatShowByte (move, col%8, 8*((col/8)%3));
      col++;
      moveLed (&xled, &yled, &colorled, move);
      matrixCall (xled, yled, move);
      move = 0;
    }

    if (modeChange != 0) {
      armTimerStop();
      muLogString ("Mode changed 1234 5678 9ABC");
      //muLogString ("Leaving HYP to SVC");
      hatClear();
      res = _getCPSR();
      muLogInt2Hex(res);
      piprintf("CPSR (back in main loop) = %08X\n\r", res);
      //res &= 0x000001F;
      //hatShowByte(res, 5, 16);
      hatShowInt(res, 3, 16);
      modeChange = 0;
      if ((res & CPSR_MODE_SVC) == CPSR_MODE_SVC) {
        // Entry in SVC node
        muLogString ("Leaving HYP to SVC");



        //_svcInit();
        //supervisor_main();
      }
    }
    
    kbdScan();
    /*
    if (muRxFifoNotEmpty()) {
      // Echo the received char
      rxb = muGetRxByte();
      muLogInt2Hex(rxb);
      inputKbd (rxb);
      if (rxb == 0x30) {
        _enable_interrupts();
      }
    }*/
  }
  

}




void moveLed (unsigned char * xled, unsigned char * yled, uint32_t * colorled,  unsigned char move) {
  
  hatSetLed (*xled, *yled, 0, 0, 0);
  //hatClear();
  if (move & JOY_UP) {
    (*yled)++;
  }
  else if (move & JOY_DOWN) {
    (*yled)--;
  }
  else if (move & JOY_RIGHT) {
    (*xled)++;
  }
  else if (move & JOY_LEFT) {
    (*xled)--;
  }
  else if (move & JOY_CLICK) {
    (*colorled) += 0xD0;
    (*colorled) &= 0x003F3F3F;
  }
  else {
    // Erroneous move...
  }
  *yled %= 8;
  *xled %= 8;
  *colorled %= 0x1000000;
  
  hatSetLed (*xled, *yled, ((*colorled) & 0x3F0000)>>16, ((*colorled) & 0x3F00)>>8, ((*colorled) & 0x3F));

}

/*
  x, y [0..7]
  move is a joystick move constant char

*/
void matrixCall (unsigned char x, unsigned char y, unsigned char move) {
  unsigned int res;
  if (move == JOY_CLICK) {
    //------------------------------------------------------
    if (x == 0) {
      if (y == 0) {
        hatClear();
      }
      if (y == 1) {
        hatShowByte (0xFF, 7, 0);
      }
      if (y == 2) {
        hatShowInt (0x123557FA, 3, 0);
      }
      if (y == 3) {
        muLogInt2Hex (_getCPSR());
      }
      if (y == 4) {
        muLogInt2Bin (_getCPSR());
      }
      if (y == 6) {
        //armTimerStop();  // ok
        intDisable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
      }
      if (y == 7) {
        //armTimerStart();  // ok
        intEnable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
      }
    }

    //------------------------------------------------------
    if (x == 1) {
      // Shows the Proc mode
      if (y == 0) {
        res = _getCPSR();
        //res &= 0x1F;
        //hatShowByte(res,6,16);
        hatShowInt (res,3,8);
        muLogStrInt ("CPSR: ",res);
      }
      // Generate an undef instruction exception
      if (y == 1) {
        _trigUndef();
      }
      if (y == 6) {
        //armTimerStop();  // ok
        //intDisable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ); // ok
        _disable_interrupts();
      }
      if (y == 7) {
        //armTimerStart();  // ok
        //intEnable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ); // ok
        _enable_interrupts();
      }
    }

    //------------------------------------------------------
    if (x == 2) {
      if (y == 0) {
        // VBAR can be accessed only from HYP or MON when SCR.NS = 1
        res = _getCPSR();
        if ((res & CPSR_MODE_HYP) != CPSR_MODE_HYP) {
          muLogString ("HVBAR cannot be accessed out of HYP mode");
        }
        else {
          res = _getHVBAR();
          hatShowInt(res, 3, 8);
          muLogStrInt ("HVBAR: ",res);
        }
      }
      if (y == 1) {
        _callSVC();
      }
    }

    //------------------------------------------------------
    if (x == 3) {
      if (y == 0) {
        res = _getVBAR();
        hatShowInt(res, 3, 8);
        muLogStrInt ("VBAR: ",res);
      }
      if (y == 1) {
        // To be invoked from SVC
        _callHYP();
      }
    }
    
    //------------------------------------------------------
    if (x == 4) {
      if (y == 0) {
        // SCR can be accessed only from PL1 secure mode
        res = _getSCR();
        hatShowInt(res, 3, 8);
        muLogStrInt ("SCR: ",res);
      }
    }
    
    //------------------------------------------------------
    if (x == 5) {
      if (y == 0) {
        res = _getSCTLR();
        hatShowInt(res, 3, 8);
        muLogStrInt ("SCTLR: ",res);
      }
    }
    
    //------------------------------------------------------
    if (x == 6) {
      if (y == 0) {
        res = _getHCR();
        hatShowInt(res, 3, 8);
        muLogStrInt ("HCR: ",res);
      }
    }
    
  }




  
}


#if 0
void LeaveHypervisor() {
  asm (
    "push {r0-r8}\n"
    "mov r8, sp\n"
    "mrs r0,cpsr\n"
    "eor r0, r0, #0x1a\n" // hypervisor
    "tst r0, #0x1f\n"
    "bic r0, r0, #0x1f\n"
    "orr r0, r0, #0xd3\n"
    "bne lab1\n"
    "orr r0, r0, #0x100\n"
    "adr lr, lab2\n"
    "msr spsr_cxsf, r0\n"
    ".word 0xe12ef30e\n" // msr elr_hyp, lr
    ".word 0xe160006e\n" // eret
    "lab1: "
    "msr cpsr_c, r0\n"
    "b lab3\n"
    "lab2: "
    "nop\n"
    "mrc p15, 0, r0, c1, c0, 0\n" // read SCTLR
    "bic r0, #(1 << 13)\n" // force V = 0
    "mcr p15, 0, r0, c1, c0, 0\n" // write SCTLR
    "mrc p15, 0, r0, c1, c0, 2\n" // read CPACR
    "orr r0, #(0xf << 20)\n" // full access to CP10 & CP11
    "mcr p15, 0, r0, c1, c0, 2\n" // write CPACR
    "isb\n"
    "mov r0, #0x40000000\n"
    ".word 0xeee80a10\n" // vmsr fpexc, r0
    "mov r0, #0x8000\n"
    "mcr p15, 0, r0, c12, c0, 0\n" // write VBAR
    "lab3: "
    "mov sp, r8\n"
    "pop {r0-r8}\n"
  );
}
#endif
