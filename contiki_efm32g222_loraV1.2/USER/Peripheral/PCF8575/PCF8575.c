#include "PCF8575.h"
#include "includes.h"
#include "stm32f10x.h"



volatile uint16_t IIC1_Read_Data;
uint8_t  IIC_DATA[2];

static uint16_t  PCF8575_ISR1_Prev=0Xffff; //存储当前io口状态
static uint16_t  PCF8575_ISR2_Prev=0Xffff;

static uint8_t reconfirm_flag=0;


#define I2C1_WR	0		/* 写控制bit */
#define I2C1_RD	1		/* 读控制bit */



void PCF8575_Confing(void)
{  

    
    I2C1_Confing();
    PCF8575_Read_Enable(PCF8575_1_ADDRESS);
    PCF8575_Read_Enable(PCF8575_2_ADDRESS);
    
//    PCF8575_Write_Enable(PCF8575_1_ADDRESS);
//    PCF8575_Write_Enable(PCF8575_2_ADDRESS);
    
    PCF8575_EXTI_Config();
    
    
    PCF8575_ByteRead( PCF8575_1_ADDRESS , IIC_DATA) ; 
    PCF8575_ISR1_Prev=(IIC_DATA[1]<<8) | IIC_DATA[0];

    PCF8575_ByteRead( PCF8575_2_ADDRESS , IIC_DATA) ; 
    PCF8575_ISR2_Prev=(IIC_DATA[1]<<8) | IIC_DATA[0];
    
    
}




//数据读取方向P07-P00  P17-P10
void PCF8575_Read_Enable(uint8_t addr)
{   
    /*设置为读取时，16个口要设置为高电平*/
    PCF8575_ByteWrite( addr , 0xffff);
    PCF8575_ByteRead(  addr , IIC_DATA) ; 
}


//数据写入方向P07-P00  P17-P10
void PCF8575_Write_Enable(uint8_t addr)
{       
    PCF8575_ByteWrite(addr ,  0xffff);       
}


//数据写入方向P07-P00  P17-P10
void PCF8575_ByteWrite(uint8_t addr ,  uint16_t dataValue)
{    
    while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));
    
    /* Send STRAT condition */
    I2C_GenerateSTART(I2C1,ENABLE);
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
    
        
    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
    
    
    /* Send the byte to be written */
    I2C_SendData(I2C1,(dataValue>>8) & 0xff);//取高八位
    /* Test on EV8 and clear it */    
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
    
    /* Send the byte to be written */
    I2C_SendData(I2C1,dataValue & 0xff);//取低八位
    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    
        
    /* Send STOP condition */
    I2C_GenerateSTOP(I2C1,ENABLE);
    

}




//数据读取方向P07-P00  P17-P10
void PCF8575_ByteRead(uint8_t addr , uint8_t *pBuffer)
{  
    
    uint8_t NumByteToRead=2;


    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));   
    
    

    /* Send START condition */
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));



    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Transmitter);
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));



    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2C1, ENABLE);


    /* Send STRAT condition a second time */  
    I2C_GenerateSTART(I2C1, ENABLE);
    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));



    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C1, addr, I2C_Direction_Receiver);
    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));




    /* While there is data to be read */
    while(NumByteToRead)  
    {
        if(NumByteToRead == 1)
        {
            /* Disable Acknowledgement */
            I2C_AcknowledgeConfig(I2C1, DISABLE);
            /* Send STOP Condition */
            I2C_GenerateSTOP(I2C1, ENABLE);
        }

        /* Test on EV7 and clear it */
        if(I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED)==1)  
        {      
            /* Read a byte from the EEPROM */
            *pBuffer = I2C_ReceiveData(I2C1);
            /* Point to the next location where the byte read will be saved */
            pBuffer++; 
            /* Decrement the read bytes counter */
            NumByteToRead--;        
        }   
    }

    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C1, ENABLE);
}



/*设置PCF8575 1和2中断模式  上拉输入  下降沿触发*/
void PCF8575_EXTI_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure; 
    EXTI_InitTypeDef EXTI_InitStructure;
    
    

    /* config the extiline clock and AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE | RCC_APB2Periph_GPIOD | RCC_APB2Periph_AFIO,ENABLE);
                                        

    
    
    NVIC_Configuration( EXTI0_IRQn   ,0 , 1);    //PE0  PCF8575 1  中断口配置   
    NVIC_Configuration( EXTI9_5_IRQn ,0 , 2);    //PD7  PCF8575 2  中断口配置
    
    

    /* EXTI line gpio config*/	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;       
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* EXTI line mode config */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0); 
    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure); 
    
    
    
    /* EXTI line gpio config*/	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;       
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	 // 上拉输入
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    
    /* EXTI line mode config */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOD, GPIO_PinSource7); 
    EXTI_InitStructure.EXTI_Line = EXTI_Line7;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //下降沿中断
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);    
}




