///*****************************************
//  Copyright (C) 2009-2014
//  ITE Tech. Inc. All Rights Reserved
//  Proprietary and Confidential
///*****************************************
//   @file   <mcu.h>
//   @author Jau-Chih.Tseng@ite.com.tw
//   @date   2011/08/12
//   @fileversion: CAT9883_V43
//******************************************/

uint8_t CAT9883_ReadI2C_Byte(uint8_t RegAddr);
uint32_t CAT9883_WriteI2C_Byte(uint8_t RegAddr, uint8_t d);
uint32_t CAT9883_WriteI2C_ByteN(uint8_t RegAddr, uint8_t *pData, int N);
uint32_t CAT9883_WriteI2c_ByteMask(uint8_t RegAddr, uint8_t data, uint8_t mask);