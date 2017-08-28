#include "contiki.h"
#include "mac_include.h"
#include "app_task.h"
#include "EFM32_include.h"

#include "mac_include.h"
#include "Comissioning.h"
#include "GY30.h"

#include "LoRaMac-api.h"
#include "LoRaMac-api-v3.h"


#define   RandomSeed    0XFFFFFFFF

/*!
 * Join requests trials duty cycle.
 */
#define OVER_THE_AIR_ACTIVATION_DUTYCYCLE           128*10// 10 [s] value in ms

/*!
 * Defines the application data transmission duty cycle. 5s, value in [ms].
 */
#define APP_TX_DUTYCYCLE                            5000

/*!
 * Defines a random delay for application data transmission duty cycle. 1s,
 * value in [ms].
 */
#define APP_TX_DUTYCYCLE_RND                        1000

/*!
 * LoRaWAN confirmed messages
 */
#define LORAWAN_CONFIRMED_MSG_ON                    false

/*!
 * LoRaWAN Adaptive Data Rate
 *
 * \remark Please note that when ADR is enabled the end-device should be static
 */
#define LORAWAN_ADR_ON                              1






/*!
 * LoRaWAN application port
 */
#define LORAWAN_APP_PORT                            2




/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_SIZE                       16








static uint8_t DevEui[] = LORAWAN_DEVICE_EUI;
static uint8_t AppEui[] = LORAWAN_APPLICATION_EUI;
static uint8_t AppKey[] = LORAWAN_APPLICATION_KEY;







/*!
 * Indicates if the MAC layer has already joined a network.
 */
static bool IsNetworkJoined = false;

/*!
 * Application port
 */
static uint8_t AppPort = LORAWAN_APP_PORT;

/*!
 * User application data size
 */
static uint8_t AppDataSize = LORAWAN_APP_DATA_SIZE;

/*!
 * User application data buffer size
 */
#define LORAWAN_APP_DATA_MAX_SIZE                           64

/*!
 * User application data
 */
static uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];

/*!
 * Indicates if the node is sending confirmed or unconfirmed messages
 */
static uint8_t IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;

/*!
 * Defines the application data transmission duty cycle
 */
static uint32_t TxDutyCycleTime;

/*!
 * Timer to handle the application data transmission duty cycle
 */
static TimerEvent_t TxNextPacketTimer;




#if( OVER_THE_AIR_ACTIVATION != 0 )

/*!
 * Defines the join request timer
 */
static TimerEvent_t JoinReqTimer;

#endif




/*!
 * Indicates if a new packet can be sent
 */
static bool TxNextPacket = true;
static bool ScheduleNextTx = false;

static LoRaMacCallbacks_t LoRaMacCallbacks;




/*!
 * LoRaWAN compliance tests support data
 */
struct ComplianceTest_s
{
    bool Running;
    uint8_t State;
    bool IsTxConfirmed;
    uint8_t AppPort;
    uint8_t AppDataSize;
    uint8_t *AppDataBuffer;
    uint16_t DownLinkCounter;
    bool LinkCheck;
    uint8_t DemodMargin;
    uint8_t NbGateways;
}ComplianceTest;



/*!
 * \brief   Prepares the payload of the frame
 */
static void PrepareTxFrame( uint8_t port )
{
    switch( port )
    {
    case 2:
        {

            AppData[0] = 0;
            AppData[1] = 1;
            AppData[2] = 2;
            AppData[3] = 3;
            AppData[4] = 4;
            AppData[5] = 5;
            AppData[6] = 5;
            AppData[7] = 6;
            AppData[8] = 7;
            AppData[9] = 8;
            AppData[10] = 9;
            AppData[11] = 10;
            AppData[12] = 11;
            AppData[13] = 12;
            AppData[14] = 13;
            AppData[15] = 14;
        }
        
        
        break;
    case 224:
        if( ComplianceTest.LinkCheck == true )
        {
            ComplianceTest.LinkCheck = false;
            AppDataSize = 3;
            AppData[0] = 5;
            AppData[1] = ComplianceTest.DemodMargin;
            AppData[2] = ComplianceTest.NbGateways;
            ComplianceTest.State = 1;
        }
        else
        {
            switch( ComplianceTest.State )
            {
            case 4:
                ComplianceTest.State = 1;
                break;
            case 1:
                AppDataSize = 2;
                AppData[0] = ComplianceTest.DownLinkCounter >> 8;
                AppData[1] = ComplianceTest.DownLinkCounter;
                break;
            }
        }
        break;
    default:
        break;
    }
}



