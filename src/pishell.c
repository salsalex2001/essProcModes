/*
 * pishell.c
 *
 *  Created on: Jul 17, 2017
 *      Author: ALeX
 */

#ifdef RPI2
//-----------------------------------------------------------------------------
#include <stdint.h>

#include "RPI_BASE.h"
#include "RPI2_GPIO.h"
#include "RPI2_ARMTIMER.h"
#include "RPI2_SYSTIMER.h"
#include "RPI2_INTERRUPTS.h"
#include "RPI2_SWIIC.h"
#include "RPI2_AUX.h"

//#include "syscst.h"
//#include "svcMode.h"

#include "RPI_HAT.h"

#else
//-----------------------------------------------------------------------------
#include <stdint.h>
#include "stub.h"

#include "RPI_BASE.h"
#include "RPI2_GPIO.h"
#include "RPI2_ARMTIMER.h"
#include "RPI2_SYSTIMER.h"
#include "RPI2_INTERRUPTS.h"
#include "RPI2_SWIIC.h"
#include "RPI2_AUX.h"

#include "RPI_HAT.h"

#endif

#include "syscst.h"
#include "registers.h"
#include "convert.h"
#include "printf.h"
#include "keyboard.h"
#include "pishell.h"


/*
 * THIS TABLE SHALL BE SET/UPDATED ALONG WITH cmdList BELOW. BOTH SHALL BE CONSISTENT TOGETHER !
 * */
cmdidx_t cIndex[] = {
  { 2,  0 },  // Commands 2 chars long start at index 0
  { 3,  4 },  // Commands 3 chars long start at index 2
  { 4,  9 },  // Commands 4 chars long start at index 6
  { 6, 11 }   // Commands 6 chars long start at index 7
};

/*
 * THIS TABLE SAHLL BE SET/UPDATE ALONG WITH cIndex ABOVE. BOTH SHALL BE CONSITENT TOGETHER.
 * */
command_t cmdList[CMDS_NB] = {
  {2, "di", cmd_di},
  {2, "ei", cmd_ei},
  {2, "rd", cmd_rd},
  {2, "wr", cmd_wr},
  {3, "hat", cmd_hat},
  {3, "int", cmd_int},
  {3, "ab2", cmd_ab2},
  {3, "ab3", cmd_ab3},
  {3, "ab4", cmd_ab4},
  {4, "dump", cmd_dump},
  {4, "echo", cmd_echo},
  {4, "help", cmd_help},
  {4, "mode", cmd_mode},
  {6, "abcdef", cmd_abcdef},
  {6, "bacdef", cmd_bacdef},
  {6, "cabdef", cmd_cabdef}
};

allCommands_t pishellCommands;



void pishellInit () {

  // Shell initialisation
  pishellCommands.nbBlock = sizeof(cIndex)/sizeof(cmdidx_t);
  pishellCommands.cmdIdx = cIndex;
  pishellCommands.commands = cmdList;
  setEcho (ECHO_CHAR);

}

//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
//
// Built in command of piShell
//
//--------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
void cmd_di (argList_t args) {
  muLogString ("Command DI");
  if ( args.nb == 0 ) {
    di ();
  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }
}

//--------------------------------------------------------------------------------------------------
void cmd_ei (argList_t args) {
  muLogString ("Command EI");
  if ( args.nb == 0 ) {
    ei ();
  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }
}

