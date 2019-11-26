#include <sys/ioctl.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ite/itp.h"
#include "iic/mmp_iic.h"
#include <pthread.h>

#ifdef _WIN32
#include <crtdbg.h>
#else
#include "openrtos/FreeRTOS.h"
#include "openrtos/task.h"
#endif

#include "project.h"


#define TEST_STACK_SIZE 1024000
#define IIC_BUFFER_LEN	1024 * 1024

static uint8_t	IicMasterWriteBuffer[IIC_BUFFER_LEN] = {0};
static uint8_t	IicSlaveReceviceBuffer[IIC_BUFFER_LEN] = {0};
static uint8_t	IicMasterWriteBuffer1[IIC_BUFFER_LEN] = {0};
static uint8_t	IicSlaveReceviceBuffer1[IIC_BUFFER_LEN] = {0};
static uint8_t	buf[256] = {0};
static	int       gMasterDev = 0;
static	int       gMasterDev1 = 0;
static	int       gSlaveDev = 0;

static void _TestMasterReadSlave_IicReceiveCallback(uint8_t* recvBuffer, uint32_t recvBufferSize)
{
    uint32_t i = 0;
	for(i = 0; i < recvBufferSize; i++)
	{
		printf("IIC Slave receive write cmd(0x%02X)\n", recvBuffer[i]);
		if(recvBuffer[i] != IicMasterWriteBuffer[i])
		{
			printf("IIC slave receive cmd compare error!!!!!!!!\n");
			while(1);
		}
	}
}

static int _TestMasterReadSlave_IicWriteCallback(IIC_DEVICE dev)
{
	static uint32_t ii = 0;
	
	//uint8_t		buf[256] = {0};
	uint32_t	i = 0;
	bool		result = false;

	printf("IIC Slave output...\n");

	for(i = 0; i < 256; i++)
	{
		buf[i] = 'a' + i + ii;
		//printf("0x%02x,", buf[i]);
	}
	printf("\n");
	result = mmpIicSlaveFiFoWrite(IIC_PORT_3, buf, 256);
	assert(result == true);
	ii++;
}

void TestMasterReadSlave()
{
	bool iicResult = false;
	ITPI2cInfo evt;
    uint8_t		cmd = 0xFE;
	uint8_t		recvBuffer[256] = {0};
	uint32_t	i;

	iicResult = mmpIicSetSlaveModeCallback(IIC_PORT_3, 0x77, _TestMasterReadSlave_IicReceiveCallback, _TestMasterReadSlave_IicWriteCallback);
	mmpIicSetSlaveMode(IIC_PORT_3, 0x77);
	assert(iicResult == true);

    IicMasterWriteBuffer[0] = 0xAD;
    IicMasterWriteBuffer[1] = 0xB8;
	IicMasterWriteBuffer[2] = 0xB7;
	IicMasterWriteBuffer[3] = 0xB6;
	IicMasterWriteBuffer[4] = 0xB5;
    
    evt.slaveAddress   = 0x77;
	evt.cmdBuffer      = IicMasterWriteBuffer;
	evt.cmdBufferSize  = 5;
	evt.dataBuffer     = recvBuffer;
	evt.dataBufferSize = 256;
        
	while(1)
	{
		uint32_t	result = 0;

        result = write(gMasterDev, &evt, 1);
		usleep(100000);
	    
	    result = read(gMasterDev, &evt, 1);
		//result = mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, 0x77, &cmd, 1, recvBuffer, 256);

		if(result != 0)
		{
			printf("master read slave failed!!!\n");
			break;
		}
		printf("Master received:\n");
		for (i = 0; i < 256; i++)
		{
			printf("0x%02X ", recvBuffer[i]);
			if(buf[i] != recvBuffer[i])
			{
				printf("data compare error(0x%x!=0x%x)!!!\n", buf[i], recvBuffer[i]);
				while(1);
			}
			else if(i == 255)
				printf("IIC master read slave data compare ok, test success\n");
		}
		//break;
	}
}

