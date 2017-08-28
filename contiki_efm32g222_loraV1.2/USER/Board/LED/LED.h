#ifndef __LED_H
#define __LED_H

//#include "EFM32_include.h"
#include "em_gpio.h"

void led_Init(void);

#define LED1_ON()        GPIO_PinOutClear(gpioPortC,  2)    //��LED  PC2
#define LED1_OFF()       GPIO_PinOutSet(gpioPortC,    2)    //��LED  PC2

#define LED2_ON()        GPIO_PinOutClear(gpioPortC,  3)    //��LED  PC3
#define LED2_OFF()       GPIO_PinOutSet(gpioPortC,    3)    //��LED  PC3

#define LED3_ON()        GPIO_PinOutClear(gpioPortC,  4)    //��LED  PC3
#define LED3_OFF()       GPIO_PinOutSet(gpioPortC,    4)    //��LED  PC3


#endif
























