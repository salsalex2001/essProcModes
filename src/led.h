#ifndef LED_H
#define LED_H

#include "RPI2_GPIO.h"

// Constants for the red led (led 1, power) mapping 
#if defined( RPIBPLUS ) || defined( RPI2 )
// The led1 is mapped on GPIO 35 
  #define LED1_GPIO_BIT    GPIO35
//(35-32) // modulo 32 (sizeof int)
#else
// The led is mapped on GPIO ??
  #define LED1_GPIO_BIT    16
#endif

// Constants for the green led (led 2, OK) mapping 
#if defined( RPIBPLUS ) || defined( RPI2 )
// The led2 is mapped on GPIO 47
  #define LED2_GPIO_BIT    GPIO47
  #define LED_GPIO_BIT    LED2_GPIO_BIT
#else
// The led is mapped on GPIO 16
  #define LED2_GPIO_BIT    16
#endif

#define LED1_MASK 0x01
#define LED2_MASK 0x02

extern void ledsInit(void);
extern void setLeds (char leds, char state);
extern void blinkLeds (char leds, unsigned int n, unsigned int period, char duty);

// OLD APIs, to remove
extern void ledInit(void);
extern void ledON(void);
extern void ledOFF(void);

// OLD APIS, should be private
extern void led1ON(void);
extern void led1OFF(void);
extern void led2ON(void);
extern void led2OFF(void);

// OLD APIs...
extern void blink (unsigned int);
extern void blink2 (unsigned int);
extern void blink3 (unsigned int);
extern void blink4 (unsigned int);

void standaloneSOS (void);

#define LED_OFF()   do { RPI_getGpio()->LED_GPCLR = ( 1 << LED_GPIO_BIT ); } while (0)
#define LED_ON()  do { RPI_getGpio()->LED_GPSET = ( 1 << LED_GPIO_BIT ); } while (0)

#define ledON  led2ON
#define ledOFF led2OFF

#endif /* LED_H */
