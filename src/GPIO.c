#include "Gpio.h"

//GPIO
//input : GPIOx?Pin?Direction(in/out)?Set/clear


void GPIO_SetPin(GPIO_Type *PTx, uint8_t pinNumber)
{
	if (PTx != NULL && pinNumber <= PIN_NUMBER_LIMIT)
	{
		PTx->PCOR |= (1U << pinNumber);
	}
}

void GPIO_ClearPin(GPIO_Type *PTx, uint8_t pinNumber)
{
	if (PTx != NULL && pinNumber <= PIN_NUMBER_LIMIT)
	{
		PTx->PSOR |= (1U << pinNumber);
	}
}

uint8_t GPIO_ReadPin(GPIO_Type *PTx, uint8_t pinNumber)
{
	if (PTx != NULL && pinNumber <= PIN_NUMBER_LIMIT)
    {
        if ((PTx->PDDR & (1u << pinNumber)) == 0)
        {
            return (uint8_t)((PTx->PDIR >> pinNumber) & 1u);//read status pin input
        }
        else
        {
            return (uint8_t)((PTx->PDOR >> pinNumber) & 1u);//read status pin ouput
        }
    }
    else
    {
        return INVALID_PIN_NUMBER;
    }
}

void GPIO_TogglePin(GPIO_Type *PTx, uint8_t pinNumber) {
//    if (GPIO_ReadPin(GPIOx, pinNumber)!=0)
//    {
//    	GPIOx->PCOR |= (1u << pinNumber); // Clear the pin
//    } else {
//    	GPIOx->PSOR |= (1u << pinNumber); // Set the pin
//    }
	if (PTx != NULL && pinNumber <= PIN_NUMBER_LIMIT)
	{
		PTx->PTOR |= (1U << pinNumber);
	}
}

void myGPIO_Init(GPIOConfigType* gpio_config){
	 if (gpio_config != NULL && gpio_config->PTx != NULL)
	 {
		 //Direction(in/out)
		 	if ( gpio_config->pinDirection == input)
		 		{
		 			gpio_config->PTx->PDDR &= ~(1U << gpio_config->pin);
		 		}
		 	else
		 		{
		 			gpio_config->PTx->PDDR |=(1U << gpio_config->pin);
		 		}

		 	//GPIO_Type->pin setting (set/clear)
		 	if (gpio_config->setting == clear)
		 		{
		 //			gpio_config->GPIOx->PCOR |= (1U << gpio_config->pin);
		 			GPIO_ClearPin(gpio_config->PTx,gpio_config->pin);
		 		}
		 		else
		 		{
		 //			gpio_config->GPIOx->PSOR |= (1U << gpio_config->pin);
		 			GPIO_SetPin(gpio_config->PTx,gpio_config->pin);
		 		}
	 }
}
