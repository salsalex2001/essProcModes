


#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "RPI2_GPIO.h"
#include "RPI2_INTERRUPTS.h"



#define JOY_INT_PORT GPIO23

#define JOY_INT_GRP   INT_GRP_IRQ2
#define JOY_INT_FLAG  50

#define JOY_UP    (1<<1)
#define JOY_DOWN  (1<<4)
#define JOY_LEFT  (1<<2)
#define JOY_RIGHT (1<<0)
#define JOY_CLICK (1<<3)

extern void joyInit (void);


#endif /* JOYSTICK_H */
