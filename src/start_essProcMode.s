
#.section ".text.startup"
.section ".text.hypvectors"

.global _start	
.global _startHyp
.global _svcInit
.global _exception_table
  
.global _enable_interrupts
.global _disable_interrupts
.global _setSP
.global _getSP
.global _getSPA
.global _getSPhyp
.global _getSPsvr
  
.global _getCPSR
.global _getSCTLR
.global _getHVBAR
.global _getVBAR
.global _getSCR
.global _getHCR

.global _trigUndef
.global _callSVC
.global _callHYP
  
.global hyp2svc

.global _deadReset

# From the ARM ARM (Architecture Reference Manual). Make sure you get the
# ARMv7-A documentation (which includes the ARMv6 and ARM5 documentations)
# which is the correct processor type for the Broadcom BCM2836

# See ARM section A2.2 (Processor Modes)
# See ARM section B1.3.1 (ARM processor modes)

.equ    CPSR_MODE_USER,         0x10
.equ    CPSR_MODE_FIQ,          0x11
.equ    CPSR_MODE_IRQ,          0x12
.equ    CPSR_MODE_SVR,          0x13
.equ    CPSR_MODE_MONITOR,      0x16 /* for Security extention only */
.equ    CPSR_MODE_ABORT,        0x17
.equ    CPSR_MODE_HYPERVISOR,   0x1A /* for Virtualization extensions only */
.equ    CPSR_MODE_UNDEFINED,    0x1B
.equ    CPSR_MODE_SYSTEM,       0x1F

# See ARM section A2.5 (Program Status Registers)
# See ARM section B1.3.3 (Program Status Registers)
.equ    CPSR_IRQ_INHIBIT,       0x80 /* bit I in mask bits of S/CPSR */
.equ    CPSR_FIQ_INHIBIT,       0x40 /* bit F in mask bits of S/CPSR */
.equ    CPSR_THUMB,             0x20 /* bit T in S/CPSR */

/* Hypervisor vectors table -----------------------------------------------------------------------*/
_startHyp:
_start:	
    ldr pc, _reset_h                            /* offset 0x00 - supposed to be not used, but needed for Boot loader */
    ldr pc, _undefined_instruction_hyp_vector_h /* offset 0x04 - undef instruction */
    ldr pc, _hypervisor_call_vector_h           /* offset 0x08 - hypervisor call or secure monitor */
    ldr pc, _prefetch_abort_hyp_vector_h        /* offset 0x0C - prefetch abort */
    ldr pc, _data_abort_hyp_vector_h            /* offset 0x10 - data abort */
    ldr pc, _hyp_trap_handler_h                 /* offset 0x14 - hyp trap */
    ldr pc, _interrupt_vector_h                 /* offset 0x18 - IRQ interrupt */
    ldr pc, _fast_interrupt_vector_h            /* offset 0x1C - FIQ interrupt */

#   ldr pc, _software_interrupt_vector_h /* SWI is the former name of the SVC call exception */
#   ldr pc, _supervisor_call_vector_h

_reset_h:                            .word   _resetFromHyp_
#_reset_h:                            .word   _resetFromHyp_toSupervisor
_reset2_h:                           .word   standaloneSOS
_undefined_instruction_hyp_vector_h: .word   undefined_instruction_hyp_event
_hypervisor_call_vector_h:           .word   hypervisor_call_event
_prefetch_abort_hyp_vector_h:        .word   prefetch_abort_hyp_event
_data_abort_hyp_vector_h:            .word   data_abort_hyp_event
_hyp_trap_handler_h:                 .word   hyp_trap_event
_interrupt_vector_h:                 .word   interrupt_hyp_event
_fast_interrupt_vector_h:            .word   fast_interrupt_hyp_event

#_interrupt_vector_h:                .word   interrupt_vector

.section ".text.nsvectors"  
/* Non-secure vectors table ----------------------------------------------------------------------*/
_startNS:
    ldr pc, _resetNS_h                          /* offset 0x00 - not used */
    ldr pc, _undefined_instruction_vectorNS_h	/* offset 0x04 - undefined instruction */
    ldr pc, _supervisor_call_vectorNS_h		/* offset 0x08 - supervisor call */
    ldr pc, _prefetch_abort_vectorNS_h		/* offset 0x0C - prefectch abort */
    ldr pc, _data_abort_vectorNS_h		/* offset 0x10 - data abort */
    ldr pc, _unused_handlerNS_h			/* offset 0x14 - not used */
    ldr pc, _interrupt_vectorNS_h		/* offset 0x18 - IRQ interrupt */
    ldr pc, _fast_interrupt_vectorNS_h          /* offset 0x1C - FIQ interrupt */

