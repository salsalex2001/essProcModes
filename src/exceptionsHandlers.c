

#include <stdint.h>
#include <stdbool.h>

#include "RPI_BASE.h"
#include "RPI2_AUX.h"
#include "RPI2_ARMTIMER.h"
#include "RPI2_GPIO.h"
#include "RPI2_INTERRUPTS.h"

#include "led.h"
#include "joystick.h"
#include "RPI_HAT.h"

#include "printf.h"


extern void hyp2svc (void);
extern void PUT32 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );


extern unsigned char move;
extern unsigned char nextMode;

unsigned int icount1;
unsigned int icount2;
extern unsigned int undefCount;
extern unsigned char modeChange;



//------------------------------------------------------------------------------
// Private services
//------------------------------------------------------------------------------
static void intTimer (void);
static void intGPIO (void);



/*
See ARM Arch 7 B1.9.2
*/
void c_hypUndefHandler (void) {
  uint32_t res;
  muLogString ("hypUndefHandler");
  //hatSetLed (7,7,20,20,undefCount%64);
  hatSetLed (7,7,20,20,0); // YELLOW top right led
  undefCount++;
  if (nextMode == 1) {
    //modeChange = 1;
    //hyp2svc();
    hatSetLed(0,7,20,15,0);
    //res = _getCPSR(); // Not allowed in PL2, only in PL1
    //hatShowInt(res,3,16); // ok untill here, the return of exception seems nok
  }
}

void c_nsUndefHandler (void) {
  uint32_t res;
  muLogString ("nsUndefHandler");
  //hatSetLed (7,7,0,undefCount%64,20);
  hatSetLed (7,7,20,0,0); // RED top right led
  undefCount++;
  if (nextMode == 1) {
    //modeChange = 1;
    //hyp2svc();
    hatSetLed(0,7,20,15,0);
    res = _getCPSR();
    hatShowInt(res,3,16); // ok untill here, the return of exception seems nok
  }
}

void c_sUndefHandler (void) {
  uint32_t res;
  muLogString ("sUndefHandler");
  //hatSetLed (7,7,0,20,undefCount%64);
  hatSetLed (7,7,0,20,0); // GREEN top right led
  undefCount++;
  if (nextMode == 1) {
    //modeChange = 1;
    //hyp2svc();
    hatSetLed(0,7,20,15,0); // YELLOW 0,7
    res = _getCPSR();
    hatShowInt(res,3,16); // ok untill here, the return of exception seems nok
    muLogInt2Hex(res);
  }
}

void c_hypervisorCallHandler (void) {
  uint32_t res;
  muLogString ("hypCallHandler");

  hatSetLed (7,6,20,15,0); // YELLOW 7,6

  if (nextMode == 1) {
    modeChange = 1;
    hyp2svc();
    hatSetLed(0,7,20,15,0); // YELLOW 0,7
    res = _getCPSR();
    hatShowInt(res,3,16); // ok until here, the return of exception seems nok
    muLogInt2Hex(res);
    piprintf("CPSR = 0x%08X\n\r", res);
  }



  
}

void c_monitorHandler (void) {
  muLogString ("monitortHandler");
  
  hatSetLed (7,6,0,20,20);  
}

