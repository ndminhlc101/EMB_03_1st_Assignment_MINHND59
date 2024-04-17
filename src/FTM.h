#ifndef _FTM_H_
#define _FTM_H_

#include "S32K144.h"
#include <stdint.h>
#include<string.h>

#define SOSCDIV1_CLK 8000000

/* Functions for PWM */
void set_PWM(uint8_t submodule, uint8_t channel, uint32_t frequency, uint32_t dutyCycle);
void init_PWM(uint8_t submodule, uint8_t channel);
void start_PWM (uint8_t submodule);
void stop_PWM(uint8_t submodule);


#endif