#   ldr pc, _software_interrupt_vectorNS_h /* SWI is the former name of the SVC call exception */

#_resetNS_h:                           .word   _reset_
_resetNS_h:                           .word   reset_ns_event
_undefined_instruction_vectorNS_h:    .word   undefined_instruction_ns_event
_supervisor_call_vectorNS_h:          .word   supervisor_call_ns_event
_prefetch_abort_vectorNS_h:           .word   prefetch_abort_ns_event
_data_abort_vectorNS_h:               .word   data_abort_ns_event
_unused_handlerNS_h:                  .word   standaloneSOS
_interrupt_vectorNS_h:                .word   interrupt_ns_event
_fast_interrupt_vectorNS_h:           .word   fast_interrupt_ns_event

.section ".text.svectors"  
/* Secure vectors table --------------------------------------------------------------------------*/
_startS:
    ldr pc, _resetS_h                           /* offset 0x00 - not used */
    ldr pc, _undefined_instruction_vectorS_h    /* offset 0x04 - undefined instruction */
    ldr pc, _supervisor_call_vectorS_h          /* offset 0x08 - supervisor call */
    ldr pc, _prefetch_abort_vectorS_h           /* offset 0x0C - prefectch abort */
    ldr pc, _data_abort_vectorS_h               /* offset 0x10 - data abort */
    ldr pc, _unused_handlerS_h                  /* offset 0x14 - not used */
    ldr pc, _interrupt_vectorS_h                /* offset 0x18 - IRQ interrupt */
    ldr pc, _fast_interrupt_vectorS_h           /* offset 0x1C - FIQ interrupt */


_resetS_h:                            .word   reset_s_event
_undefined_instruction_vectorS_h:     .word   undefined_instruction_s_event
_supervisor_call_vectorS_h:           .word   supervisor_call_s_event
_prefetch_abort_vectorS_h:            .word   prefetch_abort_s_event
_data_abort_vectorS_h:                .word   data_abort_s_event
_unused_handlerS_h:                   .word   standaloneSOS
_interrupt_vectorS_h:                 .word   interrupt_s_event
_fast_interrupt_vectorS_h:            .word   fast_interrupt_s_event

.section ".text.mvectors"  
/* Monitor vectors table -------------------------------------------------------------------------*/
_startM:
    ldr pc, _unused_handlerM_h                  /* offset 0x00 - not used */
    ldr pc, _unused_handlerM_h                  /* offset 0x04 - not used */
    ldr pc, _monitor_call_vectorM_h             /* offset 0x08 - secure monitor call */
    ldr pc, _prefetch_abort_vectorM_h           /* offset 0x0C - prefectch abort */
    ldr pc, _data_abort_vectorM_h               /* offset 0x10 - data abort */
    ldr pc, _unused_handlerM_h                  /* offset 0x14 - not used */
    ldr pc, _interrupt_vectorM_h                /* offset 0x18 - IRQ interrupt */
    ldr pc, _fast_interrupt_vectorM_h           /* offset 0x1C - FIQ interrupt */

@_resetM_h:                            .word   _reset_ 
@_undefined_instruction_vectorM_h:     .word   undefined_instruction_m_event
_monitor_call_vectorM_h:              .word   monitor_call_m_event
_prefetch_abort_vectorM_h:            .word   prefetch_abort_m_event
_data_abort_vectorM_h:                .word   data_abort_m_event
_unused_handlerM_h:                   .word   standaloneSOS
_interrupt_vectorM_h:                 .word   interrupt_m_event
_fast_interrupt_vectorM_h:            .word   fast_interrupt_m_event


  .section ".text.startup"
