
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


#define IRQ_BASIC 0x3F00B200
#define IRQ_PEND1 0x3F00B204
#define IRQ_PEND2 0x3F00B208
#define IRQ_FIQ_CONTROL 0x3F00B210
#define IRQ_ENABLE_BASIC 0x3F00B218
#define IRQ_DISABLE_BASIC 0x3F00B224


// Low level interrupt mgt - CPSR masking

void ei (void) {
  _enable_interrupts();
}

void di (void) {
  _disable_interrupts();

}

//------------------------------------------------------------------------------
// Forme API, disabled
//------------------------------------------------------------------------------
// volatile uint32_t* rpiIntCtlrt2 = (uint32_t*)INTCTRLR_BASE;
// /** @brief The BCM2835 Interupt controller peripheral at it's base address */
// static rpi_irq_controller_t* rpiIRQController = (rpi_irq_controller_t*)INTCTRLR_BASE;
// 
// /**
//     @brief Return the IRQ Controller register set
// */
// rpi_irq_controller_t* RPI_getIrqController (void) {
//   return rpiIRQController;
// }


/*
  intGrp: 0..2 (irq1, irq2, basic) - INT_GRP_xxx constants
  intFlag: bitfield or mask - EN_[]_xxx constants

  Not clearly specificied but considered for all GPIo that flag GPIO_int[0]..[3] allocated to, respectively GPEDS0, GPEDS1 and [2] or [3] supposed to be shared between first two.
*/
void intEnable (uint32_t intGrp, uint32_t intFlag) {
  uint32_t tmp;

  //tmp = GET32(_INTCTLR_EN_IRQ1 + 4*intGrp);
  //tmp |= (intFlag);
  PUT32 (_INTCTLR_EN_IRQ1 + 4*intGrp, intFlag);
}

void intDisable (uint32_t intGrp, uint32_t intFlag) {
  uint32_t tmp;

  //tmp = GET32(_INTCTLR_DIS_IRQ1 + 4*intGrp);
  //tmp = intFlag;
  PUT32 (_INTCTLR_DIS_IRQ1 + 4*intGrp, intFlag);
}


uint32_t intGetBasicPending (void) {
  return GET32 (_INTCTLR_IRQ_BASIC_PENDING);
}

uint32_t intGetIRQ1Pending (void) {
  return GET32 (_INTCTLR_IRQ_PENDING1);
}

uint32_t intGetIRQ2Pending (void) {
  return GET32 (_INTCTLR_IRQ_PENDING2);
}







char test (char x) {
  x++;
  return x;  
}


