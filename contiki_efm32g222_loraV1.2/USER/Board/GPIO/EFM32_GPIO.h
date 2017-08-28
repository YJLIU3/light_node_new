#ifndef __GPIO_H
#define __GPIO_H
#include "em_gpio.h"

void EFM32_GPIO_Init(void) ;


#define LED1_ON()        GPIO_PinOutClear(gpioPortF,  5)    //��LED  PE12
#define LED1_OFF()       GPIO_PinOutSet(gpioPortF,    5)    //��LED  PE12

#define LED2_ON()        GPIO_PinOutClear(gpioPortF,  4)    //��LED  PE12
#define LED2_OFF()       GPIO_PinOutSet(gpioPortF,    4)    //��LED  PE12

#define LED3_ON()        GPIO_PinOutClear(gpioPortA,  2)    //��LED  PE12
#define LED3_OFF()       GPIO_PinOutSet(gpioPortA,    2)    //��LED  PE12



#endif