/* Reset from HYP mode to hypervisor -------------------------------------------------------------*/
_resetFromHyp_:
    # We enter execution in HYPPERVISOR mode. For more information on
    # processor modes see ARM Section A2.2 (Processor Modes)

    ;@ 1/ Initialize exceptions vector tables for necesary modes.
    ;@ 1.1/ HYP table initialisation
    /*mov     r0, #0x8000
    mov     r1, #0x0000
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    ldmia   r0!,{r2, r3, r4, r5, r6, r7, r8, r9}
    stmia   r1!,{r2, r3, r4, r5, r6, r7, r8, r9}
    */
    ;@ 1.1/ HYP vectors init (PL2)
    # HYPervisor mode vectors table initialisation 
    mov r0,#0x8000
    MCR p15, 4, r0, c12, c0, 0 	;@ CP15.c12.4.c0.0 (HVBAR) = R0
    isb 			;@ change in CP15 require an ISB
  
    ;@ 1.2/  Second reset procedure to detect unexpected reset exception
    ;@ TO check if still valid !!!
    mov r3, #0x20         /* Copy in R3 the address of the reset vector */
    ldr r4, =_deadReset   /* Load R4 with the address of deadReset */
    str r4, [r3]          /* Store contents of R4 at the address contained in R3 */

    ;@ 1.3/ Non secure vectors table initialisation (Non secure because executed in HYP mode)
    ;@mov r0, #_startNS  ;@ Does not work because startNS is in another section
    mov r0, #0x8100
    ;@#add r0, r0, #0x8000 	;
    mcr p15, 0, r0, c12, c0, 0  ;@ CP15.c12.0.c0.0 (VBAR) = R0
    isb
  
   /* mrs r2, cpsr /* Copy the current PSR in R2 (third argument of kernel_main) */

    ;@ 1.4/ Secure vectors table initialisation
    ;@ THIS SHALL BE DONE IS SECURE MODE....
    ;@ Cannot be done justas it, deal with monitor mode.... TBC !!!
    mov r0, #0x8200
    /**mcr p15, 0, r0, c12, c0, 0  ;@ CP15.c12.0.c0.0 (VBAR) = R0
    */
    
    ;@ 1.5/ Monitor vectors table initialisation
    ;@ THIS CANOT BE DONE IN NON SECURE MODE  MVBAR is a restricted register.
    mov r0, #0x8300		;@ coproc.CRn.opc1.CRm.opc2
    /**mcr p15, 0, r0, c12, c0, 1	;@ CP15.c12.0.c0.1 (MVBAR) = R0
    **/
    # Not sure this is usefull here: commented. Eg Switch to SVC mode is not possible like here but requires a ERET...
  
    # 2017-08-07: Not sure 2.1, 2.2 and 2.3 are working: when swiching to SVC mode SP remains the one set in 2.4

    ;@ 2/ Initialize the stacks of the modes we will be using
    ;@ 2.1/ We're going to use INTERRUPT mode, so setup the INTERRUPT mode
    ;@ stack pointer which differs to the application stack pointer:
    mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    mov sp, #(63 * 1024 * 1024)
  
    mrs r1, cpsr /* Copy the current PSR in R1 (second argument of kernel_main) */
  
    ;@ 2.2/ We're going to use SUPERVISOR mode, so setup the SVC mode stack pointer
    # Switch back to SUPERVISOR mode (our application mode) and
    # set the stack pointer towards the end of RAM. Remember that the
    # stack works its way down memory, our heap will work it's way
    # up memory toward the application stack.
    ;@ mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT ) 
    mov r0, #(CPSR_MODE_SVR)
    msr cpsr_c, r0
    ; mov sp, #(61 * 1024 * 1024) 
    ldr sp, =__SVC_stack_core0
    /* Assumption: This, here does nothing: the mode is not changed thus, SVC SP
	   s not set... It shall be initialized in SVC mode directly. */

    ;@ 2.3/ We're going to use HYPERVISOR mode, so setup the HYP mode stack pointer
    # AB: added this to check whether HYP mode stack setup helps...
    mov r0, #(CPSR_MODE_HYPERVISOR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
    msr cpsr_c, r0
    mov sp, #(62 * 1024 * 1024)

    ;@ 2.4 This should not be necessary since above block already sets the SP. Overwritten...
    # Set the stack pointer at some point in RAM that won't harm us
    # It's different from the IRQ stack pointer above and no matter
    # what the GPU/CPU memory split, 64MB is available to the CPU
    # Keep it within the limits and also keep it aligned to a 32-bit
    # boundary!
    mov     sp, #(64 * 1024 * 1024)
  
    mrs     r0, cpsr /* Copy the current PSR in R0 (first argument of kernel_main) */

    ;@ 3/ Setup caches, MMU and branch predictors
    ;; @ Check wrt the programmer's manual... 
    /**/
    mov r12,#0			;@ Move immediate - R12 = 0
    mcr p15, 0, r12, c7, c10, 1 ;@ Move to Coproc reg from ARM reg - CP15.c7.0.c10.1 (DCCMVAC - Data cache clean by MVA to PoC) = R12
    dsb                         ;@ Data Synchronisation Barrier - Synchronizes the execution stream with memory accesses
  
    mov r12, #0                 ;@ R12 = 0
    mcr p15, 0, r12, c7, c5, 0  ;@ R12 to CP15.c7.0.c5.0 (ICIALLU - Invalidate all instructions caches to PoU)
    /*ok until here */
    mov r12, #0                 ;@ R12 = 0
    mcr p15, 0, r12, c7, c5, 6  ;@ R12 to CP15.c7.0.c5.6 (BPIALL - Invalidate all branch predictors)
    dsb                         ;@
    isb                         ;@ Instruction Synchronizatoin Barrier - Flushes the pipeline in the processor
    /*
    smc #0                     	;@ Secure Monitor Call, causes a Secure Monitor Call exception. 
                                ;@ What is the purpose of this call here ? SMC not implemented yet, crashes !!!
    */
    mrc p15, 0, r2, c1, c0, 0   ;@ R2 = CP15.c1.0.c0.0 (SCTLR, System Control Register)
    bic r2, r2, #0x1000             	;@ Bitwise bit clear - clears bit 12 of R2, SCTLR.I : Instruction cache disabled
    /* Makes no effect... readback does not return Z bit clear
    bic r2, r2, #0x0800             	;@ Clears bit 11, SCTLR.Z : Branch prediction disabled !!!ABO Added !!!
    */
    bic r2, r2, #0x0004              ;@ Clears bit 3 of R2, SCTLR.C : Data and unified caches disabled
    mcr p15, 0, r2, c1, c0, 0  	;@ CP15.c1.0.c0.0 = R2
    isb
    /**/
    
    ;@ 4/ Translations tables creations

    ;@ 5/ Neon and VFP init if needed

    ;@ 6/ C framework initialisation - if needed
    # The c-startup function which we never return from. This function will
    # initialise the ro data section (most things that have the const
    # declaration) and initialise the bss section variables to 0 (generally
    # known as automatics). It'll then call main, which should never return.
    #bl      _cstartup /* Test bypassing the cstartup */
    bl hyp_main

  # If main does return for some reason, just catch it and stay here.
