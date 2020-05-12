#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/ioctl.h>
#include <pthread.h>
#include <unistd.h>
#include "ite/ith.h"
#include "ite/itp.h"
#include "scene.h"
#include "project.h"

#ifdef _WIN32
#include <crtdbg.h>	
#else
#include "openrtos/FreeRTOS.h"
#include "openrtos/queue.h"
#endif

#define TEST_PORT ITP_DEVICE_UART1
#define TEST_DEVICE itpDeviceUart1
#define TEST_BAUDRATE CFG_UART1_BAUDRATE
#define TEST_GPIO_RX CFG_GPIO_UART1_RX

#define FrameDeviceAddr (0x10)
#define FrameHeader (0x68)
#define FrameAddrField (0xEF)
#define FrameNormalCmd (0x04)
#define FrameGetDeviceInfo ((unsigned char)0x02)
#define FrameTestCmd ((unsigned char)0x05)
#define FrameResetDeviceCmd ((unsigned char)0x10)
#define FrameNormalAckCmd ((unsigned char)0x84)
#define FrameyExceptAckCmd ((unsigned char)0xC4)
#define FrameyDataLength ((unsigned char)0x0F)
#define FrameEnd (0x16)

#define CONTINUS_WRITE ((unsigned char)0x02)
#define CONTINUS_READ ((unsigned char)0x01)

#define RANDOM_WRITE ((unsigned char)0x04)
#define RANDOM_READ ((unsigned char)0x03)

#define TEST_MODE ((unsigned char)0x09)

#define CTRLCMD_OFFSET (4)
#define STARTSECTOR_OFFSET (6)
#define SENDSECTIONSCNT_OFFSET (7)

#define USER_CMD_END_ID (NIU_ID_DB_LOC_MAX-1)
#define USER_CMD_START_ID (NIU_ID_DB_LOC_STATUS)
#define USER_CMD_DISPLAY_ID (NIU_ID_DB_LOC_STATUS)

static uint8_t GetFrameheader = 0;
static uint8_t GetLength = 0;
static bool GetNormalCmd = false;
static bool GetDeviceInfoCmd = false;
static bool GetTestDeviceCmd = false;
static bool GetResetDeviceCmd = false;

static sem_t UartSem;
// sem_t ExternaltaskSem;
// static mqd_t extInQueue = -1;
// static mqd_t extOutQueue = -1;
// static mqd_t extQueue = -1;
// static volatile bool extQuit;
// #define EXT_MAX_QUEUE_SIZE      8

typedef enum GET_UART_COMMAND_STATE_TAG
{
	STATUS_INIT,
	GET_HEADER,
	// GET_ADDRESS_DEVICE,
	// GET_ADDRESS_ONESCOMPLEMENT,
	// GET_CMD_HEADER,
	// GET_CMD,
	GET_LENGTH,
	GET_PAYLOAD,
	GET_CHECKSUM,
	GET_END
} GET_UART_COMMAND_STATE;

static void UartCallback(void *arg1, uint32_t arg2)
{
	//don`t add any codes here.
	sem_post(&UartSem);
}

unsigned char CRC8( unsigned char *ptr, unsigned int len )
{
    unsigned char crc = 0;
    unsigned char i;
    // crc = 0;
    while ( len-- )
    {
        crc ^= *ptr++;
        for ( i = 0; i < 8; i++ )
        {
            if ( crc & 0x80 )
            {
                crc = (crc << 1) ^ 0x07;
            }
            else
            {
                crc <<= 1;
            }
        }
    }
    return crc;
}

void SendModuleVer_Handle(void)
{
	unsigned char dst_str[25] = {0}, length = 14, i = 0;
	dst_str[i++] = '$';
	dst_str[i++] = 'C';
	dst_str[i++] = 'M';
	dst_str[i++] = 'C';	
	dst_str[i++] = length;
	dst_str[i++] = length>>8;	
	dst_str[i++] = 0x04;
	dst_str[i++] = 0x00;
	dst_str[i++] = 0x4E;
	dst_str[i++] = 0x00;
	memcpy(&dst_str[i], local_niu_value_loc.db_loc.data.db_module_version, 8);
	i += 8;
	dst_str[i++] = CRC8(dst_str, length + 4);
	dst_str[i++] = '&';	
	write(ITP_DEVICE_UART1, dst_str, length + 6);
}


