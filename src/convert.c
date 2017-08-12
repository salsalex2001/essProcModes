/*
 * convert.c
 *
 *  Created on: Jul 24, 2017
 *      Author: alex
 */

#include <stdint.h>
#include "convert.h"


//----------------------------------------------------------------------------
// Converts a string representing a decimal integer in an integer
// String shall be maximum 5 chars longs
//-----------------------------------------------------------------------------
uint32_t strd2int (char * str) {
  unsigned char i;
  uint32_t res;
  res = 0;
  i = 0;

  while ( (str[i] != 0) && (i<5) ) {
    res *= 10;
    res += (str[i] - 0x30);
    i++;
  }
  return res;
}

//----------------------------------------------------------------------------
// Converts a string representing a decimal integer in an integer
// String shall be maximum 5 chars longs
//-----------------------------------------------------------------------------
uint32_t strh2int (char * str) {
  unsigned char i;
  uint32_t res;
  char n;
  res = 0;
  i = 0;

  while ( (str[i] != 0) && (i<5) ) {
    res *= 16;
    n = str[i];
    if (n <= 0x39) {
      n -= 0x30;
    }
    else if (n <= 0x46) {
      n -= 0x37;
    }
    else if (n <= 0x66) {
      n -= 0x57;
    }
    i++;
    res += n;
  }
  return res;
}

void strcp (char* dest, const char* src, uint nb) {
  uint i;
  char *d;
  d = dest;
  for (i=0 ; i<nb ; i++) {
    *d++ = *src++;
  }
}

int strcomp (const char *s1, const char *s2, uint nb) {
  uint i;

  for (i=0 ; *s1 == *s2; s1++, s2++) {
    if (*s1 == '\0') {
      return 0;
    }
    if (++i > nb) {
      return 2;
    }
  }
  return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}

//------------------------------------------------------------------------------
/* ANSI sez:
 *   The `strcpy' function copies the string pointed to by `s2' (including
 *   the terminating null character) into the array pointed to by `s1'.
 *   If copying takes place between objects that overlap, the behavior
 *   is undefined.
 *   The `strcpy' function returns the value of `s1'.  [4.11.2.3]
 */
//------------------------------------------------------------------------------
char * strcpy (char *s1, const char *s2) {
    char *s = s1;
    while ((*s++ = *s2++) != 0)
  ;
    return (s1);
}

/* ANSI sez:
 *   The `strcmp' function compares the string pointed to by `s1' to the
 *   string pointed to by `s2'.
 *   The `strcmp' function returns an integer greater than, equal to, or less
 *   than zero, according as the string pointed to by `s1' is greater than,
 *   equal to, or less than the string pointed to by `s2'.  [4.11.4.2]
 */
int strcmp (const char *s1, const char *s2) {
  for ( ; *s1 == *s2; s1++, s2++) {
    if (*s1 == '\0') {
      return 0;
    }
  }
  return ((*(unsigned char *)s1 < *(unsigned char *)s2) ? -1 : +1);
}

