#ifndef __HC154_H
#define	__HC154_H

#include "stm32f10x.h"


#define HC514_A0(a)	if (a)	\
                        GPIO_SetBits(GPIOD, GPIO_Pin_6);\
					else		\
                        GPIO_ResetBits(GPIOD, GPIO_Pin_6)

#define HC514_A1(a)	if (a)	\
                        GPIO_SetBits(GPIOD, GPIO_Pin_5);\
					else		\
                        GPIO_ResetBits(GPIOD, GPIO_Pin_5)
                    
#define HC514_A2(a)	if (a)	\
                        GPIO_SetBits(GPIOD, GPIO_Pin_4);\
					else		\
                        GPIO_ResetBits(GPIOD, GPIO_Pin_4)                    

#define HC514_A3(a)	if (a)	\
                        GPIO_SetBits(GPIOD, GPIO_Pin_3);\
					else		\
                        GPIO_ResetBits(GPIOD, GPIO_Pin_3) 
                    
                    
#define HC514_EN(a)	if (a)	\
                        GPIO_SetBits(GPIOA, GPIO_Pin_12);\
                    else		\
                        GPIO_ResetBits(GPIOA, GPIO_Pin_12)                   

                    
/******************** 74HC154’Ê÷µ±Ì **************************
*   A0  A1  A2  A3            OUTPUT          PIN
                    
                    
*   0   0   0   0             SX6_REST         Y0
*   1   0   0   0             SX5_CS           Y1
*   0   1   0   0             SX5_REST         Y2
*   1   1   0   0             SX4_CS           Y3
                    
                    
*   0   0   1   0             SX4_REST         Y4
*   1   0   1   0             SX3_CS           Y5
*   0   1   1   0             SX3_REST         Y6
*   1   1   1   0             SX2_CS           Y7
                    
                    
*   0   0   0   1             SX2_REST         Y8
*   1   0   0   1             SX1_CS           Y9
*   0   1   0   1             SX1_REST         Y10
*   1   1   0   1             SX8_CS           Y11


*   0   0   1   1             SX8_REST         Y12
*   1   0   1   1             SX7_CS           Y13
*   0   1   1   1             SX7_REST         Y14
*   1   1   1   1             SX6_CS           Y15
**********************************************************************************/
                    
                                  /*0101*/
#define  SX1_REST_EN()            HC514_A0(0)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(0)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(0)  ;                                 
                                  /*1001*/
#define  SX1_CS_EN()              HC514_A0(1)  ;\
                                  HC514_A1(0)  ;\
                                  HC514_A2(0)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(0)                                      


                                  /*0001*/
#define  SX2_REST_EN()            HC514_A0(0)  ;\
                                  HC514_A1(0)  ;\
                                  HC514_A2(0)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(0)                                    
                                  /*1100*/
#define  SX2_CS_EN()              HC514_A0(1)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(0)  ;\
                                  HC514_EN(0)                                   
                                  



                                  /*0110*/
#define  SX3_REST_EN()            HC514_A0(0)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(0)  ;\
                                  HC514_EN(0)                                   
                                  /*1100*/
#define  SX3_CS_EN()              HC514_A0(1)  ;\
                                  HC514_A1(0)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(0)  ;\
                                  HC514_EN(0)                                   
                                  



                                  /*0010*/
#define  SX4_REST_EN()            HC514_A0(0)  ;\
                                  HC514_A1(0)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(0)  ;\
                                  HC514_EN(0)                                   
                                  /*1100*/
#define  SX4_CS_EN()              HC514_A0(1)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(0)  ;\
                                  HC514_A3(0)  ;\
                                  HC514_EN(0)                                   
                      

                                  /*0100*/
#define  SX5_REST_EN()            HC514_A0(0)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(0)  ;\
                                  HC514_A3(0)  ;\
                                  HC514_EN(0)                                   
                                  /*1000*/
#define  SX5_CS_EN()              HC514_A0(1)  ;\
                                  HC514_A1(0)  ;\
                                  HC514_A2(0)  ;\
                                  HC514_A3(0)  ;\
                                  HC514_EN(0)                                   



                                  /*0000*/
#define  SX6_REST_EN()            HC514_A0(0)  ;\
                                  HC514_A1(0)  ;\
                                  HC514_A2(0)  ;\
                                  HC514_A3(0)  ;\
                                  HC514_EN(0)                                   
                                  /*1111*/
#define  SX6_CS_EN()              HC514_A0(1)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(0)                                   



                                  /*0111*/
#define  SX7_REST_EN()            HC514_A0(0)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(0)                                   
                                  /*1011*/
#define  SX7_CS_EN()              HC514_A0(1)  ;\
                                  HC514_A1(0)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(0)                                   



                                  /*0011*/
#define  SX8_REST_EN()            HC514_A0(0)  ;\
                                  HC514_A1(0)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(0)                                   
                                  /*1100*/
#define  SX8_CS_EN()              HC514_A0(1)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(0)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(0)                                   
                    
  
  


#define  SX_DIS()                 HC514_A0(1)  ;\
                                  HC514_A1(1)  ;\
                                  HC514_A2(1)  ;\
                                  HC514_A3(1)  ;\
                                  HC514_EN(1)                                   






void HC154_Config(void);
void SX_REST_EN(uint8_t Num);
void SX_CS_EN(uint8_t Num);

#endif /* __LED_H */
