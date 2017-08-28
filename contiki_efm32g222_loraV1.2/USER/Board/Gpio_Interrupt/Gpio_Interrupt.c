#include "EFM32_include.h"


void GPIO_Handler_Able(GPIO_Port_TypeDef port, unsigned int pin,  bool enable)
{
    uint32_t tmp;

    EFM_ASSERT(GPIO_PORT_VALID(port) && GPIO_PIN_VALID(pin));

    /* There are two registers controlling the interrupt configuration:
    * The EXTIPSELL register controls pins 0-7 and EXTIPSELH controls
    * pins 8-15. */
    if (pin < 8)
    {
        GPIO->EXTIPSELL = (GPIO->EXTIPSELL & ~(0xF << (4 * pin))) | (port << (4 * pin));
    }
    else
    {
        tmp = pin - 8;
        GPIO->EXTIPSELH = (GPIO->EXTIPSELH & ~(0xF << (4 * tmp))) | (port << (4 * tmp));
    }

    /* Clear any pending interrupt */
    GPIO->IFC = 1 << pin;

    /* Finally enable/disable interrupt */
    BITBAND_Peripheral(&(GPIO->IEN), pin, (unsigned int)enable);
}


void GPIO_Clear_IF(GPIO_Port_TypeDef port, unsigned int pin)
{
    uint32_t tmp;

    EFM_ASSERT(GPIO_PORT_VALID(port) && GPIO_PIN_VALID(pin));

    /* There are two registers controlling the interrupt configuration:
    * The EXTIPSELL register controls pins 0-7 and EXTIPSELH controls
    * pins 8-15. */
    if (pin < 8)
    {
        GPIO->EXTIPSELL = (GPIO->EXTIPSELL & ~(0xF << (4 * pin))) | (port << (4 * pin));
    }
    else
    {
        tmp = pin - 8;
        GPIO->EXTIPSELH = (GPIO->EXTIPSELH & ~(0xF << (4 * tmp))) | (port << (4 * tmp));
    }

    /* Clear any pending interrupt */
    GPIO->IFC = 1 << pin;
    
}

///************************************************/
////函数名：void GPIO_ODD_IRQHandler(void) 
////功能：  奇数中断服务函数
////参数：  无
////返回值：无
///************************************************/
//void  GPIO_EVEN_IRQHandler (void)  
//{     
//    if(GPIO->IF & 0x0400)  
//    {  

//	
//    }
//     GPIO->IFC |= (1<<10);   
//}  













///************************************************/
////函数名：void GPIO_ODD_IRQHandler(void) 
////功能：  偶数中断服务函数
////参数：  无
////返回值：无
///************************************************/
//void GPIO_ODD_IRQHandler(void)  
//{   
//    /*HMC5983中断*/
//    if(GPIO->IF & 0x0800)  
//    {  
////        FlageGroup |=FLAGE_HMCINT;
//        GPIO->IFC |= (1<<11);
//    }
//}  



