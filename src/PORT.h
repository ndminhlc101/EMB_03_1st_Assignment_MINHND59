#ifndef _PORT_H_
#define _PORT_H_

#include"S32K144.h"
#include <stdint.h>
#include<string.h>

extern uint32_t SystemCoreClock;
typedef void(*CallBackType)(void);

typedef enum{
	PORT_MUX_ANALOG =0U,
	PORT_MUX_GPIO 	=1U,
	PORT_MUX_ALT2 	=2U,
	PORT_MUX_ALT3 	=3U,
	PORT_MUX_ALT4 	=4U,
	PORT_MUX_ALT5 	=5U,
	PORT_MUX_ALT6 	=6U,
	PORT_MUX_ALT7 	=7U,
}PORTMUXTYPE;

typedef enum{
	PORT_PULL_DISABLED 	=0U,
	PORT_PULL_DOWN	 	=2U,
	PORT_PULL_UP	 	=3U,
}PORTPULLTYPE;
typedef enum{
	PORT_INT_FALLING =10U,
	PORT_INT_DISABLE=0U,
	PORT_INT_RISING =9U,
	PORT_INT_BOTHEDGE =0xBU,
}PORTINTTYPE;
typedef struct {
	PORT_Type* PORTx;
	uint8_t pin;
	PORTMUXTYPE mux;
	PORTPULLTYPE pull;
	PORTINTTYPE interrupt;
	IRQn_Type IRQ;
	CallBackType callback;
}PortConfigType;

typedef void (*SysTickCallback_t)(void);

typedef enum _Systick_interrupt_direction
{
	Systick_Interrupt_disable = 0U, /*!< Set current pin as digital input*/
	Systick_Interrupt_enable = 1U,	/*!< Set current pin as digital output*/
} SystickINT_t;

typedef struct
{
	SystickINT_t INTERRUPT;
	SysTickCallback_t callback;
	uint32_t reload;
} systick_config_t;


void systick_delay_init(uint32_t ms);
void NVIC_EnableIRQ(IRQn_Type IRQn);
void myPORT_PinInit(PortConfigType* config );
void PORT_EXTI_Config (PortConfigType* config);
void PORT_EXTI_ClearFlag (PORT_Type* PORTx);
#endif
