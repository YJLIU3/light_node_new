/**************************************************************************//**
 * @file
 * @brief SPI
 * @author Energy Micro AS
 * @version 1.12
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
#include "em_device.h"
#include "spi0.h"
#include "em_gpio.h"


#include "em_chip.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "em_usart.h"






/**************************************************************************//**
 * @brief Setup a USART as SPI
 * @param spiNumber is the number of the USART to use (e.g. 1 USART1)
 * @param location is the GPIO location to use for the device
 * @param master set if the SPI is to be master
 *****************************************************************************/
 
 
void SPI0_Init(void)
{

	USART_TypeDef *spi;
	spi = USART0;

	/* Enabling clock to USART 1*/
	CMU_ClockEnable(cmuClock_USART0, true);
	CMU_ClockEnable(cmuClock_GPIO, true);


	/* Setting baudrate  10MHZ */
	spi->CLKDIV = 128 * (SPI_PERCLK_FREQUENCY / SPI_BAUDRATE - 2);

	/* Using synchronous (SPI) mode*/
	spi->CTRL = USART_CTRL_SYNC|USART_CTRL_OVS_X8  ;

	/* Clearing old transfers/receptions, and disabling interrupts */
	spi->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX;
	spi->IEN = 0;



	/* Enabling pins and setting location */
	spi->ROUTE |= USART_ROUTE_TXPEN | USART_ROUTE_RXPEN | USART_ROUTE_CLKPEN |  USART_ROUTE_LOCATION_LOC0 ;

	/*Set to master and to control the CS line , Enabling Master, TX and RX */
	spi->CMD   = USART_CMD_MASTEREN | USART_CMD_TXEN | USART_CMD_RXEN;



	//spi->FRAME|=USART_FRAME_STOPBITS_HALF|USART_FRAME_DATABITS_EIGHT ;
	spi->CTRL |= USART_CTRL_CLKPOL_IDLELOW|USART_CTRL_CLKPHA_SAMPLELEADING |USART_CTRL_MSBF  ;
	spi->CTRL |= USART_CTRL_AUTOCS;


	/* Clear previous interrupts */
	spi->IFC = _USART_IFC_MASK;

	/* IO configuration (USART 1, Location #3) */
	GPIO_PinModeSet(gpioPortE, 10,  gpioModePushPull,     0); /* MOSI */
	GPIO_PinModeSet(gpioPortE, 11,  gpioModeInput ,       0); /* MISO */
	GPIO_PinModeSet(gpioPortE, 13,  gpioModePushPull,     1); /* CS */
	GPIO_PinModeSet(gpioPortE, 12,  gpioModePushPull,     0); /* Clock */

    SPI0_NSS_HIGH; 
	
			
}






uint8_t SpiInOut( uint8_t data )
{

    USART_TypeDef *usart = USART0;
    while (!(usart->STATUS & USART_STATUS_TXBL))
    ;
    usart->TXDATA = (uint32_t) data;
    while (!(usart->STATUS & USART_STATUS_TXC))
    ;
    return (uint8_t) (usart->RXDATA);
}





