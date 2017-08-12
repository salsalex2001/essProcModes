/*
*/

#ifndef RPI2_INTERRUPTS_H
#define RPI2_INTERRUPTS_H

#include <stdint.h>

#include "RPI_BASE.h"

//------------------------------------------------------------------------------
// Registers
//------------------------------------------------------------------------------
#define INTCTRLR_BASE                   (PERIPHERAL_BASE + 0xB000)

#define _INTCTLR_IRQ_BASIC_PENDING      (INTCTRLR_BASE + 0x200)
#define _INTCTLR_IRQ_PENDING1           (INTCTRLR_BASE + 0x204)
#define _INTCTLR_IRQ_PENDING2           (INTCTRLR_BASE + 0x208)
#define _INTCTLR_FIQ_CTRL               (INTCTRLR_BASE + 0x20C)
#define _INTCTLR_EN_IRQ1                (INTCTRLR_BASE + 0x210)
#define _INTCTLR_EN_IRQ2                (INTCTRLR_BASE + 0x214)
#define _INTCTLR_EN_BASIC_IRQ           (INTCTRLR_BASE + 0x218)
#define _INTCTLR_DIS_IRQ1               (INTCTRLR_BASE + 0x21C)
#define _INTCTLR_DIS_IRQ2               (INTCTRLR_BASE + 0x220)
#define _INTCTLR_DIS_BASIC_IRQ          (INTCTRLR_BASE + 0x224)



//-----------------------------------------------------------------------------
// Constants
//------------------------------------------------------------------------------
#define INT_GRP_IRQ1  0
#define INT_GRP_IRQ2  1
#define INT_GRP_BASIC 2

// IRQ tables
// 1/ Basic
#define IRQ_BASIC_ARM_TIMER_IRQ         (1 << 0)
#define IRQ_BASIC_ARM_MAILBOX_IRQ       (1 << 1)
#define IRQ_BASIC_ARM_DOORBELL_0_IRQ    (1 << 2)
#define IRQ_BASIC_ARM_DOORBELL_1_IRQ    (1 << 3)
#define IRQ_BASIC_GPU_0_HALTED_IRQ      (1 << 4)
#define IRQ_BASIC_GPU_1_HALTED_IRQ      (1 << 5)
#define IRQ_BASIC_ACCESS_ERROR_1_IRQ    (1 << 6)
#define IRQ_BASIC_ACCESS_ERROR_0_IRQ    (1 << 7)

// 2/ IRQ1 0...31
#define IRQ1_AUX                        (1 << 29)

// 3/ IRQ2 32...63
#define IRQ2_I2CSPI_SLV                 (1 << (43-32))
// Supposed to cover GPIO 0 to 31
#define IRQ2_GPIO0                      (1 << (49-32))
// Supposed to cover GPIO 32 to 63
#define IRQ2_GPIO1                      (1 << (50-32))
#define IRQ2_GPIO2                      (1 << (51-32))
#define IRQ2_GPIO3                      (1 << (52-32))
#define IRQ2_I2C                        (1 << (53-32))
#define IRQ2_SPI                        (1 << (54-32))
#define IRQ2_PCM                        (1 << (55-32))
#define IRQ2_UART                       (1 << (57-32))



// IRQ_BASIC_PENDING contants
#define IRQ_BASIC_PDNG_ARM_TIMER_IRQ    IRQ_BASIC_ARM_TIMER_IRQ


// IRQ_ENABLE_BASIC contants
/** @brief Bits in the Enable_Basic_IRQs register to enable various interrupts.
    See the BCM2835 ARM Peripherals manual, section 7.5 */
#define EN_BASIC_ARM_TIMER_IRQ          IRQ_BASIC_ARM_TIMER_IRQ
#define EN_BASIC_ARM_MAILBOX_IRQ        IRQ_BASIC_ARM_MAILBOX_IRQ   
#define EN_BASIC_ARM_DOORBELL_0_IRQ     IRQ_BASIC_ARM_DOORBELL_0_IRQ
#define EN_BASIC_ARM_DOORBELL_1_IRQ     IRQ_BASIC_ARM_DOORBELL_1_IRQ
#define EN_BASIC_GPU_0_HALTED_IRQ       IRQ_BASIC_GPU_0_HALTED_IRQ  
#define EN_BASIC_GPU_1_HALTED_IRQ       IRQ_BASIC_GPU_1_HALTED_IRQ  
#define EN_BASIC_ACCESS_ERROR_1_IRQ     IRQ_BASIC_ACCESS_ERROR_1_IRQ
#define EN_BASIC_ACCESS_ERROR_0_IRQ     IRQ_BASIC_ACCESS_ERROR_0_IRQ

#define EN_IRQ1_AUX                     IRQ1_AUX

#define EN_IRQ2_GPIO0                   IRQ2_GPIO0
#define EN_IRQ2_GPIO1                   IRQ2_GPIO1
#define EN_IRQ2_GPIO2                   IRQ2_GPIO2
#define EN_IRQ2_GPIO3                   IRQ2_GPIO3

//------------------------------------------------------------------------------
// Forme API, disabled
//------------------------------------------------------------------------------

// /** @brief See Section 7.5 of the BCM2836 ARM Peripherals documentation, the base
//     address of the controller is actually xxxxB000, but there is a 0x200 offset
//     to the first addressable register for the interrupt controller, so offset the
//     base to the first register */
// 
// #define INTERRUPT_CONTROLLER_BASE   (PERIPHERAL_BASE + 0xB200)
// 
// #define INTCTLR_IRQ_BASIC_PENDING  0
// #define INTCTLR_IRQ_PENDING1       1
// #define INTCTLR_IRQ_PENDING2       2
// #define INTCTLR_FIQ_CTRL           3
// #define INTCTLR_EN_IRQ1            4
// #define INTCTLR_EN_IRQ2            5
// #define INTCTLR_EN_BASIC_IRQ       6
// #define INTCTLR_DIS_IRQ1           7
// #define INTCTLR_DIS_IRQ2           8
// #define INTCTLR_DIS_BASIC_IRQ      9
// 
// /** @brief The interrupt controller memory mapped register set */
// typedef struct {
//   volatile uint32_t IRQ_basic_pending;  // 0x200
//   volatile uint32_t IRQ_pending_1;      // 0x204
//   volatile uint32_t IRQ_pending_2;	// 0x208
//   volatile uint32_t FIQ_control;	// 0x20C
//   volatile uint32_t Enable_IRQs_1;	// 0x210
//   volatile uint32_t Enable_IRQs_2;	// 0x214
//   volatile uint32_t Enable_Basic_IRQs;	// 0x218
//   volatile uint32_t Disable_IRQs_1;	// 0x21C
//   volatile uint32_t Disable_IRQs_2;	// 0x220
//   volatile uint32_t Disable_Basic_IRQs; // 0x224
// } rpi_irq_controller_t;
// 
// 
// //-----------------------------------------------------------------------------
// // Private services
// //-----------------------------------------------------------------------------
// rpi_irq_controller_t* RPI_getIrqController (void);

//-----------------------------------------------------------------------------
// Private services
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Public services
//-----------------------------------------------------------------------------
extern void di (void);
extern void ei (void);

extern void intEnable (uint32_t intGrp, uint32_t intFlag);
extern void intDisable (uint32_t intGrp, uint32_t intFlag);

extern uint32_t intGetBasicPending (void);
extern uint32_t intGetIRQ1Pending (void);
extern uint32_t intGetIRQ2Pending (void);


#endif
