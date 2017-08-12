
#include "RPI2_SYSTIMER.h"
#include "time.h"

// Active timer
void sysWait (unsigned int ms) {
  RPI_waitMicroSeconds(ms*1000);
}