void TestPsensor(void)
{
	bool iicResult = false;
	uint32_t	result = 0;	
	ITPI2cInfo evt;    
	uint8_t		recvBuffer[256] = {0};
	uint32_t	i;

	PSensorInit();
	IicMasterWriteBuffer[0] = SC7LC30_REG_PID; 	   
    evt.slaveAddress   = SC7LC30_SLAVEADDR;
	evt.cmdBuffer      = IicMasterWriteBuffer1;
	evt.cmdBufferSize  = 1;
	evt.dataBuffer     = recvBuffer;
	evt.dataBufferSize = 256;
	//init 	
	// result = write(gMasterDev, &evt, 1);	
	// if(result != 0)
	// {
	// 	printf("master read slave failed!!!\n");			
	// }
	// else
	// {
	// 	printf("###[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);	
		
	// }		
	// IicMasterWriteBuffer[0] = SC7LC30_REG_STATUS; 
	// result = read(gMasterDev, &evt, 1);	
	// if(result != 0)
	// {
	// 	printf("master read slave failed!!!\n");			
	// }
	// else
	// {
	// 	printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);				
	// }		
	IicMasterWriteBuffer1[0] = SC7LC30_REG_PID; 
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}
	IicMasterWriteBuffer1[0] = SC7LC30_REG_STATE; 
	result = read(gMasterDev1, &evt, 1);	

	IicMasterWriteBuffer1[0] = SC7LC30_REG_ITIME;
	IicMasterWriteBuffer1[1] = 0x08;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);	

	IicMasterWriteBuffer1[0] = SC7LC30_REG_GAINALS;
	IicMasterWriteBuffer1[1] = 0x3b;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);

	IicMasterWriteBuffer1[0] = SC7LC30_REG_WTIME;
	IicMasterWriteBuffer1[1] = 0x28;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);

	IicMasterWriteBuffer1[0] = SC7LC30_REG_STATE;
	IicMasterWriteBuffer1[1] = 0x13;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);

	evt.cmdBufferSize  = 1;	

	while(1)
	{	
		// IicMasterWriteBuffer1[0] = SC7LC30_REG_PID; 
		// result = read(gMasterDev1, &evt, 1);	
		// if(result != 0)
		// {
		// 	printf("master read slave failed!!!\n");			
		// }
		// else
		// {
		// 	printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
		// }
		// IicMasterWriteBuffer1[0] = SC7LC30_REG_STATE;
		// result = read(gMasterDev1, &evt, 1);	
		// if(result != 0)
		// {
		// 	printf("master read slave failed!!!\n");			
		// }
		// else
		// {
		// 	printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);				
		// }	
		
		IicMasterWriteBuffer1[0] = SC7LC30_REG_STATUS_ADC;
		result = read(gMasterDev1, &evt, 1);	
		if(result != 0)
		{
			printf("master read slave failed!!!\n");			
		}
		else
		{
			// printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);				
		}	
		if(result == 0&&(recvBuffer[0]&0x20))
		{
			IicMasterWriteBuffer1[0] = SC7LC30_REG_STATUS; 
			result = read(gMasterDev1, &evt, 1);	
			if(result != 0)
			{
				printf("master read slave failed!!!\n");			
			}
			else
			{
				printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);				
			}	
			IicMasterWriteBuffer1[0] = SC7LC30_REG_DATAH_ALS;
			result = read(gMasterDev1, &evt, 1);		
			//result = mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, 0x77, &cmd, 1, recvBuffer, 256);
			if(result != 0)
			{
				printf("master read slave failed!!!\n");			
			}
			else
			{
				printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);				
			}		
			IicMasterWriteBuffer1[0] = SC7LC30_REG_DATAL_ALS; 
			result = read(gMasterDev1, &evt, 1);

			if(result != 0)
			{
				printf("master read slave failed!!!\n");			
			}
			else
			{
				printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);				
			}
			// IicMasterWriteBuffer[0] = SC7LC30_REG_STATUS_ADC;
			// result = read(gMasterDev1, &evt, 1);	
			// if(result != 0)
			// {
			// 	printf("master read slave failed!!!\n");			
			// }
			// else
			// {
			// 	printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);				
			// }				
		}
		
		usleep(5000000);	
	}

}

	#define AP3426_SLAVEADDR 			(0x1E)
	#define AP3426_REG_POWER			(0x00)
	#define AP3426_REG_INTSTATUS	 	(0x01)
	#define AP3426_REG_INTMANNER	 	(0x02)
	#define AP3426_REG_ALSDATAL	 		(0x0C)
	#define AP3426_REG_ALSDATAH	 		(0x0D)
	#define AP3426_REG_PSDATAL	 		(0x0E)
	#define AP3426_REG_PSDATAH	 		(0x0F)
	#define AP3426_REG_PSDRIVER			(0x21)

