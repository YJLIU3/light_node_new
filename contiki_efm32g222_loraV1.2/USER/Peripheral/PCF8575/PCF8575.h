#ifndef __PCF8575_H
#define	__PCF8575_H

#include "stm32f10x.h"




/* 
* Device Address
* 0 0 1 0 A2 A1 A0 R/W
* 0 0 1 0 0  0  0  0 = 0X40
* 0 0 1 0 0  0  1  0 = 0X42 
*/


#define PCF8575_1_ADDRESS	0x40		/* PCF8575_1地址 */
#define PCF8575_2_ADDRESS	0x42		/* PCF8575_2地址 */

#define  PCF8575_NUM1_FLAG  0X01
#define  PCF8575_NUM2_FLAG  0X02



#define  SX1DIO_0 0X01
#define  SX2DIO_0 0X02
#define  SX3DIO_0 0X04
#define  SX4DIO_0 0X08
#define  SX5DIO_0 0X10
#define  SX6DIO_0 0X20
#define  SX7DIO_0 0X40
#define  SX8DIO_0 0X80





#define  SX1DIO_1 0X01
#define  SX2DIO_1 0X02
#define  SX3DIO_1 0X04
#define  SX4DIO_1 0X08
#define  SX5DIO_1 0X10
#define  SX6DIO_1 0X20
#define  SX7DIO_1 0X40
#define  SX8DIO_1 0X80




#define  SX1DIO_2 0X01
#define  SX2DIO_2 0X02
#define  SX3DIO_2 0X04
#define  SX4DIO_2 0X08
#define  SX5DIO_2 0X10
#define  SX6DIO_2 0X20
#define  SX7DIO_2 0X40
#define  SX8DIO_2 0X80



#define  SX1DIO_3 0X01
#define  SX2DIO_3 0X02
#define  SX3DIO_3 0X04
#define  SX4DIO_3 0X08
#define  SX5DIO_3 0X10
#define  SX6DIO_3 0X20
#define  SX7DIO_3 0X40
#define  SX8DIO_3 0X80




//数据写入方向P07-P00  P17-P10
void PCF8575_Write_Enable(uint8_t addr);
void PCF8575_Read_Enable(uint8_t addr);
void PCF8575_ByteRead(uint8_t addr , uint8_t *pBuffer);
void PCF8575_ByteWrite(uint8_t addr ,  uint16_t dataValue);

void PCF8575_Confing(void);
void PCF8575_EXTI_Config(void);


extern uint8_t  IIC_DATA[2];

extern volatile uint16_t IIC1_Read_Data;

void  PCF8575_ISR_Bit( uint8_t  PCF8575_NUM_FLAG ,  uint16_t ISR_Bit );
void PCF8575_ISR_Process(uint8_t  PCF8575_NUM_FLAG , uint16_t ISR_Data);
void PCF8575_EXTI_ENTRE(FunctionalState ENABLE1 );
void Status_reconfirm(uint8_t  PCF8575_NUM_FLAG);

#endif /* __LED_H */
