/*****************************************************************************
 * @file
 * @brief LETIMER demo application, LETIMER triggered by RTC
 * @author Energy Micro AS
 * @version 1.05
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#include "EFM32_include.h"




/* Maximum value for compare registers */
//uint16_t RTC_COMP=3840;

uint32_t RTC_TOP0=256;
uint32_t RTC_TOP1=1;

/**************************************************************************//**
 * @brief  RTC_setup
 * Configures and starts the RTC
 *****************************************************************************/
void Rtc_Init(void)
{
    /* Enable necessary clocks */
    CMU_ClockEnable(cmuClock_RTC, true);

    /* Each RTC tick is 1 second */
    CMU_ClockDivSet(cmuClock_RTC, cmuClkDiv_1);
    

    
    /* Setting up RTC with compare value.
    Compare match will occur at 7.8125 ms */
    RTC_CompareSet(0, RTC_TOP0);
//    RTC_CompareSet(1, RTC_TOP1-1);

    /* Enable RTC interrupt from COMP0 */
    RTC_IntEnable(RTC_IEN_COMP0);
//    RTC_IntDisable(RTC_IEN_COMP1);

    NVIC_SetPriority(RTC_IRQn , 0);

    /* Enable RTC interrupt vector in NVIC */
    NVIC_EnableIRQ(RTC_IRQn);

    /* Enable RTC */
    RTC_Enable(true);



//  RTC_Init_TypeDef rtcInit;

//  /* Select LFXO as clock source for the RTC */
//  CMU_ClockSelectSet(cmuClock_LFA,cmuSelect_LFXO);
//  CMU_ClockEnable(cmuClock_RTC, true);
//  
//  /* Set RTC pre-scaler */
//  CMU_ClockDivSet(cmuClock_RTC,cmuClkDiv_1);
//   
//  /* Enable clock to low energy modules */
//  CMU_ClockEnable(cmuClock_CORELE, true);

//  /* RTC configuration struct */
//  rtcInit.debugRun  = false;
//  rtcInit.comp0Top  = false;
//  rtcInit.enable    = false;

//  /* Initialize RTC */
//  RTC_Init(&rtcInit);

//  /* Set RTC compare value for first display update */
//  RTC_CompareSet(0, 256);
//  
////  /* Set RTC compare value for first temperature compensation */
////  RTC_CompareSet(1, RTC_COUNTS_BETWEEN_TC );

//  /* Enable COMP0 interrupt to trigger display update, */
//  /*        COMP1 interrupt to trigger temperature compensation, and */
//  /*        OF interrupt to keep track of counter overflows */
//  RTC_IntEnable(RTC_IEN_COMP0  );
//  
//  /* Enable RTC interrupts */
//  NVIC_ClearPendingIRQ(RTC_IRQn);
//  NVIC_EnableIRQ(RTC_IRQn);

//  /* Enable RTC */
//  RTC_Enable(true);

   

}












