#include "project.h"


#if 0
unsigned char NormalAck[11] = {0x68, 0x10, 0xEF, 0x68, 0x84, 0x01, 0x34, 0x88, 0x16, 0xAA, 0xAA};
unsigned char ErrorAck[11] = {0x68, 0x10, 0xEF, 0x68, 0xC4, 0x01, 0xB3, 0x47, 0x16, 0xAA, 0xAA};
unsigned char TestAck[11] = {0x68, 0x10, 0xEF, 0x68, 0x85, 0x01, 0x33, 0x88, 0x16, 0xAA, 0xAA};
unsigned char ResetAck[10] = {0x68, 0x10, 0xEF, 0x68, 0x90, 0x00, 0x5F, 0x16, 0xAA, 0xAA};

/*Hardware Version and Firmware Version,Note: when change the version number 
the same time change the checksum byte*/
//68 10 ef 68 82 08 4e 53 2d 44 42 41 30 31
unsigned char VersionAck[18] = {0x68, 0x10, 0xEF, 0x68, 0x82, 0x08, 'N', 'S', '-', 'D', 'B', 'A', '0', '1', 0x4F, 0x16, 0xAA, 0xAA}; //NS-DBA01
//68 10 ef 68 82 08 4e 53 43 31 32 56 30 33
unsigned char EurVersionAck[18] = {0x68, 0x10, 0xEF, 0x68, 0x82, 0x08, 'N', 'S', 'C', '1', '2', 'V', '0', '3', 0x59, 0x16, 0xAA, 0xAA}; //NSHDBV01
extern ReceiveInitDef Receive;
unsigned int CommFalutCnt, CommFalutCntS; //Storing Communication falut counter
unsigned char UartCmdBuff[50];            //Storing ghe data
unsigned short UartCmdbuffSize = 0;       //Storing the data length of a vaild frame.

/* find out a vaild frame form the receive buffer */
unsigned int QueueFind_Cmd(unsigned char *buffer, unsigned short *buf_len)
{
    unsigned char cmd_size = 0;
    unsigned char data = 0;

    while (QueueSize() > 0)
    {

        /*Read the data*/ //每次从队列尾部读一个数据
        QueuePop(&data);

        if (CmdState != 4) //判断数据帧头
        {
            if (data == 0x68) //当前数据是68的情况
            {
                if (CmdState == 0)
                {
                    buffer[CmdPos++] = data;
                    CmdState = 1; //68 ?? ?? ??
                }
                else if ((CmdState == 1) || (CmdState == 2)) //防止出现 68 68数据头
                {
                    CmdPos = 0;
                    buffer[CmdPos++] = data;
                    CmdState = 1; //68 ?? ?? ??
                }
                else if (CmdState == 3)
                {
                    buffer[CmdPos++] = data; //68 10 EF 68
                    CmdState = 4;
                }
                else //不符合的情况就从头开始判断
                {
                    CmdState = 0; //?? ?? ?? ??
                    CmdPos = 0;
                }
            }
            else if (data == 0x10)
            {
                if (CmdState == 1)
                {
                    buffer[CmdPos++] = data;
                    CmdState = 2; //68 10 ?? ??
                }
                else
                {
                    CmdState = 0; //?? ?? ?? ??
                    CmdPos = 0;
                }
            }
            else if (data == 0xEF)
            {
                if (CmdState == 0x02)
                {
                    buffer[CmdPos++] = data;
                    CmdState = 3; //68 10 EF ??
                }
                else
                {
                    CmdState = 0; //?? ?? ?? ??
                    CmdPos = 0;
                }
            }
            else
            {
            }
        }
        else
        {
            if (CmdPos > 5)
            {
                if (CmdPos < ((*buf_len) - 1)) //接收数据长度的数据
                {                              //68,10,EF,68,Cmd,len,...,CS
                    buffer[CmdPos++] = data;
                }
                else
                { //68,10,EF,68,Cmd,len,.....,16
                    /* Get the vaild frame */
                    cmd_size = CmdPos;
                    CmdState = 0;
                    CmdPos = 0;
                    return cmd_size; //返回函数 返回共接收一帧长度
                }
            }
            else if (CmdPos > 4)
            {                            //68,10,EF,68,Cmd,len,...
                buffer[CmdPos++] = data; //获得数据长度
                *buf_len = data + 8;     //data length is len+8bytes("68,10,EF,68,Cmd,Len,...,CS,16")

                if (*buf_len > 30)
                { //如果接收到的数据长度字节超过设置的可接受的最大值时,即认为接收数据异常,丢弃此帧数据
                    CmdState = 0;
                    CmdPos = 0;
                }
            }
            else if (CmdPos > 3)
            { //68,10,EF,68,Cmd,...
                buffer[CmdPos++] = data;
            }
            else
            {
            }
        }
    }
    return 0;
}

