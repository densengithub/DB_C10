
#ifndef _NIU_DB_DATA_ITE_H
#define _NIU_DB_DATA_ITE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "comm_niu_type.h"
#include "niu_export_enum.h"
#include "comm_mem_manage.h"

#ifndef _WIN32
#include "openrtos/FreeRTOS.h"
// #include "openrtos/task.h"
#include "openrtos/queue.h"
// #include "openrtos/list.h"
#include "openrtos/semphr.h"
#include "openrtos/timers.h"                     // ARM.FreeRTOS::RTOS:Timers
#endif

//ite仪表端数据表
//根据R1仪表与ST转接板协议生成
typedef enum _niu_data_id_db_loc
{	
	/***************************************/
	NIU_ID_DB_LOC_STATUS,		  //	机车状态
	NIU_ID_DB_LOC_GEARS,		  //	档位
	NIU_ID_DB_LOC_SPEED,		  //	速度
	NIU_ID_DB_LOC_MILEAGE,		  //	里程数
	NIU_ID_DB_LOC_C_CUR,		  //	充电电流
	NIU_ID_DB_LOC_DC_CUR,		  //	放电电流
	NIU_ID_DB_LOC_BAT_SOC,		  // 	电池SOC
	NIU_ID_DB_LOC_FULL_C_T,		  // 	充电剩余时间 *0.1h
	NIU_ID_DB_LOC_HOUR,			  // 	当前小时
	NIU_ID_DB_LOC_MIN,			  // 	当前分钟
	NIU_ID_DB_LOC_F_CODE,		  // 	故障代码
	NIU_ID_DB_LOC_BAT2_SOC,		  // 	电池2 SOC
	NIU_ID_DB_LOC_STATUS2,		  // 	车辆状态2
	NIU_ID_DB_LOC_BAT_TOTAL_SOC,  // 	电池总电量

	NIU_ID_DB_LOC_WEATHER_CODE,					 //	天气
	NIU_ID_DB_LOC_AIR_TEMPERATURE,				 //	气温 -128c ~ 127c
	NIU_ID_DB_LOC_REALTIME_POWER,			 //	实时整车功率 *10W
	NIU_ID_DB_LOC_AVERAGE_POWER, 			//	每百米平均能耗 *0.01W
	NIU_ID_DB_LOC_AVERAGE_POWER_TRIP, 		//	此行程每公里平均能耗 *0.01W
	NIU_ID_DB_LOC_REMAINING_MILEAGE,   		//	剩余里程 *1m

	
	NIU_ID_DB_LOC_LAST_TRIP_MILEAGE,			  //	上次骑行里程 *1m
	NIU_ID_DB_LOC_LAST_CYCLING_TIME,			  //	上次骑行时间 *1s
	NIU_ID_DB_LOC_LAST_AVG_SPEED,			  	//	上次骑行均速	*1m/s
	NIU_ID_DB_LOC_CHARGE_TIME,			  		//	剩余充电时间		*1min
	NIU_ID_DB_LOC_THIS_TRIP_MILEAGE,			  //	本次骑行里程 *1m
	
	NIU_ID_DB_LOC_BATT_TEMP_1,					  //	电池1温度
	NIU_ID_DB_LOC_BATT_TEMP_2,					  //	电池2温度

	NIU_ID_DB_LOC_REALTIME_ROTATION,			  //	实时转速 rpm
	NIU_ID_DB_LOC_ESTIMATED_BATTERY_REACH,		  //	到达目的地预估电量
	NIU_ID_DB_LOC_THIS_TRIP_CONSUMES_POWER,		  //	此行程耗电量	*Wh
	NIU_ID_DB_LOC_REALTIME_TORQUE,				  //	实时扭矩
	NIU_ID_DB_LOC_REALTIME_CONVERSION_EFFICIENCY, //	实时转化效率
	NIU_ID_DB_LOC_REALTIME_OUTPUT_POWER,		  //	实时电机功率

	NIU_ID_DB_LOC_CURRENT_ALTITUDE,				//	当前海拔
	NIU_ID_DB_LOC_LONGITUDINAL_G_VALUE, 		//	纵向加速度
	NIU_ID_DB_LOC_LATERAL_G_VALUE,				//	侧向加速度
	NIU_ID_DB_LOC_CURRENT_INCLINATION,			//	当前倾角
	NIU_ID_DB_LOC_CURRENT_SLOPE,				//	当前坡度
	NIU_ID_DB_LOC_NAVIGATION_TEXT_TURN = 38,	//	导航文字转向信息
	NIU_ID_DB_LOC_LIGHT_ALS = 39,               //  光感值
	/***************************************/
	NIU_ID_DB_LOC_SERIAL_NUMBER = 70,			//	序列号
	NIU_ID_DB_LOC_99_TIMEOUT = 71,				// 	99超时

	NIU_ID_DB_LOC_SW_VER = 76,					//	软件版本号
	NIU_ID_DB_LOC_HW_VER,						//	硬件版本
	NIU_ID_DB_LOC_MODULE_VER,					//	模块软件版本号
	NIU_ID_DB_LOC_DB_STATUS,					//	仪表状态
	NIU_ID_DB_LOC_DB_FUNCTON_CONF,				// 	仪表功能配置
	NIU_ID_DB_LOC_DB_PROTCOL_VER,				// 	协议版本
	NIU_ID_DB_LOC_DB_CMD,						// 	仪表命令
	
	NIU_ID_DB_LOC_MAX // 放在最后一个
} NIU_DATA_ID_DB_LOC;

