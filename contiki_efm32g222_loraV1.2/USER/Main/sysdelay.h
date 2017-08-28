#ifndef __SYSDELAY_H
#define __SYSDELAY_H

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif
	
void Systick_Delay_Init(void);
void Systick_Delay_Cycle(unsigned long ulCycle);
void Systick_Delay_uS(unsigned long uluS);
void Systick_Delay_mS(unsigned long ulmS);
void Systick_Delay_1S(unsigned long ul1S);

void delay_10ms(uint32_t time);  //10ms—” ±
  
	
#ifdef __cplusplus
}
#endif


#endif
