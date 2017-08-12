
/*
  Duplicate with those in assembler, try to commonize afap.
*/

#define CPSR_MODE_USER         0x10
#define CPSR_MODE_FIQ          0x11
#define CPSR_MODE_IRQ          0x12
#define CPSR_MODE_SVC          0x13
#define CPSR_MODE_MONITOR      0x16 /* for Security extention only - not on PI */
#define CPSR_MODE_ABORT        0x17
#define CPSR_MODE_HYP          0x1A /* for Virtualization extensions only */
#define CPSR_MODE_UNDEFINED    0x1B
#define CPSR_MODE_SYSTEM       0x1F

#define CPSR_IRQ_INHIBIT       0x80 /* bit I in mask bits of S/CPSR */
#define CPSR_FIQ_INHIBIT       0x40 /* bit F in mask bits of S/CPSR */
#define CPSR_THUMB             0x20 /* bit T in S/CPSR */
