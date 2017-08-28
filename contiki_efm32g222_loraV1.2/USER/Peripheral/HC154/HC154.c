#include "HC154.h"

/*
 * 函数名：HC154_GPIO_Config
 * 描述  ：配置HC154用到的I/O口
 * 输入  ：无
 * 输出  ：无
 */
void HC154_Config(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOD, ENABLE);	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
    GPIO_SetBits(GPIOD, GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6 );	 // turn off all GPIO
    
    
    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;       
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_ResetBits(GPIOA, GPIO_Pin_12);	   // turn off all GPIO
}
/******************* (C) COPYRIGHT 2016 *****END OF FILE************/



void SX_REST_EN(uint8_t Num)
{
    switch( Num )
    {
        case 1:
            SX1_REST_EN();
            break;
        case 2:           
            SX2_REST_EN();        
            break;
        
        case 3:
            SX3_REST_EN();
            break;
        
        case 4:
            SX4_REST_EN();
            break;
        
        case 5:
            SX5_REST_EN();
            break;

        case 6:
            SX6_REST_EN();
            break;
        
        case 7:
            SX7_REST_EN();
            break;
        
        case 8:
            SX8_REST_EN();
            break;
        
       default:
            break;
    }
 
}











void SX_CS_EN(uint8_t Num)
{
    switch( Num )
    {
        case 1:
            SX1_CS_EN();
            break;
        case 2:           
            SX2_CS_EN();        
            break;
        
        case 3:
            SX3_CS_EN();
            break;
        
        case 4:
            SX4_CS_EN();
            break;
        
        case 5:
            SX5_CS_EN();
            break;

        case 6:
            SX6_CS_EN();
            break;
        
        case 7:
            SX7_CS_EN();
            break;
        
        case 8:
            SX8_CS_EN();
            break;
        
       default:
            break;
    }
 
}


