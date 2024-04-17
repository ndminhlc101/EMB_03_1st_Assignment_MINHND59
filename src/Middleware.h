#ifndef _MIDDLEWARE_H_
#define _MIDDLEWARE_H_
#include "clock_and_mode.h"
#include "Port.h"
#include "Gpio.h"
#include "LPIT.h"
#include "UART.h"
#include "CAN.h"
#include "FTM.h"

void LED_Init(void);
void myFTM_PWM_Init(void);
void Button_Init(void);
void myUART_Init(void);
void RGB_BlinkLed(void);
void myCAN_Init(void);
#endif
