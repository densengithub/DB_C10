///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <mcu.c>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2011/08/12
//   @fileversion: CAT9883_V43
//******************************************/
//#include "host/ahb.h"
//#include "pal/pal.h"
//#include "ite/mmp_types.h"
#include "iic/mmp_iic.h"

#include "cat9883_mcu.h"


static uint8_t CAT9883_IICADR = 0x9A >> 1;

uint8_t CAT9883_ReadI2C_Byte(uint8_t RegAddr)
{
    uint8_t p_data;
    bool result;
    
    mmpIicLockModule(IIC_PORT_0); 
    if (0 != (result = mmpIicReceiveData(IIC_PORT_0,IIC_MASTER_MODE, CAT9883_IICADR, &RegAddr,1,&p_data, 1)))
    {   
        mmpIicGenStop(IIC_PORT_0);
        ithPrintf("CAT9883 Read IIC Byte error 0x%x\n", RegAddr);
    }
    mmpIicReleaseModule(IIC_PORT_0);
    
    return p_data;  
}

uint32_t CAT9883_WriteI2C_Byte(uint8_t RegAddr, uint8_t d)
{
    uint32_t result;
 
    mmpIicLockModule(IIC_PORT_0);
    if (0 != (result = mmpIicSendData(IIC_PORT_0,IIC_MASTER_MODE, CAT9883_IICADR, RegAddr, &d, 1)))
    {   
        mmpIicGenStop(IIC_PORT_0);
        ithPrintf("CAT9883 Write IIC Byte Adr = 0x%x Value = 0x%x\n", RegAddr, d);                 
    }               
    mmpIicReleaseModule(IIC_PORT_0);

    return result;  
}

uint32_t CAT9883_WriteI2C_ByteN(uint8_t RegAddr, uint8_t *pData, int N)
{
    uint32_t result; 
    
    mmpIicLockModule(IIC_PORT_0);
    if (0 != (result = mmpIicSendData(IIC_PORT_0,IIC_MASTER_MODE, CAT9883_IICADR, RegAddr, pData, N)))
    {   
        mmpIicGenStop(IIC_PORT_0);
        ithPrintf("CAT9883 Write IIC ByteN 0x%x\n", RegAddr);              
    }   
    mmpIicReleaseModule(IIC_PORT_0);
    
    return result;    
}

uint32_t CAT9883_WriteI2c_ByteMask(uint8_t RegAddr, uint8_t data, uint8_t mask)
{
    uint8_t Value;
    uint32_t flag;

    mmpIicLockModule(IIC_PORT_0);
    
    if (0 != (flag = mmpIicReceiveData(IIC_PORT_0,IIC_MASTER_MODE, CAT9883_IICADR, &RegAddr, 1, &Value, 1)))
    {
        ithPrintf("CAT9883 I2C Read error, reg = %02x\n", RegAddr);
        mmpIicGenStop(IIC_PORT_0);
    }
    
    Value = ((Value & ~mask) | (data & mask));
        
    if (0 != (flag = mmpIicSendData(IIC_PORT_0,IIC_MASTER_MODE, CAT9883_IICADR, RegAddr, &Value, 1)))
    {
        ithPrintf("CAT9883 I2c write error, reg = %02x val =%02x\n", RegAddr, Value);
        mmpIicGenStop(IIC_PORT_0);
    }
    mmpIicReleaseModule(IIC_PORT_0);
    return flag;
}

