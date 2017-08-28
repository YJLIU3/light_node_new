#include "EFM32_include.h"
#include "contiki.h"
#include "mac_include.h"
#include "app_task.h"
#include "GY30.h"
#include "LoRaMac-api.h"

unsigned int idle_count = 0;

uint8_t Sleep_Flag;


int main(void)
{
    //EFM32内部初始化
    CHIP_Init();  
    //时钟初始化
    Rcc_Init(); 
    //看门狗初始化
    WDT_INIT();        
    //低功耗串口初始化
    Leusart_Init();

    /*LED初始化*/
    led_Init(); 
    LED1_ON();
    /*系统延时初始化*/
    Systick_Delay_Init();

    /*1278初始化*/
    SPI0_Init();
    SX1276IoInit();
    SX1276IoIrqInit( );
    GY_30_Init();  
    
    printf("board reboot\n");   
    /*contiki初始化*/
    clock_init();
    rtimer_init();
    ctimer_init();
    process_init();
    process_start(&etimer_process, NULL);
    autostart_start(autostart_processes);
    /*********************/
    while(1)
    {
        do 
        {
        } 
        while(process_run() > 0);
        idle_count++;
        /*无任务进入休眠*/
 
        EMU_EnterEM2(1);       
    }

}













/******************* (C) COPYRIGHT 2016/11/21 SIAT HPC *****END OF FILE************/
