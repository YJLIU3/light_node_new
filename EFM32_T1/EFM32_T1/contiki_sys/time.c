#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"
#include "time.h"
#include "em_leuart.h"





/**************************************************************************//**
// * @brief TIMER0_IRQHandler
// * Interrupt Service Routine TIMER0 Interrupt Line
// *****************************************************************************/
//void TIMER0_IRQHandler(void)
//{ 
//    /* Clear flag for TIMER0 overflow interrupt */
//    TIMER_IntClear(TIMER0, TIMER_IF_OF);


//    TIMER_Enable(TIMER0,0);	  /* �رռ���*/                               
//}






//void TIMER1_IRQHandler(void)
//{ 
////    /* Clear flag for TIMER0 overflow interrupt */
//    TIMER_IntClear(TIMER1, TIMER_IF_OF);
////    if(INTERRUPT_STATE.RECEIVE_ANSWER_SUCCEED==1)
////    {
////        INTERRUPT_STATE.RECEIVE_ANSWER_SUCCEED=0;
////        Timer_State.Timer0_TimeOut_State=0;
////        Timer_State.Timer0_Ask_Success_State=1;
////    }
////    else	
////    {
////        INTERRUPT_STATE.RECEIVE_ANSWER_SUCCEED=0;
////        Timer_State.Timer0_TimeOut_State=1;
////        Timer_State.Timer0_Ask_Success_State=0;
////    }
////    TIMER_Enable(TIMER1,0);  
//    printf("��ʱ������\n"); 
//}



void TIMER_Reset_Counter(TIMER_TypeDef *timer)
{
  EFM_ASSERT(TIMER_REF_VALID(timer));
  /* Reset counter */
  timer->CNT = _TIMER_CNT_RESETVALUE;

}







/**************************************************************************//**
* @brief TIMER0_IRQHandler
* Interrupt Service Routine TIMER0 Interrupt Line
*****************************************************************************/
void Pwm_Init(void)
{ 
    /* Enable clock for TIMER0 module */
    CMU_ClockEnable(cmuClock_TIMER0, true);

    /* Set CC0 location 0 pin (PA1) as output */
    GPIO_PinModeSet(gpioPortA, 1, gpioModePushPull, 0);

    /* Select CC channel parameters */
    TIMER_InitCC_TypeDef timerCCInit = 
    {

        /*  TIMER0�Ƚ�ͨ��0��ʼ���ṹ�壬�������벶�񲿷�ģ�鹦��*/
        .eventCtrl  = timerEventEveryEdge ,                             /* �����¼���Ӧ����             */
        .edge       = timerEdgeRising ,                                 /* ���ñ��ؼ���������ı���   */
        .prsSel     = timerPRSSELCh1 ,                                  /* PRSͨ��ѡ��                  */
        .cufoa      = timerOutputActionNone ,                           /* ���ö�ʱ������ʱ�������Ϊ   */
        .cofoa      = timerOutputActionNone  ,                          /* ���ö�ʱ������ʱ�������Ϊ   */
        .cmoa       = timerOutputActionNone ,                           /* ����ƥ��ʱ�������Ϊ         */
        .mode       = timerCCModePWM   ,                                /* ����PWM���ģʽ              */
        .filter     = false          ,                                  /* ���������˲�                 */ 
        .prsInput   = false,                                            /* ѡ�����������               */
        /*
        *  ��λ���ó�ʼ�����ƽ��ֻ�������ƥ���PWMģʽ����PWMģʽ���۸�λ��Ϊ��ֵ������ʱ�����Ϊ0��
        *  ����ʹ��ʱ��λ���������ʼ��ƽ�������ƥ��ģʽ����λ����λ�����ʱ����ߵ�ƽ������λ����
        *  ���෴
        */
        .coist      = false   ,                                       
        .outInvert  = false   ,                                         /* �������ת */

    };

    /* Configure CC channel 0 */
    TIMER_InitCC(TIMER0, 1, &timerCCInit);

    /* Route CC2 to location 0 (PA0) and enable pin */  
    TIMER0->ROUTE |= (TIMER_ROUTE_CC1PEN | TIMER_ROUTE_LOCATION_LOC0); 


    /* Set compare value starting at 0 - it will be incremented in the interrupt handler */
    TIMER_CompareBufSet(TIMER0, 0, 0);

    /* Select timer parameters */  
    TIMER_Init_TypeDef timerInit =
    {
        .enable     = false,                                            /* ��ʼ��������ʹ��             */
        .debugRun   = false,                                            /* ����Haltʱֹͣ����           */
        .prescale   = timerPrescale1,                                   /* ��Ƶϵ��Ϊ1                  */
        .clkSel     = timerClkSelHFPerClk,                              /* ʱ��Դѡ��HFPERCLK           */
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,                                      /* ���ϼ���ģʽ                 */ 
        .dmaClrAct  = false,                                           
        .quadModeX4 = false,
        .oneShot    = false,                                               
        .sync       = false,  
    };


    /* Configure timer */
    TIMER_Init(TIMER0, &timerInit);

    /*  ����PWMģʽ��PWM���ڵ��� TOP+1������PWM����Ϊ 0.001s*/
    TIMER_TopSet(TIMER0, 168 -1); 

    /*  ����PWM����ߵ�ƽ�ĳ���ʱ��Ϊ CCV��ֵ������PWMռ�ձ�Ϊ50% */
    TIMER_CompareBufSet(TIMER0, 0, 84);
    TIMER_CompareBufSet(TIMER0, 1, 84);
    TIMER_Enable(TIMER1,0);	
}

 



/**************************************************************************//**
* @brief  Main function
* Main is called from __iar_program_start, see assembly startup file
*****************************************************************************/
void TIMER1_Conging(void)
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
        .prescale   = timerPrescale256, 
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

    NVIC_SetPriority(TIMER1_IRQn, 1);
    /* Enable TIMER0 interrupt vector in NVIC */
    NVIC_EnableIRQ(TIMER1_IRQn);

    /* Set TIMER Top value */
    TIMER_TopSet(TIMER1, TOP1);

    /* Configure TIMER */
    TIMER_Init(TIMER1, &timerInit1);
    TIMER_Enable(TIMER1,1);
}

