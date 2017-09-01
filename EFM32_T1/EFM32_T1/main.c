#include "contiki_sys/contiki.h"
#include "../EFM32_include.h"
#include "lora_application/app_task.h"

		

	unsigned int idle_count = 0;

	uint8_t Sleep_Flag;


	int main(void)
	{
		CHIP_Init();  
		Rcc_Init();     
		Leusart_Init();
		led_Init(); 
		LED1_ON();
		Systick_Delay_Init();

	
		printf("board reboot\n");
		clock_init();
		rtimer_init();
		ctimer_init();
		process_init();
		process_start(&etimer_process, NULL);
		autostart_start(autostart_processes);
		    /*********************/
		while (1)
		{
			do 
			{		
			} while (process_run() > 0);
			idle_count++;
			EMU_EnterEM2(1);       
		}

	}
	
