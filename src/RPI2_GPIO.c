
#include <stdint.h>
#include "RPI2_SYSTIMER.h"
#include "RPI2_GPIO.h"

//static
volatile rpi_gpio_t* rpiGpio = (rpi_gpio_t*)GPIO_BASE;
volatile uint32_t* rpiGpiot = (uint32_t*)GPIO_BASE;

volatile rpi_gpio_t* RPI_getGpio(void) {
  return rpiGpio;
}

void RPI_gpioInit (void) {

}

void gpioSetDir (unsigned int io, egpioFSEL typ) {
  uint32_t tmp;
  char d, u, off;

  d = io / 10;
  u = io % 10;
  if (io < 32) {
    off = 0;
  }
  else {
    off = 1;
  }
  
  tmp = rpiGpiot [GPIO_GPFSEL0 + d];
  tmp &= ~(GPIO_GPFSEL_MASK << (u * GPIO_GPFSEL_LEN));
  tmp |= (typ << (u * GPIO_GPFSEL_LEN));
  rpiGpiot[GPIO_GPFSEL0 + d] = tmp;
  
}

void gpioSetIF (unsigned int io, egpioPUD typ) {
  char d, u, off;

  d = io / 32;
  u = io % 32;
  /*
  // Setup the pull-ups
  iicgpio->GPPUD = 0x02;     // Set the control signal for pull-up enabling
  RPI_waitMicroSeconds(150); // Wait 150 "cycles"
  iicgpio->GPPUDCLK0 = ( 1 << SDA1_GPIO_BIT + 1 << SCL1_GPIO_BIT ); // Enable the clock control
  RPI_waitMicroSeconds(150); // Wait for another 150 "cycles"
  iicgpio->GPPUD = 0x00;     // Disable the control signal
  iicgpio->GPPUDCLK0 = 0;    // Release the clock signal
  */
  rpiGpiot[GPIO_GPPUD] = typ;
  RPI_waitMicroSeconds(150); // Wait 150 "cycles"
  rpiGpiot[GPIO_GPPUDCLK0 + d] = (1 << u);
  RPI_waitMicroSeconds(150); // Wait 150 "cycles"
  rpiGpiot[GPIO_GPPUD] = 0;
  rpiGpiot[GPIO_GPPUDCLK0 + d] = 0;
}

void gpioSet (unsigned int io, char state) {
  uint32_t tmp;
  char d, u, off;

  d = io / 32;
  u = io % 32;
  /*if (io < 32) {
    off = 0;
  }
  else {
    off = 1;
    }*/
  if (state == 0) {
    rpiGpiot[GPIO_GPCLR0 + d] = (1 << u);
  }
  else{
    rpiGpiot[GPIO_GPSET0 + d] = (1 << u);
  }
}

unsigned char gpioGet (unsigned int io) {
  uint32_t tmp;
  char d, u, off;

  d = io / 32;
  u = io % 32;
  
  if (rpiGpiot[GPIO_GPLEV0 + d] &= (1 << u)) {
    return 1;
  }
  else {
    return 0;
  }
}

unsigned char gpioGetFiltered (unsigned int io) {
  char d, u;
  uint32_t ctr;
  unsigned char st, tmp;
  
  d = io / 32;
  u = io % 32;
  
  //1st sample
  st = rpiGpiot[GPIO_GPLEV0 + d] &= (1 << u);

  ctr = 0;
  while (ctr < 50) {
    tmp = rpiGpiot[GPIO_GPLEV0 + d] &= (1 << u);
    if (st == tmp) {
      // Same as previous sample, good, count...
      ctr++;
    }
    else {
      // different sample, reset counter and take new sample
      ctr = 0;
      st = tmp;
    }
  }
  
  if (st != 0) {
    return 1;
  }
  else {
    return 0;
  }
}

void gpioSetEvent (unsigned int io, sgpioEvtType * evts) {
  uint32_t tmp;
  char d, u, off;

  d = io / 32;
  u = io % 32;
  
  // Rising edge
  if (evts->re) {
    rpiGpiot[GPIO_GPREN0 + d] |= (1 << u);
  }
  // Falling edge
  if (evts->fe) {
    rpiGpiot[GPIO_GPFEN0 + d] |= (1 << u);
  }
  // High state
  if (evts->hl) {
    rpiGpiot[GPIO_GPHEN0 + d] |= (1 << u);
  }
  // Low state
  if (evts->ll) {
    rpiGpiot[GPIO_GPLEN0 + d] |= (1 << u);
  }
  // Async rising edge
  if (evts->are) {
    rpiGpiot[GPIO_GPAREN0 + d] |= (1 << u);
  }
  // Async falling edge
  if (evts->afe) {
    rpiGpiot[GPIO_GPAFEN0 + d] |= (1 << u);
  }
}

void gpioClearEvent (unsigned int io) {
  uint32_t tmp;
  char d, u, off;

  d = io / 32;
  u = io % 32;

  rpiGpiot[GPIO_GPEDS0 + d] |= (1 << u);
}

uint32_t gpioGetEvent (unsigned int io) {
  uint32_t tmp;
  char d, u, off;

  d = io / 32;
  u = io % 32;
  return rpiGpiot[GPIO_GPEDS0 + d];
}