void PCF8575_ISR_Process(uint8_t  PCF8575_NUM_FLAG , uint16_t ISR_Data)
{
    uint16_t  i=0;
    uint16_t  j=0x0001;
    
    reconfirm_flag=0;
    switch(PCF8575_NUM_FLAG)
    {
        case PCF8575_NUM1_FLAG:

            for(i=0;i<16;i++)    
            {                                      
                if( ((PCF8575_ISR1_Prev &  j)==j) &&  ((ISR_Data & j)==0)  )
                {
                    PCF8575_ISR_Bit( PCF8575_NUM1_FLAG ,  j );
                }
                j=j<<1;
            }
            PCF8575_ISR1_Prev=ISR_Data;
            
            
        break;


        case PCF8575_NUM2_FLAG:                
            for(i=0;i<16;i++)    
            {                       
                if( ((PCF8575_ISR2_Prev &  j)==j) &&  ((ISR_Data & j)==0)  )
                {
                    PCF8575_ISR_Bit( PCF8575_NUM2_FLAG ,  j );                
                }
                j=j<<1;
            }
            PCF8575_ISR2_Prev=ISR_Data;
        break;

        
       default:
            break;
    }
    
    

}






void  PCF8575_ISR_Bit( uint8_t  PCF8575_NUM_FLAG ,  uint16_t ISR_Bit )
{
    switch(PCF8575_NUM_FLAG)
    {
        case PCF8575_NUM1_FLAG:
            switch(ISR_Bit)
            {
                case 0x0001: //SX2_DIO3 IRQ
                    SXDIO_Post(Sem_SXDIO_3 , SX2DIO_3);                  
                    break;

                case 0x0002: // SX2_DIO0 IRQ
                     SXDIO_Post(Sem_SXDIO_0 , SX2DIO_0);                                 
                    break;
                
                case 0x0004://SX2_DIO1 IRQ   
                    SXDIO_Post(Sem_SXDIO_1 , SX2DIO_1);                
                    break;
                
                case 0x0008: //SX2_DIO2 IRQ 
                    SXDIO_Post(Sem_SXDIO_2 , SX2DIO_2);                 
                    break;
                
                
                
                case 0x0010: //SX1_DIO3 IRQ 
                    SXDIO_Post(Sem_SXDIO_3 , SX1DIO_3);                
                    break;
                
                case 0x0020://SX1_DIO0 IRQ
                    SXDIO_Post(Sem_SXDIO_0 , SX1DIO_0);                
                    break;
                
                case 0x0040://SX1_DIO1 IRQ
                    SXDIO_Post(Sem_SXDIO_1 , SX1DIO_1);                
                    break;
                
                case 0x0080://SX1_DIO2 IRQ 
                    SXDIO_Post(Sem_SXDIO_2 , SX1DIO_2);                
                    break;
                
                
                
                
                case 0x0100:// SX4_DIO3 IRQ
                    SXDIO_Post(Sem_SXDIO_3 , SX4DIO_3);                 
                    break;
                
                case 0x0200: //SX4_DIO0 IRQ 
                    SXDIO_Post(Sem_SXDIO_0 , SX4DIO_0);                 
                    break;
                
                case 0x0400:// SX4_DIO1 IRQ
                    SXDIO_Post(Sem_SXDIO_1 , SX4DIO_1);                 
                    break;
                
                case 0x0800://SX4_DIO2 IRQ   ; 
                    SXDIO_Post(Sem_SXDIO_2 , SX4DIO_2);                 
                    break;
                
                
                
                
                case 0x1000://SX3_DIO3 IRQ
                    SXDIO_Post(Sem_SXDIO_3 , SX3DIO_3);                      
                    break;

                case 0x2000: //SX3_DIO0 IRQ
                    SXDIO_Post(Sem_SXDIO_0 , SX3DIO_0);                   
                    break;  
                
                case 0x4000: //SX3_DIO1 IRQ 
                    SXDIO_Post(Sem_SXDIO_1 , SX3DIO_1);                
                    break;  
                
                case 0x8000://SX3_DIO2 IRQ
                    SXDIO_Post(Sem_SXDIO_2 , SX3DIO_2);                
                    break;     

                
                
                default:
                    break;
            }
        break;


        case PCF8575_NUM2_FLAG:  
            switch(ISR_Bit)
                {
                    case 0x0001: // SX6_DIO3 IRQ                     
                        SXDIO_Post(Sem_SXDIO_3 , SX6DIO_3);                     
                        break;

                    case 0x0002: //SX6_DIO0 IRQ
                        SXDIO_Post(Sem_SXDIO_0 , SX6DIO_0);                     
                        break;
                    
                    case 0x0004://SX6_DIO1 IRQ
                        SXDIO_Post(Sem_SXDIO_1 , SX6DIO_1);                     
                        break;
                    
                    case 0x0008://SX6_DIO2 IRQ
                        SXDIO_Post(Sem_SXDIO_2 , SX6DIO_2);                     
                        break;
                    
                    
                    
                    
                    case 0x0010: // SX5_DIO3 IRQ                       
                        SXDIO_Post(Sem_SXDIO_3 , SX5DIO_3);                    
                        break;
                    
                    case 0x0020://SX5_DIO0 IRQ
                        SXDIO_Post(Sem_SXDIO_0 , SX5DIO_0);                    
                        break;
                    
                    case 0x0040://SX5_DIO1 IRQ
                        SXDIO_Post(Sem_SXDIO_1 , SX5DIO_1);                    
                        break;
                    
                    case 0x0080://SX5_DIO2 IRQ
                        SXDIO_Post(Sem_SXDIO_2 , SX5DIO_2);                    
                        break;
                    
                    
                    
                    
                    case 0x0100://SX8_DIO3 IRQ
                        SXDIO_Post(Sem_SXDIO_3 , SX8DIO_3);                    
                        break;
                    
                    case 0x0200://SX8_DIO0 IRQ
                        SXDIO_Post(Sem_SXDIO_0 , SX8DIO_0);                    
                        break;
                    
                    case 0x0400://SX8_DIO1 IRQ
                        SXDIO_Post(Sem_SXDIO_1 , SX8DIO_1);                    
                        break;
                    
                    case 0x0800://SX8_DIO2 IRQ
//                        OSSemPost(Sem);                    
                        SXDIO_Post(Sem_SXDIO_2 , SX8DIO_2);                    
                        break;
                    
                    
                    
                    
                    case 0x1000://SX7_DIO2 IRQ
                        SXDIO_Post(Sem_SXDIO_2 , SX7DIO_2);                    
                        break;

                    case 0x2000: //SX7_DIO1 IRQ
                        SXDIO_Post(Sem_SXDIO_1 , SX7DIO_1);                    
                        break;  
                    
                    case 0x4000://SX7_DIO0 IRQ
                        SXDIO_Post(Sem_SXDIO_0 , SX7DIO_0);                    
                        break;  
                    
                    case 0x8000://SX7_DIO3 IRQ
                        SXDIO_Post(Sem_SXDIO_3 , SX7DIO_0);                    
                        break;                   
                    
                             
                    default:
                        break;
                }            
        break;

        
       default:
            break;
    }




}


