#include "FTM.h"

FTM_Type * FTM_MODULE[4] = FTM_BASE_PTRS;//array from FTM0 to FTM4

/******************************************************************************
* Function: initPWM
* Description: Initializes PWM by submodule(0-4),channel(0-7), frequencyEdge-Aligned PWM (EPWM) Mode
******************************************************************************/
void init_PWM(uint8_t submodule, uint8_t channel)
{
	/*PWMP PCR Init*/
//	PCC->PCCn[PCC_PORTC_INDEX ]|=PCC_PCCn_CGC_MASK;
//	PORTC->PCR[0] |= PORT_PCR_MUX(2);//FTM0 Ch0
//	PORTC->PCR[1] |= PORT_PCR_MUX(2);//FTM0 Ch1
	//Clk src
	PCC->PCCn[PCC_FTM0_INDEX] &= ~PCC_PCCn_CGC_MASK; /* Ensure clk disabled for config */
	PCC->PCCn[PCC_FTM0_INDEX] |= PCC_PCCn_PCS(1)/* Clock Src=1, 8 MHz SOSCDIV1_CLK */
								|PCC_PCCn_CGC_MASK; /* Enable clock for FTM regs */
	/* Feature Mode Selection (MODE) */
	FTM_MODULE[submodule]->MODE |= FTM_MODE_WPDIS(1); /* Write protect to registers	disabled (default) */
	/* Status and Control (SC) */
	/* TOIE (Timer Overflow Interrupt Ena) = 0 (default) */
	/* CPWMS (Center aligned PWM Select) = 0 (default, up count) */
	/* CLKS (Clock source) = 0 (default, no clock; FTM disabled) */
	FTM_MODULE[submodule]->SC = 0x00000000; /* Clear the status and control register*/
//	FTM_MODULE[submodule]->SC |= FTM_SC_CLKS(0b11); /* Select external clock */
	FTM_MODULE[submodule]->SC |= FTM_SC_PS(7);	/* PS (Prescaler factor) = 7. Prescaler = 128 */
	/* Enable the respective channel */
	FTM0->SC |= FTM_SC_PWMEN0(1);/* Enable PWM channel 0 output*/
//	FTM0->SC |= FTM_SC_PWMEN1(1);/* Enable PWM channel 1 output*/
	FTM_MODULE[submodule]->COMBINE = 0x00000000; /* FTM mode settings used: DECAPENx,MCOMBINEx, COMBINEx=0 */
	/* Channel Control */
	FTM_MODULE[submodule]->CONTROLS[channel].CnSC = 0; /* Clear the register*/
	FTM_MODULE[submodule]->CONTROLS[channel].CnSC |= FTM_CnSC_MSB(1); /* MSB : Edge	Align PWM */
	FTM_MODULE[submodule]->CONTROLS[channel].CnSC |= FTM_CnSC_MSA(0); /* MSA : Edge	Align PWM */
	FTM_MODULE[submodule]->CONTROLS[channel].CnSC |= FTM_CnSC_ELSB(1); /* ELSB :High-true pulses */
	FTM_MODULE[submodule]->CONTROLS[channel].CnSC |= FTM_CnSC_ELSA(0); /* ELSA :High-true pulses */
	/* Edge-Aligned PWM (EPWM) */
	FTM_MODULE[submodule]->CNTIN |= FTM_CNTIN_INIT(0);/* Always start counter from 0 */
	FTM_MODULE[submodule]->POL = 0x00000000; /* Polarity for all channels is active high (default) */
	FTM_MODULE[submodule]->CONF |= FTM_CONF_BDMMODE(0b11); /* Optional: enable in debug mode */


}
/******************************************************************************
* Function: setPWM
* Description: Sets the output PWM for a given channel in FTM_MODULE
******************************************************************************/
void set_PWM(uint8_t submodule, uint8_t channel, uint32_t frequency, uint32_t dutyCycle)
{

	volatile uint8_t PS= (FTM_MODULE[submodule]->SC  & FTM_SC_PS_MASK ) >>FTM_SC_PS_SHIFT;
	volatile uint8_t prescale = 1 << PS;
	volatile uint16_t modvalue= SOSCDIV1_CLK/(frequency*prescale) -1;
	volatile uint16_t cnvValue = (modvalue + 1) * dutyCycle / 100;
	FTM_MODULE[submodule]->MOD = modvalue; /* Set the PWM frequency */
	FTM_MODULE[submodule]->CONTROLS[channel].CnV = cnvValue; /* Set the PWM duty cycle	*/

}
/******************************************************************************
* Function: start_PWM
* Description: Start FTMx counter
******************************************************************************/
void start_PWM (uint8_t submodule) {
	FTM_MODULE[submodule]->SC |= FTM_SC_CLKS(3); /* Select external clock */
	 /* Start FTM0 counter with clk source = external clock (SOSCDIV1_CLK)*/
}
/******************************************************************************
* Function: start_PWM
* Description: Stop FTMx counter
******************************************************************************/
void stop_PWM(uint8_t submodule) {
	FTM_MODULE[submodule]->SC &= ~FTM_SC_CLKS_MASK;
	/* Stop generating PWM FTM0 counter*/
}
