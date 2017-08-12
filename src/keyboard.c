

#ifdef RPI2
//-----------------------------------------------------------------------------
#include <stdint.h>

#include "RPI_BASE.h"
#include "RPI2_AUX.h"

#include "syscst.h"
#include "svcMode.h"

#include "RPI_HAT.h"

#else
//-----------------------------------------------------------------------------
#include <stdint.h>
#include "stub.h"

#include "RPI2_AUX.h"

#include "RPI_HAT.h"

#endif

#include "convert.h"
#include "keyboard.h"
#include "pishell.h"

unsigned int curr;

// This is the buffer that contains the complete command line.
uchar inputBfr[CMD_SIZE];

// This is the working buffer used as output of getToken.
uchar tokenBuff[TOK_SIZE];

// This is the buffer that contains the currently parsed command, extracted from the commnd line
char currCommand[CMD_SIZE];

char * commands[CMDS_NB] = {
  "r",
  "W",
  "x"
};

// Max 5 arguments, 16 bytes each max
char a1[ARG_SIZE];
char a2[ARG_SIZE];
char a3[ARG_SIZE];
char a4[ARG_SIZE];
char a5[ARG_SIZE];

// This is the list of extracted arguments following the commands
uchar argTable1[ARGS_NB][ARG_SIZE];
char * argTable[ARGS_NB] = {
  a1, a2, a3, a4, a5
};
argList_t argList;

uchar echoType;


