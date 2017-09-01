#ifndef __LED_H
#define __LED_H
#ifdef __cplusplus
extern "C"
{
#endif
//#include "EFM32_include.h"
#include "em_gpio.h"

	void led_Init(void);

	#define LED1_ON()        GPIO_PinOutClear(gpioPortC,  2)    //开LED  PC2
	#define LED1_OFF()       GPIO_PinOutSet(gpioPortC,    2)    //关LED  PC2

	#define LED2_ON()        GPIO_PinOutClear(gpioPortC,  13)    //开LED  PC3
	#define LED2_OFF()       GPIO_PinOutSet(gpioPortC,    13)    //关LED  PC3

	#define LED3_ON()        GPIO_PinOutClear(gpioPortC,  11)    //开LED  PC3
	#define LED3_OFF()       GPIO_PinOutSet(gpioPortC,    11)    //关LED  PC3
	#ifdef __cplusplus
}
#endif

#endif
























