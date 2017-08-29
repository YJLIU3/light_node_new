#include "EFM32_include.h"
#include "contiki.h"
#include "mac_include.h"
#include "app_task.h"
#include "GY30.h"
#include "LoRaMac-api.h"

#include "DHT11.h"

unsigned int idle_count = 0;

uint8_t Sleep_Flag;


int main(void)
{
    //EFM32�ڲ���ʼ��
    CHIP_Init();  
    //ʱ�ӳ�ʼ��
    Rcc_Init(); 
    //���Ź���ʼ��
//    WDT_INIT();        
    //�͹��Ĵ��ڳ�ʼ��
    Leusart_Init();

    /*LED��ʼ��*/
    led_Init(); 
    LED1_ON();
    /*ϵͳ��ʱ��ʼ��*/
    Systick_Delay_Init();
	
    /*1278��ʼ��*/
    SPI0_Init();
    SX1276IoInit();
    SX1276IoIrqInit( ); 
    
    printf("board reboot\n");   
    /*contiki��ʼ��*/
    clock_init();
    rtimer_init();
    ctimer_init();
    process_init();
    process_start(&etimer_process, NULL);
    autostart_start(autostart_processes);
    /*********************/
    while(1)
    {
			DHT11_Read_Data();
        do 
        {
					
        } 
        while(process_run() > 0);
        idle_count++;
        /*�������������*/
 
        EMU_EnterEM2(1);       
    }

}













/******************* (C) COPYRIGHT 2016/11/21 SIAT HPC *****END OF FILE************/
