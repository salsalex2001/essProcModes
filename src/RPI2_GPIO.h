

#ifndef RPI2_GPIO_H
#define RPI2_GPIO_H

#include "RPI_BASE.h"


/** The base address of the GPIO peripheral (ARM Physical Address) */
#define GPIO_BASE  (PERIPHERAL_BASE + 0x200000UL)


// List of registers and their relative addresses
#define GPIO_GPFSEL0    0
#define GPIO_GPFSEL1    1
#define GPIO_GPFSEL2    2
#define GPIO_GPFSEL3    3
#define GPIO_GPFSEL4    4
#define GPIO_GPFSEL5    5
#define GPIO_GPSET0     7
#define GPIO_GPSET1     8
#define GPIO_GPCLR0     10
#define GPIO_GPCLR1     11
#define GPIO_GPLEV0     13
#define GPIO_GPLEV1     14
#define GPIO_GPEDS0     16
#define GPIO_GPEDS1     17
#define GPIO_GPREN0     19
#define GPIO_GPREN1     20
#define GPIO_GPFEN0     22
#define GPIO_GPFEN1     23
#define GPIO_GPHEN0     25
#define GPIO_GPHEN1     26
#define GPIO_GPLEN0     28
#define GPIO_GPLEN1     29
#define GPIO_GPAREN0    31
#define GPIO_GPAREN1    32
#define GPIO_GPAFEN0    34
#define GPIO_GPAFEN1    35
#define GPIO_GPPUD      37
#define GPIO_GPPUDCLK0  38
#define GPIO_GPPUDCLK1  39

// Masks
#define GPIO_GPFSEL_MASK  0x07
#define GPIO_GPFSEL_LEN   0x03

typedef enum {
  GPIO_FSEL_INPUT   = 0,
  GPIO_FSEL_OUTPUT  = 1,
  GPIO_FSEL_FCT0    = 4,
  GPIO_FSEL_FCT1    = 5,
  GPIO_FSEL_FCT2    = 6,
  GPIO_FSEL_FCT3    = 7,
  GPIO_FSEL_FCT4    = 3,
  GPIO_FSEL_FCT5    = 2
} egpioFSEL;

typedef enum {
  GPIO_PUD_OFF   = 0,
  GPIO_PUD_PDWN  = 1,
  GPIO_PUD_PUP   = 2
} egpioPUD;

// GPIO index constants
// To be extended...
#define GPIO02      2
#define GPIO03      3
#define GPIO14     14
#define GPIO15     15
#define GPIO16     16
#define GPIO17     17
#define GPIO23     23
#define GPIO24     24
#define GPIO35     35
#define GPIO47     47

typedef struct {
  char re;
  char fe;
  char hl;
  char ll;  
  char are;
  char afe;
} sgpioEvtType;

// Functions selectors
// To be extended when further used...
#define FSEL2   6
#define FSEL3   9

#define FSEL16 18
#define FSEL35 15
#define FSEL47 21

/***/
#define GPIO_INPUT   0
#define GPIO_OUTPUT  1

#define RPI_GPIO_FSEL0_x0_INPUT     ( 0 )
#define RPI_GPIO_FSEL0_x0_OUTPUT    ( 1 )
#define RPI_GPIO_FSEL0_x1_INPUT     ( 0 << 3 )
#define RPI_GPIO_FSEL0_x1_OUTPUT    ( 1 << 3 )
#define RPI_GPIO_FSEL0_x2_INPUT     ( 0 << 6 )
#define RPI_GPIO_FSEL0_x2_OUTPUT    ( 1 << 6 )
#define RPI_GPIO_FSEL0_x3_INPUT     ( 0 << 9 )
#define RPI_GPIO_FSEL0_x3_OUTPUT    ( 1 << 9 )
#define RPI_GPIO_FSEL0_x4_INPUT     ( 0 << 12 )
#define RPI_GPIO_FSEL0_x4_OUTPUT    ( 1 << 12 )
#define RPI_GPIO_FSEL0_x5_INPUT     ( 0 << 15 )
#define RPI_GPIO_FSEL0_x5_OUTPUT    ( 1 << 15 )
#define RPI_GPIO_FSEL0_x6_INPUT     ( 0 << 18 )
#define RPI_GPIO_FSEL0_x6_OUTPUT    ( 1 << 18 )
#define RPI_GPIO_FSEL0_x7_INPUT     ( 0 << 21 )
#define RPI_GPIO_FSEL0_x7_OUTPUT    ( 1 << 21 )
#define RPI_GPIO_FSEL0_x8_INPUT     ( 0 << 24 )
#define RPI_GPIO_FSEL0_x8_OUTPUT    ( 1 << 24 )
#define RPI_GPIO_FSEL0_x9_INPUT     ( 0 << 27 )
#define RPI_GPIO_FSEL0_x9_OUTPUT    ( 1 << 27 )

