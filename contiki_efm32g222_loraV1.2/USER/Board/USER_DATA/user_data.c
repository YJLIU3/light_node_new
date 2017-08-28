#include "em_msc.h"
#include "user_data.h"
#include "leusart.h"

#define USERDATA_BASE1     (0x0FE00000UL)
uint32_t Page_Start_Address;
uint32_t UserData_Data;
uint8_t  SAVE_DATA[16] ;  

uint8_t Node_Add[8]={0x00,0x00,0x00,0x00};

//用户区起始地址(0x0FE00000UL) 读取长度固定为4的倍数  返回内容为存储数据
uint32_t ReadUserData(uint32_t startAddress)
{
    UserData_Data=0;
    Page_Start_Address=*((__IO uint32_t *)((uint32_t *)(USERDATA_BASE1+startAddress)));
    UserData_Data |=((Page_Start_Address & 0x000000FF)<<24);
    UserData_Data |=((Page_Start_Address & 0x0000FF00)<<8 ); 
    UserData_Data |=((Page_Start_Address & 0x00FF0000)>>8 ); 
    UserData_Data |=((Page_Start_Address & 0xFF000000)>>24);  

    return UserData_Data;
}



//用户区起始地址(0x0FE00000UL) 写入长度固定为4的倍数
uint8_t WriteUserData(uint32_t startAddress,uint8_t *Data,uint16_t Length)
{
    //本页的开始地址
    Page_Start_Address=((uint32_t)((uint32_t *)(USERDATA_BASE1+startAddress))&(~0x3FF));  
    //清除一页
    MSC_ErasePage((uint32_t *)Page_Start_Address);
    //每次重本业起始地址开始写
    MSC_WriteWord((uint32_t *)Page_Start_Address, Data, Length); 

    return 0;
}



void Eeprom_Iint(void)
{
	Node_Add[0]=(ReadUserData(0)>>24)&0xff ;
    Node_Add[1]=(ReadUserData(0)>>16)&0xff;
    Node_Add[2]=(ReadUserData(0)>>8)&0xff;
    Node_Add[3]= ReadUserData(0)&0xff;
    
    Node_Add[4]=(ReadUserData(1)>>24)&0xff ;
    Node_Add[5]=(ReadUserData(1)>>16)&0xff;
    Node_Add[6]=(ReadUserData(1)>>8)&0xff;
    Node_Add[7]= ReadUserData(1)&0xff;
		
//   Leusart_Txdata(Node_Add,4);
	
}




void USE_DATA_SAVE( uint32_t *data , uint8_t save_length)   
{
    uint8_t i;

    SAVE_DATA[0]=(ReadUserData(0)>>24) & 0xff;
    SAVE_DATA[1]=(ReadUserData(0)>>16) & 0xff;
    SAVE_DATA[2]=(ReadUserData(0)>>8)  & 0xff;
    SAVE_DATA[3]= ReadUserData(0)      & 0xff;


    for(i=0;i<save_length;i++)
    {
        SAVE_DATA[4+(i*4)]=(data[i]>>24) & 0xff ;
        SAVE_DATA[5+(i*4)]=(data[i]>>16) & 0xff ;
        SAVE_DATA[6+(i*4)]=(data[i]>>8)  & 0xff ;
        SAVE_DATA[7+(i*4)]=(data[i] )    & 0xff ;
    }
    
    WriteUserData(0,SAVE_DATA,16);
}

