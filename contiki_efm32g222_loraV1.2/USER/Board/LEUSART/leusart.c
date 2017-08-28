
/**************************************************************************//**
 * @file
 * @brief LEUART Demo Application
 * @author Energy Micro AS
 * @version 1.07
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2013 Energy Micro AS, http://www.energymicro.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 * 4. The source and compiled code may only be used on Energy Micro "EFM32"
 *    microcontrollers and "EFR4" radios.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#include "em_cmu.h"
#include "em_leuart.h"
#include "em_gpio.h"
#include "leusart.h"




/* Defining the LEUART1 initialization data */
 LEUART_Init_TypeDef leuart1Init =
{
  .enable   = leuartEnable ,       /* Activate data reception on LEUn_TX pin. */
  .refFreq  = 0,                    /* Inherit the clock frequenzy from the LEUART clock source */
  .baudrate = 115200,                 /* Baudrate = 9600 bps */
  .databits = leuartDatabits8,      /* Each LEUART frame containes 8 databits */
  .parity   = leuartNoParity,       /* No parity bits in use */
  .stopbits = leuartStopbits1,      /* Setting the number of stop bits in a frame to 1 bitperiods */
};



/**************************************************************************//**
 * @brief  Initialize Low Energy UART 1
 *
 * Here the LEUART is initialized with the chosen settings. It is then routed
 * to location 0 to avoid conflict with the LCD pinout. Finally the GPIO mode
 * is set to push pull.
 *
 *****************************************************************************/
void InitLeuart(void)
{
    /* Reseting and initializing LEUART1 */
    LEUART_Reset(LEUART0);
    LEUART_Init(LEUART0, &leuart1Init);


    /* Route LEUART1 TX pin to DMA location 4 */
    LEUART0->ROUTE = LEUART_ROUTE_TXPEN |
                     LEUART_ROUTE_RXPEN |
                     LEUART_ROUTE_LOCATION_LOC0;

    /* Enable GPIO for LEUART1. TX is on F2 */
    GPIO_PinModeSet(gpioPortD,                /* GPIO port */
                    4,                        /* GPIO port number */
                    gpioModePushPull,         /* Pin mode is set to push pull */
                    1);                       /* High idle state */
    //  /* Enable GPIO for LEUART1. RX is on A0 */
    //  GPIO_PinModeSet(gpioPortD,                /* GPIO port */
    //                  5,                        /* GPIO port number */
    //                  gpioModeInputPull,        /* Pin mode is set to push pull */
    //                  1);                       /* High idle state */
}


/**************************************************************************//**
 * @brief  Main function
 *
 * This example demonstrates a way to use the Low Energy UART to maintain full
 * UART communication capabilities, while spending a great majority of the time
 * in deep sleep mode EM2. The LEUART is in this example driven by the LFXO,
 * which provide good accuracy while consuming only small amounts of energy. In
 * addition the DMA is set up to read the data to be transmitted by the LEUART
 * directly from the system memory. This relieves the CPU from doing anything
 * other than initializing the transfer, and handle possible interrupts triggered
 * when the transfer is finished. In this case the strings "HELLO" and "THERE"
 * are alternatingly transmitted every other second through the LEUART.
 *
 *****************************************************************************/
void Leusart_Init(void)
{

    //  /* Start LFXO, and use LFXO for low-energy modules */
    //  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFXO);
    //  CMU_ClockSelectSet(cmuClock_LFB, cmuSelect_LFXO);

    /* Enabling clocks, all other remain disabled */
    CMU_ClockEnable(cmuClock_LEUART0, true);    /* Enable LEUART1 clock */

    /* Initialize LEUART */
    InitLeuart();
    LEUART_IntEnable(LEUART0, LEUART_IEN_RXDATAV);
    NVIC_SetPriority(LEUART0_IRQn , 5);
    NVIC_EnableIRQ(LEUART0_IRQn);
}



void Leusart_Txdata( uint8_t *data,uint8_t length)
{
    uint8_t i;
    for(i=0;i<length;i++)	
    { 
        LEUART_Tx(LEUART0,*data);
        data++;
    }
}




void LEUART0_IRQHandler(void)
{
    if(LEUART0->STATUS & LEUART_STATUS_RXDATAV )
    {	 	  

    }
}









/// 重定向c库函数printf到USART1
int fputc(int ch, FILE *f)
{
    /* 发送一个字节数据到USART1 */
    LEUART_Tx(LEUART0, (uint8_t) ch);

    /* 等待发送完毕 */
    while (!(LEUART0->STATUS & LEUART_STATUS_TXBL))
    ;		

    return (ch);
}

/// 重定向c库函数scanf到USART1
int fgetc(FILE *f)
{
    /* 等待串口1输入数据 */
    while (!(LEUART0->STATUS & LEUART_STATUS_TXBL))
    ;

    return (int)LEUART_Rx(LEUART0);
}



