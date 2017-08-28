#ifndef __APP_TASK_H
#define __APP_TASK_H

#include "contiki.h"



#define HMC5983_Init_Time             CLOCK_SECOND*180    //地磁初始化超时时间：180S 
#define HMC5983_Init_HEART_TIME       CLOCK_SECOND*2      //地磁初始化超时时间：2S 
#define WDOG_FEED_TIME                CLOCK_SECOND*4      //看门狗喂狗时间：4S
#define RF_125K_SEND_TIME_FIRST       CLOCK_SECOND*5      //125K第一次上传时间：5S
#define RF_125K_SEND_TIME             CLOCK_SECOND*10     //125K后两次上传时间：10S

#define RF_125K_SEND_APPROACH_TIME    CLOCK_SECOND        //125K上传时间接近4438时间：1s
#define RF_125K_SEND_WAIT_TIME        CLOCK_SECOND*2      //125K上传时间接近4438时间等待时间：2s


#define Get_Voltage_Temperature_TIME  CLOCK_SECOND*5     //电压和温度采集时间：30s






#endif















