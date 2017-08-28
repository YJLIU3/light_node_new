#include "em_cmu.h"
#include "sysdelay.h"
#include "em_gpio.h"

//uint8_t Ap[16];
void Systick_Delay_Init(void);
void Systick_Delay_Cycle(unsigned long ulCycle);
void Systick_Delay_uS(unsigned long uluS);
void Systick_Delay_mS(unsigned long ulmS);
void Systick_Delay_1S(unsigned long ul1S);

__attribute__((zero_init))  unsigned long usDelay;
__attribute__((zero_init))  unsigned long msDelay;


void Systick_Delay_Init(void)
{
	SystemCoreClockUpdate();
  
	usDelay = SystemCoreClock / 1000000;
	msDelay = SystemCoreClock / 1000;
	
//  //获取内核时钟频率// 
//	Ap[0]=SystemCoreClock / 1000000;


	SysTick->CTRL |= SysTick_CTRL_CLKSOURCE_Msk;
	SysTick->CTRL &= ~(SysTick_CTRL_COUNTFLAG_Msk | SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk);
}



void Systick_Delay_Cycle(unsigned long ulCycle)
{
	SysTick->LOAD = ulCycle;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}



void Systick_Delay_uS(unsigned long uluS)     //使用内部HFRCOBand=21MHz ,uluS最大798000 ,最大延时798ms
{                                             //使用外部部晶振=24MHz    ,uluS最大699    ,最大延时699ms
	uluS = usDelay * uluS;

	SysTick->LOAD = uluS;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}



void Systick_Delay_mS(unsigned long ulmS)    //使用内部HFRCOBand=21MHz ,ulmS最大798 ,最大延时798ms
{                                            //使用外部部晶振=24MHz    ,ulmS最大699 ,最大延时699ms
	ulmS = msDelay * ulmS;

	SysTick->LOAD = ulmS;
	SysTick->VAL = 0;
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;

	while(!(SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
}


void Systick_Delay_1S(unsigned long ul1S)    //1s延时  
{

	uint8_t i;
	for(i=0;i<ul1S;i++)
	{
		Systick_Delay_mS(500);
		Systick_Delay_mS(500);
	}
}







void delay_10ms(uint32_t time)  //10ms延时
{      
   uint32_t i=0;   
   while(time--) 
   {  
      i=48000;  
      while(i--) ;     
   } 

}  