/*******************************************************************************
  Function name:   ReceiveFrame_CheckSum()
         Author:   Lucien Song   
           Date:   2016-10-27
       Function:   Calculating the frame data check sum
Input Parameter:   buffer: input data pointer
                   buf_len: data length
   Return Value:   check sum byte
  Modify Recode:   None
*******************************************************************************/
unsigned char ReceiveFrame_CheckSum(unsigned char *buffer, unsigned short buf_len)
{
    unsigned char checksum = 0;
    unsigned char i = 0;

    for (i = 0; i < (buf_len - 2); i++)
    { //Remove the last two bytes("CS,16")

        checksum += buffer[i];
    }

    return (checksum);
}
/*******************************************************************************
  Function name:   ReceiveFrame_Decode()
         Author:   Lucien Song   
           Date:   2016-10-27
       Function:   Get and explain the Receive buffer 
Input Parameter:   None
   Return Value:   None
  Modify Recode:   None
*******************************************************************************/
void ReceiveFrame_Decode(void)
{
    unsigned int down_cnt = 1000000;
    static unsigned char sizexx = 0;

    /* Search the USART receiver buffer */
    sizexx = QueueFind_Cmd(UartCmdBuff, &UartCmdbuffSize);

    /* Check wether receive a vaild frame */
    if (sizexx > 0)
    {
        /* Check the Receive checksum byte equal to calculation checksum */ //校验和
        if (ReceiveFrame_CheckSum(UartCmdBuff, UartCmdbuffSize) == UartCmdBuff[UartCmdbuffSize - 2])
        {

            /* Clear Communication error counter */
            CommFalutCnt = 0;
            CommFalutCntS = 0;

            if (UartCmdBuff[4] == 0x04)
            { //Normal Command

                /* Get the data */
                Receive.Scooter.car_state = ((UartCmdBuff[7] - 0x33) << 8) + (unsigned char)(UartCmdBuff[6] - 0x33);
                setStatusUnitFlash(Receive.Scooter.car_state); //单位切换写入eeprom
                Receive.Scooter.gear = (UartCmdBuff[8] - 0x33);
                Receive.Scooter.speed = (UartCmdBuff[9] - 0x33);
                Receive.Scooter.mileage = (unsigned long)(((unsigned char)((UartCmdBuff[13]) - 0x33)) << 24) +
                                          (unsigned long)(((unsigned char)((UartCmdBuff[12]) - 0x33)) << 16) +
                                          (unsigned long)(((unsigned char)((UartCmdBuff[11]) - 0x33)) << 8) +
                                          (unsigned long)(((unsigned char)((UartCmdBuff[10]) - 0x33)) << 0);
                Receive.Scooter.c_curr = (UartCmdBuff[14] - 0x33);
                Receive.Scooter.dc_curr = (UartCmdBuff[15] - 0x33);
                Receive.Scooter.battery = (UartCmdBuff[16] - 0x33);
                Receive.Scooter.full_c_t = (UartCmdBuff[17] - 0x33);
                Receive.Scooter.time_h = (UartCmdBuff[18] - 0x33);
                Receive.Scooter.time_m = (UartCmdBuff[19] - 0x33);
                Receive.Scooter.error_code = (UartCmdBuff[20] - 0x33);

                /* Return mormal ack frame */
                MAX485_SendNBytes(sizeof(NormalAck), NormalAck);
            }
            else if (UartCmdBuff[4] == 0x05)
            { //Test Command

                if (UartCmdBuff[6] == 0x02)
                { //68 10 EF 68 05 01 02 D7 16
                    //Turn off Display all pixel on LCD
                    Disp_Mod = 2;
                }
                else if (UartCmdBuff[6] == 0x01)
                { //68 10 EF 68 05 01 01 D6 16
                    //Turn on Display all pixel on LCD
                    Disp_Mod = 1;
                }
                else if (UartCmdBuff[6] == 0x00)
                { //68 10 EF 68 05 01 00 D5 16
                    //normal
                    Disp_Mod = 0;
                }
                else
                {
                }

                MAX485_SendNBytes(sizeof(TestAck), TestAck);
            }
            else if (UartCmdBuff[4] == 0x02)
            { //Get Firmware&Hardware Command

                if (EUROPE_Select == 1)
                {

                    MAX485_SendNBytes(sizeof(EurVersionAck), EurVersionAck);
                }
                else
                {
                    //68 10 EF 68 02 01 01 D3 16
                    MAX485_SendNBytes(sizeof(VersionAck), VersionAck);
                }
            }
            else if (UartCmdBuff[4] == 0x10)
            { //IAP Command,IWDG reset

                //68 10 EF 68 10 00 DF 16
                MAX485_SendNBytes(sizeof(ResetAck), ResetAck);

                /* Here add the delay to ensure reset frame is sent out before reset active*/
                down_cnt = 1000000;
                while (down_cnt--)
                    ;

                /* Reset MCU */ //MCU系统复位
                NVIC_SystemReset();
                //while(1);
            }
            else
            {

                MAX485_SendNBytes(sizeof(ErrorAck), ErrorAck);
            }
        }
        else
        {

            /* Sent out Error Ack */
            MAX485_SendNBytes(sizeof(ErrorAck), ErrorAck);
        }
    }
    else
    { //if no get vaild frame, increase communication error counter

        CommFalutCnt++; //通信故障计时
        CommFalutCntS++;

        if (CommFalutCntS >= (COMMFALUTCNT / 10))
        {
            CommFalutCntS = 0; //重新初始化
            USART1_Init();
        }

        if (CommFalutCnt > COMMFALUTCNT)
        { //if the counter equal to set value, display the 99 error code

            /**/
            CommFalutCnt = COMMFALUTCNT;
            /* Set the scooter status is error */ //GPS/GSM信号无状态
            Receive.Scooter.car_state |= (ERROR | GPS_NO | GSM_NO);
            /* Set the error code */
            Receive.Scooter.error_code = 99;
        }
    }
}