_inf_loop1:
    b       _inf_loop1


/* Reset from HYP mode to hypervisor -------------------------------------------------------------*/
_resetFromHyp_toSupervisor:
  # We enter execution in HYPPERVISOR mode and want to enter in SVC asap.


  ;@ HYP mode steup before entering in SVC

    ;@ 1.1/ HYP table initialisation (PL2)
  mov r0,#0x8000
  MCR p15, 4, r0, c12, c0, 0 	;@ CP15.c12.4.c0.0 (HVBAR) = R0
  isb 				;@ change in CP15 require an ISB
  
    ;@ 1.2/  Second reset procedure to detect unexpected reset exception
    ;@ TO check if still valid !!! No more exception table at 0x02...
  ;@mov r3, #0x20         /* Copy in R3 the address of the reset vector */
  ;@ldr r4, =_deadReset   /* Load R4 with the address of deadReset */
  ;@str r4, [r3]          /* Store contents of R4 at the address contained in R3 */
  
  ;@----------------------------------------------------------------------------
    ;@ Now, switch the proc mode from HYP to SVC
  ;@b skip		     
  mrs r0,cpsr                 ;@ Move to register from banked or special register - CPSR to R0
  bic r0,r0,#0x1F             ;@ Bitwise bit clear - R0 = R0 & 0x1F (5 lower bits are mode M)
  orr r0,r0,#0x13             ;@ Bitwise OR - R0 =  0x13 (SVC mode)
  msr spsr_cxsf,r0            ;@ Move to special register from register - SPSR all bytes (c 0-7, x 8-15, s 16-24 and f 24-31)
  add r0,pc,#4                ;@ R0 = PC + 4
  msr ELR_hyp,r0              ;@ ELR_hyp = R0
  eret                        ;@ Exception return (HYP mode), load the PC from ELR_hyp which is "skip:" and sets CPSR from SPSR_hyp
  
