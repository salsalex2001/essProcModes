/*
*/

#ifndef RPI_BASE_H
#define RPI_BASE_H

#include <stdint.h>

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
#ifdef RPI2
  #define PERIPHERAL_BASE     0x3F000000UL
#else
  #define PERIPHERAL_BASE     0x20000000UL
#endif

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
typedef volatile uint32_t rpi_reg_rw_t;
typedef volatile const uint32_t rpi_reg_ro_t;
typedef volatile uint32_t rpi_reg_wo_t;

typedef volatile uint64_t rpi_wreg_rw_t;
typedef volatile const uint64_t rpi_wreg_ro_t;


//------------------------------------------------------------------------------
// Common services
//------------------------------------------------------------------------------



extern void PUT32 ( unsigned int, unsigned int );
extern void PUT16 ( unsigned int, unsigned int );
extern void PUT8 ( unsigned int, unsigned int );
extern unsigned int GET32 ( unsigned int );

extern void _enable_interrupts (void);
extern void _disable_interrupts (void);

extern uint32_t _getSP (void);
extern uint32_t _getSPA (void);
extern uint32_t _getSPhyp (void);
extern uint32_t _getSPsvr (void);
extern uint32_t _getSPB (void);
extern uint32_t _getCPSR (void);
extern uint32_t _getSCTLR (void);
extern uint32_t _getHVBAR (void);
extern uint32_t _getHVBAR (void);
extern uint32_t _getVBAR (void);
extern uint32_t _getSCR (void);
extern uint32_t _getHCR (void);

extern void _deadReset (void);

extern void _trigUndef (void);
extern void _callSVC (void);
extern void _callHYP (void);
extern void _svcInit (void);


#endif
