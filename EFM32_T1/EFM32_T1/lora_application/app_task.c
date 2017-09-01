#include "contiki.h"
#include "app_task.h"
#include "../EFM32_include.h"




/*��������*/
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






/*�����ϵ��ʼ������*/













/*����ι������*/
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
















//*����������*/
AUTOSTART_PROCESSES(&app_led  );

//AUTOSTART_PROCESSES( &app_led);



















