#include "em_gpio.h"
#include "include.h"
#include "EFM32_gpio.h"
void EFM32_GPIO_Init(void)
{ 
    RF125K_IO_INIT() ;
    HMC_IO_INIT();
    GPIO_PinModeSet(gpioPortA, 2,  gpioModePushPull,     1); /*  LED2   */
//    GPIO_PinModeSet(gpioPortF, 5,  gpioModePushPull,     1); /*  LED1   */
//    GPIO_PinModeSet(gpioPortF, 4,  gpioModePushPull,     1); /*  LED2   */
}






										
		

