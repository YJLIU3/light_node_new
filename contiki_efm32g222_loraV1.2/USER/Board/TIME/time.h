#ifndef __TIME_H
#define __TIME_H
#include <stdbool.h>
#include "stdint.h"
#include "stdint.h"
#include "em_timer.h"
#include "em_assert.h"

/* 11718 Hz -> 12Mhz (clock frequency) / 1024 (prescaler)
  Setting TOP to 13761 results in an overflow each 1 seconds */				

#define TOP0  0x8000 // 
#define TOP1  46875    // 5ms


void TIMER0_IRQHandler(void);
void TIMER1_IRQHandler(void);
void TIMER1_Conging(void);
void Pwm_Init(void);









void TIMER_Reset_Counter(TIMER_TypeDef *timer);





#endif

