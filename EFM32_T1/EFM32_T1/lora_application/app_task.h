#ifndef __APP_TASK_H
#define __APP_TASK_H

#include "contiki.h"


void OS_Post_RF_JOIN_REQ(void);
void OS_RF_Heartbeat_Post(void);


#define HMC5983_Init_Time             CLOCK_SECOND*180    //�شų�ʼ����ʱʱ�䣺180S 
#define HMC5983_Init_HEART_TIME       CLOCK_SECOND*2      //�شų�ʼ����ʱʱ�䣺2S 
#define WDOG_FEED_TIME                CLOCK_SECOND*15      //���Ź�ι��ʱ�䣺15S
#define RF_125K_SEND_TIME_FIRST       CLOCK_SECOND*5      //125K��һ���ϴ�ʱ�䣺5S
#define RF_125K_SEND_TIME             CLOCK_SECOND*10     //125K�������ϴ�ʱ�䣺10S

#define RF_125K_SEND_APPROACH_TIME    CLOCK_SECOND        //125K�ϴ�ʱ��ӽ�4438ʱ�䣺1s
#define RF_125K_SEND_WAIT_TIME        CLOCK_SECOND*2      //125K�ϴ�ʱ��ӽ�4438ʱ��ȴ�ʱ�䣺2s


#define Get_Voltage_Temperature_TIME  CLOCK_SECOND*5     //��ѹ���¶Ȳɼ�ʱ�䣺30s






#endif















