#include"LPIT.h"

void SystemCoreClockUpdate(void);

void LPIT0_ch0_SetTime(uint32_t ms)
{
	SystemCoreClockUpdate();
	//SystemCoreClock=80Mhz , PLL clock= 40Mhz
	volatile uint32_t loadvalue = ms*(SystemCoreClock/2000) -1;
	LPIT0->TMR[0].TVAL = loadvalue; /* Chan 0 Timeout period: 40M clocks */
}

void LPIT0_ch1_SetTime(uint32_t ms)
{
	SystemCoreClockUpdate();
	//SystemCoreClock=80Mhz , PLL clock= 40Mhz
	volatile uint32_t loadvalue = ms*(SystemCoreClock/2000) -1;
	LPIT0->TMR[1].TVAL = loadvalue; /* Chan 1 Timeout period: 40M clocks */
}

void LPIT0_Init(void)
{
	PCC->PCCn[PCC_LPIT_INDEX] = PCC_PCCn_PCS(6); /* Clock Src = 6 (SPLL2_DIV2_CLK)*/
	PCC->PCCn[PCC_LPIT_INDEX] |= PCC_PCCn_CGC_MASK; /* Enable clk to LPIT0 regs */
	LPIT0->MCR |= LPIT_MCR_M_CEN(1U);

	// LPIT0 channel 0 init
//	LPIT0->MIER |= LPIT_MIER_TIE0_MASK; /* TIE0=1: Timer Interrupt Enabled fot Chan 0 */
//	LPIT0->TMR[0].TVAL = 40000000-1;//PLL clock=40M ->1s~loadvalue=40M
//	LPIT0->TMR[0].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK; /* T_EN=1: Timer channel is enabled */
//	S32_NVIC->ICPR[1] = 1 << (LPIT0_Ch0_IRQn % 32); //clear pending IRQ
//	NVIC_EnableIRQ(LPIT0_Ch0_IRQn);
//	S32_NVIC->IP[LPIT0_Ch0_IRQn] =0x01;/* IRQ48-LPIT0 ch0: priority 1 of 0-15*/

	// LPIT0 channel 1 init
	LPIT0->MIER |= LPIT_MIER_TIE1_MASK; /* TIE1=1: Timer Interrupt Enabled fot Chan 1 */
	LPIT0->TMR[1].TVAL = 20000000; //PLL clock=40M ->0.5s~loadvalue=40M
	LPIT0->TMR[1].TCTRL |= LPIT_TMR_TCTRL_T_EN_MASK;/* T_EN=1: Timer channel is enabled */
	S32_NVIC->ICPR[1] = 1 << (LPIT0_Ch1_IRQn % 32); //clear pending IRQ
	NVIC_EnableIRQ(LPIT0_Ch1_IRQn);
	S32_NVIC->IP[LPIT0_Ch1_IRQn] =0x00;/* IRQ48-LPIT0 ch0: priority 0 of 0-15*/
}

