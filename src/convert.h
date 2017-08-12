/*
 * convert.h
 *
 *  Created on: Jul 24, 2017
 *      Author: alex
 */

#ifndef CONVERT_H_
#define CONVERT_H_

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

typedef unsigned char uchar;
typedef unsigned int   uint;


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

extern uint32_t strd2int (char * str);
extern uint32_t strh2int (char * str);

/*
  strcp copies the nb chars of src string to dest string. Any other
  characters are ignored (eg 0 terminator char)
*/
void strcp (char* dest, const char* src, uint nb);

/*
 * strcomp is a clone of strcmp with limitation of the compared characters.
 * */
int strcomp (const char *s1, const char *s2, uint nb);



#endif /* CONVERT_H_ */
