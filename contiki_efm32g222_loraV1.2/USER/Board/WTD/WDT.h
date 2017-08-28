#ifndef __WTD_H
#define __WTD_H


#include "em_device.h"
#include "em_wdog.h"
#include "em_cmu.h"
#include "em_rmu.h"
#include "em_chip.h"
//#include "rtcdrv.h"



extern unsigned long resetCause;



void WDT_INIT(void)  ;
uint8_t WDT_Cause(void);


#endif

