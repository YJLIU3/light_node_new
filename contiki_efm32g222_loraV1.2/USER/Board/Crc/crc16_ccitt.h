#ifndef __CRC16_CCITT_H
#define __CRC16_CCITT_H
#include <stdint.h>


extern uint8_t CRC16_CCITT[2];

unsigned short GetRevCrc_16(unsigned char * pData, int nLength, unsigned short init, const unsigned short *ptable);
unsigned short GetCrc_16(unsigned char * pData, int nLength, unsigned short init, const unsigned short *ptable);
unsigned short CRC_GetModbus16(unsigned char *pdata, int len);
uint8_t* Crc16_Ccitt(uint8_t *pdata, uint8_t len);
unsigned char CRC8_Tab(unsigned char * ucPtr, unsigned char ucLen);

#endif