//--------------------------------------------------------------------------------------------------
void cmd_rd (argList_t args) {
  uint32_t res;

  muLogString ("Command RD");
  if ( args.nb == 1 ) {
    if (strcomp(args.args[0], "sp", ARG_SIZE) == 0) {
      res = _getSP();
      piprintf("SP = 0x%08X\n\r", res);
    }
    else if (strcomp(args.args[0], "spa", ARG_SIZE) == 0) {
      res = _getSPA();
      piprintf("SPa = 0x%08X\n\r", res);
    }
    else if (strcomp(args.args[0], "vbar", ARG_SIZE) == 0) {
      muLogString ("VBAR = ");
      muLogInt2Hex (_getVBAR());
    }
    else if (strcomp(args.args[0], "hvbar", ARG_SIZE) == 0) {
      res = _getCPSR();
      if ((res & CPSR_MODE_HYP) != CPSR_MODE_HYP) {
        piprintf /*muLogString*/ ("HVBAR cannot be accessed out of HYP mode");
      }
      else {
        res = _getHVBAR();
        piprintf("HVBAR = %08X", res);
        /*muLogString ("HVBAR = ");
        muLogInt2Hex (_getHVBAR());*/
      }
    }
    else if (strcomp(args.args[0], "scr", ARG_SIZE) == 0) {
      muLogString ("SCR = ");
      res = _getSCR();
      muLogInt2Hex (res);
    }
    else if (strcomp(args.args[0], "sctlr", ARG_SIZE) == 0) {
      muLogStrInt ("SCTLR: ",_getSCTLR());
    }
    else if (strcomp(args.args[0], "hcr", ARG_SIZE) == 0) {
      muLogStrInt ("HCR: ",_getHCR());
     }
    else if (strcomp(args.args[0], "cpsr", ARG_SIZE) == 0) {
      muLogString ("CPSR = ");
      //muLogInt2Bin (_getCPSR());
      muLogInt2Hex (_getCPSR());
    }
    else if (strcomp(args.args[0], "hvbar", ARG_SIZE) == 0) {
      muLogString ("HAT OFF");
      hatClear();
    }
    else {
      muLogString ("Unexpected argument:");
      muLogString (args.args[0]);
    }
  }
  else if ( args.nb == 2 ) {
    if (strcomp(args.args[0], "sp", ARG_SIZE) == 0) {
      if (strcomp(args.args[1], "s", ARG_SIZE) == 0) {
        res = _getSPsvr();
        piprintf("SP SVR = 0x%08X\n\r", res);
      }
      else if (strcomp(args.args[1], "h", ARG_SIZE) == 0) {
        res = _getSPhyp();
        piprintf("SP HYP = 0x%08X\n\r", res);
      }
    }
    else { /*if (strcomp(args.args[0], "spa", ARG_SIZE) == 0) {
      res = _getSPA();
      piprintf("SPa = 0x%08X\n\r", res);*/
    }
  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }
}

//--------------------------------------------------------------------------------------------------
void cmd_wr (argList_t args) {
  uint32_t arg, res;

  muLogString ("Command WR");
  if ( args.nb == 2 ) {
    if (strcomp(args.args[0], "sp", ARG_SIZE) == 0) {
      arg = strh2int (args.args[1]);
      _setSP(arg);
      piprintf("SP <- 0x%08X\n\r", arg);
    }
  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }
}

//--------------------------------------------------------------------------------------------------
void cmd_dump (argList_t args) {
  //uint32_t res;
  uint32_t add, sz;
  uint32_t * ptr;

  muLogString ("Command DUMP");
  if ( args.nb == 1 ) {
    // dump address (16 words)
    add = strh2int (args.args[0]);
    sz = 16;
    piprintf("Will dump %d words from 0x%08X.\n\r", sz, add);
    ptr = (uint32_t*)add;
    piprintf("%08X %08X %08X %08X\n\r", *ptr++, *ptr++, *ptr++, *ptr++);
    piprintf("%08X %08X %08X %08X\n\r", *ptr++, *ptr++, *ptr++, *ptr++);
    piprintf("%08X %08X %08X %08X\n\r", *ptr++, *ptr++, *ptr++, *ptr++);
    piprintf("%08X %08X %08X %08X\n\r", *ptr++, *ptr++, *ptr++, *ptr++);
  }
  else if ( args.nb == 2 ) {
    // Dump address size
    add = strh2int (args.args[0]);
    sz = strh2int (args.args[1]);
    piprintf("Will dump %d words from 0x%08X.", sz, add);

  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }

}

//--------------------------------------------------------------------------------------------------
void cmd_echo (argList_t args) {
  uint32_t res;

  muLogString ("Command ECHO");
  if ( args.nb == 0 ) {
    res = getEcho();
    piprintf("Echo %d\n\r", res);
  }
  else if ( args.nb == 1 ) {
    // TBD !!!
    res = strd2int (args.args[0]);
    muLogInt2Hex(res);
    res = strh2int (args.args[0]);
    muLogInt2Hex(res);
    // Get number from string for 1st arg
    // return an error if not a number

    if ( (res >= ECHO_NONE) && (res < ECHO_INVALID) ) {
      setEcho (res);
    }
    else {
      muLogString ("Unexpected argument:");
      muLogString (args.args[0]);
    }
    /*
    //res = strcomp(args[0], "on", ARG_SIZE);
    muLogInt2Hex(res);
    if (strcomp(args.args[0], "on", ARG_SIZE) == 0) {
      muLogString ("INT ON: Interrupts enabled");
      intEnable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
    }
    else if (strcomp(args.args[0], "off", ARG_SIZE) == 0) {
      muLogString ("INT OFF: Interrupts disabled");
      intDisable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
    }
    else {
      muLogString ("Unexpected argument:");
      muLogString (args.args[0]);
    }
    */
  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }

}

