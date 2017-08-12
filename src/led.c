
#include "RPI2_SYSTIMER.h"
#include "led.h"


//void ledInit (void) {
//  // As a default statem set the output in low state. Do it before
//  // setting the IO function so that state will be ready and will not
//  // change after setup.
//  RPI_getGpio()->LED_GPCLR = (1 << LED_GPIO_BIT);
//
//  /* Write 1 to the GPIO47 init nibble in the Function Select 4 GPIO
//     peripheral register to enable GPIO47 as an output */
//  // Former syntax: gpio[LED_GPFSEL] |= (1 << LED_GPFBIT);
//  RPI_getGpio()->LED_GPFSEL |= (1 << LED_GPFBIT);
//  
//}

void ledsInit (void) {
  // As a default statem set the output in low state. Do it before
  // setting the IO function so that state will be ready and will not
  // change after setup.
  gpioSet (LED1_GPIO_BIT, 0);
  gpioSet (LED2_GPIO_BIT, 0);

  // Set both pins in output
  gpioSetDir (35, GPIO_OUTPUT);
  gpioSetDir (47, GPIO_OUTPUT);
}

void setLeds (char leds, char state) {
  if (state == 0) {
    if (leds&LED1_MASK) led1OFF();
    if (leds&LED2_MASK) led2OFF();    
  }
  else {
    if (leds&LED1_MASK) led1ON();
    if (leds&LED2_MASK) led2ON();
  }
}

void led1ON (void) {
  /* Set the LED GPIO pin high */
  //getGPIO->LED1_GPSET = (1 << (LED1_GPIO_BIT-32));
  gpioSet (LED1_GPIO_BIT, 1);
}

void led2ON (void) {
  /* Set the LED GPIO pin high */
  //getGPIO->LED2_GPSET = (1 << (LED2_GPIO_BIT-32));
  gpioSet (LED2_GPIO_BIT, 1);
}

void led1OFF (void) {
  /* Set the LED GPIO pin low */
  //RPI_getGpio()->LED1_GPCLR = (1 << LED1_GPIO_BIT);
  //getGPIO->LED1_GPCLR = (1 << (LED1_GPIO_BIT-32));
  gpioSet (LED1_GPIO_BIT, 0);
}

void led2OFF (void) {
  /* Set the LED GPIO pin low */
  //RPI_getGpio()->LED_GPCLR = (1 << LED_GPIO_BIT);
  //getGPIO->LED2_GPCLR = (1 << (LED2_GPIO_BIT-32));
  gpioSet (LED2_GPIO_BIT, 0);
}

/* Blink the leds [leds] [n] times.
   [leds] is a bit field for selecting led1 (Power, red) and led2 (OK, green)
   T = [period] in us,
   duty cycle = [duty] in % (0 to 100)
   Final state is OFF.
   Synchronous function.
*/
void blinkLeds (char leds, unsigned int n, unsigned int period, char duty) {
  unsigned int i;
  unsigned int t1, t2;
  
  if (duty >= 100)
    t2 = period;
  else if (duty == 0)
    t2 = 0;
  else
    t2 = (period*duty) / 100;
  t1 = period - t2;
  
  for (i=0; i<n ; i++) {
    RPI_waitMicroSeconds(t1);
    if (leds&LED1_MASK) led1ON();
    if (leds&LED2_MASK) led2ON();
    RPI_waitMicroSeconds(t2);    
    if (leds&LED1_MASK) led1OFF();
    if (leds&LED2_MASK) led2OFF();
  }
}

// Blink the led [n] times.
// T=1s, d=80%
// Final state is OFF.
void blink (unsigned int n) {
  unsigned int i;

  for (i=0; i<n ; i++) {
    RPI_waitMicroSeconds(200000);    
    ledON();
    RPI_waitMicroSeconds(800000);    
    ledOFF();
  }
}

// Blink the led [n] times.
// T=1s, d=20%
// Final state is OFF.
void blink2 (unsigned int n) {
  unsigned int i;

  for (i=0; i<n ; i++) {
    RPI_waitMicroSeconds(800000);    
    ledON();
    RPI_waitMicroSeconds(200000);    
    ledOFF();
  }
}

// Blink the led [n] times.
// T=300ms, d=33%
// Final state is OFF.
void blink3 (unsigned int n) {
  unsigned int i;

  for (i=0; i<n ; i++) {
    RPI_waitMicroSeconds(200000);    
    ledON();
    RPI_waitMicroSeconds(100000);    
    ledOFF();
  }
}

// Blink the led [n] times.
// T=200ms, d=50%
// Final state is OFF.
void blink4 (unsigned int n) {
  unsigned int i;

  for (i=0; i<n ; i++) {
    RPI_waitMicroSeconds(100000);    
    ledON();
    RPI_waitMicroSeconds(100000);    
    ledOFF();
  }
}

#define MORSESHORT 200000
#define MORSELONG  600000
/*
  SOS blinking led hard coded to run from purely bare metal
  environment. Shalle be runnable from reset exception vector without
  any prio init.
*/
void standaloneSOS (void) {
  unsigned int i;
  ledsInit();
  for (i=0;i<MORSELONG;i++);

  while (1) {
    
    // S : . . .
    led1ON(); led2ON();
    for (i=0;i<MORSESHORT;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSESHORT;i++);
    
    led1ON(); led2ON();
    for (i=0;i<MORSESHORT;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSESHORT;i++);
    
    led1ON(); led2ON();
    for (i=0;i<MORSESHORT;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSELONG;i++);
    
    // O : _ _ _
    led1ON(); led2ON();
    for (i=0;i<MORSELONG;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSESHORT;i++);
    
    led1ON(); led2ON();
    for (i=0;i<MORSELONG;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSESHORT;i++);
    
    led1ON(); led2ON();
    for (i=0;i<MORSELONG;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSELONG;i++);
    
    // S : . . .
    led1ON(); led2ON();
    for (i=0;i<MORSESHORT;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSESHORT;i++);
    
    led1ON(); led2ON();
    for (i=0;i<MORSESHORT;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSESHORT;i++);
    
    led1ON(); led2ON();
    for (i=0;i<MORSESHORT;i++);
    led1OFF(); led2OFF();
    for (i=0;i<MORSELONG;i++);
    
    for (i=0;i<5*MORSELONG;i++);
    
  }
}
