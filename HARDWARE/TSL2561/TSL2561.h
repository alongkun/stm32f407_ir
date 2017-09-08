#ifndef __TSL2561__
#define __TSL2561__

#include "sys.h"
#include "delay.h"

u8 tsl2561_init(void);
u8 tsl2561_read(u8 reg);
void tsl2561_write(u8 reg, u8 val);
u16 tsl2561_get_ch1(void) ;

#endif 
