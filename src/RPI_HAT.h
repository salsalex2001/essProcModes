
#ifndef RPI_HAT_H
#define RPI_HAT_H


extern void hatClear (void);
extern void hatFull (char rgb);
extern void hatSetLed (unsigned char x, unsigned char y, char r, char g, char b);

extern void hatShowByte (unsigned char b, unsigned char col, char rgb);
extern void hatShowInt (uint32_t b, unsigned char col, char rgb);

extern unsigned char hatReadJoy (void);

#define HAT_I2C_ADDRESS     0x46
#define HAT_JOY_SUBADDRESS  0xF2

#endif /* RPI_HAT_H */

