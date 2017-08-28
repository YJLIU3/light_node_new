#ifndef __MODE_5310_H
#define	__MODE_5310_H

#include "stm32f10x.h"

void MODE2_5310_Config(void);


#define POWSWON    GPIO_SetBits(GPIOE,GPIO_Pin_13)                   
#define POWSWOFF   GPIO_ResetBits(GPIOE,GPIO_Pin_13)

#define MODEON     GPIO_SetBits(GPIOB,GPIO_Pin_1)
#define MODEOFF    GPIO_ResetBits(GPIOB,GPIO_Pin_1)
					
					
					

#endif /* __LED_H */
