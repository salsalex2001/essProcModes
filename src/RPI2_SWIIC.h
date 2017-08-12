

#ifndef RPI2_SWIIC_H
#define RPI2_SWIIC_H

#include "RPI2_GPIO.h"

extern volatile rpi_gpio_t * iicgpio;

#define I2C_READ   1
#define I2C_WRITE  0


// Define SDA1 on GPIO02
#define SDA1_GPSEL     GPFSEL0
#define SDA1_MASK      7<<GPIOx2
#define SDA1_GPFBIT    FSEL2
#define SDA1_GPSET     GPSET0
#define SDA1_GPCLR     GPCLR0
#define SDA1_GPIO_BIT  GPIO02

// Define SCL1 on GPIO03
#define SCL1_GPSEL     GPFSEL0
#define SCL1_MASK      7<<GPIOx3
#define SCL1_GPFBIT    FSEL3
#define SCL1_GPSET     GPSET0
#define SCL1_GPCLR     GPCLR0
#define SCL1_GPIO_BIT  GPIO03


// These macros can be use AFTER init has been invoked
#define SCL1_L    iicgpio->SCL1_GPCLR = (1<<SCL1_GPIO_BIT)
#define SCL1_H    iicgpio->SCL1_GPSET = (1<<SCL1_GPIO_BIT)
#define SDA1_L    iicgpio->SDA1_GPCLR = (1<<SDA1_GPIO_BIT)
#define SDA1_H    iicgpio->SDA1_GPSET = (1<<SDA1_GPIO_BIT)





extern void swiicInit (void);
extern void swiicStart (void);
extern void swiicRestart (void);
extern void swiicStop (void);

extern void swiicSendBit (char bit);
extern void swiicSendByte (char Byte);
extern void swiicSendBytes (unsigned char * bs, unsigned char nb);
extern char swiicGetAck (void);

extern void swiicReadByte (unsigned char * buf);
extern void swiicSendAck (char type);
extern void swiicReadData (unsigned char * buf, unsigned char nb);

extern void swiicResetBus (void);

#endif /* RPI2_SWIIC_H */