static void ProcessRxFrame( LoRaMacEventFlags_t *flags, LoRaMacEventInfo_t *info )
{
    switch( info->RxPort ) // Check Rx port number
    {
    case 1: // The application LED can be controlled on port 1 or 2
    case 2:
        if( info->RxBufferSize == 1 )
        {
//            AppLedStateOn = info->RxBuffer[0] & 0x01;
//            Led3StateChanged = true;
        }
        break;
    case 224:
        if( ComplianceTest.Running == false )
        {
            // Check compliance test enable command (i)
            if( ( info->RxBufferSize == 4 ) && 
                ( info->RxBuffer[0] == 0x01 ) &&
                ( info->RxBuffer[1] == 0x01 ) &&
                ( info->RxBuffer[2] == 0x01 ) &&
                ( info->RxBuffer[3] == 0x01 ) )
            {
                IsTxConfirmed = false;
                AppPort = 224;
                AppDataSize = 2;
                ComplianceTest.DownLinkCounter = 0;
                ComplianceTest.LinkCheck = false;
                ComplianceTest.DemodMargin = 0;
                ComplianceTest.NbGateways = 0;
                ComplianceTest.Running = true;
                ComplianceTest.State = 1;
                
                LoRaMacSetAdrOn( true );
            }
        }
        else
        {
            ComplianceTest.State = info->RxBuffer[0];
            switch( ComplianceTest.State )
            {
            case 0: // Check compliance test disable command (ii)
                IsTxConfirmed = LORAWAN_CONFIRMED_MSG_ON;
                AppPort = LORAWAN_APP_PORT;
                AppDataSize = LORAWAN_APP_DATA_SIZE;
                ComplianceTest.DownLinkCounter = 0;
                ComplianceTest.Running = false;
                LoRaMacSetAdrOn( LORAWAN_ADR_ON );
                break;
            case 1: // (iii, iv)
                AppDataSize = 2;
                break;
            case 2: // Enable confirmed messages (v)
                IsTxConfirmed = true;
                ComplianceTest.State = 1;
                break;
            case 3:  // Disable confirmed messages (vi)
                IsTxConfirmed = false;
                ComplianceTest.State = 1;
                break;
            case 4: // (vii)
                AppDataSize = info->RxBufferSize;

                AppData[0] = 4;
                for( uint8_t i = 1; i < AppDataSize; i++ )
                {
                    AppData[i] = info->RxBuffer[i] + 1;
                }
                break;
            case 5: // (viii)
                LoRaMacLinkCheckReq( );
                break;
            default:
                break;
            }
        }
        break;
    default:
        break;
    }
}




static bool SendFrame( void )
{
    uint8_t sendFrameStatus = 0;

    if( IsTxConfirmed == false )
    {
        sendFrameStatus = LoRaMacSendFrame( AppPort, AppData, AppDataSize );
    }
    else
    {
        sendFrameStatus = LoRaMacSendConfirmedFrame( AppPort, AppData, AppDataSize, 8 );
    }

    switch( sendFrameStatus )
    {
    case 3: // LENGTH_ERROR
        // Send empty frame in order to flush MAC commands
        LoRaMacSendFrame( 0, NULL, 0 );
        return false;
    case 5: // NO_FREE_CHANNEL
        // Try again later
        return true;
    default:
        return false;
    }
}

 

/*!
* \brief Function executed on JoinReq Timeout event
*/
static void OnJoinReqTimerEvent( void )
{
    printf("4\n");
    TimerStop( &JoinReqTimer );
    TxNextPacket = true;
    OS_Post_RF_JOIN_REQ();
}

 



