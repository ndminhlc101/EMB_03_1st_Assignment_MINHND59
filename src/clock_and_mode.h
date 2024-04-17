#ifndef _CLOCK_AND_MODE_H_
#define _CLOCK_AND_MODE_H_

#include "S32K144.h"
#include <stdint.h>
#include<string.h>

#define SPLLDIV2_4 3
#define SPLLDIV2_8 4

void SOSC_init_8MHz(void);
void SPLL_init_160MHz(void);
void NormalRUNmode_80MHz (void);
void Clock_init (void);

#endif
