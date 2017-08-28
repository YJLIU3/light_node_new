#include "em_cmu.h"
#include "em_gpio.h"
#include "rcc.h"

//////使用外部晶振 HFRCOBand=21MHz，和外部部32.768晶振//
//void Rcc_Init(void)
//{
//	
//	CMU_ClockSelectSet(cmuClock_HF,  cmuSelect_HFXO); //使用外部24MHz  
//	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); //使用外部32.768khz
//	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO); //使用外部32.768khz

//	CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_2);     //HFPERCLK = 12mhz
//	CMU_ClockDivSet(cmuClock_CORE,  cmuClkDiv_2);     //    CORE = 24mhz

//    CMU_ClockEnable(cmuClock_HFPER,  true);  //总线时钟使能
//	CMU_ClockEnable(cmuClock_CORE,   true);  //内核时钟使能
//    CMU_ClockEnable(cmuClock_CORELE, true);  //RTC时钟使能
//	CMU_ClockEnable(cmuClock_GPIO,   true);  //GPIO时钟使能
//  

////	//PA1,PA2输出时钟//
////	GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);  //将相应的GPIO口设置成输出，低频
////	GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1);  //将相应的GPIO口设置成输出，高频
////	CMU->CTRL |= CMU_CTRL_CLKOUTSEL0_HFXO + CMU_CTRL_CLKOUTSEL1_LFXO;
////	CMU->ROUTE = CMU_ROUTE_CLKOUT0PEN | CMU_ROUTE_CLKOUT1PEN | CMU_ROUTE_LOCATION_LOC0; //允许GPIO的复用功能，并且置位允许位

//}



//使用内部晶振 HFRCOBand=21MHz，和内部32.768晶振//
void Rcc_Init(void)
{
//    CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
//    /* Starting LFRCO and waiting until it is stable */
//    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
 
	CMU_HFRCOBandSet(cmuHFRCOBand_21MHz);
	CMU_ClockSelectSet(cmuClock_HF,  cmuSelect_HFRCO);//使用内部21MHz
    CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);//使用内部32.768KHZ    
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);//使用内部32.768KHZ
    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_1);    //HFPERCLK =21MHZ
	CMU_ClockDivSet(cmuClock_CORE, cmuClkDiv_1);     //CORE = 21MHZ

    CMU_ClockEnable(cmuClock_HFPER,  true);  //总线时钟使能
	CMU_ClockEnable(cmuClock_CORE,   true);  //内核时钟使能
    CMU_ClockEnable(cmuClock_CORELE, true);  //RTC时钟使能
	CMU_ClockEnable(cmuClock_GPIO,   true);  //GPIO时钟使能

//	//PA1,PA2输出时钟//
//	GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);  //将相应的GPIO口设置成输出，低频
//	GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1);  //将相应的GPIO口设置成输出，高频
//	CMU->CTRL |= CMU_CTRL_CLKOUTSEL0_HFRCO + CMU_CTRL_CLKOUTSEL1_LFRCO;//CMU_CTRL_CLKOUTSEL0_HFXO; //配置为HFRCO输出	
//	CMU->ROUTE = CMU_ROUTE_CLKOUT0PEN | CMU_ROUTE_CLKOUT1PEN | CMU_ROUTE_LOCATION_LOC0; //允许GPIO的复用功能，并且置位允许位	

}
 