// Alternate interrupt handler which is wrapped with HYP comapatible
// prologue and epilogue.
void c_hypInterruptHandler (void) {
  uint32_t pending;

  //----------------------------------------------------------------------------
  // Check BASIC group first 
  pending = intGetBasicPending();
  // Check the interrupt source 
  //if (rpiIntCtlrt[INTCTLR_IRQ_BASIC_PENDING] != 0) {
  if (pending != 0) {
    // There is a basic interrupt...
    //if (rpiIntCtlrt[INTCTLR_IRQ_BASIC_PENDING] & 0x01) {
    if (pending & IRQ_BASIC_PDNG_ARM_TIMER_IRQ) {
      intTimer();
    }
    // Other interrupts we ignore for now (no need)
  }
  
  //----------------------------------------------------------------------------
  // Check IRQ1 group then
  //if (rpiIntCtlrt[INTCTLR_IRQ_PENDING1] != 0) {
  if (intGetIRQ1Pending() != 0) {
    // There is a IRQ1 group interrupt (0..31)...
    
  }
  
  //----------------------------------------------------------------------------
  // Check IRQ2 group then
  pending = intGetIRQ2Pending();
  //if (rpiIntCtlrt[INTCTLR_IRQ_PENDING2] != 0) {
  if (pending != 0) {
    // There is an IRQ2 group interrupt (32..63)... 
    //if (rpiIntCtlrt[INTCTLR_IRQ_PENDING2] & 0x001E0000) { // This let all GPIO it pass...
    if (pending & 0x001E0000) { // This let all GPIO it pass...
      //if (rpiIntCtlrt[INTCTLR_IRQ_PENDING2] & EN_IRQ2_GPIO0) { // gpio_int[0] only
      if (pending & EN_IRQ2_GPIO0) { // gpio_int[0] only
        intGPIO();
        //hatShowInt(gpioGetEvent(JOY_INT_PORT), 4, 16); // OK, no more need
        // Acknowledge the key event to the Hat (will reset the GPIO JOY_INT_PORT)
        move = hatReadJoy();
        // Clear the GPIO event clears the interrupt
        gpioClearEvent(JOY_INT_PORT);
      }
      else {
        // Other GPIO groups...
        //hatShowInt(gpioGetEvent(JOY_INT_PORT), 4, 8); // OK, no more need
      }
    }
    else {
      
    }
  }  
  
}

// Interrupt handler for Non secure modes
void c_nsInterruptHandler (void) {
  //muLogString ("nsInterruptHandler");
  c_hypInterruptHandler(); // TEMP !!!!
  
}

// Interrupt handler for Secure modes
void c_sInterruptHandler (void) {
  muLogString ("sInterruptHandler");
  
  
}

void c_mInterruptHandler (void) {
  muLogString ("mInterruptHandler");

}

void c_mFastInterruptHandler (void) {
  muLogString ("mFastInterruptHandler");

}

// Alternate fast interrupt handler which is wrapped with HYP comapatible
// prologue and epilogue.
void c_hypFastInterruptHandler (void) {
  muLogString ("hypFastInterruptHandler");

  
}

void c_fastInterruptHandler (void) {
  muLogString ("fastInterruptHandler");
  
}




void c_hypervisorHandler (void) {
  muLogString ("Hypervisor");

  hatClear();
  hatShowInt(0xFFFFFFFF,0,8); // GREEN full "screen"
  hatShowInt(0xFFFFFFFF,4,8);
  
}





void intGPIO (void) {
  icount1++;
  if (icount1 & 1) {
    led1ON();
    //muLogString ("Button pressed");
  }
  else {
    led1OFF();
  }

  // Should be more selective here....
  //rpiGpiot[GPIO_GPEDS0 + INT_GRP_IRQ2] = 0;
}

void intTimer (void) {
  icount2++;
  if (icount2 & 1) {
    led2ON();
  }
  else {
    led2OFF();
  }

  // Clear the ARM timer interrupt
  armTimerInterruptClear();
}


//------------------------------------------------------------------------------
// C-style interrupts handler
//------------------------------------------------------------------------------



/**
    @brief The Reset vector interrupt handler

    This can never be called, since an ARM core reset would also reset the
    GPU and therefore cause the GPU to start running code again until
    the ARM is handed control at the end of boot loading
*/
void __attribute__((interrupt("ABORT"))) reset_vector(void) {
  while (1) {
    //gpio[GPIO_GPSET1] = (1 << LED_GPIO_BIT);
    // DOES NOT COMPILE BUT CANNOT GET WHY ???????? getGPIO->GPSET1 = (1 << LED2_GPIO_BIT);
    led2ON();
    //blink3(5);
  }
}