//--------------------------------------------------------------------------------------------------
void cmd_mode (argList_t args) {
  uint32_t res;
  enum PROCMODE md;

  muLogString ("Command MODE");
  if ( args.nb == 0 ) {
    res = _getCPSR();
    res &= 0x1F;
    md = res;
    //piprintf("Current mode is 0x%02X (%08b)\n\r", res,res);
    piprintf("Current mode is 0x%02X (0b%05b)\n\r", res, res);
    //piprintf("Current mode is %s (0x%02X)\n\r", procmode_name[md], md);
    piprintf("Current mode is %s (0x%02X)\n\r", getProcModeName(md), md);
  }
  else if ( args.nb == 1 ) {
    // TBD !!!
    if (strcomp(args.args[0], "svc", ARG_SIZE) == 0) {
      muLogString ("Switch to SVC mode...\n\r");
      hatClear();
      hatFull(0);
      //_svcInit();
      //hyp2svc();
      _callSVC();
    }
    else if (strcomp(args.args[0], "hyp", ARG_SIZE) == 0) {
      muLogString ("Switch to HYP...\n\r");
      hatClear();
      hatFull(8);
      //_trigUndef();
      _callHYP();
    }
    else {
      muLogString ("Unexpected argument:");
      muLogString (args.args[0]);
    }
/*
    res = strd2int (args.args[0]);
    muLogInt2Hex(res);
    res = strh2int (args.args[0]);
    muLogInt2Hex(res);
    // Get number from string for 1st arg
    // return an error if not a number

    if ( (res >= ECHO_NONE) && (res < ECHO_INVALID) ) {
      setEcho (res);
    }
    else {
      muLogString ("Unexpected argument:");
      muLogString (args.args[0]);
    }
*/
    /*
    //res = strcomp(args[0], "on", ARG_SIZE);
    muLogInt2Hex(res);
    if (strcomp(args.args[0], "on", ARG_SIZE) == 0) {
      muLogString ("INT ON: Interrupts enabled");
      intEnable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
    }
    else if (strcomp(args.args[0], "off", ARG_SIZE) == 0) {
      muLogString ("INT OFF: Interrupts disabled");
      intDisable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
    }
    else {
      muLogString ("Unexpected argument:");
      muLogString (args.args[0]);
    }
    */
  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }

}

//--------------------------------------------------------------------------------------------------
void cmd_help (argList_t args) {
  //muLogString ("Command HELP");
  piprintf ("Command HELP\n\r");

}

//--------------------------------------------------------------------------------------------------
void cmd_hat (argList_t args) {
  //int res;
  muLogString ("Command HAT\n\r");
  if ( args.nb == 1 ) {
    //res = strcomp(args[0], "on", ARG_SIZE);
    //muLogInt2Hex(res);
    if (strcomp(args.args[0], "on", ARG_SIZE) == 0) {
      muLogString ("HAT ON\n\r");
      hatFull(16);
    }
    else if (strcomp(args.args[0], "off", ARG_SIZE) == 0) {
      muLogString ("HAT OFF\n\r");
      hatClear();
    }
    else {
      muLogString ("Unexpected argument:");
      muLogString (args.args[0]);
    }
  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }
}

//--------------------------------------------------------------------------------------------------
void cmd_int (argList_t args) {
  //int res;
  muLogString ("Command INT");
  if ( args.nb == 1 ) {
    //res = strcomp(args[0], "on", ARG_SIZE);
    //muLogInt2Hex(res);
    if (strcomp(args.args[0], "on", ARG_SIZE) == 0) {
      muLogString ("INT ON: Interrupts enabled");
      intEnable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
    }
    else if (strcomp(args.args[0], "off", ARG_SIZE) == 0) {
      muLogString ("INT OFF: Interrupts disabled");
      intDisable (INT_GRP_BASIC, EN_BASIC_ARM_TIMER_IRQ);
    }
    else {
      muLogString ("Unexpected argument:");
      muLogString (args.args[0]);
    }
  }
  else {
    muLogStrInt ("Wrong number of argument: ", args.nb);
  }
}

//--------------------------------------------------------------------------------------------------
void cmd_ab1 (argList_t args) {
  muLogString ("Command AB1");

}

//--------------------------------------------------------------------------------------------------
void cmd_ab2 (argList_t args) {
  muLogString ("Command AB2");

}

//--------------------------------------------------------------------------------------------------
void cmd_ab3 (argList_t args) {
  muLogString ("Command AB3");

}

//--------------------------------------------------------------------------------------------------
void cmd_ab4 (argList_t args) {
  muLogString ("Command AB4");

}

//--------------------------------------------------------------------------------------------------
void cmd_abcdef (argList_t args) {
  muLogString ("Command ABCDEF");

}

//--------------------------------------------------------------------------------------------------
void cmd_bacdef (argList_t args) {
  muLogString ("Command BACDEF");

}

//--------------------------------------------------------------------------------------------------
void cmd_cabdef (argList_t args) {
  muLogString ("Command CABDEF");

}