skip:
    ;@--------------------------------------------------------------------------
    ;@ 1/ Initialize exceptions vector tables for necessary modes.

    ;@ 1.3/ Non Secure vectors table initialisation (Non sEcure because this code is executed in HYP mode)
    ;@mov r0, #_startNS  ;@ Does not work because startNS is in another section
  mov r0, #0x8100
  mcr p15, 0, r0, c12, c0, 0  ;@ CP15.c12.0.c0.0 (VBAR) = R0
  isb
  
    ;@ 1.4/ Secure vectors table initialisation
    ;@ THIS SHALL BE DONE IS SECURE MODE....
    ;@ Cannot be done just as it, deal with monitor mode.... TBC !!!
  mov r0, #0x8200
  /**mcr p15, 0, r0, c12, c0, 0  ;@ CP15.c12.0.c0.0 (VBAR) = R0
  */
    
    ;@ 1.5/ Monitor vectors table initialisation
    ;@ THIS CANOT BE DONE IN NON SECURE MODE  MVBAR is a restricted register.
  mov r0, #0x8300		;@ coproc.CRn.opc1.CRm.opc2
    /**mcr p15, 0, r0, c12, c0, 1	;@ CP15.c12.0.c0.1 (MVBAR) = R0
    **/
    # Not sure this is usefull here: commented. Eg Switch to SVC mode is not possible like here but requires a ERET...

    ;@--------------------------------------------------------------------------
    ;@ 2/ Initialize the stacks of the modes we will be using

    ;@ 2.1/ We're going to use INTERRUPT mode, so setup the INTERRUPT mode
    ;@ stack pointer which differs to the application stack pointer:
  mov r0, #(CPSR_MODE_IRQ | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
  msr cpsr_c, r0
  mov sp, #(63 * 1024 * 1024)
  
    ;@ 2.2/ We're going to use SUPERVISOR mode, so setup the SVC mode stack pointer
    # Switch back to SUPERVISOR mode (our application mode) and
    # set the stack pointer towards the end of RAM. Remember that the
    # stack works its way down memory, our heap will work it's way
    # up memory toward the application stack.
  mov r0, #(CPSR_MODE_SVR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
  msr cpsr_c, r0
  mov sp, #(61 * 1024 * 1024)
  /* This, here does nothing: the mode is not changed thus, SVC SP
  is not set... It shall be initialized in SVC mode directly. */

    ;@ 2.3/ We're going to use HYPERVISOR mode, so setup the HYP mode stack pointer
    # AB: added this to check whether HYP mode stack setup helps...
  mov r0, #(CPSR_MODE_HYPERVISOR | CPSR_IRQ_INHIBIT | CPSR_FIQ_INHIBIT )
  msr cpsr_c, r0
  mov sp, #(62 * 1024 * 1024)
  
    ;; @ This should not be necessary since above block already sets the SP. Overwritten...
    # Set the stack pointer at some point in RAM that won't harm us
    # It's different from the IRQ stack pointer above and no matter
    # what the GPU/CPU memory split, 64MB is available to the CPU
    # Keep it within the limits and also keep it aligned to a 32-bit
    # boundary!
 # mov     sp, #(64 * 1024 * 1024) 
  
  
    ;@ 3/ Setup caches, MMU and branch predictors
    ;; @ Check wrt the programmer's manual... 
    /**/
    mov r12,#0			;@ Move immediate - R12 = 0
    mcr p15, 0, r12, c7, c10, 1 ;@ Move to Coproc reg from ARM reg - CP15.c7.0.c10.1 (DCCMVAC - Data cache clean by MVA to PoC) = R12
    dsb                         ;@ Data Synchronisation Barrier - Synchronizes the execution stream with memory accesses
  
    mov r12, #0                 ;@ R12 = 0
    mcr p15, 0, r12, c7, c5, 0  ;@ R12 to CP15.c7.0.c5.0 (ICIALLU - Invalidate all instructions caches to PoU)
    /*ok until here */
    mov r12, #0                 ;@ R12 = 0
    mcr p15, 0, r12, c7, c5, 6  ;@ R12 to CP15.c7.0.c5.6 (BPIALL - Invalidate all branch predictors)
    dsb                         ;@
    isb                         ;@ Instruction Synchronizatoin Barrier - Flushes the pipeline in the processor
    /*
    smc #0                     	;@ Secure Monitor Call, causes a Secure Monitor Call exception. 
                                ;@ What is the purpose of this call here ? SMC not implemented yet, crashes !!!
    */
    mrc p15, 0, r2, c1, c0, 0   ;@ R2 = CP15.c1.0.c0.0 (SCTLR, System Control Register)
    bic r2, r2, #0x1000             	;@ Bitwise bit clear - clears bit 12 of R2, SCTLR.I : Instruction cache disabled
    /* Makes no effect... readback does not return Z bit clear
    bic r2, r2, #0x0800             	;@ Clears bit 11, SCTLR.Z : Branch prediction disabled !!!ABO Added !!!
    */
    bic r2, r2, #0x0004              ;@ Clears bit 3 of R2, SCTLR.C : Data and unified caches disabled
    mcr p15, 0, r2, c1, c0, 0  	;@ CP15.c1.0.c0.0 = R2
    isb
    /**/
    
    ;@ 4/ Translations tables creations

    ;@ 5/ Neon and VFP init if needed

    ;@ 6/ C framework initialisation - if needed
    # The c-startup function which we never return from. This function will
    # initialise the ro data section (most things that have the const
    # declaration) and initialise the bss section variables to 0 (generally
    # known as automatics). It'll then call main, which should never return.
    #bl      _cstartup /* Test bypassing the cstartup */
    bl supervisor_main

  # If main does return for some reason, just catch it and stay here.
_inf_loop2:
    b       _inf_loop2






smc:
  ;@ Clears bit 0 of the SCR (Secure Configuration Register, NS). NS = 0 enters secure state
  mrc p15, 0, r1, c1, c1, 0  	;@ R1 = CP15.c1.0.c1.0 (SCR)
  bic r1, r1, #1             	;@ R1 = R1 & ~0x01 = R1 & 0xFE
  mcr p15, 0, r1, c1, c1,   0   ;@ CP15.c0.0.c0.0 (SCR) = R1
  movs    pc, lr

