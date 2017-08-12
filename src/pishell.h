/*
 * pishell.h
 *
 *  Created on: Jul 17, 2017
 *      Author: alex
 */

#ifndef PISHELL_H_
#define PISHELL_H_

#define CMDS_NB 16


//#define MK_CMD(x) void cmd_ ## x (arg_t*)
#define MK_CMD(x) extern void cmd_ ## x (argList_t)
//Commands definitions
MK_CMD(a0);
MK_CMD(di);
MK_CMD(ei);
MK_CMD(rd);
MK_CMD(wr);
MK_CMD(hat);
MK_CMD(int);
MK_CMD(dump);
MK_CMD(ab2);
MK_CMD(ab3);
MK_CMD(ab4);
MK_CMD(echo);
//MK_CMD(add);
//MK_CMD(mul);
//MK_CMD(sqrt);
//MK_CMD(exit);
MK_CMD(help);
MK_CMD(mode);

MK_CMD(abcdef);
MK_CMD(bacdef);
MK_CMD(cabdef);


extern void pishellInit (void);

// the command from keyboard module. Made simply external (no accessor, keep it light)
extern allCommands_t pishellCommands;

#endif /* PISHELL_H_ */
