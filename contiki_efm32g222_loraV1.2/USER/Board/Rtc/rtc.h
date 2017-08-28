#ifndef __RTC_H
#define __RTC_H

#include "stdint.h"
#include "stdbool.h"




void Rtc_Init(void);

void RTC_IRQHandler(void);

extern uint32_t RTC_TOP0;
extern uint32_t RTC_TOP1;


#endif



