#ifndef _LPIT_H_
#define _LPIT_H_

#include "clock_and_mode.h"
#include "S32K144.h"
#include "Port.h"

void LPIT0_Init(void);
void LPIT0_ch0_SetTime(uint32_t ms);
//void LPIT1_Init(void);
void LPIT0_ch1_SetTime(uint32_t ms);

static inline void LPIT0_ch0_Enable(void) {
	LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
}

static inline void LPIT0_ch0_Disable(void) {
	LPIT0->TMR[0].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK;
}

static inline void LPIT0_ch1_Enable(void) {
	LPIT0->TMR[1].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;
}

static inline void LPIT0_ch1_Disable(void) {
	LPIT0->TMR[1].TCTRL &= ~LPIT_TMR_TCTRL_T_EN_MASK;
}
#endif


