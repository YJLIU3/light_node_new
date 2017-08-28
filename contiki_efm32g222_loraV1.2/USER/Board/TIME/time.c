#include "em_cmu.h"
#include "em_gpio.h"
#include "em_timer.h"
#include "time.h"
#include "em_leuart.h"


#include "sysdelay.h"


/**************************************************************************//**
// * @brief TIMER0_IRQHandler
// * Interrupt Service Routine TIMER0 Interrupt Line
// *****************************************************************************/
//void TIMER0_IRQHandler(void)
//{ 
//    /* Clear flag for TIMER0 overflow interrupt */
//    TIMER_IntClear(TIMER0, TIMER_IF_OF);


//    TIMER_Enable(TIMER0,0);	  /* 关闭计数*/                               
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
//    printf("定时器工作\n"); 
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

        /*  TIMER0比较通道0初始化结构体，配置输入捕获部分模块功能*/
        .eventCtrl  = timerEventEveryEdge ,                             /* 配置事件响应边沿             */
        .edge       = timerEdgeRising ,                                 /* 配置边沿检测器所检测的边沿   */
        .prsSel     = timerPRSSELCh1 ,                                  /* PRS通道选择                  */
        .cufoa      = timerOutputActionNone ,                           /* 配置定时器下溢时的输出行为   */
        .cofoa      = timerOutputActionNone  ,                          /* 配置定时器上溢时的输出行为   */
        .cmoa       = timerOutputActionNone ,                           /* 配置匹配时的输出行为         */
        .mode       = timerCCModePWM   ,                                /* 配置PWM输出模式              */
        .filter     = false          ,                                  /* 禁能输入滤波                 */ 
        .prsInput   = false,                                            /* 选择从引脚输入               */
        /*
        *  该位配置初始输出电平，只用于输出匹配和PWM模式。在PWM模式不论该位置为何值，禁能时输出都为0，
        *  但当使能时该位决定输出初始电平。在输出匹配模式，该位若置位则禁能时输出高电平，若该位清零
        *  则相反
        */
        .coist      = false   ,                                       
        .outInvert  = false   ,                                         /* 输出不翻转 */

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
        .enable     = false,                                            /* 初始化结束不使能             */
        .debugRun   = false,                                            /* 调试Halt时停止计数           */
        .prescale   = timerPrescale1,                                   /* 分频系数为1                  */
        .clkSel     = timerClkSelHFPerClk,                              /* 时钟源选择HFPERCLK           */
        .fallAction = timerInputActionNone,
        .riseAction = timerInputActionNone,
        .mode       = timerModeUp,                                      /* 向上计数模式                 */ 
        .dmaClrAct  = false,                                           
        .quadModeX4 = false,
        .oneShot    = false,                                               
        .sync       = false,  
    };


    /* Configure timer */
    TIMER_Init(TIMER0, &timerInit);

    /*  上升PWM模式的PWM周期等于 TOP+1，配置PWM周期为 0.001s*/
    TIMER_TopSet(TIMER0, 168 -1); 

    /*  配置PWM输出高电平的持续时间为 CCV的值，配置PWM占空比为50% */
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

