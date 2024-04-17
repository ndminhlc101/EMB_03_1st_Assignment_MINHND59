#ifndef _GPIO_H_
#define _GPIO_H_

#include "S32K144.h"
#include <stdint.h>
#include<string.h>

#define PIN_NUMBER_LIMIT 31
#define INVALID_PIN_NUMBER 255

typedef enum
{
    input = 0U,  /*!< Set current pin as digital input*/
    output = 1U, /*!< Set current pin as digital output*/
} GPIO_Pin_Direction;

typedef enum
{
    clear = 0U,
    set = 1U,
} GPIO_Pin_Setting;

typedef struct
{
	GPIO_Type* PTx;
    uint8_t pin;
    GPIO_Pin_Direction pinDirection;
    GPIO_Pin_Setting setting;
}GPIOConfigType ;

void GPIO_SetPin(GPIO_Type *PTx, uint8_t pinNumber);
void GPIO_ClearPin(GPIO_Type *PTx, uint8_t pinNumber);
uint8_t GPIO_ReadPin(GPIO_Type *PTx, uint8_t pinNumber);
void GPIO_TogglePin(GPIO_Type *PTx, uint8_t pinNumber);
void myGPIO_Init(GPIOConfigType* gpio_config);

#endif


