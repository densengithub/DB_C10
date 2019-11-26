///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <iT6607.c>
//   @author jau-chih.tseng@ite.com.tw
//   @date   2012/05/16
//   @fileversion: HDMIRX_SAMPLE_3.11
//******************************************/

#include "hdmirx/it6607/typedef.h"
#include "hdmirx/it6607/it6607.h"
#include "hdmirx/it6607/io.h"
#include "iic/mmp_iic.h"
#define GPIO_BASE (0xDE000000)
static BYTE   IT6607_IICADDR = 0x90 >> 1;

#define INTERNAL_IIC_MODE 0x3
#define EXTERNAL_IIC_MODE 0x1

BYTE HDMIRX_ReadI2C_Byte(BYTE RegAddr)
{
    BYTE d;
    BOOL flag;
    mmpIicLockModule(IIC_PORT_0);
    
    //IIC Mode internal IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (INTERNAL_IIC_MODE << 28), (0x3 << 28));
    if (0 != (flag = mmpIicReceiveData(IIC_PORT_0,IIC_MASTER_MODE, IT6607_IICADDR, &RegAddr, 1, &d, 1)))
    {
        printf("HDMIRX I2C Read error, reg = %02x\n", RegAddr);
        mmpIicGenStop(IIC_PORT_0);
    }
    //IIC Mode external IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (EXTERNAL_IIC_MODE << 28), (0x3 << 28));
        
    mmpIicReleaseModule(IIC_PORT_0);
    return d;
}


SYS_STATUS HDMIRX_WriteI2C_Byte(BYTE RegAddr, BYTE d)
{
    BOOL flag;

    if (gtTriStateEnable && RegAddr == REG_RX_TRISTATE_CTRL)
        return 0;
            
    mmpIicLockModule(IIC_PORT_0);

    //IIC Mode internal IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (INTERNAL_IIC_MODE << 28), (0x3 << 28));    
    if (0 != (flag = mmpIicSendData(IIC_PORT_0, IIC_MASTER_MODE, IT6607_IICADDR, RegAddr, &d, 1)))
    {
        printf("HDMIRX I2c write error, reg = %02x val =%02x\n", RegAddr, d);
        mmpIicGenStop(IIC_PORT_0);
    }
    
    //IIC Mode external IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (EXTERNAL_IIC_MODE << 28), (0x3 << 28));
        
    mmpIicReleaseModule(IIC_PORT_0);
    return flag;

}


SYS_STATUS HDMIRX_ReadI2C_ByteN(BYTE RegAddr,BYTE *pData,int N)
{
    BOOL flag; 
    
    mmpIicLockModule(IIC_PORT_0); 
    
    //IIC Mode internal IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (INTERNAL_IIC_MODE << 28), (0x3 << 28));     
    
    if (0 != (flag = mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, IT6607_IICADDR, &RegAddr, 1, pData, N)))
    {
        printf("HDMIRX Read byte error\n");
        mmpIicGenStop(IIC_PORT_0);
    }   
    
    //IIC Mode external IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (EXTERNAL_IIC_MODE << 28), (0x3 << 28));
        
    mmpIicReleaseModule(IIC_PORT_0);
    
    return flag;

}


SYS_STATUS HDMIRX_WriteI2C_ByteN(BYTE RegAddr,BYTE *pData,int N)
{

    BOOL flag; 
    
    mmpIicLockModule(IIC_PORT_0);
    
    //IIC Mode internal IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (INTERNAL_IIC_MODE << 28), (0x3 << 28));  
            
    if ( 0 != (flag = mmpIicSendData(IIC_PORT_0, IIC_MASTER_MODE, IT6607_IICADDR, RegAddr, pData , N)))
    {
        printf("HDMIRX Write i2c byte error\n");
        mmpIicGenStop(IIC_PORT_0);
    }

    //IIC Mode external IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (EXTERNAL_IIC_MODE << 28), (0x3 << 28));
        
    mmpIicReleaseModule(IIC_PORT_0);    
    
    return flag;    

}

SYS_STATUS HDMIRX_SetI2C_Byte(SHORT RegAddr, BYTE Mask, BYTE Value)
{
    BYTE  p_data;
    BOOL    FLAG;
    BYTE i;
    BYTE regAddr8 = (BYTE) RegAddr;

    mmpIicLockModule(IIC_PORT_0); 

    //IIC Mode internal IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (INTERNAL_IIC_MODE << 28), (0x3 << 28));  
        
    if( Mask != 0xFF )
    {
        for( i = 0; i < 3; i++ )
        {
            FLAG = mmpIicReceiveData(IIC_PORT_0, IIC_MASTER_MODE, IT6607_IICADDR, &regAddr8, 1, &p_data, 1);
            if( FLAG == 0 ) break;
            printf("RX Read I2C ERROR !!!");
            printf("=====  Read Reg0x%x=  \n", regAddr8);
            mmpIicGenStop(IIC_PORT_0);
        }

        // printf("reg%02X = %02X ->",(int)RegAddr,(int)p_data) ;
        p_data &= ~Mask; // printf(" %02x ->",(int)p_data) ;
        Value &= Mask;    // printf(" %02x ->",(int)p_data) ;
        p_data |= Value; // printf(" %02x\n",(int)p_data) ;
    }
    else
    {
        p_data = Value;
    }

    for( i = 0; i < 3; i++ )
    {
        FLAG = mmpIicSendData(IIC_PORT_0, IIC_MASTER_MODE, IT6607_IICADDR, regAddr8, &p_data, 1);
        if(FLAG == 0) break;
        printf("RX Read I2C ERROR !!!");
        printf("=====  Write Reg0x%x <- %x  \n", regAddr8, p_data);
        mmpIicGenStop(IIC_PORT_0);
    }

    //IIC Mode external IIC + IIC controller 
    ithWriteRegMaskA(GPIO_BASE + 0xD0, (EXTERNAL_IIC_MODE << 28), (0x3 << 28));
        
    mmpIicReleaseModule(IIC_PORT_0);

    return !FLAG;

}
