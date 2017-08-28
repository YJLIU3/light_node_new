//#include <stm32f10x_map.h>
//#include <nvic.h>
#include <sys/clock.h>
#include <sys/cc.h>
#include <sys/etimer.h>
//#include <debug-uart.h>

#include "contiki.h"
//#include "app_task.h"
#include "EFM32_include.h"


static volatile clock_time_t current_clock = 0;
static volatile unsigned long current_seconds = 0;
static unsigned int second_countdown = CLOCK_SECOND;

//void SysTick_handler(void) __attribute__ ((interrupt));



void RTC_IRQHandler(void)
{ 
    

    
    uint32_t rtc_if;
    rtc_if=RTC_IntGet();  
    RTC_IntClear(rtc_if); 
//     GPIO_PinOutToggle(gpioPortC, 3);
//    
    /*COMP0ÖÐ¶Ï*/ 
    if(rtc_if & RTC_IF_COMP0)
    {     
        RTC_CounterReset();               
       
        current_clock++;
        if(etimer_pending() && etimer_next_expiration_time() <= current_clock) 
        {
            etimer_request_poll();
//            printf("%d,%d\n", clock_time(),etimer_next_expiration_time  	()); 

        }
        if (--second_countdown == 0)
        {
            current_seconds++;
            second_countdown = CLOCK_SECOND;
        }

    }  
    

}




void clock_init()
{
    Rtc_Init();
}

clock_time_t clock_time(void)
{
    return current_clock;
}

#if 0
/* The inner loop takes 4 cycles. The outer 5+SPIN_COUNT*4. */

#define SPIN_TIME 2 /* us */
#define SPIN_COUNT (((MCK*SPIN_TIME/1000000)-5)/4)

#ifndef __MAKING_DEPS__

void clock_delay(unsigned int t)
{
    #ifdef __THUMBEL__ 
        asm volatile("1: mov r1,%2\n2:\tsub r1,#1\n\tbne 2b\n\tsub %0,#1\n\tbne 1b\n":"=l"(t):"0"(t),"l"(SPIN_COUNT));
    #else
        #error Must be compiled in thumb mode
    #endif
}
#endif
#endif /* __MAKING_DEPS__ */

unsigned long clock_seconds(void)
{
    return current_seconds;
}
