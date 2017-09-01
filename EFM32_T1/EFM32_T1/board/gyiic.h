#ifndef __GYIIC_H
#define __GYIIC_H
#include "../EFM32_include.h"

#ifdef __cplusplus
extern "C"
{
#endif
	
	void SDA_OUT(void);

	void SDA_IN(void);

	void SDA_0(void);
	void SDA_1(void);

	void SCL_0(void);
	void SCL_1(void);
	void IIC_Init(void);                //��ʼ��IIC��IO��				 
	void IIC_Start(void);				//����IIC��ʼ�ź�
	void IIC_Stop(void);	  			//����IICֹͣ�ź�
	void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
	uint8_t IIC_Read_Byte(unsigned char ack);
	uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
	void IIC_Ack(void);					//IIC����ACK�ź�
	void IIC_NAck(void);				//IIC������ACK�ź�

	void IIC_Write_One_Byte(uint8_t daddr, uint8_t addr, uint8_t data);
	uint8_t IIC_Read_One_Byte(uint8_t daddr, uint8_t addr);	
	
		#ifdef __cplusplus
}
#endif
	
#endif
