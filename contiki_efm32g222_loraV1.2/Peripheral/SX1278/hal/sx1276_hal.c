/*
 / _____)             _              | |
( (____  _____ ____ _| |_ _____  ____| |__
 \____ \| ___ |    (_   _) ___ |/ ___)  _ \
 _____) ) ____| | | || |_| ____( (___| | | |
(______/|_____)_|_|_| \__)_____)\____)_| |_|
    (C)2013 Semtech

Description: SX1276 driver specific target board functions implementation

License: Revised BSD License, see LICENSE.TXT file include in the project

Maintainer: Miguel Luis and Gregory Cristian
*/
#include "mac_include.h"
#include "EFM32_include.h"
#include "contiki.h"

 




//Delay  1Ms
void DelayMs( uint32_t ms )
{
   uint32_t i=0;   
   while(ms--) 
   {  
      i=2600;  
      while(i--) ;     
   } 
}





void TimerInit( TimerEvent_t *obj, funCallBack fc)
{
    obj->contiki_time_t = 0;
    obj->Contiki_Callback.f = fc;   
} 
 
void TimerSetValue( TimerEvent_t *obj, uint32_t value )
{
    obj->contiki_time_t = value;
}

void TimerStart( TimerEvent_t *obj )
{   
     ctimer_set(&obj->Contiki_Callback,obj->contiki_time_t, obj->Contiki_Callback.f, NULL);
     ctimer_restart(&obj->Contiki_Callback);
}



void TimerStop( TimerEvent_t *obj )
{
    ctimer_stop(&obj->Contiki_Callback)  ;
}


TimerTime_t TimerGetCurrentTime( void )
{
    return RtcGetTimerValue( );
}


TimerTime_t TimerGetElapsedTime( TimerTime_t savedTime )
{
   
    TimerTime_t elapsedTime = 0;

    // Needed at boot, cannot compute with 0 or elapsed time will be equal to current time
    if( savedTime == 0 )
    {
        return 0;
    }

    elapsedTime = clock_time();

    if( elapsedTime < savedTime )
    { // roll over of the counter
        return( elapsedTime + ( 0xFFFFFFFF - savedTime ) );
    }
    else
    {
        return( elapsedTime - savedTime );
    }  
    
}



TimerTime_t RtcGetTimerValue( void )
{
    return(clock_time());
}




/*!
 * Flag used to set the RF switch control pins in low power mode when the radio is not active.
 */
static bool RadioIsActive = false;

/*!
 * Radio driver structure initialization
 */
const struct Radio_s Radio =
{
    SX1276Init,
    SX1276GetStatus,
    SX1276SetModem,
    SX1276SetChannel,
    SX1276IsChannelFree,
    SX1276Random,
    SX1276SetRxConfig,
    SX1276SetTxConfig,
    SX1276CheckRfFrequency,
    SX1276GetTimeOnAir,
    SX1276Send,
    SX1276SetSleep,
    SX1276SetStby, 
    SX1276SetRx,
    SX1276StartCad,
    SX1276ReadRssi,
    SX1276Write,
    SX1276Read,
    SX1276WriteBuffer,
    SX1276ReadBuffer,
    SX1276SetMaxPayloadLength
};

/*!
 * Antenna switch GPIO pins objects
 */
//Gpio_t AntSwitchLf;


void SX1276IoInit( void )
{   
    //初始化SX1278:DI00,DI01,DI02,DI03,DI04,DI05,RESET//
	GPIO_PinModeSet(gpioPortA, 2,     gpioModeInput   ,     0); /* SX_DIO0   */
	GPIO_PinModeSet(gpioPortC, 0,     gpioModeInput   ,     0); /* SX_DIO1   */
	GPIO_PinModeSet(gpioPortC, 1,     gpioModeInput   ,     0); /* SX_DIO2   */
	GPIO_PinModeSet(gpioPortF, 3,     gpioModeInput   ,     0); /* SX_DIO3   */
//	GPIO_PinModeSet(gpioPortF, 2,     gpioModeInput   ,     0); /* SX_DIO4   */
//	GPIO_PinModeSet(gpioPortF, 4,     gpioModeInput   ,     0); /* SX_DIO5   */
	
	GPIO_PinModeSet(gpioPortF, 5,     gpioModePushPull,     0); /* SX_RESET  */
}

