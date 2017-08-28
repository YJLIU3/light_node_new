#ifndef __ADC_H
#define __ADC_H

#include "stdint.h"


extern uint32_t Battery_Voltage;
uint32_t  Adc_Get_Voltage(void);
static void ADCConfig(void);

#endif