void TestNewPsensor(void)
{
	bool iicResult = false;
	uint32_t	result = 0;	
	ITPI2cInfo evt;    
	uint8_t		recvBuffer[256] = {0};
	uint32_t	i;

	PSensorInit();
	IicMasterWriteBuffer1[0] = AP3426_REG_POWER; 	   
    evt.slaveAddress   = AP3426_SLAVEADDR;
	evt.cmdBuffer      = IicMasterWriteBuffer1;
	evt.cmdBufferSize  = 1;
	evt.dataBuffer     = recvBuffer;
	evt.dataBufferSize = 256;
	#if 0
	IicMasterWriteBuffer1[0] = 0x00; 
	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}
	IicMasterWriteBuffer1[0] = 0x21; 
	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}
	IicMasterWriteBuffer1[0] = 0x02; 
	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}
	#endif
	IicMasterWriteBuffer1[0] = 0x00;
	IicMasterWriteBuffer1[1] = 0x02;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	
	#if 0
	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x06;
	IicMasterWriteBuffer1[1] = 0x00;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x20;
	IicMasterWriteBuffer1[1] = 0x0C;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x21;
	IicMasterWriteBuffer1[1] = 0x03;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x23;
	IicMasterWriteBuffer1[1] = 0x03;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x25;
	IicMasterWriteBuffer1[1] = 0x3f;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x2a;
	IicMasterWriteBuffer1[1] = 0x26;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x2b;
	IicMasterWriteBuffer1[1] = 0x03;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x2c;
	IicMasterWriteBuffer1[1] = 0xa3;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}

	IicMasterWriteBuffer1[0] = 0x2d;
	IicMasterWriteBuffer1[1] = 0x01;
	evt.cmdBufferSize  = 2;
	result = write(gMasterDev1, &evt, 1);
	if(result != 0)
	{
		printf("master write slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[w][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],IicMasterWriteBuffer1[1]);			
	}	

	evt.dataBufferSize = 1;
	result = read(gMasterDev1, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#AP3426#[r][0x%02X]:0x%02X \n", IicMasterWriteBuffer1[0],recvBuffer[0]);			
	}
	#endif
	printf("init AP3426 ok \r\n");	

	evt.cmdBufferSize  = 1;	
	evt.dataBufferSize = 1;
	while(1)
	{		
		usleep(1000000);
		IicMasterWriteBuffer1[0] = 0x0e;
		result = read(gMasterDev1, &evt, 1);
		//result = mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, 0x77, &cmd, 1, recvBuffer, 256);
		if (result != 0)
		{
			printf("master read slave failed!!!\n");
		}
		else
		{
			printf("#AP3426#[0x%02X]:0x%02X%02X\n", IicMasterWriteBuffer1[0], recvBuffer[1],recvBuffer[0]);
			printf("#AP3426#PX is: %d   \r\n",recvBuffer[1]*256+recvBuffer[0] );	
		}		
		IicMasterWriteBuffer1[0] = 0x0f;
		result = read(gMasterDev1, &evt, 1);
		//result = mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, 0x77, &cmd, 1, recvBuffer, 256);
		if (result != 0)
		{
			printf("master read slave failed!!!\n");
		}
		else
		{
			printf("#AP3426#[0x%02X]:0x%02X%02X\n", IicMasterWriteBuffer1[0], recvBuffer[1],recvBuffer[0]);
			printf("#AP3426#PX is: %d   \r\n",recvBuffer[1]*256+recvBuffer[0] );	
		}		
		// usleep(800000000);	
		usleep(5000000);
	}

}