void SX1276IoIrqInit( void)
{
    /*   SX1278中断初始化   */
    GPIO_IntConfig(gpioPortA, 2,  true, false, true);  /* SX_DIO0 下降沿触发*/
    GPIO_IntConfig(gpioPortC, 0,  true, false, true);  /* SX_DIO1 下降沿触发*/
    GPIO_IntConfig(gpioPortC, 1,  true, false, true);  /* SX_DIO2 下降沿触发*/
    GPIO_IntConfig(gpioPortF, 3,  true, false, true);  /* SX_DIO3 下降沿触发*/
//    GPIO_IntConfig(gpioPortF, 2,  true, false, true);  /* SX_DIO4 下降沿触发*/
//    GPIO_IntConfig(gpioPortF, 4,  true, false, true);  /* SX_DIO5 下降沿触发*/
        
    NVIC_DisableIRQ(GPIO_ODD_IRQn);   //打开奇中断
    NVIC_DisableIRQ(GPIO_EVEN_IRQn);  //打开偶中断 
}



void SX1276IoIrqEnable( void)
{  


    GPIO->IFC |= (1<<2);
    GPIO->IFC |= (1<<0);
    GPIO->IFC |= (1<<1);
    GPIO->IFC |= (1<<3);
    GPIO->IFC |= (1<<4);

    NVIC_SetPriority(GPIO_ODD_IRQn,3);  
    NVIC_SetPriority(GPIO_EVEN_IRQn,4); 
    NVIC_EnableIRQ(GPIO_ODD_IRQn);   //打开奇中断
    NVIC_EnableIRQ(GPIO_EVEN_IRQn);  //打开偶中断    
}




uint8_t SX1276GetPaSelect( uint32_t channel )
{
    if( channel < RF_MID_BAND_THRESH )
    {
        return RF_PACONFIG_PASELECT_PABOOST;
    }
    else
    {
        return RF_PACONFIG_PASELECT_RFO;
    }
}

void SX1276SetAntSwLowPower( bool status )
{
    if( RadioIsActive != status )
    {
        RadioIsActive = status;
    
        if( status == false )
        {
            SX1276AntSwInit( );
        }
        else
        {
            SX1276AntSwDeInit( );
        }
    }
}

void SX1276AntSwInit( void )
{
//    GpioInit( &AntSwitchLf, RADIO_ANT_SWITCH_LF, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 1 );
//    GpioInit( &AntSwitchHf, RADIO_ANT_SWITCH_HF, PIN_OUTPUT, PIN_PUSH_PULL, PIN_PULL_UP, 0 );
}

void SX1276AntSwDeInit( void )
{
//    GpioInit( &AntSwitchLf, RADIO_ANT_SWITCH_LF, PIN_OUTPUT, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
//    GpioInit( &AntSwitchHf, RADIO_ANT_SWITCH_HF, PIN_OUTPUT, PIN_OPEN_DRAIN, PIN_NO_PULL, 0 );
}

void SX1276SetAntSw( uint8_t rxTx )
{
    if( rxTx != 0 ) // 1: TX, 0: RX
    {
//        GpioWrite( &AntSwitchLf, 0 );
//        GpioWrite( &AntSwitchHf, 1 );
    }
    else
    {
//        GpioWrite( &AntSwitchLf, 1 );
//        GpioWrite( &AntSwitchHf, 0 );
    }
}

bool SX1276CheckRfFrequency( uint32_t frequency )
{
    // Implement check. Currently all frequencies are supported
    return true;
}
 

      

