
#include <stdint.h>
#include "RPI2_ARMTIMER.h"

static rpi_arm_timer_t* rpiArmTimer = (rpi_arm_timer_t*)RPI_ARMTIMER_BASE;

rpi_arm_timer_t* RPI_getArmTimer (void) {
  return rpiArmTimer;
}

/*
*/
void armTimerInit (void) {
  PUT32 (ARMTIMER_CTRL, ARMTIMER_CTRL_INT_ENABLE | ARMTIMER_CTRL_23BIT | ARMTIMER_CTRL_PRESCALE_256);
}

/*
*/
void armTimerStart (void) {
  uint32_t r;
  r = GET32 (ARMTIMER_CTRL);
  r |= ARMTIMER_CTRL_ENABLE;
  PUT32 (ARMTIMER_CTRL, r);
}

/*
*/
void armTimerStop (void) {
  uint32_t r;
  r = GET32 (ARMTIMER_CTRL);
  r &= ~ARMTIMER_CTRL_ENABLE;
  PUT32 (ARMTIMER_CTRL, r);
}

/*
*/
void armTimerLoad (uint32_t ld) {
  PUT32 (ARMTIMER_LOAD, ld);
  return;
}

/*
*/
void armTimerReload (uint32_t ld) {
  PUT32 (ARMTIMER_RELOAD, ld);
  return;
}

/*
  Returns the effective interrupt status (if enabled).
*/
char armTimerInterrupted (void) {
  return (char)(GET32 (ARMTIMER_IRQ_MASK));
}

void armTimerInterruptClear() {
  PUT32 (ARMTIMER_IRQ_CLR, 0);
}
