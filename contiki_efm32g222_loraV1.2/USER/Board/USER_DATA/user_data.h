#ifndef __USER_DATA_H
#define __USER_DATA_H
#include "stdint.h"

extern uint8_t  SAVE_DATA[16] ;
extern uint8_t Node_Add[8]; 
uint32_t ReadUserData(uint32_t startAddress);
uint8_t WriteUserData(uint32_t startAddress,uint8_t *Data,uint16_t Length);
void Eeprom_Iint(void);
void USE_DATA_SAVE( uint32_t *data , uint8_t save_length)   ;

#endif