/*-----------------------------------------------------------------------------
   Initialises the keyboard services
-----------------------------------------------------------------------------*/
void kbdInit () {
  unsigned int i;

  curr = 0;
  for (i = 0 ; i < CMD_SIZE ; i++) {
    inputBfr[i] = 0;
  }
  
  pishellInit();

  // Arg list initialization
  argList.args = argTable;
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
void inputKbd (uchar c) {
  if (c == KEY_CR) {
    muLogChar(KEY_LF,0);
    inputBfr[curr] = 0; // Insert a termination char
    processCmd(curr);
    curr = 0;
  }
  else if (curr < CMD_SIZE) {
    inputBfr[curr] = c;
    curr++;
  }
  else {
    // Drop the char
  }
}

/*-----------------------------------------------------------------------------

-----------------------------------------------------------------------------*/
uint32_t kbdScan () {
//void kbdScan () {
  uint32_t rxb;

  if (muRxFifoNotEmpty()) {
    // Echo the received char
    rxb = muGetRxByte();
    if (echoType >= ECHO_CHARLF) {
      muLogChar(rxb, 2);
    }
    else if (echoType >= ECHO_CHAR) {
      muLogChar(rxb, 0);
    }
    if (echoType >= ECHO_CHARHEX) {
      muLogInt2Hex(rxb);
    }
    inputKbd (rxb);
    if (rxb == 0x30) {
      _enable_interrupts();
    }
    return rxb;
  }
  else {
    return EOF;
  }

}

/*------------------------------------------------------------------------------
  Searches a token in a string.

  Search is limited to TOK_SIZE first chars.
  
  In
    buff : pointer to a char buffer that needs to be parsed
    del : character used as a delimiters of the token in buff
    
  In, Out
    tk, pointer in buff to a token when found
    NO: filled in with the found token characters

  Returns
    0 when a token is found
    1 if no token is found
------------------------------------------------------------------------------*/
uint getToken (token_t *tk, uchar * buff, uchar del) {
  uint i; // iterator in the inut buffer
  uint j; // index of first char of the token (in case it starts with delimiters)
  uchar c;
  uchar s;  // boolean: 1 start of a token found
  uint res;
  
  // Result set to not found by default
  res = 1;
  i = 0;
  s = 0;
  j = 0;
  
  do {
    c = buff[i];
    
    if (c == del) {
      tokenBuff[i] = 0;
      // Increase iterator only if not already found a token start
      if (s == 0) {
        i++;
      }
    }
    else if (c != 0) {
      tokenBuff[i] = c;
      if (s == 0) {
        s = 1;   // One token (start of) is found already
        j = i;   // Index of token first char
        res = 0; // Found one token
      }
      i++;
    }
    else {
      // Termination char, end of function.
      // Also put a term char in token string for proper display
      tokenBuff[i] = 0;
    }
  } while ( ( ((c == del) && (s == 0)) || ((c != del) && (s == 1)) ) &&
      (i < (TOK_SIZE - 1)) &&
      (c != 0) );
  // Keep one char for null terminator char '0'

  // Initialize the token size
  if (res == 0) {
    tk->sz = i-j;
  }
  else {
    // If no token found, size is 0
    tk->sz = 0;
  }
  // .. and the index of first char (if buffer is shared - but not a good idea, to be removed !!!)
  tk->six = j;

  // That function just locates the token on the command line and returns pointer in it.
  tk->tkn = &tokenBuff[j];

  return res;
}

/*------------------------------------------------------------------------------
  
------------------------------------------------------------------------------*/
void processCmd (unsigned int nbc) {
  uint i, ag, j ;
  token_t tk, cmd;
  uint res;
  
  // variables to search the pishellCommands structure
  char blockFound;
  uint16_t blockPos;
  char cmdFound;
  uchar cmdSize;
  uint cmdix;
  /* Silenet now...
  muLogString ("processCmd:");
  muLogInt2Hex (nbc);
  muLogString ((char*)inputBfr);
   */

  // Parse the command line ---------------------------------------------------
  
  // Search the command first...
  i = 0;
  ag = 0;
  // not yet !!! cmd.tkn = currCommand;
  res = getToken (&cmd, &inputBfr[i], MRKR_SEP);
  if (cmd.sz != 0) {
    // Got a command...
    if (cmd.sz != 0) {
      /* Silent now...
      muLogString("  Command found:");
      muLogInt2Hex (cmd.sz);
      muLogString ((char*)(cmd.tkn));
       */
      // Copy the output temp buffer in the command buffer
      strcp (currCommand, (const char*)cmd.tkn, cmd.sz);
      // Ugly coding... but for now
      // Set cmd.tkn to point to the command buffer
      cmd.tkn = (uchar*)currCommand;
    }

    // ...then parse for arguments
    i += (cmd.six + cmd.sz);
       
    do {
      res = getToken (&tk, &inputBfr[i], MRKR_SEP);
      //if (res == 0) {
      if (tk.sz != 0) {
        /* Silent now...
        muLogString ("  Arg found:");
        muLogInt2Hex (tk.sz);
        muLogString ((char*)tk.tkn);
        */
        strcp (argList.args[ag], (const char*)tk.tkn, tk.sz+1); // copy also terminal 0 (+1)
        ag++;
      }
      i += (tk.six + tk.sz);
    } while ( (res == 0) && (ag < ARGS_NB) );
  }

  // Check parameters
  argList.nb = ag;
  /* silent now...
  muLogString("  Nb of args found:");
  muLogInt2Hex (ag);
  for (i=0 ; i<ag; i++) {
    muLogString (argTable[i]);
  }
  */
  
  // Treat the command ---------------------------------------------------------
  
  // 1) Search a container block
  blockFound = 0;
  for (i=0 ; i<pishellCommands.nbBlock ; i++) {
    if (pishellCommands.cmdIdx[i].sz == cmd.sz) {
      // Found a block that matches the command size. Need to search that block now
      blockFound = 1;
      blockPos = pishellCommands.cmdIdx[i].pos;
    }
    else {

    }
  }
  
  if (blockFound == 1) {
    // 2) In a found block, search the command
    i = blockPos;
    cmdSize = cmd.sz;
    cmdFound = 0;
    // Iterate all commands of the block...
    do {
      j=0 ;
      cmdFound = 1;
      // Iterate all chars of each of the commands of the block
      do {
        if ( pishellCommands.commands[i].name[j] == cmd.tkn[j] ) {
          // God match, might be the right command
          cmdix = i;
        }
        else {
          // Wrong match, stop checking that command !
          cmdFound = 0;
        }
        j++;
      } while ( (cmdFound == 1) && (j < cmdSize) );

      i++;
    } while ( (cmdFound == 0) && (pishellCommands.commands[i].sz == cmd.sz) );

    if (cmdFound == 1) {
      // 3) Finally invoke the function for the command, with its arguments...
      pishellCommands.commands[cmdix].fct(argList);
    }
    else {
      muLogString ("No such command available !!!");
    }
  }
  else {
    muLogString ("No command that size available.");
  }
  
}

//-----------------------------------------------------------------------------
void setEcho (uchar et) {
  echoType = et;
}

uchar getEcho (void) {
  return echoType;
}


