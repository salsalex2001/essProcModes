
#include <stdlib.h>
#include"RPI2_SYSTIMER.h"

// The RPI2 PLLs are already configured so that the system timer tick is 1us.

static rpi_sys_timer_t* rpiSystemTimer = (rpi_sys_timer_t*)TIMER_BASE;

// Active wait routine
void RPI_waitMicroSeconds (uint32_t us) {
  // Fetch current value of the lower counter (highest part is ignored here)
  volatile uint32_t ts = rpiSystemTimer->counter_lo;
  
  // Compare current counter with target value
  while ((rpiSystemTimer->counter_lo - ts ) < us) {
  // while (rpiSystemTimer->counter_lo < (ts + us)) {
    /* BLANK */
  }
}