#define GPIOx0 0
#define GPIOx1 3
#define GPIOx2 6
#define GPIOx3 9
#define GPIOx4 12
#define GPIOx5 15
#define GPIOx6 18
#define GPIOx7 21
#define GPIOx8 24
#define GPIOx9 27




/** The GPIO Peripheral is described in section 6 of the BCM2835 Peripherals
    documentation.

    There are 54 general-purpose I/O (GPIO) lines split into two banks. All
    GPIO pins have at least two alternative functions within BCM. The
    alternate functions are usually peripheral IO and a single peripheral
    may appear in each bank to allow flexibility on the choice of IO voltage.
    Details of alternative functions are given in section 6.2. Alternative
    Function Assignments.

    The GPIO peripheral has three dedicated interrupt lines. These lines are
    triggered by the setting of bits in the event detect status register. Each
    bank has its’ own interrupt line with the third line shared between all
    bits.

    The Alternate function table also has the pull state (pull-up/pull-down)
    which is applied after a power down. */
typedef struct {
  // GPFSEL registers to select the pin function
  rpi_reg_rw_t    GPFSEL0;
  rpi_reg_rw_t    GPFSEL1;
  rpi_reg_rw_t    GPFSEL2;
  rpi_reg_rw_t    GPFSEL3;
  rpi_reg_rw_t    GPFSEL4;
  rpi_reg_rw_t    GPFSEL5;
  rpi_reg_ro_t    Reserved0;
  // GPSET registers to set output state to high
  rpi_reg_wo_t    GPSET0;
  rpi_reg_wo_t    GPSET1;
  rpi_reg_ro_t    Reserved1;
  // GPCLR registers to reset the output state to low
  rpi_reg_wo_t    GPCLR0;
  rpi_reg_wo_t    GPCLR1;
  rpi_reg_ro_t    Reserved2;
  // GPLEV registers to get the input logical level
  rpi_reg_wo_t    GPLEV0;
  rpi_reg_wo_t    GPLEV1;
  rpi_reg_ro_t    Reserved3;
  // GPEDS registers for event status
  rpi_reg_wo_t    GPEDS0;
  rpi_reg_wo_t    GPEDS1;
  rpi_reg_ro_t    Reserved4;
  // GPREN register for rising edge detection
  rpi_reg_wo_t    GPREN0;
  rpi_reg_wo_t    GPREN1;
  rpi_reg_ro_t    Reserved5;
  // GPREN register for falling edge detection
  rpi_reg_wo_t    GPFEN0;
  rpi_reg_wo_t    GPFEN1;
  rpi_reg_ro_t    Reserved6;
  // GPREN register for high level detection
  rpi_reg_wo_t    GPHEN0;
  rpi_reg_wo_t    GPHEN1;
  rpi_reg_ro_t    Reserved7;
  // GPREN register for low level detection
  rpi_reg_wo_t    GPLEN0;
  rpi_reg_wo_t    GPLEN1;
  rpi_reg_ro_t    Reserved8;
  // GPREN register for asynchronous rising edge detection
  rpi_reg_wo_t    GPAREN0;
  rpi_reg_wo_t    GPAREN1;
  rpi_reg_ro_t    Reserved9;
  // GPREN register for asynchronous falling edge detection
  rpi_reg_wo_t    GPAFEN0;
  rpi_reg_wo_t    GPAFEN1;
  rpi_reg_ro_t    Reserved10;
  // GPPUD for pull-up or pull-down setup
  rpi_reg_wo_t    GPPUD;
  // GPPUD for pull-up or pull-down clock setup
  rpi_reg_wo_t    GPPUDCLK0;
  rpi_reg_wo_t    GPPUDCLK1;
  rpi_reg_ro_t    Reserved11;
} rpi_gpio_t;



// Public data
extern volatile rpi_gpio_t * rpiGpio;
extern volatile uint32_t* rpiGpiot;
// Public services
extern volatile rpi_gpio_t* RPI_getGpio(void);

/*
  Currently N/A
*/
extern void RPI_gpioInit(void);

/*
  gpioSetDir set the GPIO pin direction or function.
*/
extern void gpioSetDir (unsigned int io, egpioFSEL typ);

/*
  gpioSetIF set the GPIO pull-up or down, for one single pin.
*/
extern void gpioSetIF (unsigned int io, egpioPUD typ);

extern void gpioSet (unsigned int io, char state);
extern unsigned char gpioGet (unsigned int io);
extern unsigned char gpioGetFiltered (unsigned int io);

extern void gpioSetEvent (unsigned int io, sgpioEvtType * evts);
extern void gpioClearEvent (unsigned int io);
extern uint32_t gpioGetEvent (unsigned int io);


#define getGPIO rpiGpio

#endif /* RPI2_GPIO_H */
