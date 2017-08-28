#ifndef __GY30_H
#define __GY30_H
#include "EFM32_include.h"
#include "gyiic.h"

void Single_Write_GY30(uint8_t REG_Address);
void GY_30_Init(void);
uint8_t GY_30_ReadOneByte(uint16_t ReadAddr);
void GY_30_WriteOneByte(uint16_t WriteAddr,uint8_t DataToWrite);
void GY_30_WriteLenByte(uint16_t WriteAddr,uint32_t DataToWrite,uint8_t Len);
uint32_t GY_30_ReadLenByte(uint16_t ReadAddr,uint8_t Len);
void GY_30_Write(uint16_t WriteAddr,uint8_t *pBuffer,uint16_t NumToWrite);
uint8_t* GY_30_Read(void);


uint8_t check_GY30(uint8_t Address);

#endif