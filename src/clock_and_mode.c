#include "clock_and_mode.h"

void SOSC_init_8MHz(void) {
 SCG->SOSCDIV=0x00000101; /* SOSCDIV1 & SOSCDIV2 =1: divide by 1 */
 SCG->SOSCCFG=0x00000024; /* Range=2: Medium freq (SOSC between 1MHz-8MHz)*/
 /* HGO=0: Config xtal osc for low power */
/* EREFS=1: Input is external XTAL */
 while(SCG->SOSCCSR & SCG_SOSCCSR_LK_MASK); /* Ensure SOSCCSR unlocked */
 SCG->SOSCCSR=0x00000001; /* LK=0: SOSCCSR can be written */
 /* SOSCCMRE=0: OSC CLK monitor IRQ if enabled */
/* SOSCCM=0: OSC CLK monitor disabled */
/* SOSCERCLKEN=0: Sys OSC 3V ERCLK output clk disabled */
/* SOSCLPEN=0: Sys OSC disabled in VLP modes */
/* SOSCSTEN=0: Sys OSC disabled in Stop modes */
/* SOSCEN=1: Enable oscillator */
 while(!(SCG->SOSCCSR & SCG_SOSCCSR_SOSCVLD_MASK)); /* Wait for sys OSC clk valid */
}
void SPLL_init_160MHz(void) {
 while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
 SCG->SPLLCSR = 0x00000000; /* SPLLEN=0: SPLL is disabled (default) */
 SCG->SPLLDIV = 0x00000302; /* SPLLDIV1 divide by 2; SPLLDIV2 divide by 4 */
 SCG->SPLLCFG = 0x00180000; /* PREDIV=0: Divide SOSC_CLK by 0+1=1 */
 /* MULT=24: Multiply sys pll by 4+24=40 */
/* SPLL_CLK = 8MHz / 1 * 40 / 2 = 160 MHz */
 while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
 SCG->SPLLCSR = 0x00000001; /* LK=0: SPLLCSR can be written */
 /* SPLLCMRE=0: SPLL CLK monitor IRQ if enabled */
/* SPLLCM=0: SPLL CLK monitor disabled */
/* SPLLSTEN=0: SPLL disabled in Stop modes */
/* SPLLEN=1: Enable SPLL */
 while(!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)); /* Wait for SPLL valid */
}
void NormalRUNmode_80MHz (void) { /* Change to normal RUN mode with 8MHz SOSC, 80 MHz PLL*/
 SCG->RCCR=SCG_RCCR_SCS(6) /* PLL as clock source*/
 |SCG_RCCR_DIVCORE(1) /* DIVCORE=1, div. by 2: Core clock = 160/2 MHz = 80 MHz*/
 |SCG_RCCR_DIVBUS(1) /* DIVBUS=1, div. by 2: bus clock = 40 MHz*/
 |SCG_RCCR_DIVSLOW(2); /* DIVSLOW=2, div. by 3: SCG slow, flash clock= 26 2/3 MHz*/
 while (((SCG->CSR & SCG_CSR_SCS_MASK) >> SCG_CSR_SCS_SHIFT ) != 6) {}
 /* Wait for sys clk src = SPLL */
}
void Clock_init (void){
	SOSC_init_8MHz(); /* Initialize system oscillator for 8 MHz xtal */
	SPLL_init_160MHz(); /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
	NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
}
//void SPLL_init_160MHz(uint8_t div) {
// while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
// SCG->SPLLCSR &= ~SCG_SPLLCSR_SPLLEN_MASK; /* SPLLEN=0: SPLL is disabled (default) */
// SCG->SPLLDIV = SCG_SPLLDIV_SPLLDIV2(div); /* SPLLDIV1 divide by 2; SPLLDIV2 divide by 4 */
// SCG->SPLLCFG = 0x00180000; /* SCG_SPLLCFG_MULT(24);PREDIV=0: Divide SOSC_CLK by 0+1=1 */
// /* MULT=24: Multiply sys pll by 4+24=40 */
///* SPLL_CLK = 8MHz / 1 * 40 / 2 = 160 MHz */
// while(SCG->SPLLCSR & SCG_SPLLCSR_LK_MASK); /* Ensure SPLLCSR unlocked */
// SCG->SPLLCSR |= SCG_SPLLCSR_SPLLEN_MASK; /* LK=0: SPLLCSR can be written */
// /* SPLLCMRE=0: SPLL CLK monitor IRQ if enabled */
///* SPLLCM=0: SPLL CLK monitor disabled */
///* SPLLSTEN=0: SPLL disabled in Stop modes */
///* SPLLEN=1: Enable SPLL */
// while(!(SCG->SPLLCSR & SCG_SPLLCSR_SPLLVLD_MASK)); /* Wait for SPLL valid */
//}

//void Clock_init (void){
//	SOSC_init_8MHz(); /* Initialize system oscillator for 8 MHz xtal */
//	SPLL_init_160MHz(SPLLDIV2_4); /* Initialize SPLL to 160 MHz with 8 MHz SOSC */
//	NormalRUNmode_80MHz(); /* Init clocks: 80 MHz sysclk & core, 40 MHz bus, 20 MHz flash */
//}
