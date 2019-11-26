///*****************************************
//  Copyright (C) 2009-2018
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <IO.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2018/05/10
//   @fileversion: ITE_MHLRX_SAMPLE_V1.27
//******************************************/

#ifndef _IO_h_
#define _IO_h_
#include "typedef.h"


#ifdef SUPPORT_UART_CMD
void UartCommand();
#endif

#define IT6802A0_HDMI_ADDR 0x92     //Hardware Fixed I2C address of IT6802 HDMI
//#define IT6802B0_HDMI_ADDR 0x90	//Hardware Fixed I2C address of IT6802 HDMI
#define MHL_ADDR  0xE0	//Software programmable I2C address of IT6802 MHL
#define EDID_ADDR 0xA8 	//Software programmable I2C address of IT6802 EDID RAM
#define CEC_ADDR  0xC8	//Software programmable I2C address of IT6802 CEC

//BYTE HDMI_IIC_Read( BYTE RegAddr);
//BOOL HDMI_IIC_Write( BYTE RegAddr,BYTE DataIn);
//BYTE HDMI_IIC_SET( BYTE offset, BYTE mask, BYTE datain );

//BOOL i2c_write_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device );
//BOOL i2c_read_byte( BYTE address,BYTE offset,BYTE byteno,BYTE *p_data,BYTE device );

SYS_STATUS HDMIRX_ReadI2C_ByteN(unsigned char iicaddress, unsigned char RegAddr,unsigned char *pData,int N);

SYS_STATUS HDMIRX_WriteI2C_ByteN(unsigned char iicaddress, unsigned char RegAddr,unsigned char *pData,int N);



#ifdef Enable_IT6802_CEC
#include "debug.h"
BYTE IT6802_CEC_ReadI2C_Byte(BYTE RegAddr);
SYS_STATUS IT6802_CEC_WriteI2C_Byte(BYTE offset,BYTE buffer );
#endif


//#define hdmirxrd(x) HDMI_IIC_Read(x)
//#define hdmirxwr(x,y) HDMI_IIC_Write(x,y)



//void SetEDIDWp(BOOL ENA);


//void HotPlug(BYTE Enable);

//void init_printf(void);

//void SetintActive(BOOL bactive);


#endif
