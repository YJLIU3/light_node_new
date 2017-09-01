#include "contiki.h"
#include "app_task.h"
#include "../EFM32_include.h"




/*创建进程*/
PROCESS(app_led, "app_led");
PROCESS_THREAD(app_led, ev, data)
{
    PROCESS_BEGIN();
    while(1)
    {
        static struct etimer et;

        etimer_set(&et, 3*64 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        LED1_ON();
	    LED2_OFF();
	    LED3_ON();
 
        
        etimer_set(&et, 3*64 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        LED1_OFF();  
	    LED2_ON();  
	    LED3_OFF();  
    }
    PROCESS_END();
}






/*创建上电初始化任务*/













/*创建喂狗进程*/
//PROCESS(WDOG_Feed_Task, "WDOG_Feed_Task");
//PROCESS_THREAD(WDOG_Feed_Task, ev, data)
//{
//    PROCESS_BEGIN();
//    while(1)
//    {
//        static struct etimer et;
//        etimer_set(&et, WDOG_FEED_TIME );
//        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
//        WDOG_Feed();   
//    }
//    PROCESS_END();
//}
















//*自启动任务*/
AUTOSTART_PROCESSES(&app_led  );

//AUTOSTART_PROCESSES( &app_led);



















