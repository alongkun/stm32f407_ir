#ifndef __TIM2__
#define __TIM2__

#include "sys.h"

#define RDATA_TIM2 PAin(1)

void timer2_init(void);
u32 Remote_Scan_tim2(void);

#endif

