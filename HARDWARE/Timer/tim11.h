#ifndef __TIM11__
#define __TIM11__

#include "sys.h"

#define RDATA_TIM11 PFin(7)

void timer11_init(void);
u32 Remote_Scan_tim11(void);

#endif