void SX1276Reset( void )
{
    // Set RESET pin to 0
    SX_REST_LOW;  /* SX1278_RESET  */
   
    // Wait 1 ms
    DelayMs( 1 );

    // Configure RESET as input
    SX_REST_HIGH ; /* SX1278_RESET  */

    // Wait 6 ms
    DelayMs( 6 );
}

void SX1276WriteBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    SPI0_NSS_LOW; 

    SpiInOut( addr | 0x80 );
    for( i = 0; i < size; i++ )
    {
       SpiInOut( buffer[i] );
    }

    //NSS = 1;
    SPI0_NSS_HIGH; 
}

void SX1276ReadBuffer( uint8_t addr, uint8_t *buffer, uint8_t size )
{
    uint8_t i;

    //NSS = 0;
    SPI0_NSS_LOW; 

    SpiInOut( addr & 0x7F );

    for( i = 0; i < size; i++ )
    {
         buffer[i] = SpiInOut( 0 );
    }

    //NSS = 1;
    SPI0_NSS_HIGH; 
}





/************************************************/
//函数名：void GPIO_ODD_IRQHandler(void) 
//功能：  偶中断服务函数
//参数：  无
//返回值：无
/************************************************/
void  GPIO_EVEN_IRQHandler (void)  
{   
    /*SXDIO_0中断*/    
    if(GPIO->IF & (1<<2))  
    { 
        if(GPIO_PinInGet(gpioPortA, 2)==1)
        {          
            #if defined (usart_enter ) 
            printf("interrupt  0  \n");
            #endif
            SX1276OnDio0Irq();
        
        }
        
//         /*SXDIO_4中断*/   
//        if(GPIO_PinInGet(gpioPortF, 2)==1)
//        {
//            SX1276OnDio4Irq();
//            #if defined usart_enter  
//            printf("interrupt  4  \n");
//            #endif
//       
//        }
        
        
        GPIO->IFC |= (1<<2);           
    }
    
    
    /*SXDIO_1中断*/    
    if(GPIO->IF & (1<<0))  
    { 
        if(GPIO_PinInGet(gpioPortC, 0)==1)
        {           
            #if defined usart_enter  
            printf("interrupt  1  \n");
            #endif  
            SX1276OnDio1Irq();            
        }                      
        GPIO->IFC |= (1<<0);   
    } 
    
    
    
        /*SXDIO_5中断*/    
    if(GPIO->IF & (1<<4))  
    { 
        if(GPIO_PinInGet(gpioPortF, 4)==1)
        {
            SX1276OnDio5Irq();
            #if defined usart_enter  
            printf("interrupt  5  \n");
            #endif          
        }                      
        GPIO->IFC |= (1<<4);   
    } 
    
    
}  




/************************************************/
//函数名：void GPIO_ODD_IRQHandler(void) 
//功能：  奇中断服务函数
//参数：  无
//返回值：无
/************************************************/
void GPIO_ODD_IRQHandler(void)  
{   
    
    
    
    /*SXDIO_2中断*/    
    if(GPIO->IF & (1<<1))  
    { 
        if(GPIO_PinInGet(gpioPortC, 1)==1)
        {           
            #if defined usart_enter  
            printf("interrupt  2  \n");
            #endif 
            SX1276OnDio2Irq();             
        }                      
        GPIO->IFC |= (1<<1);   
    } 
    


    
    /*SXDIO_3中断*/    
    if(GPIO->IF & (1<<3))  
    { 
        if(GPIO_PinInGet(gpioPortF, 3)==1)
        {            
            #if defined usart_enter  
            printf("interrupt  3  \n");
            #endif  
            SX1276OnDio3Irq();             
        }                      
        GPIO->IFC |= (1<<3);   
    } 
    
    

}  




void BoardGetUniqueId( uint8_t *id )
{
    id[7] = 0X12;
    id[6] = 0X34;
    id[5] = 0X56;
    id[4] = 0X78;
    id[3] = 0X90;
    id[2] = 0Xab;
    id[1] = 0Xcd;
    id[0] = 0Xef;
}
