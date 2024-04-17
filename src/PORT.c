#include "Port.h"

static CallBackType PortCDCallBack =NULL;

void myPORT_PinInit(PortConfigType* config )
{
	if (config != NULL && config->PORTx != NULL)
	    {
		//MUX
			config->PORTx->PCR[config->pin]&= ~PORT_PCR_MUX_MASK;
			if ( PORT_MUX_ANALOG != config->mux )
			{
				config->PORTx->PCR[config->pin] |= PORT_PCR_MUX(config->mux);
			}
			//Pull
			if(config->pull==0)
			{
				config->PORTx->PCR[config->pin] &= ~PORT_PCR_PE_MASK;
				config->PORTx->PCR[config->pin] &= ~PORT_PCR_PS_MASK;
			}
			else if (config->pull==1)
			{
				config->PORTx->PCR[config->pin] |= PORT_PCR_PE(1U);
				config->PORTx->PCR[config->pin] &= ~PORT_PCR_PS_MASK;
			}
			else if (config->pull==2)
			{
				config->PORTx->PCR[config->pin] |= PORT_PCR_PE(1U);
				config->PORTx->PCR[config->pin] |= PORT_PCR_PS(1U);
			}
	    }
}

void NVIC_EnableIRQ(IRQn_Type IRQn)
{
  if (0<= (int32_t)(IRQn) &&  (int32_t)(IRQn)<= 32)
  {
	  S32_NVIC->ISER[0U] = (uint32_t)(1UL << (((uint32_t)IRQn) & 0x1FUL));
  }
  if (32<= (int32_t)(IRQn) &&  (int32_t)(IRQn) <= 63)
    {
  	  S32_NVIC->ISER[1U] = (uint32_t)(1UL << (((uint32_t)(IRQn-32)) & 0x1FUL));
    }
  if (64<= (int32_t)(IRQn) &&  (int32_t)(IRQn) <= 95)
    {
  	  S32_NVIC->ISER[2U] = (uint32_t)(1UL << (((uint32_t)(IRQn-32*2)) & 0x1FUL));
    }
  if ((int32_t)(IRQn)>=96 &&  (int32_t)(IRQn) <= 127)
    {
  	  S32_NVIC->ISER[3U] = (uint32_t)(1UL << (((uint32_t)(IRQn-32*3)) & 0x1FUL));
    }
  if ( (int32_t)(IRQn)>=128 &&  (int32_t)(IRQn) <= 159)
    {
  	  S32_NVIC->ISER[4U] = (uint32_t)(1UL << (((uint32_t)(IRQn-32*4)) & 0x1FUL));
    }
  if (160<= (int32_t)(IRQn) &&  (int32_t)(IRQn) <= 191)
     {
   	  S32_NVIC->ISER[5U] = (uint32_t)(1UL << (((uint32_t)(IRQn-32*5)) & 0x1FUL));
     }
  if (192<= (int32_t)(IRQn) &&  (int32_t)(IRQn) <= 223)
     {
   	  S32_NVIC->ISER[6U] = (uint32_t)(1UL << (((uint32_t)(IRQn-32*6)) & 0x1FUL));
     }
  if (224<= (int32_t)(IRQn) &&  (int32_t)(IRQn) <= 255)
     {
   	  S32_NVIC->ISER[7U] = (uint32_t)(1UL << (((uint32_t)(IRQn-32*7)) & 0x1FUL));
     }
}

void PORT_EXTI_Config (PortConfigType* config){
	if (config != NULL && config->PORTx != NULL)
	{
		config->PORTx->PCR[config->pin] &= ~PORT_PCR_IRQC_MASK;
		if(PORT_INT_DISABLE != config->interrupt)
		{
			NVIC_EnableIRQ(config->IRQ);//Enable interrupt
			//S32_NVIC->ISER[1U] = (uint32_t)(1UL << (((uint32_t)29)& 0x1FUL));

			config->PORTx->PCR[config->pin] |= PORT_PCR_IRQC(config->interrupt);//(Falling/Rising)
			//Assign callback function
			if(PORTC==config->PORTx||PORTD==config->PORTx)
			{
				PortCDCallBack = config->callback;
			}
		}
	}
}

void PORT_EXTI_ClearFlag (PORT_Type* PORTx)
{
	if (PORTx != NULL)
	{
		uint8_t idx;
		for(idx=0;idx<32;idx++){
			if((PORTx->ISFR &= (1<<idx))!=0U)
			{
				 //clear flag
				 PORTx->ISFR |=(1<<idx);
			}
		}
	}
}

//SysTickCallback_t s_SYSTcallback = NULL;
//void Systick_init(systick_config_t *UserConfig)
//{
//	// Clock configuration - clock core
//	S32_SysTick->CSR |= S32_SysTick_CSR_CLKSOURCE_MASK;
//	// Enable interrupt, call back
//	if (0 != UserConfig->INTERRUPT)
//	{
//		S32_SysTick->CSR |= S32_SysTick_CSR_TICKINT_MASK;
//		s_SYSTcallback = UserConfig->callback;
//	}
//	// Reload value, clear counter
//	S32_SysTick->RVR = UserConfig->reload;
//	// Clear counter
//	S32_SysTick->CVR |= 0U;
//	// Enable timer
//	S32_SysTick->CSR |= S32_SysTick_CSR_ENABLE_MASK;
//}

void systick_delay_init(uint32_t ms)
{
	uint32_t i;
	volatile uint32_t reload=ms*(SystemCoreClock/4000) -1;
	for (i = 0; i < 4; ++i) {
		S32_SysTick->CSR |= S32_SysTick_CSR_CLKSOURCE_MASK;
	//	S32_SysTick->CSR |= S32_SysTick_CSR_TICKINT(1U);
	//	S32_SysTick->CVR |= 0U;
		S32_SysTick->CSR |= S32_SysTick_CSR_ENABLE_MASK;
		S32_SysTick->RVR = S32_SysTick_RVR_RELOAD(reload);
		while ((S32_SysTick->CSR & S32_SysTick_CSR_COUNTFLAG_MASK) == 0) ;
		S32_SysTick->CSR &= ~S32_SysTick_CSR_ENABLE_MASK;
	}
}



//void SysTick_Handler()
//{
//
//}

//void PORTC_PORTD_IRQHandler(void)
//{
//	uint8_t idx;
//	 for(idx=0;idx<32;idx++){
//		 if((PORTC->ISFR &= (1<<idx))!=0U)
//		 {
//			 //clear flag
//			 PORTC->ISFR |=(1<<idx);
//			 //callback function to call application code
//			 (*PortCDCallBack)(idx);
//		 }
//	 }
//}
