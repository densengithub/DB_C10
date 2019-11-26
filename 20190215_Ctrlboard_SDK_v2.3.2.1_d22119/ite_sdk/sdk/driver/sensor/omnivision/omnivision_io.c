#include "ite/ith.h"
#include "ite/mmp_types.h"
#include "iic/iic.h"
#include "iic/mmp_iic.h"
#include "sensor/omnivision_io.h"
#include "sensor/sensor_gpio_config.h"
//=============================================================================
//                Constant Definition
//=============================================================================

//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================
static uint8_t OMNIVISION_IICADDR = 0x42 >> 1;

//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================
uint8_t
Omnivision_ReadI2C_Byte(
    uint16_t RegAddr)
{
    int         result;
    uint8_t     dbuf[256];
    uint8_t     *pdbuf = dbuf;
    uint8_t     value;
	char        portname[4];
	ITPI2cInfo  evt;
	int         gMasterDev = 0;

	sprintf(portname, ":i2c%d", SN1_IIC_PORT);	
	gMasterDev = open(portname, 0);
	
    *pdbuf++ = (uint8_t)(RegAddr & 0x00ff);

	evt.slaveAddress   = OMNIVISION_IICADDR;    //對接裝置salve address
	evt.cmdBuffer	   = dbuf;				   //欲傳送給slave裝置的data buffer(包含cmooand)
	evt.cmdBufferSize	= 1;					//傳送data size,單位為byte
	evt.dataBuffer	   = pdbuf; 			   //欲接收slave裝置的data buffer
	evt.dataBufferSize	= 1;					//接收data size,單位為byte

	result = read(gMasterDev, &evt, 1);
	
    if (result != 0)
    {
        printf("OMNIVISION_ReadI2C_8Bit read address 0x%02x error!\n", RegAddr);
        return;
    }
    value = (dbuf[1] & 0xFF);

    return value;
}

int
Omnivision_WriteI2C_Byte(
    uint16_t    RegAddr,
    uint8_t     data)
{
    int         result;
    uint8_t     dbuf[256];
    uint8_t     *pdbuf = dbuf;
	char        portname[4];
	ITPI2cInfo  evt;
	int         gMasterDev = 0;

	sprintf(portname, ":i2c%d", SN1_IIC_PORT);
	gMasterDev = open(portname, 0);
	
    *pdbuf++    = (uint8_t)(RegAddr & 0x00ff);
    *pdbuf      = (uint8_t)(data);

	evt.slaveAddress   = OMNIVISION_IICADDR;		//對接裝置salve address
	evt.cmdBuffer	   = dbuf;				   //欲傳送給slave裝置的data buffer(包含cmooand)
	evt.cmdBufferSize  = 2; 				   //傳送data size,單位為byte
	
    if(0 != (result = write(gMasterDev, &evt, 1)))
    {
        printf("OMNIVISION_WriteI2C_8Bit I2c Write Error, reg=%02x val=%02x\n", RegAddr, data);
    }

    return result;
}