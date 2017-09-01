#include "led.h"


void led_Init(void)
{ 
    GPIO_PinModeSet(gpioPortC, 2,  gpioModePushPull,     1); /*  LED1   */
    GPIO_PinModeSet(gpioPortC, 13,  gpioModePushPull,     1); /*  LED2   */
    GPIO_PinModeSet(gpioPortC, 11,  gpioModePushPull,     1); /*  LED2   */
//    GPIO_PinOutToggle(gpioPortD, 7);
}
