#include "gyiic.h"


void SDA_OUT(void)
{
	GPIO_PinModeSet(gpioPortA , 0 ,gpioModePushPull ,0);
}

void SDA_IN(void)
{
	GPIO_PinModeSet(gpioPortA , 0 ,gpioModeWiredAndPullUpFilter ,1);
}

void SDA_0(void)
{
	GPIO_PinOutClear(gpioPortA , 0 );
}
void SDA_1(void)
{
	GPIO_PinOutSet(gpioPortA , 0 );
}

void SCL_0(void)
{
	GPIO_PinOutClear(gpioPortA , 1 );
}

void SCL_1(void)
{
	
	GPIO_PinOutSet(gpioPortA , 1 );
}

void IIC_Init()
{
	GPIO_PinModeSet(gpioPortA , 0 ,gpioModePushPull ,0);
	GPIO_PinModeSet(gpioPortA , 1 ,gpioModePushPull ,0);
	SDA_1();
	SCL_1();
}

void IIC_Start()
{
	SDA_OUT();
	SDA_1();
	SCL_1();
	Systick_Delay_uS(5);
	SDA_0();
	Systick_Delay_uS(5);
	SCL_0();
}

void IIC_Stop()
{
	SDA_OUT();
	SDA_0();
	SCL_1();
	Systick_Delay_uS(5);
	SDA_1();	
	Systick_Delay_uS(5);
}

uint8_t IIC_Wait_Ack()
{
	uint8_t ucErrTime = 0;
	SDA_1();
	SDA_IN();
	Systick_Delay_uS(1);
	SCL_1();
	Systick_Delay_uS(1);
	
	while(GPIO_PinInGet(gpioPortA,    0))
	{
		ucErrTime++;
		if(ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	SCL_0();
	Systick_Delay_uS(1);
	return 0;
}

void IIC_Ack(void)
{
	SCL_0();
	SDA_OUT();
	SDA_0();
	Systick_Delay_uS(2);
	SCL_1();
	Systick_Delay_uS(2);
	SCL_0();
}

void IIC_NAck(void)
{
	SCL_0();
	SDA_OUT();
	SDA_1();
	Systick_Delay_uS(2);
	SCL_1();
	Systick_Delay_uS(2);
	SCL_0();
}

void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    SCL_0();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
        if((txd & 0x80)>>7)
		{
			SDA_1();
		}
		else
		{
			SDA_0();
		}
        txd<<=1; 	  
		Systick_Delay_uS(2);  
		SCL_1();
		Systick_Delay_uS(2); 
		SCL_0();	
		Systick_Delay_uS(2);
    }	
	IIC_Wait_Ack();	
} 	    

uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_1();
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        receive<<=1;
		SCL_1(); 
        Systick_Delay_uS(5);
		receive |= GPIO_PinInGet(gpioPortA,    0);
		SCL_0();  
//        if(GPIO_PinInGet(gpioPortA,    0))
//		{
//			receive++;
//		}			
		Systick_Delay_uS(5); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}






















 