/*!
 * \brief Function executed on TxNextPacket Timeout event
 */
static void OnTxNextPacketTimerEvent( void )
{
    TimerStop( &TxNextPacketTimer );
    TxNextPacket = true;
}




/*!
 * \brief Function to be executed on MAC layer event
 */
static void OnMacEvent( LoRaMacEventFlags_t *flags, LoRaMacEventInfo_t *info )
{   
   
    if( flags->Bits.JoinAccept == 1 )
    {
         printf("OnMacEvent\n");
        TimerStop( &JoinReqTimer );
        IsNetworkJoined = true;
        OS_RF_Heartbeat_Post( );
    }
    else
    {
        printf("OnMacEvent 111\n");
        if( flags->Bits.Tx == 1 )
        {
        }

        if( flags->Bits.Rx == 1 )
        {
            if( ComplianceTest.Running == true )
            {
                ComplianceTest.DownLinkCounter++;
                if( flags->Bits.LinkCheck == 1 )
                {
                    ComplianceTest.LinkCheck = true;
                    ComplianceTest.DemodMargin = info->DemodMargin;
                    ComplianceTest.NbGateways = info->NbGateways;
                }
            }
            if( flags->Bits.RxData == true )
            {
                ProcessRxFrame( flags, info );
            }
        }
				if(IsNetworkJoined == false)
				{
					OS_Post_RF_JOIN_REQ();
				}
    }
    // Schedule a new transmission
    ScheduleNextTx = true;
}


/*!
 * \brief   Prepares the payload of the frame
 
//static void TxFrame( void )
//{
//    AppDataSize=0;
//    AppData[AppDataSize++] = 0;
//    AppData[AppDataSize++] = 1;
//    AppData[AppDataSize++] = 2;
//    AppData[AppDataSize++] = 3;
//    AppData[AppDataSize++] = 4;
//    AppData[AppDataSize++] = 5;
//    AppData[AppDataSize++] = 6;
//    AppData[AppDataSize++] = 7;
//    AppData[AppDataSize++] = 8;
//    AppData[AppDataSize++] = 9;
//    AppData[AppDataSize++] = 0x0a;
//    AppData[AppDataSize++] = 0x0b;
//    AppData[AppDataSize++] = 0x0c;
//    AppData[AppDataSize++] = 0x0d;
//    AppData[AppDataSize++] = 0x0e;
//    AppData[AppDataSize++] = 0x0f;    
//    AppPort=AppDataSize;
//    
//}
*/

static void TxFrame( void )
{
	uint8_t* BUF;
	
	Single_Write_GY30(0x01);   // power on
		
	Single_Write_GY30(0x10);


	Systick_Delay_mS(180);
	
	BUF = GY_30_Read();
	
    AppDataSize=0;
    AppData[AppDataSize++] = BUF[0];
    AppData[AppDataSize++] = BUF[1];  
    AppPort=AppDataSize;
	
	printf("light1=%d	light1=%d\n",0,0);
    
}







