#include "em_cmu.h"
#include "em_gpio.h"
#include "rcc.h"

//////ʹ���ⲿ���� HFRCOBand=21MHz�����ⲿ��32.768����//
//void Rcc_Init(void)
//{
//	
//	CMU_ClockSelectSet(cmuClock_HF,  cmuSelect_HFXO); //ʹ���ⲿ24MHz  
//	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO); //ʹ���ⲿ32.768khz
//	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO); //ʹ���ⲿ32.768khz

//	CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_2);     //HFPERCLK = 12mhz
//	CMU_ClockDivSet(cmuClock_CORE,  cmuClkDiv_2);     //    CORE = 24mhz

//    CMU_ClockEnable(cmuClock_HFPER,  true);  //����ʱ��ʹ��
//	CMU_ClockEnable(cmuClock_CORE,   true);  //�ں�ʱ��ʹ��
//    CMU_ClockEnable(cmuClock_CORELE, true);  //RTCʱ��ʹ��
//	CMU_ClockEnable(cmuClock_GPIO,   true);  //GPIOʱ��ʹ��
//  

////	//PA1,PA2���ʱ��//
////	GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);  //����Ӧ��GPIO�����ó��������Ƶ
////	GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1);  //����Ӧ��GPIO�����ó��������Ƶ
////	CMU->CTRL |= CMU_CTRL_CLKOUTSEL0_HFXO + CMU_CTRL_CLKOUTSEL1_LFXO;
////	CMU->ROUTE = CMU_ROUTE_CLKOUT0PEN | CMU_ROUTE_CLKOUT1PEN | CMU_ROUTE_LOCATION_LOC0; //����GPIO�ĸ��ù��ܣ�������λ����λ

//}



//ʹ���ڲ����� HFRCOBand=21MHz�����ڲ�32.768����//
void Rcc_Init(void)
{
//    CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
//    /* Starting LFRCO and waiting until it is stable */
//    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
 
	CMU_HFRCOBandSet(cmuHFRCOBand_21MHz);
	CMU_ClockSelectSet(cmuClock_HF,  cmuSelect_HFRCO);//ʹ���ڲ�21MHz
    CMU_OscillatorEnable(cmuOsc_HFRCO, true, true);
	CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);//ʹ���ڲ�32.768KHZ    
	CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_CORELEDIV2);//ʹ���ڲ�32.768KHZ
    CMU_OscillatorEnable(cmuOsc_LFXO, true, true);
	CMU_ClockDivSet(cmuClock_HFPER, cmuClkDiv_1);    //HFPERCLK =21MHZ
	CMU_ClockDivSet(cmuClock_CORE, cmuClkDiv_1);     //CORE = 21MHZ

    CMU_ClockEnable(cmuClock_HFPER,  true);  //����ʱ��ʹ��
	CMU_ClockEnable(cmuClock_CORE,   true);  //�ں�ʱ��ʹ��
    CMU_ClockEnable(cmuClock_CORELE, true);  //RTCʱ��ʹ��
	CMU_ClockEnable(cmuClock_GPIO,   true);  //GPIOʱ��ʹ��

//	//PA1,PA2���ʱ��//
//	GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 1);  //����Ӧ��GPIO�����ó��������Ƶ
//	GPIO_PinModeSet(gpioPortA, 2, gpioModePushPull, 1);  //����Ӧ��GPIO�����ó��������Ƶ
//	CMU->CTRL |= CMU_CTRL_CLKOUTSEL0_HFRCO + CMU_CTRL_CLKOUTSEL1_LFRCO;//CMU_CTRL_CLKOUTSEL0_HFXO; //����ΪHFRCO���	
//	CMU->ROUTE = CMU_ROUTE_CLKOUT0PEN | CMU_ROUTE_CLKOUT1PEN | CMU_ROUTE_LOCATION_LOC0; //����GPIO�ĸ��ù��ܣ�������λ����λ	

}
 

