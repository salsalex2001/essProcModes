/*
 * registers.h
 *
 *  Created on: Aug 3, 2017
 *      Author: alex
 */

#ifndef REGISTERS_H_
#define REGISTERS_H_

//-----------------------------------------------------------------------------
/*
 * Usage of X macros for register litterals defintions.
 *
 * Small change is introduced compared to orginal ones, in case of first value
 * has an offset. In order to save memory:
 * - the XXXX_OFFSET macros is added
 * - the getXxxxXXXXName macro is added as an accessor, using the above offset.
 * */
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
#define PROCMODE_TABLE \
X(usr, =0x10, "USER") \
X(fiq, =0x11, "FIQ")   \
X(irq, =0x12, "IRQ") \
X(svc, =0x13, "SUPERVISOR")   \
X(mon, =0x16, "MONITOR")   \
X(abt, =0x17, "ABORT")   \
X(hyp, =0x1A, "HYPERVISOR")   \
X(und, =0x1B, "UNDEFINED")   \
X(sys, =0x1F, "SYSEM")

#define PROCMODE_OFFSET 0x10

#define X(a, b, c) a b,
enum PROCMODE {
  PROCMODE_TABLE
};
#undef X

#define X(a, b, c) [a - PROCMODE_OFFSET]=c,
const char *procmode_name[] = {
  PROCMODE_TABLE
};
#undef X

#define getProcModeName(m) procmode_name[m - PROCMODE_OFFSET]


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------











#endif /* REGISTERS_H_ */
