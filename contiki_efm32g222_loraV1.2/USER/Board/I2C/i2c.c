#include "EFM32_include.h"
#include "i2c.h"

unsigned long i2c_error = 0;










#define I2C_WRITE_COMMAND  0x3C
#define I2C_READ_COMMAND   0x3D 


uint8_t app[10]={0x01,0x02};

void I2C_Initial(void)
{
    i2c_error = 0;
    unsigned long Delay = 1000;
    
    /* Enabling clock to the I2C, GPIO*/
    CMU_ClockEnable(cmuClock_I2C0, true);
    
    
    
    //Set as pushpull mode
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 1);//SDA
    GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1);//SCL
    
    //send 9 clock of SCL
    for(unsigned char i = 0; i < 9; i++)
    {
        Delay = 3000;
        GPIO_PinOutClear(gpioPortE, 13);
        while(Delay--);
        Delay = 3000;
        GPIO_PinOutSet(gpioPortE, 13);
        while(Delay--);
    }
    
    
    
    /* Using PC6 (SDA) and PC7 (SCL) */
    GPIO_PinModeSet(gpioPortE, 12, gpioModeWiredAndPullUpFilter , 1);
    GPIO_PinModeSet(gpioPortE, 13, gpioModeWiredAndPullUpFilter , 1);  
    
    /* Enable pins at location 2 */
    I2C0->ROUTE = (I2C_ROUTE_SDAPEN |I2C_ROUTE_SCLPEN |I2C_ROUTE_LOCATION_LOC6);
    
    I2C0->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX | I2C_CMD_ABORT;
    
    I2C0->CTRL &= ~_I2C_CTRL_MASK;
    /* Set the CLHR (clock low to high ratio). */
    I2C0->CTRL |= i2cClockHLRStandard <<_I2C_CTRL_CLHR_SHIFT;
    // Set SCK clock low timeout and bus idle timeout
    I2C0->CTRL |= I2C_CTRL_CLTO_1024PPC | I2C_CTRL_GIBITO | I2C_CTRL_BITO_160PCC;
    // Set auto STOP when NACK
    I2C0->CTRL |= I2C_CTRL_AUTOSN;
    
    /* Frequency is given by fSCL = fHFPERCLK/((Nlow + Nhigh)(DIV + 1) + 4), thus */
    /* DIV = ((fHFPERCLK - 4fSCL)/((Nlow + Nhigh)fSCL)) - 1 */
    //Assume system clock = 1000000, i2c clock = 30kHz;
    //div = (1000000 - (4 * 30000)) / (8 * 30000) = 3.67 = 4;
    //CLKDIV = div - 1;
    I2C0->CLKDIV = 64; 
    
    NVIC_ClearPendingIRQ(I2C0_IRQn);
    I2C0->IFC = _I2C_IFC_MASK;
    
//    I2C0->IEN = I2C_IEN_ARBLOST | I2C_IEN_CLTO | I2C_IEN_BITO | I2C_IEN_NACK | I2C_IEN_BUSERR;
    I2C0->IEN = I2C_IEN_ARBLOST ;
    NVIC_EnableIRQ(I2C0_IRQn);
    

    
    BITBAND_Peripheral(&(I2C0->CTRL),_I2C_CTRL_EN_SHIFT,true);




}


unsigned char I2C_WriteByte(unsigned char CFG_add,unsigned char CFG_data)  
{
    //Perpare for a new transmit
    I2C0->CMD = I2C_CMD_ABORT;
    I2C0->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX;
    
    //Clear all the flags
    I2C0->IFC = _I2C_IFC_MASK;
    
    //Begin to transmit
    I2C0->CMD = I2C_CMD_START;                   //Start
    I2C0->TXDATA = I2C_WRITE_COMMAND;  //I2C hardware address
    I2C_WaitForAck();                            
    
    I2C0->TXDATA  = CFG_add;                      //I2C internal address
    I2C_WaitForAck();
    
    I2C0->TXDATA = CFG_data;                         //I2C data 
    I2C_WaitForAck();
    
    I2C0->CMD = I2C_CMD_STOP;                    //Stop
    
    unsigned char Result = i2c_error;
    i2c_error = 0;
    return Result;
}