void TestGyrosensor(void)
{
	bool iicResult = false;
	uint32_t	result = 0;	
	ITPI2cInfo evt;    
	uint8_t		recvBuffer[256] = {0};
	uint32_t	i;

	PSensorInit();
	IicMasterWriteBuffer[0] = 0x00; 	   
    evt.slaveAddress   = 0x69;
	evt.cmdBuffer      = IicMasterWriteBuffer;
	evt.cmdBufferSize  = 1;
	evt.dataBuffer     = recvBuffer;
	evt.dataBufferSize = 256;

	IicMasterWriteBuffer[0] = 0x00; 
	result = read(gMasterDev, &evt, 1);	
	if(result != 0)
	{
		printf("master read slave failed!!!\n");			
	}
	else
	{
		printf("#BMI160#[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);			
	}
	IicMasterWriteBuffer[0] = 0x1B; 
	result = read(gMasterDev, &evt, 1);	
	evt.cmdBufferSize  = 1;	

	while(1)
	{	
		// IicMasterWriteBuffer[0] = SC7LC30_REG_PID; 
		// result = read(gMasterDev, &evt, 1);	
		// if(result != 0)
		// {
		// 	printf("master read slave failed!!!\n");			
		// }
		// else
		// {
		// 	printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);			
		// }
		// IicMasterWriteBuffer[0] = SC7LC30_REG_STATE;
		// result = read(gMasterDev, &evt, 1);	
		// if(result != 0)
		// {
		// 	printf("master read slave failed!!!\n");			
		// }
		// else
		// {
		// 	printf("#SC7LC30#[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);				
		// }	
		
		IicMasterWriteBuffer[0] = 0x1B; 
		result = read(gMasterDev, &evt, 1);		
		if(result != 0)
		{
			printf("master read slave failed!!!\n");			
		}
		else
		{
			printf("#BMI160#[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);				
		}	
		IicMasterWriteBuffer[0] = 0x00; 
		result = read(gMasterDev, &evt, 1);
		if(result != 0)
		{
			printf("master read slave failed!!!\n");			
		}
		else
		{
			printf("#BMI160#[0x%02X]:0x%02X \n", IicMasterWriteBuffer[0],recvBuffer[0]);				
		}
		usleep(5000000);	
	}
}




void* Test_IIC_Func(void* arg)
{   
    IIC_OP_MODE iic_port2_mode = IIC_MASTER_MODE;

 	// printf("Init Test_IIC_ComunicationTask!.......\n");	
	printf("Init TestGyrosensor!.......\n");	
    // itpRegisterDevice(ITP_DEVICE_I2C1, &itpDeviceI2c1);
	//gMasterDev = open(":i2c1", 0);
	gMasterDev = open(":i2c2", 0);
    // ioctl(ITP_DEVICE_I2C1, ITP_IOCTL_INIT, (void*)iic_port2_mode);
	//printf("run Test_IIC_ComunicationTask!.......\n");
	printf("run TestGyrosensor!.......\n");	
	// TestMasterReadSlave();
	//TestPsensor();
	TestGyrosensor();
	/*
	while(1)
	{
		printf("execute Test_IIC_ComunicationTask!.......\n");	
		usleep(500000);

	}
	*/

	return NULL;
}

void* Test_IIC_Func1(void* arg)
{   
    IIC_OP_MODE iic_port2_mode = IIC_MASTER_MODE;

 	// printf("Init Test_IIC_ComunicationTask!.......\n");	
	printf("Init TestPsensor!.......\n");	
    // itpRegisterDevice(ITP_DEVICE_I2C1, &itpDeviceI2c1);
	gMasterDev1 = open(":i2c1", 0);
	//gMasterDev = open(":i2c2", 0);
    // ioctl(ITP_DEVICE_I2C1, ITP_IOCTL_INIT, (void*)iic_port2_mode);
	// printf("run Test_IIC_ComunicationTask!.......\n");	
	printf("run TestPsensor!.......\n");	
	// TestMasterReadSlave();
	
	// TestPsensor();
	TestNewPsensor();
	//TestGyrosensor();
	/*
	while(1)
	{
		printf("execute Test_IIC_ComunicationTask!.......\n");	
		usleep(500000);

	}
	*/

	return NULL;
}



int	Test_IIC_ComunicationTask(void)
{
#ifdef _WIN32

	return 0;

#else

	int res = 0, res1 = 0;
	int len = 0;
	unsigned char str[10] = {'s', '_', 't', '_', 'r', 0};	
	pthread_t task,task1;
    pthread_attr_t attr,attr1;    
    pthread_attr_init(&attr);
	// attr.schedparam.sched_priority = 2;
    // attr.stacksize = 10240;
	attr.stacksize = TEST_STACK_SIZE;
	printf("Creat Test_IIC_ComunicationTask!.......\n");
	// printf("Init Test_IIC_ComunicationTask!.......\n");	
   	// res = pthread_create(&task, &attr, Test_IIC_Func, NULL);
	// if(res)
	// {
	// 	printf( "[TouchPanel]%s() L#%ld: ERROR, create Test_IIC_ComunicationTask() thread fail! res=%ld\n", res );
	// 	return 1;
	// }	
	// printf("Creat Test_IIC_ComunicationTask  finished!.......\n");


	pthread_attr_init(&attr1);
	// attr.schedparam.sched_priority = 2;
    // attr.stacksize = 10240;
	attr1.stacksize = TEST_STACK_SIZE;
	printf("Creat Test_IIC_ComunicationTask!.......\n");
	
	res1 = pthread_create(&task1, &attr1, Test_IIC_Func1, NULL);
	if(res1)
	{
		printf( "[TouchPanel]%s() L#%ld: ERROR, create Test_IIC_ComunicationTask() thread fail! res=%ld\n", res );
		return 1;
	}	
	printf("Creat Test_IIC_ComunicationTask  finished!.......\n");

    return 0;
#endif
}