/* HYPERVISOR exceptions handlers ----------------------------------------------------------------*/

strHReset:
  .asciz "H Reset"
  .align 2
 
strHundef:
  .asciz "H Undefined instruction"
  .align 2
 
strHhypcall:
  .asciz "H HYP call"
  .align 2
 
strHpabt:
  .asciz "H Prefetch abort"
  .align 2
 
strHdabt:
  .asciz "H Data abort"
  .align 2
 
strHhyptrap:
  .asciz "H HYP trap"
  .align 2
 
strHInt:
  .asciz "H Interrupt"
  .align 2

strHFInt:
  .asciz "H Fast Interrupt"
  .align 2

undefined_instruction_hyp_event:
    push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    bl c_hypUndefHandler
    pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    ;@ R0 = ELR_hyp , store the HYP LR to R0. that LR points to the undef instruction
  /*setup the SPSR so that IT keep active*/
  /**/
    mrs r0, ELR_hyp 
    add r0,r0,#4           ;@ R0 = R0 + 4 go to the following instruction
    msr ELR_hyp,r0         ;@ ELR_hyp = R0
    eret		   ;@ return from exception (from HYP)

  /*
  BE CAREFULL THE AMOUNT OF DATA is even (mod 4) !!! Otherwise this creates misalignment !!!
  */
  
  /*
  - On entry of any excpetion IRQ is set which disable interrupts (APG 11.1.1)
  - Calls to C function damage the registers values. Need a kind of prologue...
  */
hypervisor_call_event:
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
;@  mov r1, lr		       ;@Save lr asap (it might be lost later)
  msr ELR_hyp, lr
  mov r0, lr
  bl muLogInt2Hex
  ldr r0, =strHhypcall
  bl muLogString
  bl c_hypervisorCallHandler
  /*
  ;@mov r0,lr	
  msr ELR_hyp, r1*/
  mrs r0, ELR_hyp
  bl muLogInt2Hex
  /**/
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  eret

prefetch_abort_hyp_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strHpabt
  bl muLogString
  #bl c_interruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  eret

data_abort_hyp_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strHdabt
  bl muLogString
  #bl c_interruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  eret

hyp_trap_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strHhyptrap
  bl muLogString
  bl c_hypervisorHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  eret

interrupt_hyp_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ;@ldr r0, =strHInt
  ;@bl muLogString
  bl c_hypInterruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  eret

fast_interrupt_hyp_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strHFInt
  bl muLogString
  #bl c_hypFastInterruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  eret

/* NON SECURE exceptions handlers ----------------------------------------------------------------*/

strNSReset:
  .asciz "NS Reset"
  .align 2
 
strNSundef:
  .asciz "NS Undefined instruction"
  .align 2
 
strNSsvc:
  .asciz "NS SVCall"
  .align 2
 
strNSpabt:
  .asciz "NS Prefetch abort"
  .align 2
 
strNSdabt:
  .asciz "NS Data abort"
  .align 2
 
strNSInt:
  .asciz "NS Interrupt"
  .align 2
 
strNSFInt:
  .asciz "NS Fast Interrupt"
  .align 2
 
reset_ns_event:
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strNSReset
  bl muLogString
  bl reset_ns_event
  
  
undefined_instruction_ns_event:
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strNSundef
  bl muLogString
  bl c_nsUndefHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    ;@ R0 = ELR_hyp , store the HYP LR to R0. that LR points to the undef instruction
    ;@mrs r0, ELR_hyp 
    ;@add r0,r0,#4           ;@ R0 = R0 + 4 go to the following instruction
    ;@msr ELR_hyp,r0         ;@ ELR_hyp = R0
    ;@eret		   ;@ return from exception (from HYP)
  subs pc,lr,#4

supervisor_call_ns_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strNSsvc
  bl muLogString


    ;#;@ Clears bit 0 of the SCR (Secure Configuration Register, NS). NS = 0 enters secure state
    ;#mrc p15, 0, r1, c1, c1, 0  	;@ R1 = CP15.c1.0.c1.0 (SCR)
    ;#bic r1, r1, #1             	;@ R1 = R1 & ~0x01 = R1 & 0xFE
    ;#mcr p15, 0, r1, c1, c1, 0   ;@ CP15.c0.0.c0.0 (SCR) = R1

    ;@ Sets bit 0 of the SCR (Secure Configuration Register, NS). NS = 1 enters non-secure state
    ;@ mrc p15, 0, r1, c1, c1, 0  	;@ R1 = CP15.c1.0.c1.0 (SCR)
    ;@ orr r1, r1, #1             	;@ R1 = R1 | 0x01
    ;@ mcr p15, 0, r1, c1, c1, 0   ;@ CP15.c0.0.c0.0 (SCR) = R1
  
  ;@bl c_hypInterruptHandler	
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  movs    pc, lr

