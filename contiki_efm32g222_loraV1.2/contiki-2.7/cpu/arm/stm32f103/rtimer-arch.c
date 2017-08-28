/**
* \addtogroup mbxxx-platform
*
* @{
*/

/*
* Copyright (c) 2010, STMicroelectronics.
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions
* are met:
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above
*    copyright notice, this list of conditions and the following
*    disclaimer in the documentation and/or other materials provided
*    with the distribution.
* 3. The name of the author may not be used to endorse or promote
*    products derived from this software without specific prior
*    written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
* OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
* GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
* NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/

/**
* \file
*			Real-timer specific implementation for STM32W.
* \author
*			Salvatore Pitrulli <salvopitru@users.sourceforge.net>
*/

#include "sys/energest.h"
#include "sys/rtimer.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"

#include "time.h"
extern uint8_t Sleep_Flag;

#define DEBUG 0
#if DEBUG
#include <stdio.h>
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

static uint32_t TOP2=375;


static uint32_t time_msb = 0;   /* Most significant bits of the current time. */


//static rtimer_clock_t now, clock_to_wait;
static rtimer_clock_t  clock_to_wait;
/* time of the next rtimer event. Initially is set to the max
value. */
//static rtimer_clock_t next_rtimer_time = 0;




void TIMER1_IRQHandler(void)
{ 
    /* Clear flag for TIMER0 overflow interrupt */
    TIMER_IntClear(TIMER1, TIMER_IF_OF);
    
    time_msb++;
    if(time_msb==clock_to_wait)
    {
        time_msb=0;
        /* Compare event. */

        ENERGEST_ON(ENERGEST_TYPE_IRQ);
        
        rtimer_run_next();
        TIMER_Enable(TIMER1,0);  
        rtimer_arch_disable_irq(); 
        
        ENERGEST_OFF(ENERGEST_TYPE_IRQ);
               
    }
    else
    {
        TIMER_Reset_Counter(TIMER1);
        TIMER_TopSet(TIMER1, TOP2);
        TIMER_Enable(TIMER1,1);       
    }

    
}


/*---------------------------------------------------------------------------*/
void rtimer_arch_init(void)
{
    /* Initialize chip */
    /* Enable clock for TIMER0 module */
    CMU_ClockEnable(cmuClock_TIMER1, true); 
    /* Configure pin as push pull output for LED/pin drive */

    /* Select TIMER0 parameters */  
    TIMER_Init_TypeDef timerInit1 =
    {
    .enable     = false, 
    .debugRun   = true, 
    .prescale   = timerPrescale32, 
    .clkSel     = timerClkSelHFPerClk, 
    .fallAction = timerInputActionNone, 
    .riseAction = timerInputActionNone, 
    .mode       = timerModeUp, 
    .dmaClrAct  = false,
    .quadModeX4 = false, 
    .oneShot    = false, 
    .sync       = false, 
    };

    /* Enable overflow interrupt */  
    TIMER_IntEnable(TIMER1, TIMER_IF_OF);

    NVIC_SetPriority(TIMER1_IRQn, 2);
    /* Enable TIMER0 interrupt vector in NVIC */
    NVIC_DisableIRQ(TIMER1_IRQn);

    /* Set TIMER Top value */
    TIMER_TopSet(TIMER1, TOP1);

    /* Configure TIMER */
    TIMER_Init(TIMER1, &timerInit1);
    TIMER_Enable(TIMER1,0);

}
/*---------------------------------------------------------------------------*/
void rtimer_arch_disable_irq(void)
{
    NVIC_DisableIRQ(TIMER1_IRQn);
}
/*---------------------------------------------------------------------------*/
void rtimer_arch_enable_irq(void)
{
    NVIC_EnableIRQ(TIMER1_IRQn);
}
/*---------------------------------------------------------------------------*/
rtimer_clock_t rtimer_arch_now(void)
{
    rtimer_clock_t t;
    t=TIMER_CounterGet(TIMER1);
//    ATOMIC(t = ((rtimer_clock_t) time_msb << 16) | TIM1_CNT;)
    return t;
}

/*---------------------------------------------------------------------------*/
void rtimer_arch_schedule(rtimer_clock_t t)
{
//    rtimer_clock_t now, clock_to_wait;

//    next_rtimer_time = t;
//    now = rtimer_arch_now();
//    clock_to_wait = t - now;
    clock_to_wait = t ;
    time_msb=0;

    TIMER_Reset_Counter(TIMER1);
    TIMER_TopSet(TIMER1, TOP2);
    TIMER_Enable(TIMER1,1);
    TIMER_IntClear(TIMER1, TIMER_IF_OF);
    rtimer_arch_enable_irq();
/* else compare register will be set at overflow interrupt closer to the rtimer event. */
}
/*---------------------------------------------------------------------------*/
/** @} */

