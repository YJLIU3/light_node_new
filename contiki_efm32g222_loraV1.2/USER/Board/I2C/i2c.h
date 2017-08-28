#ifndef __I2C_H
#define __I2C_H

void I2C_Initial(void);
void I2C_WaitForAck(void);
void I2C_Buslock_Process(void);
unsigned char I2C_WriteByte(unsigned char CFG_add,unsigned char CFG_data) ; 
unsigned char HMC5983_ReadNByte(unsigned char CFG_add, unsigned char n,unsigned char * buff) ;



#endif






















