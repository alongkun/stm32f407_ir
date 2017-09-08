#ifndef __TIM10__
#define __TIM10__

#include "sys.h"

#define RDATA_TIM10 PFin(6)

void timer10_init(void);
u32 Remote_Scan_tim10(void);

#endif