unsigned char HMC5983_ReadNByte(unsigned char CFG_add, unsigned char n,unsigned char * buff)  
{
    //Perpare for a new transmit
    I2C0->CMD = I2C_CMD_ABORT;
    I2C0->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX;
  
    //Clear all the flags
    I2C0->IFC = _I2C_IFC_MASK;
    
    //Begin to transmit
    I2C0->CMD = I2C_CMD_START;                   //S
    I2C0->TXDATA = I2C_WRITE_COMMAND;  //AD+W hardware address
    I2C_WaitForAck();
    
    I2C0->TXDATA  = CFG_add;                      //internal register address
    I2C_WaitForAck();
    
    I2C0->CMD = I2C_CMD_START;                   //S
    I2C0->TXDATA = I2C_READ_COMMAND;   //AD+R internal register address
    I2C_WaitForAck();
    

    while(n > 0)  
    {   
        while(!((I2C0->IF) & I2C_IF_RXDATAV));
        if(n>1)
        {
            I2C0->CMD = I2C_CMD_ACK;
        }
    
        *buff = (unsigned char)(I2C0->RXDATA);
        buff++;
        n--;  
    }
    
    
    

    I2C0->CMD = I2C_CMD_NACK;                     //NACK
    I2C0->CMD = I2C_CMD_STOP;                     //P
    
    unsigned char Result = i2c_error;
    i2c_error = 0;
 
    return Result;
}

void I2C_WaitForAck(void)
{
    while(!((I2C0->IF) & I2C_IF_ACK))
    {
        //STOP auto send
        if(i2c_error)
        {
            break;
        }
    }
    I2C0->IFC = _I2C_IFC_MASK;
}

void I2C0_IRQHandler(void)
{
    static unsigned char Arblost = 0;
    unsigned long Flag = I2C0->IF;
    Leusart_Txdata(app+3,31);
    if(Flag & I2C_IF_NACK)
    {
        //auto STOP send
        i2c_error = 1;
    }
    if(Flag & I2C_IF_BUSERR)
    {
        I2C0->CMD = I2C_CMD_ABORT;
        I2C0->CMD = I2C_CMD_CLEARPC | I2C_CMD_CLEARTX;
        i2c_error = 1;
    }
    
    if(Flag & I2C_IF_ARBLOST)
    {
        //arbitration losses, maybe SDA is stuck
        if(Arblost++ > 5)
        {
            Arblost = 0;
            I2C_Buslock_Process();
        }
        i2c_error = 1;
    }
    if((Flag & I2C_IF_CLTO) || (Flag & I2C_IF_BITO))
    {
        I2C_Buslock_Process();
        i2c_error = 1;
    }
    
    I2C0->IFC = Flag;
}

void I2C_Buslock_Process(void)
{
    unsigned long Delay = 1000;
    
    //Disable I2C module, and clearing the route register
    BITBAND_Peripheral(&(I2C0->CTRL),_I2C_CTRL_EN_SHIFT,false);
    I2C0->ROUTE = 0;
    //Set as pushpull mode
    GPIO_PinModeSet(gpioPortE, 12, gpioModePushPull, 1);//SDA
    GPIO_PinModeSet(gpioPortE, 13, gpioModePushPull, 1);//SCL
    
    //send 9 clock of SCL
    for(unsigned char i = 0; i < 9; i++)
    {
        Delay = 3000;
        GPIO_PinOutClear(gpioPortE, 13);
        while(Delay--);
        Delay = 3000;
        GPIO_PinOutSet(gpioPortE, 13);
        while(Delay--);
    }
    
    /* Using PD6 (SDA) and PD7 (SCL) */
    GPIO_PinModeSet(gpioPortE, 12, gpioModeWiredAndPullUp, 1);
    GPIO_PinModeSet(gpioPortE, 13, gpioModeWiredAndPullUp, 1);  
    
    /* Enable pins at location 2 */
    I2C0->ROUTE = (I2C_ROUTE_SDAPEN |I2C_ROUTE_SCLPEN |I2C_ROUTE_LOCATION_LOC1);
    //enable again
    BITBAND_Peripheral(&(I2C0->CTRL),_I2C_CTRL_EN_SHIFT,true);
   
}

