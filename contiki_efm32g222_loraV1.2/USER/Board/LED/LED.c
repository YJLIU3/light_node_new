#include "led.h"


void led_Init(void)
{ 
    GPIO_PinModeSet(gpioPortC, 2,  gpioModePushPull,     1); /*  LED1   */
    GPIO_PinModeSet(gpioPortC, 3,  gpioModePushPull,     1); /*  LED2   */
    GPIO_PinModeSet(gpioPortC, 4,  gpioModePushPull,     1); /*  LED2   */
//    GPIO_PinOutToggle(gpioPortD, 7);
}
