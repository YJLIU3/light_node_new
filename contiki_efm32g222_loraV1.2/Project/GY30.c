#include "GY30.h"
#define SlaveAddress	0x46
uint8_t    BUF[8]; 
float light;
int dis_data;

void GY_30_Init(void)
{
	IIC_Init();
//	IIC_Stop();
	Single_Write_GY30(0x01);
}

void Single_Write_GY30(uint8_t REG_Address)
{
	IIC_Start();
	IIC_Send_Byte(SlaveAddress);
	IIC_Send_Byte(REG_Address);
	IIC_Stop();
}

uint8_t* GY_30_Read(void)
{
	
	uint8_t i;
	IIC_Start();
	IIC_Send_Byte(SlaveAddress+1);
	
	for(i = 0;i < 2; i++ )
	{
		BUF[i] = IIC_Read_Byte(1);
		if(i == 2)
		{
			IIC_NAck();
		}
		else
		{
			IIC_Ack();
		}
	}
	IIC_Stop();
	Systick_Delay_mS(5);
	dis_data=BUF[0];
    dis_data=(dis_data<<8)+BUF[1];
	light = (float)dis_data/1.2;
	printf(" 光照强度是： %f lx\n",light);
	return BUF;
}

uint8_t check_GY30(uint8_t Address)
{
	uint8_t ucAck;
	IIC_Start();
	
	IIC_Send_Byte(Address);
	
	ucAck = IIC_Wait_Ack();
	
	IIC_Stop();
	
	printf("%d\n",ucAck);
	
	return ucAck;
}