prefetch_abort_ns_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strNSpabt
  bl muLogString
  #bl c_interruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

data_abort_ns_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strNSdabt
  bl muLogString
  #bl c_interruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4
  
interrupt_ns_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ;@ldr r0, =strNSInt
  ;@bl muLogString
  bl c_nsInterruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

fast_interrupt_ns_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strNSFInt
  bl muLogString
  bl c_fastInterruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

/* SECURE exceptions handlers --------------------------------------------------------------------*/
  
strSReset:
  .asciz "S Reset"
  .align 2
 
strSundef:
  .asciz "S Undefined instruction"
  .align 2
 
strSsvc:
  .asciz "S SVCcall"
  .align 2
 
strSpabt:
  .asciz "S Prefetch abort"
  .align 2
 
strSdabt:
  .asciz "S Data abort"
  .align 2
 
strSInt:
  .asciz "S Interrupt"
  .align 2
 
strSFInt:
  .asciz "S Fast Interrupt"
  .align 2
 
reset_s_event:
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strSReset
  bl muLogString
  bl reset_s_event
  
undefined_instruction_s_event:
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strSundef
  bl muLogString
  bl c_sUndefHandler
   pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    ;@ R0 = ELR_hyp , store the HYP LR to R0. that LR points to the undef instruction
    ;@mrs r0, ELR_hyp 
    ;@add r0,r0,#4           ;@ R0 = R0 + 4 go to the following instruction
    ;@msr ELR_hyp,r0         ;@ ELR_hyp = R0
    ;@eret		   ;@ return from exception (from HYP)
  subs pc,lr,#4

supervisor_call_s_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strSsvc
  bl muLogString
  #bl c_interruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

prefetch_abort_s_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  #bl c_interruptHandler
  ldr r0, =strSpabt
  bl muLogString
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

data_abort_s_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strSdabt
  bl muLogString
  #bl c_interruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4
  
interrupt_s_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strSInt
  bl muLogString
  bl c_sInterruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

fast_interrupt_s_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strSFInt
  bl muLogString
  bl c_fastInterruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

/* SECURE MONITOR exceptions handlers ------------------------------------------------------------*/

strMReset:
  .asciz "M Reset"
  .align 2
 
strMundef:
  .asciz "M Undefined instruction"
  .align 2
 
strMcall:
  .asciz "M Monitor call"
  .align 2
 
strMpabt:
  .asciz "M Prefetch abort"
  .align 2
 
strMdabt:
  .asciz "M Data abort"
  .align 2
 
strMInt:
  .asciz "M Interrupt"
  .align 2
 
strMFInt:
  .asciz "M Fast Interrupt"
  .align 2

monitor_call_m_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strMcall
  bl muLogString
  bl c_monitorHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

prefetch_abort_m_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strMpabt
  bl muLogString
  #bl c_prefectAbortHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

data_abort_m_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strMdabt
  bl muLogString
  #bl c_dataAbortHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4
  
interrupt_m_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strMInt
  bl muLogString
  bl c_mInterruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

