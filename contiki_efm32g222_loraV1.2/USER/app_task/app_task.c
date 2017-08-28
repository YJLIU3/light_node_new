#include "contiki.h"

#include "app_task.h"
#include "EFM32_include.h"


#include "radio.h"
#include "sx1276.h"
#include "sx1276-hal.h"
#include "LoRaMac.h"

extern uint8_t app[10];




/*创建进程*/
PROCESS(app_led, "app_led");
PROCESS_THREAD(app_led, ev, data)
{
    PROCESS_BEGIN();
    while(1)
    {
        static struct etimer et;

        etimer_set(&et, 64 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        LED1_ON();
 
        
        etimer_set(&et, 64 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        LED1_OFF();      
    }
    PROCESS_END();
}





/*创建进程*/
PROCESS(send_test, "send_test");
PROCESS_THREAD(send_test, ev, data)
{
    PROCESS_BEGIN();
    while(1)
    {
        static struct etimer et;
    
        
        /*第一次发送50个字节，超时时间设置4s*/
        etimer_set(&et, 128*5 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        Radio.SetTxConfig( MODEM_LORA, 20, 0, 0, 12, 1, 8, false, true, 0, 0, false, 128*4 );//timeout=7.812ms*128*4=4s
        Radio.SetChannel( 470000000 ); 

        SX1276Send( app, 50 );
        LED2_ON();
        etimer_set(&et, 64 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        LED2_OFF();
        
        
        /*第二次发送50个字节，超时时间设置500ms*/
        etimer_set(&et, 128*5 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        Radio.SetTxConfig( MODEM_LORA, 20, 0, 0, 12, 1, 8, false, true, 0, 0, false, 64 );//timeout=500ms
        Radio.SetChannel( 470000000 ); 
        SX1276Send( app, 50 );
        LED2_ON();
        
        etimer_set(&et, 64 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        LED2_OFF();
        
        etimer_set(&et, 128*4 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        // Clear Irq
        SX1276Write( REG_LR_IRQFLAGS, RFLR_IRQFLAGS_TXDONE );
        
    }
    PROCESS_END();
}












/*创建喂狗进程*/
PROCESS(WDOG_Feed_Task, "WDOG_Feed_Task");
PROCESS_THREAD(WDOG_Feed_Task, ev, data)
{
    PROCESS_BEGIN();
    while(1)
    {
        static struct etimer et;
        etimer_set(&et, WDOG_FEED_TIME );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        WDOG_Feed();   
    }
    PROCESS_END();
}












//*自启动任务*/
AUTOSTART_PROCESSES(&WDOG_Feed_Task , &app_led ,&send_test);


















//    RxChainCalibration( );
//    
  //Radio.SetTxConfig( MODEM_LORA, txPower, 0, 1, datarate, 1, 8, false, true, 0, 0, false, 3e3 );

//   Radio.SetTxConfig( MODEM_LORA, 20, 0, 0, 12, 1, 8, false, true, 0, 0, false, 1 );
//   Radio.SetChannel( 470000000 ); 
   
   // Radio.SetRxConfig( MODEM_LORA, 0, downlinkDatarate, 1, 0, 8, timeout, false, 0, false, 0, 0, true, rxContinuous );
   
//    SX1276SetTxConfig( RadioModems_t modem, int8_t power, uint32_t fdev,
//                        uint32_t bandwidth, uint32_t datarate,
//                        uint8_t coderate, uint16_t preambleLen,
//                        bool fixLen, bool crcOn, bool freqHopOn,
//                        uint8_t hopPeriod, bool iqInverted, uint32_t timeout )
//   SX1276Send( uint8_t *buffer, uint8_t size )
                        
                    // Radio.SetRxConfig( modem, 50e3, downlinkDatarate * 1e3, 0, 83.333e3, 5, 0, false, 0, true, 0, 0, false, rxContinuous );
               
//  SX1276SetRxConfig( RadioModems_t modem, uint32_t bandwidth,
//                         uint32_t datarate, uint8_t coderate,
//                         uint32_t bandwidthAfc, uint16_t preambleLen,
//                         uint16_t symbTimeout, bool fixLen,
//                         uint8_t payloadLen,
//                         bool crcOn, bool freqHopOn, uint8_t hopPeriod,
//                         bool iqInverted, bool rxContinuous )                      
                        
                        
//    while(1)
//    {
//          
//          LED2_ON(); 
//          SX1276Send( app, 4 );
//          Systick_Delay_1S(1);         
//          LED2_OFF(); 
//          Systick_Delay_1S(1);  
//          app[3]++;

//    }