static int CreateRespondFrameHandle(uint8_t *dst_str, uint16_t num)
{	
	uint16_t length = 0, i = 0,total = num;
	length = total - 6;
	dst_str[i++] = '$';
	dst_str[i++] = 'C';
	dst_str[i++] = 'M';
	dst_str[i++] = 'C';	
	dst_str[i++] = length;
	dst_str[i++] = length>>8;	
	dst_str[total - 2] = CRC8(dst_str, total - 2);
	dst_str[total - 1] = '&';	
	write(TEST_PORT, dst_str, total);
}

static void CheckReceiveFrameHandle(char *str, int num)
{
	uint8_t i = 0, j = 0;
	ExternalEvent ev;
	uint16_t loc_start_id = 0, start_id = 0, loc_cnt_id = 0, id_cnt = 0, status_temp = 0, cmd = 0;
	uint16_t id_data_len = 0,eqp_data_id = 0;
	uint8_t sendflag = 0;
	uint16_t len = 0,valuelen = 0;
	uint8_t *p_data = NULL;
	uint8_t sectordata_length[100] = {0};
	// uint8_t tempstr[100] = {0};
	uint8_t requeststr[256] = {0};
	cmd = str[6];
	status_temp = 0;
	id_data_len = num - 9;
	switch (cmd)
	{
	case CONTINUS_WRITE:
		start_id = (uint16_t)str[6+1+1]|(((uint16_t)str[6+1+2])<<8);
		id_cnt = str[6+1+3];
		loc_start_id = start_id;
		loc_cnt_id = id_cnt;
		// printf("CONTINUS_WRITE\t\n");
		// printf("start_id = %d,%d,%d\n", start_id,str[6 + 1 + 1],str[6 + 1 + 2]);
		// printf("id_cnt = %d\n", id_cnt);
		len = 1;
		if (loc_cnt_id >= 1)
		{
			if (start_id >= NIU_ID_DB_LOC_STATUS && start_id <= NIU_ID_DB_LOC_LIGHT_ALS)
			{
				loc_start_id += (id_cnt - 1);
				if (loc_start_id > (NIU_ID_DB_LOC_LIGHT_ALS - NIU_ID_DB_LOC_STATUS))
				{
					status_temp = 1;
					loc_cnt_id = (NIU_ID_DB_LOC_LIGHT_ALS - NIU_ID_DB_LOC_STATUS + 1);
				}
			}
			else if (start_id >= NIU_ID_DB_LOC_SERIAL_NUMBER && start_id <= NIU_ID_DB_LOC_99_TIMEOUT)
			{
				loc_start_id += (id_cnt - 1);
				if (loc_start_id > (NIU_ID_DB_LOC_99_TIMEOUT - NIU_ID_DB_LOC_SERIAL_NUMBER))
				{
					status_temp = 1;
					loc_cnt_id = (NIU_ID_DB_LOC_99_TIMEOUT - NIU_ID_DB_LOC_SERIAL_NUMBER + 1);
				}
			}
			else if(start_id >= NIU_ID_DB_LOC_SW_VER && start_id <= NIU_ID_DB_LOC_DB_CMD)
			{
				loc_start_id += (id_cnt - 1);
				if (loc_start_id > (NIU_ID_DB_LOC_DB_CMD - NIU_ID_DB_LOC_SW_VER))
				{
					status_temp = 1;
					loc_cnt_id = (NIU_ID_DB_LOC_DB_CMD - NIU_ID_DB_LOC_SW_VER + 1);
				}
			}
			else
			{
				status_temp = 2;
				sendflag = 1;
			}
		}
		else
		{
			status_temp = 3;
			sendflag = 1;
		}
		if (status_temp == 0 || status_temp == 1)
		{
			len = 0;
			for (i = 0; i <= loc_cnt_id - 1; i++)
			{
				valuelen = niu_data_get_datalen_loc(NIU_DB,start_id + i);
				// printf("id:%d,length:%d,", start_id + i, valuelen);
				niu_data_write_data_loc(NIU_DB, start_id + i, &str[11 + len]);
				// for (j = 0; j < valuelen;j++)
				// {
				// 	printf("0x%02x ", str[11 + len + j]);
				// }
				// printf("\r\n");
				// len += niu_data_get_datalen_loc(NIU_DB, start_id + i);
				len += valuelen;
				
			}

			ev.type = EXTERNAL_UPDATA;
			// ev.arg1 = str[4];
			// ev.arg2 = str[5];
			// ev.arg3 = str[6];
			// ev.arg4 = str[7];
			// printf("ExternalSend(&ev)...");
			if (ExternalSend(&ev) == -1)
			{
				printf("evup fail...\t\n");
			}
			// else
			// {
			// 	printf("evup success...\t\n");
			// }
			// printf("sem_post(&ExternaltaskSem)...\n");
			// sem_post(&ExternaltaskSem);
			// len = 10;
			// requeststr[6] = 0x84;
			// requeststr[7] = 0x00;
			// sendflag = 1;
		}
		// else if (status_temp == 2)
		// {
		// 	len = 10;
		// 	requeststr[6] = cmd|0xC0;
		// 	requeststr[7] = 0x40;
		// 	sendflag = 1;
		// }
		// else if (status_temp == 3)
		// {
		// 	len = 10;
		// 	requeststr[6] = cmd|0xC0;
		// 	requeststr[7] = 0x10;	
		// 	sendflag = 1;		
		// }		
		break;
	case CONTINUS_READ:
		start_id = (uint16_t)str[6 + 1 + 1]|(((uint16_t)str[6 + 1 + 2]) << 8);
		id_cnt = str[6 + 1 + 3];
		loc_start_id = start_id;
		loc_cnt_id = id_cnt;
		printf("CONTINUS_READ\t\n");
		// printf("start_id = %d,%d,%d\n", start_id,str[6 + 1 + 1],str[6 + 1 + 2]);
		// printf("id_cnt = %d\n", id_cnt);
		if (loc_cnt_id >= 1)
		{
			if (start_id >= NIU_ID_DB_LOC_SERIAL_NUMBER && start_id <= NIU_ID_DB_LOC_99_TIMEOUT)
			{
				loc_start_id += (id_cnt - 1);
				if (loc_start_id > (NIU_ID_DB_LOC_99_TIMEOUT - NIU_ID_DB_LOC_SERIAL_NUMBER))
				{
					status_temp = 1;
					loc_cnt_id = (NIU_ID_DB_LOC_99_TIMEOUT - NIU_ID_DB_LOC_SERIAL_NUMBER + 1);
				}
			}
			else if (start_id >= NIU_ID_DB_LOC_SW_VER && start_id <= NIU_ID_DB_LOC_DB_CMD)
			{
				loc_start_id += (id_cnt - 1);
				if (loc_start_id > (NIU_ID_DB_LOC_DB_CMD - NIU_ID_DB_LOC_SW_VER))
				{
					status_temp = 1;
					loc_cnt_id = (NIU_ID_DB_LOC_DB_CMD - NIU_ID_DB_LOC_SW_VER + 1);
				}
			}
			else
			{
				status_temp = 2;
			}
		}
		else
		{
			status_temp = 3;
		}
		sendflag = 1;

		if (status_temp == 0 || status_temp == 1)
		{
			len = 0;
			for (i = 0; i <= loc_cnt_id - 1; i++)
			{
				p_data = (uint8_t *)niu_data_get_pdata_loc(NIU_DB, start_id + i);
				sectordata_length[i] = niu_data_get_datalen_loc(NIU_DB, start_id + i);

				for (j = 0; j < sectordata_length[i]; j++)
				{
					// tempstr[len+j] = p_data[j];
					requeststr[11 + len + j] = p_data[j];
				}
				len += sectordata_length[i];
			}

			len += (4+2+1+4+2);
			// printf("len = %d\n",len);
			requeststr[6] = CONTINUS_WRITE;
			memcpy(&requeststr[7], &str[7], 4);
			
		}
		// else if (status_temp == 2)
		// {
		// 	len = 10;
		// 	requeststr[6] = cmd|0xC0;
		// 	requeststr[7] = 0x40;			
		// }
		// else if (status_temp == 3)
		// {
		// 	len = 10;
		// 	requeststr[6] = cmd|0xC0;
		// 	requeststr[7] = 0x10;			
		// }		
		break;

	case RANDOM_WRITE:
		printf("RANDOM_WRITE\t\n");
		j = 0;
		i = j;
		while ( j < id_data_len)
		{
			eqp_data_id = (uint16_t)str[6 + 1 + 1 + j] | ((uint16_t)str[6 + 1 + 2 + j] << 8);
			// printf("eqp_data_id=%d!\n", eqp_data_id);
			if (eqp_data_id <= NIU_ID_DB_LOC_LIGHT_ALS ||	\
				eqp_data_id == NIU_ID_DB_LOC_SERIAL_NUMBER ||			\
				eqp_data_id == NIU_ID_DB_LOC_99_TIMEOUT ||				\
				(eqp_data_id >= NIU_ID_DB_LOC_SW_VER && eqp_data_id <= NIU_ID_DB_LOC_DB_CMD))
			{				
				valuelen = niu_data_get_datalen_loc(NIU_DB,eqp_data_id);
				
				j += valuelen;
				j += 3;
				// for (i = 0; i < valuelen;i++)
				// {
				// 	printf("eqp_data=%d!\n", str[6 + 1 + j-valuelen+i]);
				// }
				if (j > id_data_len)
				{
					// niu_data_write_data_loc(NIU_DB,eqp_data_id, &str[6 + 1 + 3 + j]);
					status_temp = 2;
					sendflag = 1;
					break;
				}
				else
				{
					niu_data_write_data_loc(NIU_DB,eqp_data_id, &str[6 + 1 + j-valuelen]);					
				}	
			}
			else
			{
				status_temp = 3;
				sendflag = 1;
				break;
			}
		}	
		if(status_temp==0)
		{
			ev.type = EXTERNAL_UPDATA;
			// ev.arg1 = str[4];
			// ev.arg2 = str[5];
			// ev.arg3 = str[6];
			// ev.arg4 = str[7];
			// printf("ExternalSend(&ev)...");
			if (ExternalSend(&ev) == -1)
			{
				printf("evup fail...\t\n");
			}
			// else
			// {
			// 	printf("evup success...\t\n");
			// }
			// printf("sem_post(&ExternaltaskSem)...\n");
			// sem_post(&ExternaltaskSem);
		}
		
		break;
	case RANDOM_READ:
		printf("RANDOM_READ\t\n");
		status_temp = 0;
		j = 0;
		i = 0;
		while ( j < id_data_len)
		{
			eqp_data_id = (uint16_t)str[6 + 1 + 1 + j] + ((uint16_t)str[6 + 1 + 2 + j] << 8);
			if ((eqp_data_id >= NIU_ID_DB_LOC_SERIAL_NUMBER && eqp_data_id <= NIU_ID_DB_LOC_99_TIMEOUT) ||					
				(eqp_data_id >= NIU_ID_DB_LOC_SW_VER && eqp_data_id <= NIU_ID_DB_LOC_DB_CMD))
			{				
				memcpy(&requeststr[6 + 1 + i], &str[6 + 1 + j], 3);
				i += 3;
				valuelen = niu_data_get_datalen_loc(NIU_DB,eqp_data_id);
				niu_data_read_value_loc(NIU_DB,eqp_data_id, &requeststr[6 + 1 + i], valuelen);
				i += valuelen;
				j += 3;
				// id_count++;
			}
			else
			{
				status_temp = 3;
				sendflag = 1;
				break;
			}			
		}
		if(status_temp == 0)
		{
			len = i+(4+2+1+2);
			requeststr[6] = RANDOM_WRITE;
			sendflag = 1;
		}
		break;

	// case 0x81:
	// 	break;
	// case 0x82:
	// 	break;
	// case 0x83:
	// 	break;
	// case 0x84:
	// 	break;
	// case 0xC1:
	// 	break;
	// case 0xC2:
	// 	break;
	// case 0xC3:
	// 	break;
	// case 0xC4:
	// 	break;

	case TEST_MODE:
		// printf("TEST_MODE\t\n");
		valuelen = ((uint16_t)str[5] << 8) + ((uint16_t)str[4]);
		// printf("valuelen = 0x%04X...\t\n", valuelen);
		// for (i = 0; i < num; i++)
		// {
		// 	printf("str[%d] = 0x%02X...\t\n", i, str[i]);
		// }
		if (valuelen == (9 + 2 + 3))
		{
			status_temp = 0;
			j = 0;
			i = 0;
			ev.type = EXTERNAL_TESTMODE;
			// ev.arg1 = str[4];
			// ev.arg2 = str[5];
			// ev.arg3 = str[6];
			// ev.arg4 = str[7];
			// printf("ExternalSend(&ev)...");
			memcpy(ev.buf1, &str[7], 11);
			if (ExternalSend(&ev) == -1)
			{
				printf("evdemo fail...\t\n");
			}
			// else
			// {
			// 	printf("evdemo success...\t\n");
			// }
			// printf("sem_post(&ExternaltaskSem)...\n");
			// sem_post(&ExternaltaskSem);
		}
		
		
		

		sendflag = 1;
		if(status_temp == 0)
		{
			len = 10;
			requeststr[6] = TEST_MODE;
			// sendflag = 1;
		}
		break;
	default:
		status_temp = 4;
		sendflag = 1;
		// len = 10;
		// requeststr[6] = cmd|0xC0;		
		// requeststr[7] = 0x20;
		// sendflag = 1;
		break;
	}

	if(sendflag == 1)
	{		
		if (status_temp == 2)
		{
			len = 10;
			requeststr[6] = cmd|0xC0;
			requeststr[7] = 0x40;			
		}
		else if (status_temp == 3)
		{
			len = 10;
			requeststr[6] = cmd|0xC0;
			requeststr[7] = 0x10;			
		}
		else if(status_temp == 4)
		{
			len = 10;
			requeststr[6] = cmd|0xC0;
			requeststr[7] = 0x20;
		}		
		CreateRespondFrameHandle(requeststr, len);
		sendflag = 0;
	}
}

