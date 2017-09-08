#ifndef __TIM14__
#define __TIM14__

#include "sys.h"

#define RDATA_TIM14 PAin(7)

void timer14_init(void);
u32 Remote_Scan_tim14(void);

#endif

