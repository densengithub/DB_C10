#ifndef UI_CONTROL_H
#define UI_CONTROL_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>


#ifdef __cplusplus
extern "C" {
#endif

typedef enum _user_time_typedef_
{
	TEXT_TIME_HOUR = 1,
	TEXT_TIME_MINUTE
}user_time_type;

typedef enum _user_drivemode_typedef_
{
	NO_DRIVER_MODE = 0,
	ICON_DRIVER_MODE_E,
	ICON_DRIVER_MODE_D,
	ICON_DRIVER_MODE_S,
	ICON_DRIVER_MODE_MAX
}user_drivemode_type;

typedef enum _user_indicationlist_typedef_
{
	ICON_TurnRightlight = 1,
	ICON_TurnLeftlight,	
	ICON_Headlight,
	ICON_Autolight,
	ICON_CruiseControl,
	ICON_EnergyConservation,
	ICON_EnergyRecovery,
	ICON_CloudService,
	ICON_Ready,
	ICON_MobileNetSignal,
	ICON_SatelliteSignal,
	ICON_Doublelight,
	ICON_MAX_LIST
}user_indicationlist_type;

typedef enum _user_indicationaction_typedef_
{
	STATUS_NULL = 0,
	STATUS_SHOW = 1,
	STATUS_HIDDEN,
	STATUS_FLASH_NORMAL,	
	STATUS_FLASH_QUICKLY,
	// STATUS_DOUBLE_FLASH,
	STATUS_SIGNAL_LEVEL3,
	STATUS_SIGNAL_LEVEL2,
	STATUS_SIGNAL_LEVEL1,
	STATUS_SIGNAL_LEVEL0,
	STATUS_MAX_ACTION
}user_indicationaction_type;

typedef enum _user_unitlist_typedef_
{
	STATUS_METRIC_UNIT = 1,
	STATUS_IMPERIAL_UNIT,	
	STATUS_MAX_UNIT
}user_unitlist_type;

typedef enum _user_bat_typedef_
{
	STATUS_SINGLE_BAT = 1,
	STATUS_DUAL_BAT,
	STATUS_MAX_BAT
}user_bat_type;

#define Text_configmainLayerTimeHour	NULL
#define Text_configmainLayerTimeMinute	NULL

bool Layer_SideMistake_DisplayConfig(bool status_mistake);

//mainLayer
bool mainLayer_Time_DisplayConfig(user_time_type timetype, unsigned char dis_param);
bool mainLayer_DriveMode_DisplayConfig(user_drivemode_type drivemode);
// bool mainLayer_SpeedColor_DisplayConfig(int dis_speed);
bool mainLayer_SpeedValue_DisplayConfig(int dis_speed,user_unitlist_type dis_unit);
// bool mainLayer_DischargeCurrent_DisplayConfig(unsigned char dis_ampere);
bool mainLayer_BatteryCapacity_DisplayConfig(int dis_capacity);
bool mainLayer_Temperature_DisplayConfig(int dis_temperature);
// bool mainLayer_Range_DisplayConfig(int dis_ragne,user_unitlist_type dis_unit);
bool mainLayer_Total_DisplayConfig(int dis_total,user_unitlist_type dis_unit);
// bool mainLayer_Turnlight_DisplayConfig(user_indicationlist_type dis_turnlight, user_indicationaction_type dis_action, int dis_per);
bool mainLayer_NetworksSignal_DisplayConfig(user_indicationlist_type dis_signallist, user_indicationaction_type dis_action);
bool mainLayer_Workstatus_DisplayConfig(user_indicationlist_type dis_worklist, user_indicationaction_type dis_action);
bool mainLayer_Score_DisplayConfig(unsigned char dis_score, user_indicationaction_type dis_action);
bool mainLayer_ErrorCode_DisplayConfig(unsigned char dis_errorcode,user_indicationaction_type dis_action);
bool mainLayer_SpeedUnit_DisplayConfig(user_unitlist_type dis_unit, user_indicationaction_type dis_action);

bool mainLayer_RealtimeEnergy_DisplayConfig(int dis_realtime_energy,int dis_rtenergy);
// bool mainLayer_RealtimeEnergy_DisplayConfig(int dis_realtime_energy);
bool mainLayer_RealtimeEnergyCurve_DisplayConfig(int dis_realtime_energy);
bool mainLayer_RemainMileage_DisplayConfig(int dis_range, user_unitlist_type dis_unit);
bool mainLayer_BatteryBCapacity_DisplayConfig(int dis_capacity);
bool mainLayer_BatteryACapacity_DisplayConfig(int dis_capacity);
bool mainLayer_AverageEnergy_DisplayConfig(int dis_avg_energy,user_unitlist_type dis_unit);
bool mainLayer_Weather_DisplayConfig(int status_weather, user_indicationaction_type dis_action);

bool mainLayer_WeatherTemperature_DisplayConfig(int status_weather, int dis_temperature);


bool mainLayer_LastTripTime_DisplayConfig(int dis_lasttrip_time);
bool mainLayer_LastTripAvgSpeed_DisplayConfig(int dis_lasttrip_avgspeed, user_unitlist_type dis_unit);
bool mainLayer_LastTripMileage_DisplayConfig(int dis_lasttrip_mileage, user_unitlist_type dis_unit);

bool mainLayer_BatterySingle_DisplayConfig(int dis_capacity);
bool mainLayer_BatteryDual_DisplayConfig(int dis_capacity_a, int dis_capacity_b);
// bool mainLayer_Battery_DisplayConfig(int dis_capacity_bata, int dis_capacity_batb, user_bat_type bat_type);
bool mainLayer_BatteryBalance_DisplayConfig(bool dis_balanced);

bool mainLayer_Sidestand_DisplayConfig(bool status_side);
// bool mainLayer_SideMistake_DisplayConfig(bool status_mistake);
#define mainLayer_SideMistake_DisplayConfig(x)		Layer_SideMistake_DisplayConfig(x)
// bool mainLayer_Ready_DisplayConfig(bool status_ready);
bool mainLayer_Ready_DisplayConfig(bool status_ready, bool status_sidestand);
bool mainLayer_ScoreError_DisplayConfig(int dis_type, int dis_data);

void layer_SideStand_Display(void);
void layer_Signal_Display(void);


//chargeLayer
bool Layer_charge_speedpower_init(void);
bool Layer_charge_bottom_init(void);
bool Layer_charge_time_init(void);
bool chargeLayer_Sidestand_DisplayConfig(bool status_ready, bool status_sidestand);
bool chargeLayer_ChargeTime_DisplayConfig(int dis_charge);
#define chargeLayer_SideMistake_DisplayConfig(x)		Layer_SideMistake_DisplayConfig(x)

//upgradeLayer
bool Layer_upgrade_time_init(void);
bool UpgradeLayer_Precent_Display(int dis_pre);
bool UpgradeLayer_Remaintime_Display(int dis_time);

//testmodeLayer
bool testmodeLayer_bluetooth_DisplayConfig(int bt_dis);
bool testmodeLayer_rs485_DisplayConfig(int rs485_dis);
bool testmodeLayer_uart_DisplayConfig(int uart_dis);
bool testmodeLayer_usb_DisplayConfig(int usb_dis);
bool testmodeLayer_spiflash_DisplayConfig(int flash_dis);
bool testmodeLayer_ntc_DisplayConfig(int ntc_ad_dis, int ntc_dat_dis);
bool testmodeLayer_psensor1_DisplayConfig(int psensor_ad_dis);
bool testmodeLayer_psensor2_DisplayConfig(int psensor_ad_dis);
bool testmodeLayer_input_DisplayConfig(int input_dis);
bool testmodeLayer_led_DisplayConfig(int led_dis);
bool testmodeLayer_pass_DisplayConfig(int pass_dis);
// bool testmodeLayer_module_Display(bool dis_visible);
bool testmodeLayer_module_Display(bool dis_visible, int dis_list);

void Layer_MainMode_Task(ExternalEvent_layer *ev_testmode);

#ifdef __cplusplus
}
#endif

#endif /* UI_CONTROL_H */

/** @} */ // end of template