#endif 

#define TEST_PORT       ITP_DEVICE_UART0
#define TEST_DEVICE     itpDeviceUart0
#define TEST_BAUDRATE   CFG_UART0_BAUDRATE
#define TEST_GPIO_RX    CFG_GPIO_UART0_RX

#define	FrameDeviceAddr					(0x10)
#define FrameHeader						(0x68)
#define	FrameAddrField					(((unsigned shor)FrameDeviceAddr<<8)|(0xEF))
#define	FrameNormalCmd					(0x04)
#define	FrameGetDeviceInfo				(0x02)
#define	FrameTestCmd					(0x05)
#define	FrameResetDeviceCmd				(0x10)
#define	FrameNormalAckCmd				(0x84)
#define	FrameyExceptAckCmd				(0xC4)
#define	FrameyDataLength				(0x0F)
#define	FrameEnd						(0x16)
/*
typedef union {
        struct
        {
            unsigned short  car_status1;                //Car status1
            unsigned char   car_gear;                   //Gear
            unsigned char   car_speed;                  //Speed
            unsigned int    car_mileage;                //Mileage
            unsigned char   car_chargecurrent;          //Charge Current
            unsigned char   car_dischargecurrent;       //Discharge Current
            unsigned char   car_bat1_soc;               //Battery1 State of Charge
            unsigned char   car_full_c_t;               //full charge time
            unsigned char   time_hour;                  //hour
            unsigned char   time_minute;                //minute
            unsigned char   car_fault_code;             //fault Code
            unsigned char   car_bat2_soc;               //Battery2 State of Charge
            unsigned short  car_status2;                //Car status2
            unsigned char   lightsenor;                   //light sensor' value
            unsigned char   temperature;                //temperature
        } ScooterInfo;

        unsigned char DisplayDatabuffer[20];

    } ReceiveDisplayData_Typedef;
*/
ReceiveDisplayData_Typedef ReceiveDataBuffer;
//R1C01V01
unsigned char NormalAck[11]  ={0x68,0x10,0xEF,0x68,0x84,0x01,0x34,0x88,0x16,0xAA,0xAA};
unsigned char ErrorAck[11]   ={0x68,0x10,0xEF,0x68,0xC4,0x01,0xB3,0x47,0x16,0xAA,0xAA};
unsigned char TestAck[11]    ={0x68,0x10,0xEF,0x68,0x85,0x01,0x33,0x88,0x16,0xAA,0xAA};
unsigned char ResetAck[10]   ={0x68,0x10,0xEF,0x68,0x90,0x00,0x5F,0x16,0xAA,0xAA};
unsigned char VersionAck[18] = {0x68,0x10,0xEF,0x68,0x82,0x08,'R','1','C','0','1','V','0','1',0x4F,0x16,0xAA,0xAA};     //R1C01V01
static bool RunRS485Quit;
static bool RunuartQuit;
static bool GetCmdheader = false;
static bool GetNormalCmd = false;
static bool GetDeviceInfoCmd = false;
static bool GetTestDeviceCmd = false;
static bool GetResetDeviceCmd = false;
static unsigned char framedatalength = 0;
static sem_t RS485Sem;
static sem_t UartSem;