#pragma pack(1) 
//ite仪表端数据表
//根据R1仪表与ST转接板协议生成
typedef struct _niu_data_body_db_loc_
{	
	NUINT16 db_status;				// 机车状态							
	NUINT8 db_gears;				// 档位
	NUINT8 db_speed;				// 速度
	NUINT32 db_mileage;				// 里程数
	NUINT8 db_c_cur;				// 充电电流
	NUINT8 db_dc_cur;				// 放电电流
	NUINT8 db_bat_soc;				// 电池SOC
	NUINT8 db_full_c_t;				// 充电剩余时间
	NUINT8 db_hour;					// 当前小时
	NUINT8 db_min;					// 当前分钟
	NUINT8 db_f_code;				// 故障代码
	NUINT8 db_bat2_soc;				// 电池2SOC
	NUINT16 db_status2;				// 车辆状态2
	NUINT8 db_bat_total_soc;		// 电池总电量SOC

	NUINT16 db_weather_code;					  // 天气
	NINT8 db_air_temperature;					  // 气温
	NINT16 db_realtime_input_power;				  // 实时整车功率
	NINT16 db_average_power; 					  // 每百米平均能耗
	NUINT32 db_average_power_trip; 				  // 此行程每公里平均能耗
	NUINT32 db_remaining_mileage;		  		// 预估剩余里程
	
	
	NUINT32 db_last_trip_mileage;			// 上次骑行里程
	NUINT32 db_last_cycling_time;			// 上次骑行时间
	NUINT16 db_last_avg_speed;				// 上次骑行均速	
	NUINT16 db_charge_time;					// 充电时间
	NUINT32 db_this_trip_mileage;			// 本次骑行里程

	NINT8 db_batt_temp_1;						  // 电池1温度
	NINT8 db_batt_temp_2;						  // 电池2温度

	NUINT32 db_realtime_rotation;				// 实时转速
	NUINT16 db_estimated_battery_reach;		   // 到达目的地预估电量
	NUINT16 db_this_trip_consumes_power;	   // 此行程耗电量
	NUINT32 db_realtime_torque;				   // 实时扭矩
	NUINT32 db_realtime_conversion_efficiency; // 实时转化效率
	NUINT32 db_realtime_output_power;		   // 实时电机功率

	NINT16 db_current_altitude;				// 当前海拔
	NINT16 db_longitudinal_g_value; 		// 纵向加速度
	NINT16 db_lateral_g_value;				// 侧向加速度
	NINT8 db_current_inclination;			// 当前倾角
	NINT8 db_current_slop;					// 当前坡度
	NUINT16 db_navigation_text_turn;		// 导航转向
	NUINT8 db_light_percent;   				// 光感

	NUINT8 db_reserve_1[0x80 - 0x50 - 4];   // 预留
	NUINT8 db_serial_number[16];			//	序列号
	NUINT16 db_99_timeout;					//	99报警超时时间
	NUINT8 db_reserve_2[0xA0 - 0x90 - 3];   // 预留
	NUINT8 db_sw_version[8];				//	软件版本
	NUINT8 db_hw_version[8];				//	硬件版本
	NUINT8 db_module_version[8];			//	模块软件版本号
	NUINT32 db_db_status;				// 	仪表状态		
	NUINT32 db_db_function_config;		// 	仪表功能配置
	NUINT32 db_db_protcol_ver;			// 	协议版本
	NUINT16 db_db_command;				// 	仪表命令

} NIU_DATA_BODY_DB_LOC;
#pragma pack() 

