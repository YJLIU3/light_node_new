#ifndef __EXTI_INTERRUPT_H
#define __EXTI_INTERRUPT_H
#include <stdint.h>
#include <stdbool.h>
#include "em_gpio.h"

void Gpio_EXTI_Config(void); 
void GPIO_ODD_IRQHandler(void) ; 
void GPIO_EVEN_IRQHandler(void); 
void Gpio_ODD_Config(void) ; 

void GPIO_Handler_Able(GPIO_Port_TypeDef port, unsigned int pin,  bool enable);

void GPIO_Clear_IF(GPIO_Port_TypeDef port, unsigned int pin);



#endif