typedef enum GET_UART_COMMAND_STATE_TAG
{
	GET_HEADER,
	GET_ADDRESS_DEVICE,
	GET_ADDRESS_ONESCOMPLEMENT,
	GET_CMD,
	GET_LENGTH,
	GET_PAYLOAD,
	GET_CHECKSUM,
	GET_END
}GET_UART_COMMAND_STATE;

static void RS485Callback(void* arg1, uint32_t arg2)
{	
	//don`t add any codes here.
	sem_post(&RS485Sem);
}

static void UartCallback(void* arg1, uint32_t arg2)
{
	//don`t add any codes here.
	sem_post(&UartSem);
}

static unsigned int Rs485RequestCallBack(unsigned char* str,unsigned char length)
{
	unsigned int i = 0;
	if(length>8)
	{
		switch(str[4])
		{
			case 0x84:
			case 0xC4:
			case 0x85:
				// str[6] += 0x33;
				break;
			case 0x82:
				// for (i = 0; i < str[5];i++)
				// {
				// 	str[6 + i] += 0x33;
				// }
				default:
					break;
		}		
		write(TEST_PORT,str, length);
		return 0;
	}
	else
	{
		return 1;
	}	
}

void* UartttlComunicationTask(void* arg)
{
	uint8_t sendtr1[8] = {0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
    char getstr[256];
    char sendtr[256];
	int len = 0 ,count =0,sendPos =0,countx = 0;	
	int i=0,PayloadCount =0,GetCheckSum =0;
	bool isCmdCompleted = false;
	
	GET_UART_COMMAND_STATE   gState = GET_HEADER;	

	printf("Run UartComunicationTask!\n");

	sem_init(&UartSem, 0, 0);

    itpRegisterDevice(TEST_PORT, &TEST_DEVICE);
    ioctl(TEST_PORT, ITP_IOCTL_INIT, NULL);
    ioctl(TEST_PORT, ITP_IOCTL_RESET, TEST_BAUDRATE);
    
	ioctl(TEST_PORT, ITP_IOCTL_REG_UART_CB, (void*)UartCallback);
	//ioctl(TEST_PORT, ITP_IOCTL_REG_UART_DEFER_CB , (void*)UartCallback);

	memset(getstr, 0, 256);
	memset(sendtr, 0, 256);
	write(TEST_PORT, sendtr1, 8);
	GetCmdheader = false;
	GetNormalCmd = false;
	GetDeviceInfoCmd = false;
	GetTestDeviceCmd = false;
	GetResetDeviceCmd = false;
	RunuartQuit = false;
	while(1)
	{			
		sem_wait(&UartSem);      	
		len = read(TEST_PORT,getstr,256);	
		// write(TEST_PORT, getstr, len);	
		printf("Get New data %d bytes!\n",len);			
		count = 0;
		if(len)
		{
			while(len--)
			{
				switch(gState)
				{
					case GET_HEADER:												
						framedatalength = 0;
						GetNormalCmd = false;
						GetDeviceInfoCmd = false;
						GetTestDeviceCmd = false;
						GetResetDeviceCmd = false;
						// printf("getstr[%d] = 0x%x\n",count,getstr[count]);
						if(FrameHeader == getstr[count])
						{
							if(GetCmdheader == true)
							{
								gState = GET_CMD;
								GetCmdheader = false;
							}
							else
							{
								gState = GET_ADDRESS_DEVICE;
							}																			
							sendtr[sendPos++] = getstr[count];	
						}
						else
						{						
							sendPos = 0;
							memset(sendtr, 0, 256);
							gState = GET_HEADER;
						}
						break;
					case GET_ADDRESS_DEVICE:
						
						if(FrameDeviceAddr == getstr[count])
						{
							sendtr[sendPos++] = getstr[count];
							gState = GET_ADDRESS_ONESCOMPLEMENT;
						}
						else
						{
							sendPos = 0;
							memset(sendtr, 0, 256);
							gState = GET_HEADER;
						}
												
						break;
					case GET_ADDRESS_ONESCOMPLEMENT:	
										
						if(0xEF == getstr[count])
						{				
							sendtr[sendPos++] = getstr[count];
							GetCmdheader = true;							
						}
						else
						{
							GetCmdheader = false;
							sendPos = 0;
							memset(sendtr, 0, 256);							
						}		
						gState = GET_HEADER;				
						break;
					case GET_CMD:	
						
						sendtr[sendPos++] = getstr[count];
						gState = GET_LENGTH;						
						break;
					case GET_LENGTH:
						
						sendtr[sendPos++] = getstr[count];
						framedatalength = getstr[count];
						PayloadCount = 0;
						printf("[frame data length] = %d\n",framedatalength);
						gState = GET_PAYLOAD;						
						break;
					case GET_PAYLOAD:						
						if(PayloadCount!=framedatalength)
						{
							gState = GET_PAYLOAD;
							sendtr[sendPos++] = getstr[count];	
							PayloadCount++;
							if(PayloadCount==framedatalength)
							{
								gState = GET_CHECKSUM;
								PayloadCount = 0;
								GetCheckSum = 0;
							}						
						}											
						break;
					case GET_CHECKSUM:
						
						for(i=0;i<framedatalength+6;i++) 
							GetCheckSum += sendtr[i];

						if((char)GetCheckSum == getstr[count])
						{
							sendtr[sendPos++] = getstr[count];
							gState = GET_END;								
							printf("[commandcode] = 0x%02x\n",sendtr[4]);
													
						}else {
								sendPos = 0;
								memset(sendtr, 0, 256);
								gState = GET_HEADER;	
								// printf("[GET_CHECKSUM] Wrong,getstr[count]=0x%x\n",getstr[count]);
								// printf("[GET_CHECKSUM] Wrong,CheckSum=0x%x\n",(char)GetCheckSum);
						}						
						break;
					case GET_END:
						
						if(FrameEnd == getstr[count])
						{
							sendtr[sendPos++] = getstr[count];
							isCmdCompleted = true;
                            printf("[frame length] = %d\n",sendPos);
						}
						else
						{
							isCmdCompleted = false;
						}							
						if(isCmdCompleted)
						{
							switch(sendtr[4])
							{
								case FrameNormalCmd:								
									GetNormalCmd = true;
									for (i = 0; i < 15;i++)
									{
										ReceiveDataBuffer.DisplayDatabuffer[i] = sendtr[6 + i]-0x33;						
									}
									Rs485RequestCallBack(NormalAck, sizeof(NormalAck));
									break;
								case FrameGetDeviceInfo:								
									GetDeviceInfoCmd = true;
									Rs485RequestCallBack(VersionAck, sizeof(VersionAck));
									break;	
								case FrameTestCmd:								
									GetTestDeviceCmd = true;
									Rs485RequestCallBack(TestAck, sizeof(TestAck));	
									break;					
								case FrameResetDeviceCmd:								
									GetResetDeviceCmd = true;
									Rs485RequestCallBack(ResetAck, sizeof(ResetAck));					
									break;					
								default:
									Rs485RequestCallBack(ErrorAck, sizeof(ErrorAck));	
									break;
							}	
							isCmdCompleted = false;			
						}										
						sendPos = 0;
						memset(sendtr, 0, 256);
						gState = GET_HEADER;												
				}	
				printf("getstr[%d] = 0x%02x\n",count,getstr[count]);				
				count++;
				printf("gState:%d\n",gState);
				printf("count:%d\n",count);
			}
		}		
	}
}

/******************************END OF FILE*************************************/