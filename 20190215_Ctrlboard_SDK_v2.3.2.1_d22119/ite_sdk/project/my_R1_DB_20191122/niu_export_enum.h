
#ifndef __NIU_EXPORT_ENUM_H_
#define __NIU_EXPORT_ENUM_H_

#ifdef __cplusplus
 extern "C" {
#endif


typedef enum _niu_device_type_
{
	NIU_ECU = 1,       // 中控		
	NIU_LCU,
	NIU_NBC,
	NIU_LKU,
	NIU_VCU,
	NIU_DB,  
	NIU_FOC,      
	NIU_BMS,      
	NIU_BMS2,     
	NIU_MAX,
	NIU_ALL=0xff,

}NIU_DATA_ADDR_BASE;


typedef enum _niu_data_type
{
	NIU_INT8 = 0,
	NIU_UINT8,
	NIU_INT16,
	NIU_UINT16,
	NIU_INT32,
	NIU_UINT32,
	NIU_STRING,
	NIU_ARRAY,
	NIU_FLOAT,
	NIU_DOUBLE,
	NIU_NULL = 0xFF
}NIU_DATA_TYPE;


#ifdef __cplusplus
}
#endif

#endif /* USER_NIU_BUS_ENUM_H_ */