/*创建进程*/
PROCESS(app_led, "app_led");
PROCESS_THREAD(app_led, ev, data)
{
    PROCESS_BEGIN();
    while(1)
    {
        static struct etimer et;

        etimer_set(&et, 64 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        LED1_ON();

 
        
        etimer_set(&et, 64 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        LED1_OFF();  
    
    }
    PROCESS_END();
}



/*加网任务*/
process_event_t RF_JOIN_REQ;
PROCESS(RF_JOIN_TASK,"RF_JOIN_TASK");
PROCESS_THREAD(RF_JOIN_TASK, ev, data)
{
    static   uint8_t sendFrameStatus = 0;
    PROCESS_BEGIN();
    
    while(1)
    {
			
        PROCESS_WAIT_EVENT_UNTIL(ev==RF_JOIN_REQ);
        sendFrameStatus = LoRaMacJoinReq( DevEui, AppEui, AppKey );
        switch( sendFrameStatus )
        {
            case 1: // BUSY
                  printf("6\n");
            break;
            case 0: // OK
            case 2: // NO_NETWORK_JOINED
            case 3: // LENGTH_PORT_ERROR
            case 4: // MAC_CMD_ERROR
            case 6: // DEVICE_OFF
            default:
            // Relaunch timer for next trial
//            TimerStart( &JoinReqTimer );
            break;
        }
//        printf("5\n");
    }
    
    PROCESS_END();
}





/*加网任务*/
process_event_t RF_Heartbeat_Post;
PROCESS(RF_HEARTBEAT_TASK,"RF_HEARTBEAT_TASK");
PROCESS_THREAD(RF_HEARTBEAT_TASK, ev, data)
{
    static   uint8_t sendFrameStatus = 0;
    static struct etimer et;
    PROCESS_BEGIN();
    

    PROCESS_WAIT_EVENT_UNTIL(ev==RF_Heartbeat_Post);
    while(1)
    {
        etimer_set(&et, 128*10 );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        
         printf("1111111111111111111111111111111111111111111111111111111111111\n");

        
        TxFrame();
        sendFrameStatus = LoRaMacSendConfirmedFrame( AppPort, AppData, AppDataSize ,1);//重传次数最大8条
        printf("sendFrameStatus:%X\n",sendFrameStatus);
        printf("22222222222222222222222222222222222222222222222222222222222\n");

    }
    
    PROCESS_END();
}









/*创建上电初始化任务*/
PROCESS(Board_Init_Task, "Board_Init_Task");
PROCESS_THREAD(Board_Init_Task, ev, data)
{
    PROCESS_BEGIN();

    bool trySendingFrameAgain = false;

    LoRaMacCallbacks.MacEvent = OnMacEvent;
//    LoRaMacCallbacks.GetBatteryLevel = BoardGetBatteryLevel;
    LoRaMacInit( &LoRaMacCallbacks );//mac初始化

    IsNetworkJoined = false;


    // Initialize LoRaMac device unique ID
    BoardGetUniqueId( DevEui );

    // Sends a JoinReq Command every OVER_THE_AIR_ACTIVATION_DUTYCYCLE
    // seconds until the network is joined
    TimerInit( &JoinReqTimer, (funCallBack)OnJoinReqTimerEvent );
    TimerSetValue( &JoinReqTimer, OVER_THE_AIR_ACTIVATION_DUTYCYCLE );//设置加网请求时间10s


    TxNextPacket = true;
    TimerInit( &TxNextPacketTimer, (funCallBack)OnTxNextPacketTimerEvent );//初始化TxNextPacketTimer


    LoRaMacSetAdrOn( LORAWAN_ADR_ON );
    LoRaMacSetPublicNetwork( LORAWAN_PUBLIC_NETWORK );

   
    process_start(&RF_HEARTBEAT_TASK,"RF_HEARTBEAT_TASK");
    process_start(&RF_JOIN_TASK,"RF_JOIN_TASK");
    OS_Post_RF_JOIN_REQ();
   

    PROCESS_END();   
}












/*创建喂狗进程*/
PROCESS(WDOG_Feed_Task, "WDOG_Feed_Task");
PROCESS_THREAD(WDOG_Feed_Task, ev, data)
{
    PROCESS_BEGIN();
    while(1)
    {
        static struct etimer et;
        etimer_set(&et, WDOG_FEED_TIME );
        PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
        WDOG_Feed();   
    }
    PROCESS_END();
}













void OS_Post_RF_JOIN_REQ(void)
{ 
   process_post(&RF_JOIN_TASK, RF_JOIN_REQ , NULL);//同步传递      
}




void OS_RF_Heartbeat_Post(void)
{ 
   process_post(&RF_HEARTBEAT_TASK, RF_Heartbeat_Post , NULL);//同步传递      
}



//*自启动任务*/
AUTOSTART_PROCESSES(&Board_Init_Task ,&app_led  , &WDOG_Feed_Task);

//AUTOSTART_PROCESSES( &app_led);



















