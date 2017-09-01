#ifndef __LEUSART_H
#define __LEUSART_H

#include "stdint.h"
#include "stdio.h"
#ifdef __cplusplus
extern "C"
{
#endif
	
	void Leusart_Init(void);
	void Leusart_Txdata(uint8_t *data, uint8_t length);
	void LEUART0_IRQHandler(void);
//	int fputc(int ch, FILE *f);
	int fgetc(FILE *f);

	#ifdef __cplusplus
}
#endif

#endif
