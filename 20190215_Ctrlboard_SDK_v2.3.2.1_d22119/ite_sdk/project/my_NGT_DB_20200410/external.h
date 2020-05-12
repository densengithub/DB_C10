/** @file
 * Template project definition.
 *
 */
/** @defgroup template
 *  @{
 */
#ifndef EXTERNAL_H
#define EXTERNAL_H
/** @defgroup ctrlboard_external External
*  @{
*/

#include "ite/itp.h"


#define EXTERNAL_BUFFER_SIZE 	12 ///< External buffer size
#define EXT_MAX_QUEUE_SIZE      8

typedef enum
{
	EXTERNAL_UPDATA = 1,
	EXTERNAL_TESTMODE,
	EXTERNAL_TESTMODE_PSENSOR,
	EXTERNAL_DEMO,
	EXTERNAL_CMD,
	EXTERNAL_TIMEOUT,
	EXTERNAL_BACKLIGHT,
	EXTERNAL_ST_VER,
	EXTERNAL_PL_VER
} ExternalEventType;

typedef struct
{
	ExternalEventType type;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
	uint8_t buf1[EXTERNAL_BUFFER_SIZE];
} ExternalEvent;

#pragma pack(1)
typedef struct ScooterInfo_typedef
{
	unsigned short car_status1;			//Car status1
	unsigned char car_gear;				//Gear
	unsigned char car_speed;			//Speed
	unsigned int car_mileage;			//Mileage
	unsigned char car_chargecurrent;	//Charge Current
	unsigned char car_dischargecurrent; //Discharge Current
	unsigned char car_bat1_soc;			//Battery1 State of Charge
	unsigned char car_full_c_t;			//full charge time
	unsigned char time_hour;			//hour
	unsigned char time_minute;			//minute
	unsigned char car_fault_code;		//fault Code
	unsigned char car_bat2_soc;			//Battery2 State of Charge
	unsigned short car_status2;			//Car status2
	unsigned char car_total_soc;		//Battery total State of Charge

	unsigned short weather_code; 		// 天气
	signed char air_temperature; 		// 气温
	signed short realtime_input_power;	//x10w
	signed short average_power; 		//x0.01w/h
	unsigned int average_power_trip;	// 此行程每公里平均能耗
	unsigned int remaining_mileage;		//	剩余里程x1m

	unsigned int last_trip_mileage;
	unsigned int last_cycling_time;
	unsigned short last_avg_speed;
	unsigned short charge_time;
	unsigned int this_trip_mileage;

	signed char batt_temp_1; 			//battery 1 temperature
	signed char batt_temp_2; 			//battery 2 temperature

	unsigned int realtime_rotation;
	unsigned short estimated_battery_reach;
	unsigned short this_trip_consumes_power;
	unsigned int realtime_torque;
	unsigned int realtime_conversion_efficiency;	// 实时转化效率
	unsigned int realtime_output_power;				// 实时电机功率

	signed short current_altitude;			  		// 当前海拔
	signed short longitudinal_g_value;		// 纵向加速度
	signed short lateral_g_value;			// 侧向加速度
	signed char current_inclination;		   		// 当前倾角
	signed char current_slop;				   		// 当前坡度
	signed short navigation_text_turn;		   		// 导航转向
	unsigned char light_percent;   				// 光感

	unsigned char car_db_reserve_1[0x80 - 0x50 - 4];
	
	unsigned char serinal_number[16];			//	序列号
	unsigned short car_99_timeout;					//	99报警超时时间
	unsigned char car_db_reserve_2[0xA0 - 0x90 - 3]; // 预留
	unsigned char sw_version[8];				//	软件版本
	unsigned char hw_version[8];				//	硬件版本
	unsigned char module_version[8];			//	模块软件版本号
	unsigned int  car_db_status;				// 	仪表状态		
	unsigned int  car_db_function_config;		// 	仪表功能配置
	unsigned int  car_db_protcol_ver;			// 	协议版本
	unsigned short car_db_command;				// 	仪表命令
} ScooterInfo_Typedef;

typedef struct DashBoardInfo_Tag
{
	unsigned short test_psensor1_ad;		//psensor1
	unsigned short test_mode;		 		//mode
	unsigned short test_psensor2_ad;  		//psensor2
	unsigned short test_ntc_ad;		 		//ntc
	signed char test_temperature;			//temperature
	unsigned char test_bt_flash_485; 		//bluetooth,spi flash,rs485
	unsigned char test_input;		 		//input
	unsigned short test_led;		 		//led
} DashBoardInfo_typedef;

#pragma pack() 

typedef union {

	ScooterInfo_Typedef ScooterInfo;
	DashBoardInfo_typedef DashBoardInfo;
	struct
	{
		unsigned short car_99_timeout;			//99_timeout		
	} Scootertimeout;

	struct
	{		
		unsigned short car_cmd;					//car_command	
	} Scootercmd;

	struct
	{
		unsigned char stm32_soft_version[8];		
	} Scooterstm32ver;

	struct
	{
		unsigned char panel_soft_version[8];		
	} Scooterpanelver;

	unsigned char DisplayDatabuffer[sizeof(ScooterInfo_Typedef)];
} ReceiveDisplayData_Typedef;

typedef struct
{
	ExternalEventType type;
	int arg1;
	int arg2;
	int arg3;
	int arg4;
	ReceiveDisplayData_Typedef layer_displaydata;
} ExternalEvent_layer;

extern ReceiveDisplayData_Typedef ReceiveDataBuffer;

extern sem_t ExternaltaskSem;

extern ExternalEvent_layer ev_layer2;

/**
* Initializes external module.
*/
void ExternalInit(void);

/**
* Exits external module.
*/
void ExternalExit(void);

/**
* Receives external module event.
*
* @param ev The external event.
* @return 0 for no event yet, otherwise for event occured.
*/
int ExternalReceive(ExternalEvent* ev);

/**
* Sends external module event.
*
* @param ev The external event.
* @return 0 for success, otherwise for failure.
*/
int ExternalSend(ExternalEvent* ev);

int Layer_ExternalSend(ExternalEvent *ev);

int Layer_ExternalReceive(ExternalEvent *ev);

/**
* Initializes external process module.
*/
void ExternalProcessInit(void);

/**
* Processes external module event.
*
* @param ev The external event.
* @return 0 for no event yet, otherwise for event occured.
*/
void ExternalProcessEvent(ExternalEvent* ev);

/** @} */ // end of ctrlboard_external

#endif /* EXTERNAL_H */

/** @} */ // end of template