fast_interrupt_m_event:	
  push {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  ldr r0, =strMFInt
  bl muLogString
  bl c_mFastInterruptHandler
  pop  {r0,r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
  subs pc,lr,#4

  
/*------------------------------------------------------------------------------------------------*/

_svcInit:
  mov sp, #(61*1024*1024)	;@ SP init
  ##smc #0
  isb
  # Here should also set the stacks of others modes of PL1
  
  bl supervisor_main


  

  # B1.2.9
_trigUndef: 
  # Trigger an undefined instruction condition in order to invoque the Undef Exception
  # Return from the function
  # HYPervisor mode interrupt vector table initialisation CORRUPTED to generate the under exception. OK.
  mov r0,#0x8000
  MCR p15, 4, r0, c12, c0, 1 	;@ CP15.c12.4.c0.0 (HVBAR) = R0
  isb
  mov pc,lr
  


hyp2svc:
    ;@ This switches the mode from HYP to SVC. Shall be called before a eret in HYP mode
  mrs r0,cpsr                 ;@ Move to register from banked or special register - CPSR to R0
  bic r0,r0,#0x1F             ;@ Bitwise bit clear - R0 = R0 & 0x1F (5 lower bits are mode M)
  orr r0,r0,#0x13             ;@ Bitwise OR - R0 =  0x13 (SVC mode)
  bic r0,r0,#CPSR_IRQ_INHIBIT ;@ Bitwise bit clear the IRQ flag to (re)enable interrupts when mode will switch
  msr spsr_cxsf,r0            ;@ Move to special register from register - SPSR all bytes (c 0-7, x 8-15, s 16-24 and f 24-31)
  isb			      ;@ Needed ? no visible effect...
  # add r0,pc,#4      ;@ R0 = PC + 4
  # msr ELR_hyp,r0    ;@ ELR_hyp = R0
  # eret              ;@ Exception return (HYP mode), load the PC from ELR_hyp which is "skip:" and sets CPSR from SPSR_hyp
  mov pc,lr

# _enable_interrupts enables ALL interrupts
# Cannot be invoked from UserMode (returned flags values would be unknown)
_enable_interrupts:
    mrs     r0, cpsr
    bic     r0, r0, #0x80
    msr     cpsr_c, r0
    mov     pc, lr

# _disable_interrupts disables ALL interrupts
# Cannot be invoked from UserMode (returned flags values would be unknown)
_disable_interrupts:
    mrs     r0, cpsr
    orr     r0, r0, #0x80
    msr     cpsr_c, r0
    mov     pc, lr

_getSP:
  # Return the stack pointer value
  str     sp, [sp]
  ldr     r0, [sp]
  # Return from the function
  mov     pc, lr

_setSP:
  # Return the stack pointer value
  ldr     sp, [r0]
  # Return from the function
  mov     pc, lr

_getSPhyp:
  mrs r0, sp_hyp
  # Return from the function
  mov     pc, lr

_getSPsvr:
  mrs r0, sp_svc
  # Return from the function
  mov     pc, lr


_getSPA:
  # Return the stack pointer value
  str     sp, [sp]
  ;@ldr     r0, [sp]
  mov    r0, sp
  # Return from the function
  mov     pc, lr

  # This function returns the CPSR which contains the  processor mode (bits 4..0).
  # Returned value is a 32 bits unsigned integer.
  # See B1.3.3
_getCPSR:
    mrs     r0, cpsr
    mov     pc, lr

_getSCTLR:
   ;@       cop, opc1, Rt, CRn, CRm, opc2
    mrc     p15, 0,    r0, c1,  c0,  0 	 ;@ R0 = CP15.c1.0.c0.0 (SCTLR) (CP15.CRn.opc1.CRm.opc2)
    mov     pc, lr

_getHVBAR:
   ;@       cop, opc1, Rt, CRn, CRm, opc2
    mrc     p15, 4,    r0, c12, c0,  0 	 ;@ R0 = CP15.c12.4.c0.0 (HVBAR) (CP15.CRn.opc1.CRm.opc2)
    mov     pc, lr
  
_getVBAR:
   ;@       cop, opc1, Rt, CRn, CRm, opc2
    mrc     p15, 0,    r0, c12, c0,  0 	 ;@ R0 = CP15.c12.0.c0.0 (VBARR) (CP15.CRn.opc1.CRm.opc2)
    mov     pc, lr

_getSCR:
   ;@       cop, opc1, Rt, CRn, CRm, opc2
    mrc     p15, 0,    r0, c1,  c1,  0   ;@ R0 = CP15.c0.0.c0.0 (SCR) 
    mov     pc, lr

_getHCR:			;@ ONLY from HYP mode !!!
   ;@       cop, opc1, Rt, CRn, CRm, opc2
    mrc     p15, 4,    r0, c1,  c1,  0   ;@ R0 = CP15.c1.4.c1.0 (HCR) 
    mov     pc, lr

_callSVC:
    SVC #0
    mov     pc, lr

_callHYP:
    HVC #0
    mov     pc, lr

_deadReset:
    bl standaloneSOS

.globl PUT32 ;@ PUT32 (dest, src) <==> dest = src
PUT32:
    str r1,[r0]
    bx lr

.globl GET32
GET32:
    ldr r0,[r0]
    bx lr

.globl PUT16
PUT16:
    strh r1,[r0]
    bx lr

.globl PUT8
PUT8:
    strb r1,[r0]
    bx lr

.globl GETPC
GETPC:
    mov r0,lr
    bx lr

.globl BRANCHTO
BRANCHTO:
    mov r12,#0
    mcr p15, 0, r12, c7, c10, 1
    dsb
    mov r12, #0
    mcr p15, 0, r12, c7, c5, 0
    mov r12, #0
    mcr p15, 0, r12, c7, c5, 6
    dsb
    isb
    bx r0

.globl dummy
dummy:
    bx lr

.globl GETCPSR
GETCPSR:
    mrs r0,cpsr
    bx lr

.globl GETSCTLR
GETSCTLR:
    mrc p15,0,r0,c1,c0,0
    bx lr
