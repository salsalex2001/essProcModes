/*
 * keyboard.h
 *
 *  Created on: Jun 16, 2017
 *      Author: ALeX
 *
 *  REQUIRES stdint.h included beforehand. Shall be done in including C file.
 */

#ifndef KEYBOARD_H
#define KEYBOARD_H

#define ARGS_NB 5
#define CMDLINE_SIZE 512
#define CMD_SIZE 32
#define ARG_SIZE 16
#define TOK_SIZE 33   // Max size for a token, reserved for getToken function

#define NULL_TERM  0x00
#ifdef RPI2
// In PI2, minicom sends a CR (0x0D) char
#define KEY_CR     0x0D
#define KEY_LF     0x0A
#define EOF (-1)
#else
// In linux desktop pressing enter returns a LF (0x0A) char
#define KEY_CR     0x0A
#define KEY_LF     0x0A

#endif

#define MRKR_SEP  0x20 // Use space as a separator between arguments

enum {
  ECHO_NONE = 0,
  ECHO_CHAR,
  ECHO_CHARLF,
  ECHO_CHARHEX,
  ECHO_INVALID
} echo_e;


typedef struct {
  uchar * tkn;
  uint sz;    // size of the token
  uint six;   // start index in the command line buffer
} token_t;

typedef union {
  char  *s;
  char   c;
  float  f;
} arg_t_prev;

typedef char ** argTable_t;

typedef struct {
  uint        nb;
  argTable_t  args;
} argList_t;

/* Index entry for the list of commands. Contains information for commands blocks in the command list:
 * sz: size of the command for a block
 * pos: position of the command block in the list (index)
 *
 * An array of cmdidx_t helps to search faster commands based on their size.
*/
typedef struct {
  uchar    sz;
  uint16_t pos;
} cmdidx_t;

/* Command descriptor.
 *
 * An array of this is used for the 'shell' commands list.
 *
 */
typedef struct {
  uchar   sz;            // Size of the name for faster search
  char  * name;          // Command name
  //void  (*fct)(arg_t*); // Function to invoke for that command
  void  (*fct)(argList_t); // Function to invoke for that command
} command_t;

/*
 * nbBlock is the number of block of commands the same size
 * cmdIdx is a pointer to an array of nbBlock cmdidx_t objects
 * commands is the complete list of commands available in the "shell"
 *
 * The last member cannot be an undefined size array (aka flexible
 * array), otherwise the structure size will be unknown at compilation
 * time and will require a dynamic allocation (cannot be statically
 * instantiated !)
 */
typedef struct {
  uchar       nbBlock;
  cmdidx_t  * cmdIdx;
  //command_t   commands[];
  command_t * commands;
} allCommands_t;


//-----------------------------------------------------------------------------
// Public services
//-----------------------------------------------------------------------------
extern void kbdInit ();
extern void inputKbd (unsigned char c);
//extern void kbdScan();
extern uint32_t kbdScan();

extern void setEcho (uchar et);
extern uchar getEcho (void);

//-----------------------------------------------------------------------------
// Private services
//-----------------------------------------------------------------------------
void processCmd (unsigned int nbc);
uint getToken (token_t *, uchar *, uchar);



#endif // KEYBOARD_H