/****PCF8575_1 优先级别高，PCF8575_2执行时需要关闭PCF8575_1中断***/
void PCF8575_EXTI_ENTRE(FunctionalState ENABLE )
{
    EXTI_InitTypeDef PCF8575_EXTI;


    /* EXTI line mode config */
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOE, GPIO_PinSource0); 
 
    PCF8575_EXTI.EXTI_Line = EXTI_Line0;
    PCF8575_EXTI.EXTI_Mode = EXTI_Mode_Interrupt;
    PCF8575_EXTI.EXTI_Trigger = EXTI_Trigger_Falling; 
    PCF8575_EXTI.EXTI_LineCmd = ENABLE;
    EXTI_Init(&PCF8575_EXTI);    
}



void Status_reconfirm( uint8_t  PCF8575_NUM_FLAG)
{
    while(reconfirm_flag==0)
    {                  
        if(PCF8575_NUM_FLAG==PCF8575_NUM1_FLAG)
        {
            PCF8575_ByteRead( PCF8575_1_ADDRESS , IIC_DATA) ;  
            IIC1_Read_Data=(IIC_DATA[1]<<8) | IIC_DATA[0];
              //printf("3 \n");
            if(PCF8575_ISR1_Prev==IIC1_Read_Data)
            {
                //printf("1 \n"); 
                reconfirm_flag=1;           
            }
            else
            {
//                printf("2 \n"); 
                reconfirm_flag=0;
                PCF8575_ISR_Process(PCF8575_NUM1_FLAG , IIC1_Read_Data);
            }
        }
        
        
        if(PCF8575_NUM_FLAG==PCF8575_NUM2_FLAG)
        {
            PCF8575_ByteRead( PCF8575_2_ADDRESS , IIC_DATA) ;  
            IIC1_Read_Data=(IIC_DATA[1]<<8) | IIC_DATA[0];
              //printf("3 \n");
            if(PCF8575_ISR2_Prev==IIC1_Read_Data)
            {
                //printf("1 \n"); 
                reconfirm_flag=1;           
            }
            else
            {
//                printf("2 \n"); 
                reconfirm_flag=0;
                PCF8575_ISR_Process(PCF8575_NUM2_FLAG , IIC1_Read_Data);
            }
        }   
    }
}