void *UartttlComunicationTask(void *arg)
{
	// uint8_t sendtr1[8] = {0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
	uint8_t *p_sectordata = NULL;
	uint8_t sectordata_length = 0;
	char getstr[512];
	char sendtr[256];		
	unsigned short framedatalength = 0;
	int len = 0, count = 0, sendPos = 0, countx = 0;
	int i = 0, PayloadCount = 0, GetCheckSum = 0;
	bool isCmdCompleted = false;

	GET_UART_COMMAND_STATE gState = GET_HEADER;

	printf("Run UartttlComunicationTask!\n");

	#if defined(CFG_UART_INTR) 
		sem_init(&UartSem, 0, 0);	
		itpRegisterDevice(TEST_PORT, &TEST_DEVICE);
		printf("Start uart INT!\t\n");
		ioctl(TEST_PORT, ITP_IOCTL_INIT, NULL);
		ioctl(TEST_PORT, ITP_IOCTL_RESET, TEST_BAUDRATE);

		ioctl(TEST_PORT, ITP_IOCTL_REG_UART_CB, (void *)UartCallback);
		//ioctl(TEST_PORT, ITP_IOCTL_REG_UART_DEFER_CB , (void*)UartCallback);
	#elif defined(CFG_UART_DMA)
		printf("Start uart DMA!\t\n");	
		// ioctl(TEST_PORT, ITP_IOCTL_RESET, TEST_BAUDRATE);	
	#endif

	memset(getstr, 0, 512);
	memset(sendtr, 0, 256);
	GetFrameheader = 0;	

	while (1)
	{
		#if	defined(CFG_UART_INTR)
			sem_wait(&UartSem);
		#endif

		len = read(TEST_PORT, getstr, 512);
		// write(TEST_PORT, getstr, len);
		// printf("Get New data %d bytes!\n", len);
		count = 0;
		if (len)
		{
			while (len--)
			{
				switch (gState)
				{
				case GET_HEADER:
					// printf("GET_HEADER:%d\n", gState);
					framedatalength = 0;					
					// printf("getstr[%d] = 0x%x\n",count,getstr[count]);
					if(GetFrameheader == 0)
					{
						if('$' == getstr[count])
						{
							GetFrameheader = 1;
							sendtr[sendPos++] = getstr[count];
						}
						else
						{
							gState = STATUS_INIT;
						}
					}
					else if(GetFrameheader == 1)
					{
						if('C' == getstr[count])
						{
							GetFrameheader = 2;
							sendtr[sendPos++] = getstr[count];
						}
						else
						{
							gState = STATUS_INIT;
						}	
					}
					else if(GetFrameheader == 2)
					{
						if('M' == getstr[count])
						{
							GetFrameheader = 3;
							sendtr[sendPos++] = getstr[count];
						}
						else
						{
							gState = STATUS_INIT;
						}	
					}
					else if(GetFrameheader == 3)
					{
						if('C' == getstr[count])
						{
							GetFrameheader = 0;
							sendtr[sendPos++] = getstr[count];
							gState = GET_LENGTH;
						}
						else
						{
							gState = STATUS_INIT;
						}	
					}					
					break;				
				case GET_LENGTH:
					// printf("GET_LENGTH:%d\n", gState);
					sendtr[sendPos++] = getstr[count];
					GetLength++;
					if(GetLength==1)
					{
						framedatalength = getstr[count];						
					}
					else if(GetLength==2)
					{
						framedatalength |= ((unsigned short)getstr[count]<<8);
						gState = GET_PAYLOAD;
						GetLength = 0;
						PayloadCount = 0;
						// printf("[frame data length] = %d\n", framedatalength);
					}					
					// PayloadCount = 0;
					// printf("[frame data length] = %d\n", framedatalength);					
					break;
				case GET_PAYLOAD:
					// printf("GET_PAYLOAD:%d\n", gState);
					if (PayloadCount != framedatalength)
					{
						gState = GET_PAYLOAD;
						sendtr[sendPos++] = getstr[count];
						PayloadCount++;
						if (PayloadCount == framedatalength-2)
						{
							gState = GET_CHECKSUM;
							PayloadCount = 0;
							GetCheckSum = 0;
						}
					}
					break;
				case GET_CHECKSUM:
					// printf("GET_CHECKSUM:%d\n", gState);
					// for (i = 0; i < framedatalength + 6; i++)
					// 	GetCheckSum += sendtr[i];

					if (CRC8(sendtr,framedatalength+6-2) == getstr[count])
					{
						sendtr[sendPos++] = getstr[count];
						gState = GET_END;
						// printf("sucecss!\n");
						// printf("getstr[%d] = 0x%02x...crc8=0x%02x\n", count,getstr[count],CRC8(sendtr,framedatalength+6-2));
					}
					else
					{						
						// printf("fail!\n");
						// printf("getstr[%d] = 0x%02x...crc8=0x%02x\n", count,getstr[count],CRC8(sendtr,framedatalength+6-2));
						// for (i = 0; i < framedatalength + 6; i++)
						// printf("sendtr[%d] = 0x%02x\n", i,sendtr[i]);					

						gState = STATUS_INIT;
					}
					// GetCheckSum = 0;
					break;
				case GET_END:
					// printf("GET_END:%d\n", gState);
					isCmdCompleted = false;
					if ('&' == getstr[count])
					{
						sendtr[sendPos++] = getstr[count];
						isCmdCompleted = true;
						// printf("[frame length] = %d\n", sendPos);
					}
					if (isCmdCompleted)
					{
						// printf("run CheckReceiveFrameHandle!........\n");
						// for (i = 0; i < framedatalength + 6; i++)
						// 	printf("sendtr[%d] = 0x%02x\n", i, sendtr[i]);
						CheckReceiveFrameHandle(sendtr, sendPos);
						isCmdCompleted = false;
					}
			
					gState = STATUS_INIT;
					break;
				default:
					// gState = STATUS_INIT;
					break;
				}
				if (gState == STATUS_INIT)
				{
					sendPos = 0;
					memset(sendtr, 0, 256);
					GetFrameheader = 0;
					framedatalength = 0;
					GetLength = 0;
					gState = GET_HEADER;
				}
				// printf("getstr[%d] = 0x%02x\n", count, getstr[count]);
				count++;
				// printf("gState:%d\n", gState);
				// printf("count:%d\n", count);
			}
		}
		usleep(10*1000);
	}
}