typedef union _ndata_value_db_loc_
{
    NIU_DATA_BODY_DB_LOC data;
    NCHAR buff[sizeof(NIU_DATA_BODY_DB_LOC)];
}NDATA_VALUE_DB_LOC;


typedef struct _ndata_value_loc_
{
	NDATA_VALUE_DB_LOC db_loc;
	
}NDATA_VALUE_LOC;

//保存设备每个参数的相关特性
typedef struct _niu_data_value_loc_
{
	NVOID *addr;
	NIU_DATA_TYPE type; //cwpack_item_types 或者用msgpack规定的类型
	NUINT8 len;
	NUINT8 offset;		//字段偏移
}NIU_DATA_VALUE_LOC;

// #ifndef TASK_SUSPEND() 
#define TASK_SUSPEND()  vTaskSuspendAll()
// #endif

// #ifndef TASK_RESUME() 
#define TASK_RESUME()  xTaskResumeAll()
// #endif

#define TAKE_WRITE_KEY_LOC()       xSemaphoreTake( write_loc_data_key, portMAX_DELAY )

#define GIVE_WRITE_KEY_LOC()       xSemaphoreGive( write_loc_data_key)

#ifdef APP_VERSION
#undef APP_VERSION
#endif

#ifdef CFG_NIU_PANEL_VERSION
	#define     APP_VERSION    	CFG_NIU_PANEL_VERSION
	#pragma  message(CFG_NIU_PANEL_VERSION)
#else
	#define     APP_VERSION    	"ITEPKG03"	
	#pragma  message(APP_VERSION)
#endif
#define     HARD_VERSION    "R1C10V03"



#if PRINTDEBUG_ITE
#define ITE_TRACE  printf
#else
#define ITE_TRACE
#endif

extern NDATA_VALUE_LOC local_niu_value_loc;
extern SemaphoreHandle_t write_loc_data_key;

void DeviceDataSheetInit(void);

NBOOL niu_eqp_is_vaild_loc( NIU_DATA_ADDR_BASE base );
NBOOL niu_eqp_id_is_vaild_loc( NIU_DATA_ADDR_BASE base, NUINT32 id );
NIU_DATA_TYPE niu_data_get_type_loc(NIU_DATA_ADDR_BASE base, NUINT32 id);
NUINT32 niu_data_get_datalen_loc(NIU_DATA_ADDR_BASE base, NUINT32 id);
NVOID* niu_data_read_value_loc(NIU_DATA_ADDR_BASE base, NUINT32 id, NVOID* data, NUINT32 len);
NUINT32 niu_data_read_data_loc(NIU_DATA_ADDR_BASE base, NUINT32 id);
NBOOL niu_data_clean_value_loc(NIU_DATA_ADDR_BASE base, NUINT32 id);
NVOID* niu_data_get_pdata_loc(NIU_DATA_ADDR_BASE base, NUINT32 id);
NVOID write_local_niu_value_loc(NIU_DATA_ADDR_BASE base,NUINT16 addr_offset,NUINT16 byte_offset,void *databuff,NUINT16 len);
NUINT32 niu_data_write_data_loc(NIU_DATA_ADDR_BASE base, NUINT32 id, NVOID* data);
NUINT32 niu_data_write_data_len_loc(NIU_DATA_ADDR_BASE base, NUINT32 id, NVOID* data, NUINT32 len);

#ifdef __cplusplus
}
#endif

#endif