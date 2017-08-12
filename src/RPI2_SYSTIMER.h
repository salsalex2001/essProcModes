

#ifndef RPI2_TIMER_H
#define RPI2_TIMER_H

#include "RPI_BASE.h"

#ifdef RPI2
  #define TIMER_BASE 0x3F003000UL
#else
  #define TIMER_BASE 0x20003000UL
#endif


#define TIMER_CTRL_STS   0x00
#define TIMER_CNT_LOW    0x04
#define TIMER_CNT_HIGH   0x08
#define TIMER_COMP0      0x0C
#define TIMER_COMP1      0x10
#define TIMER_COMP2      0x14
#define TIMER_COMP3      0x18




typedef struct {
  volatile uint32_t control_status;
  volatile uint32_t counter_lo;
  volatile uint32_t counter_hi;
  volatile uint32_t compare0;
  volatile uint32_t compare1;
  volatile uint32_t compare2;
  volatile uint32_t compare3;
} rpi_sys_timer_t;


extern void RPI_waitMicroSeconds (uint32_t us);

#endif // RPI2_TIMER_H
