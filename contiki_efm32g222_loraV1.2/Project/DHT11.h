#ifndef __DHT11_H__
#define __DHT11_H__
#include "string.h"
#include "math.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "stdint.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"




/*read temp and humidity data*/
uint8_t* DHT11_Read_Data();
void SendData(uint8_t *a);
void DHTDelayMs( uint32_t ms );
void TxDHTFrame();


#endif