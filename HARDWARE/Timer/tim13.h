#ifndef __TIM13__
#define __TIM13__

#include "sys.h"

#define RDATA_TIM13 PAin(6)

void timer13_init(void);
u32 Remote_Scan_tim13(void);

#endif

