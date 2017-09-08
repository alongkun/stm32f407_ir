#ifndef __TIM9__
#define __TIM9__

#include "sys.h"

#define RDATA_TIM9 PAin(2)

void timer9_init(void);
u32 Remote_Scan_tim9(void);

#endif

