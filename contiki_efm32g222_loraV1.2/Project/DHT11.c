#include "DHT11.h"
#include "EFM32_include.h"

#define APP_DATA_SIZE                       16
static uint8_t AppDataSize = APP_DATA_SIZE;
#define LORAWAN_APP_DATA_MAX_SIZE                           64
static uint8_t AppData[LORAWAN_APP_DATA_MAX_SIZE];
#define LORAWAN_APP_PORT                            2
static uint8_t AppPort = LORAWAN_APP_PORT;


#define OUT_Port 	gpioPortC
#define OUT_Pin 	11


uint8_t Data_Buf[5];

void DHT_IOInput()
{
	GPIO_PinModeSet(OUT_Port, OUT_Pin,     gpioModeInput   ,     0); 
}
void DHT_IOPushPull()
{
	GPIO_PinModeSet(OUT_Port, OUT_Pin,     gpioModePushPull   ,     0); 
}

static void DHT11_Rst()
{
	DHT_IOPushPull();
	GPIO_PinOutClear(OUT_Port, OUT_Pin);
	Systick_Delay_mS(25);					//拉低至少18ms
	GPIO_PinOutSet(OUT_Port, OUT_Pin);		//总线拉高20-40us 会返回一个低电平
	Systick_Delay_uS(30);
	DHT_IOInput();
}

static uint8_t DHT11_Check(void)
{
	return GPIO_PinInGet(OUT_Port,OUT_Pin);
}	

static uint8_t DHT11_Read_Bit(void)
{
	uint8_t rt = 0;
	while(GPIO_PinInGet(OUT_Port, OUT_Pin)&&(rt < 100))							//等待变高电平，每次发送数据前都会有一个50us的电平信号
	{
		rt++;
		Systick_Delay_uS(1);
	}
	rt = 0;
	while(!GPIO_PinInGet(OUT_Port, OUT_Pin)&&(rt<100))
	{
		rt++;
		Systick_Delay_uS(1);
	}
	Systick_Delay_uS(40);
	if(GPIO_PinInGet(OUT_Port, OUT_Pin))return 1;
	else return 0;
}

static uint8_t DHT11_Read_Byte(void)
{
	uint8_t i;
	uint8_t Data = 0;
	for(i = 0;i < 8; i++)
	{
		Data <<= 1;
		Data |= DHT11_Read_Bit();
	}
	return Data;
}

uint8_t* DHT11_Read_Data()
{
	uint8_t i = 0;
	char check[] = "Device Not Available\n";
	char error_data[] = "Error Data\n";
	
	
	DHT11_Rst();
	
	Systick_Delay_uS(50);
	
	if(DHT11_Check() == 0)		//当检测到DHT11给主控芯片的输入为低电压时，说明DHT11开始响应
	{
		while(!GPIO_PinInGet(OUT_Port, OUT_Pin));			//等待80us的低电平响应信号结束
		while(GPIO_PinInGet(OUT_Port, OUT_Pin));				//等待80us的高电平结束，开始接收数据
		for(i = 0; i < 5 ; i++)
		{
			Data_Buf[i] = DHT11_Read_Byte();
		}
		while(!GPIO_PinInGet(OUT_Port, OUT_Pin));
		DHT_IOPushPull();
		GPIO_PinOutSet(OUT_Port, OUT_Pin);
	
		if(Data_Buf[0]+Data_Buf[1]+Data_Buf[2]+Data_Buf[3]==Data_Buf[4])
		{
			printf("SHI DU WEI:");
			printf("%c",Data_Buf[0]/10+48);
			printf("%c",Data_Buf[0]%10+48);
			printf("%c",'.');
			printf("%c",Data_Buf[1]/10+48);
			printf("%c",Data_Buf[1]%10+48);
			printf("\n");
			printf("WEN DU WEI:");
			printf("%c",Data_Buf[2]/10+48);
			printf("%c",Data_Buf[2]%10+48);
			printf("%c",'.');
			printf("%c",Data_Buf[3]/10+48);
			printf("%c",Data_Buf[3]%10+48);
			printf("\n");
			return Data_Buf;
		}
		else
		{
			printf("%s",error_data);
			return 0;
		}
	}
	else
	{
		printf("%s",check);
		return 0;
	}
}

void TxDHTFrame( void )
{
    AppDataSize=0;
    AppData[AppDataSize++] = Data_Buf[0];
    AppData[AppDataSize++] = Data_Buf[1];
    AppData[AppDataSize++] = Data_Buf[2];
    AppData[AppDataSize++] = Data_Buf[3];
    AppData[AppDataSize++] = Data_Buf[4];   
    AppPort=AppDataSize; 
	for(int i= 0 ; i<=AppDataSize ; i++)
		printf("AppData[%d]=%u\n",i,AppData[i]);
}