int CreateAppComunicationTask(void)
{

#ifdef _WIN32

	return 0;
#else

	int res = 0;
	pthread_t task;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	// attr.stacksize = TEST_STACK_SIZE;
	/* 	struct mq_attr qattr;   
	// sem_init(&ExternaltaskSem, 0, 0);
    qattr.mq_flags = 0;
    qattr.mq_maxmsg = EXT_MAX_QUEUE_SIZE;
    qattr.mq_msgsize = sizeof(ExternalEvent);

    extInQueue = mq_open("external_in", O_CREAT | O_NONBLOCK, 0644, &qattr);
    // assert(extInQueue != -1);
	if(extInQueue == -1)
	{
		printf("Create external_in fail!.......\n");
	}
	else
	{
		printf("Create external_in success!.......\n");
	}
	

    extOutQueue = mq_open("external_out", O_CREAT | O_NONBLOCK, 0644, &qattr);
    // assert(extOutQueue != -1); */

	printf("Create UartttlComunicationTask!.......\n");
	res = pthread_create(&task, &attr, UartttlComunicationTask, NULL);
	if (res)
	{
		printf("[TouchPanel]%s() L#%ld: ERROR, create UartttlComunicationTask() thread fail! res=%ld\n", res);
		return 1;
	}
	// printf("Creat UartttlComunicationTask  finished!.......\n");

	return 0;

#endif
}