/**
    @brief The undefined instruction interrupt handler

    If an undefined intstruction is encountered, the CPU will start
    executing this function. Just trap here as a debug solution.
*
void __attribute__((interrupt("UNDEF"))) undefined_instruction_vector(void) {
  while (1) {
    // gpio[GPIO_GPSET1] = (1 << LED_GPIO_BIT);
    led2ON();
    // Do Nothing!
    //blink3(5);
  }
}
*/

/**
    @brief The supervisor call interrupt handler

    The CPU will start executing this function. Just trap here as a debug
    solution.
*/
//void __attribute__((interrupt("SWI"))) software_interrupt_vector(void) {
void __attribute__((interrupt("SWI"))) supervisor_call_vector(void) {
  while (1) {
    //gpio[GPIO_GPSET1] = (1 << LED_GPIO_BIT);
    led2ON();
    /* Do Nothing! */
    //blink3(5);
  }
}


/**
    @brief The prefetch abort interrupt handler

    The CPU will start executing this function. Just trap here as a debug
    solution.
*/
void __attribute__((interrupt("ABORT"))) prefetch_abort_vector(void) {
  led2ON();
  while (1) {
    //gpio[GPIO_GPSET1] = (1 << LED_GPIO_BIT);
    //blink3(5);
  }
}


/**
   @brief The Data Abort interrupt handler
   
   The CPU will start executing this function. Just trap here as a debug
   solution.
*/
void __attribute__((interrupt("ABORT"))) data_abort_vector(void) {
  led2ON();
  while (1) {
    //gpio[GPIO_GPSET1] = (1 << LED_GPIO_BIT);
    //blink3(5);
  }
}


/**
   @brief The IRQ Interrupt handler
   
   This handler is run every time an interrupt source is triggered. It's
   up to the handler to determine the source of the interrupt and most
   importantly clear the interrupt flag so that the interrupt won't
   immediately put us back into the start of the handler again.
   
   This cannot run in HYP mode because need an ERET to exit that is
   not set by gcc..
*/
void __attribute__((interrupt("IRQ"))) interrupt_vector(void) {
  static int lit = 0;
  
  /* Flip the LED */
  if (lit) {
    led2OFF();
    lit = 0;
  }
  else {
    led2ON();
    lit = 1;
  }

  /* Clear the ARM Timer interrupt - it's the only interrupt we have
     enabled, so we want don't have to work out which interrupt source
     caused us to interrupt */
  RPI_getArmTimer()->IRQClear = 0;
}

/**
    @brief The FIQ Interrupt Handler

    The FIQ handler can only be allocated to one interrupt source. The FIQ has
    a full CPU shadow register set. Upon entry to this function the CPU
    switches to the shadow register set so that there is no need to save
    registers before using them in the interrupt.

    In C you can't see the difference between the IRQ and the FIQ interrupt
    handlers except for the FIQ knowing it's source of interrupt as there can
    only be one source, but the prologue and epilogue code is quite different.
    It's much faster on the FIQ interrupt handler.

    The prologue is the code that the compiler inserts at the start of the
    function, if you like, think of the opening curly brace of the function as
    being the prologue code. For the FIQ interrupt handler this is nearly
    empty because the CPU has switched to a fresh set of registers, there's
    nothing we need to save.

    The epilogue is the code that the compiler inserts at the end of the
    function, if you like, think of the closing curly brace of the function as
    being the epilogue code. For the FIQ interrupt handler this is nearly
    empty because the CPU has switched to a fresh set of registers and so has
    not altered the main set of registers.
*/
void __attribute__((interrupt("FIQ"))) fast_interrupt_vector(void) {
  led2ON();
  while (1) {
    //gpio[GPIO_GPSET1] = (1 << LED_GPIO_BIT);
    //blink3(5);
  }
}
