#include "niu_db_data_ite.h"
// #include "comm_os_debug.h"

SemaphoreHandle_t write_loc_data_key = NULL;

#define NIU_DATA_ITEM_LOC(m, n, t)     \
	{                                  \
		(NVOID *)&(m), n, sizeof(m), t \
	}

NDATA_VALUE_LOC local_niu_value_loc = {0};

const NIU_DATA_VALUE_LOC niu_data_value_db_ite[] =
	{
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_status, NIU_UINT16, 0),	   // 机车状态
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_gears, NIU_UINT8, 0),		   // 档位
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_speed, NIU_UINT8, 0),		   // 速度
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_mileage, NIU_UINT32, 0),	  // 里程数
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_c_cur, NIU_UINT8, 0),		   // 充电电流
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_dc_cur, NIU_UINT8, 0),		   // 放电电流
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_bat_soc, NIU_UINT8, 0),	   // 电池SOC
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_full_c_t, NIU_UINT8, 0),	  // 充电剩余时间
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_hour, NIU_UINT8, 0),		   // 当前小时
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_min, NIU_UINT8, 0),		   // 当前分钟
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_f_code, NIU_UINT8, 0),		   // 故障代码
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_bat2_soc, NIU_UINT8, 0),	  // 电池2SOC
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_status2, NIU_UINT16, 0),	  // 车辆状态2
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_bat_total_soc, NIU_UINT8, 0), // 电池总电量soc

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_weather_code, NIU_UINT16, 0),		  // 天气
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_air_temperature, NIU_INT8, 0),		  // 气温
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_realtime_input_power, NIU_INT16, 0), // 实时整车功率
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_average_power, NIU_INT16, 0),		  // 每百米平均能耗
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_average_power_trip, NIU_UINT32, 0),  // 此行程每公里平均能耗
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_remaining_mileage, NIU_UINT32, 0),   // 预估剩余里程

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_last_trip_mileage, NIU_UINT32, 0), // 上次骑行里程
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_last_cycling_time, NIU_UINT32, 0), // 上次骑行时间
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_last_avg_speed, NIU_UINT16, 0),	// 上次骑行均速
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_charge_time, NIU_UINT16, 0),		// 充电时间
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_this_trip_mileage, NIU_UINT32, 0), // 本次骑行里程

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_batt_temp_1, NIU_INT8, 0), // 电池1温度
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_batt_temp_2, NIU_INT8, 0), // 电池2温度

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_realtime_rotation, NIU_UINT32, 0),				 // 实时转速
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_estimated_battery_reach, NIU_UINT16, 0),		 // 到达目的地预估电量
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_this_trip_consumes_power, NIU_UINT16, 0),		 // 此行程耗电量
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_realtime_torque, NIU_UINT32, 0),				 // 实时扭矩
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_realtime_conversion_efficiency, NIU_UINT32, 0), // 实时转化效率
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_realtime_output_power, NIU_UINT32, 0),			 // 实时电机功率

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_current_altitude, NIU_UINT16, 0),	 // 当前海拔
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_longitudinal_g_value, NIU_UINT16, 0), // 纵向加速度
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_lateral_g_value, NIU_UINT16, 0),	  // 侧向加速度
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_current_inclination, NIU_INT8, 0),	// 当前倾角
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_current_slop, NIU_INT8, 0),		   // 当前坡度
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_navigation_text_turn, NIU_INT16, 0),   // 导航转向
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_light_percent, NIU_UINT8, 0),   // 光感
		
		//	NIU_DATA_ITEM_LOC( local_niu_value_loc.db_loc.data.db_reserve_1, NIU_STRING,0x52),
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[0], NIU_UINT16, 0x52), // 预留	39
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[2], NIU_UINT16, 0x52+2), // 预留	40

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[4], NIU_UINT16, 0x52+4), // 预留	41
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[6], NIU_UINT16, 0x52+6), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[8], NIU_UINT16, 0x52+8), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[10], NIU_UINT16, 0x52+10), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[12], NIU_UINT16, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[14], NIU_UINT16, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[16], NIU_UINT16, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[18], NIU_UINT16, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[20], NIU_UINT16, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[22], NIU_UINT16, 0), // 预留	50

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[24], NIU_UINT16, 0), // 预留	51
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[26], NIU_UINT16, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[28], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[29], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[30], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[31], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[32], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[33], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[34], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[35], NIU_UINT8, 0), // 预留	60

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[36], NIU_UINT8, 0), // 预留	61
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[37], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[38], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[39], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[40], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[41], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[42], NIU_UINT8, 0), // 预留
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[43], NIU_UINT8, 0), // 预留
	//    NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1[44], NIU_UINT8, 0x52+44), // 预留	69
	//    NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_1, NIU_STRING, 0), // 预留	70

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_serial_number, NIU_STRING, 0),		 // 序列号
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_99_timeout, NIU_UINT16, 0),			 // 99报警超时时间
	//	NIU_DATA_ITEM_LOC( local_niu_value_loc.db_loc.data.db_reserve_2, NIU_UINT8,0x92),

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_2[0], NIU_UINT32, 0x92),			 // 预留 	72
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_2[4], NIU_UINT32, 0x92+4),			 // 预留	73
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_2[8], NIU_UINT32, 0x92+8),			 // 预留	74
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_reserve_2[12], NIU_UINT8, 0x92+12),		 // 预留	75

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_sw_version, NIU_STRING, 0),			 // 软件版本
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_hw_version, NIU_STRING, 0),			 // 硬件版本
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_module_version, NIU_STRING, 0),		 // 模块软件版本号
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_db_status, NIU_UINT32, 0),			 // 仪表状态
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_db_function_config, NIU_UINT32, 0), // 仪表功能配置

		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_db_protcol_ver, NIU_UINT32, 0), // 协议版本
		NIU_DATA_ITEM_LOC(local_niu_value_loc.db_loc.data.db_db_command, NIU_UINT16, 0),	 // 仪表命令
};

/**
 * @brief 判断设备类型是否有效(线程安全函数)
 * @param base 设备类型
 * @return 返回结果 NTRUE or NFALSE
 */
NBOOL niu_eqp_is_vaild_loc( NIU_DATA_ADDR_BASE base)
{
	if(base==NIU_DB||base==NIU_ALL)
	{
		return NTRUE;
	}
	else
	{
		return NFALSE;
	}	
}

/**
 * @brief 判断设备字段号是否有效(线程安全函数)
 * @param base 设备类型
 * @param id 字段号
 * @return 返回结果 NTRUE or NFALSE
 */
NBOOL niu_eqp_id_is_vaild_loc( NIU_DATA_ADDR_BASE base, NUINT32 id)
{
	if(base==NIU_DB&&id < NIU_ID_DB_LOC_MAX)
	{
		return NTRUE;
	}
	else
	{
		return NFALSE;
	}	
}

/**
 * @brief 获取设备字段的类型(线程安全函数)
 * @param base 设备类型
 * @param id 字段号
 * @return 返回字段类型
 */
NIU_DATA_TYPE niu_data_get_type_loc(NIU_DATA_ADDR_BASE base, NUINT32 id)
{
    if(niu_eqp_id_is_vaild_loc(base, id) == NFALSE)
    {
        ITE_TRACE("niu_data_get_type,niu_eqp_id_is_vaild(%X,%d).\n", base, id);
        return NIU_NULL;
    }
	if(base==NIU_DB)
	{
		return (niu_data_value_db_ite[id].type);
	}    
}

/**
 * @brief 获取设备字段数据长度(线程安全函数)
 * @param base 设备类型
 * @param id 字段号
 * @return 返回长度
 */
NUINT32 niu_data_get_datalen_loc(NIU_DATA_ADDR_BASE base, NUINT32 id)
{
    if(niu_eqp_id_is_vaild_loc(base, id) == NFALSE)
    {
        ITE_TRACE("niu_data_get_datalen,niu_eqp_id_is_vaild(%X,%d).\n", base, id);
        return 0;
    }
	if(base==NIU_DB)//仪表
	{
		return (niu_data_value_db_ite[id].len);
	}    
}

/**
 * @brief 获取设备字段数据整形数值，非整形返回指针地址(线程安全函数)
 * @param base 设备类型
 * @param id 字段号
 * @return 返回 整形数值
 */
NUINT32 niu_data_read_data_loc(NIU_DATA_ADDR_BASE base, NUINT32 id)
{
    NVOID* temp = NNULL;
    NUINT32 result = 0;

    if(niu_eqp_id_is_vaild_loc(base, id) == NFALSE)
    {
        return 0;
    }

    temp = niu_data_get_pdata_loc(base, id);
	TAKE_WRITE_KEY_LOC();
    if (temp)
    {
        switch (niu_data_get_type_loc(base, id))
        {
            case NIU_INT8:
                result = *((NINT8 *)temp);
                break;
            case NIU_UINT8:
                result = *((NUINT8 *)temp);
                break;
            case NIU_INT16:
                result = *((NINT16 *)temp);
                break;
            case NIU_UINT16:
                result = *((NUINT16 *)temp);
                break;
            case NIU_INT32:
                result = *((NINT32 *)temp);
                break;
            case NIU_UINT32:
                result = *((NUINT32 *)temp);
                break;
            default:
                break;
        }
    }

	GIVE_WRITE_KEY_LOC();
    return result;
}

/**
 * @brief 写入设备字段数据(线程安全函数)
 * @param base 设备类型
 * @param id 字段号
 * @param data 写缓存指针
 * @param len 写缓存大小
 * @return 返回已写入长度
 */
/*NUINT32 niu_data_write_data_len(NIU_DATA_ADDR_BASE base, NUINT32 id, NVOID* data, NUINT32 len)
{
    NVOID* temp = NNULL;
    NUINT32 cpylen = 0;

    if(niu_eqp_id_is_vaild(base, id) == NFALSE)
    {
        return 0;
    }

    temp = niu_data_get_pdata(base, id);
    cpylen = niu_data_get_datalen(base, id);
	TAKE_WRITE_KEY();
    if (temp)
    {
        cpylen = (len > cpylen) ? cpylen : len;
        nmemcpy(temp, data, cpylen);
    }

    GIVE_WRITE_KEY();

    niu_data_remoteupdate(base, id);


    return cpylen;
}*/

/**
 * @brief 写入设备字段数据(线程安全函数)
 * @param base 设备类型
 * @param id 字段号
 * @param data 写缓存指针
 * @param len 写缓存大小
 * @return 返回已写入长度
 */
NUINT32 niu_data_write_data_loc(NIU_DATA_ADDR_BASE base, NUINT32 id, NVOID* data)
{
    NVOID* temp = NNULL;
    NUINT32 cpylen = 0;

    if(niu_eqp_id_is_vaild_loc(base, id) == NFALSE)
    {
        return 0;
    }

    temp = niu_data_get_pdata_loc(base, id);
    cpylen = niu_data_get_datalen_loc(base, id);
	TAKE_WRITE_KEY_LOC();
    if (temp)
    {
        //cpylen = (len > cpylen) ? cpylen : len;
        nmemcpy(temp, data, cpylen);
    }

    GIVE_WRITE_KEY_LOC();

    // niu_data_remoteupdate_loc(base, id);


    return cpylen;
}

/**
 * @brief 清零设备字段(线程安全函数)
 * @param base 设备类型
 * @param id 字段号
 * @return 返回成功 失败
 */


NBOOL niu_data_clean_value_loc(NIU_DATA_ADDR_BASE base, NUINT32 id)
{
    NBOOL ret = NFALSE;
    NUINT8 *value = NNULL;
    NUINT32 len = 0;

    if(niu_eqp_id_is_vaild_loc(base, id) == NFALSE)
    {
        ITE_TRACE("niu_data_clean_value,niu_eqp_id_is_vaild(%X,%d).\n", base, id);
        return NFALSE;
    }	

	TAKE_WRITE_KEY_LOC();

	if(base==NIU_DB&&id < NIU_ID_DB_MAX)
	{
		nmemset(niu_data_value_db_ite[id].addr,0, niu_data_value_db_ite[id].len);
        ret = NTRUE;
	}
    GIVE_WRITE_KEY_LOC();

    return ret;
}




/**
 * @brief 获取设备字段数据(线程安全函数)
 * @param base 设备类型
 * @param id 字段号
 * @param data 读取缓存地址
 * @param len 读取缓存大小
 * @return 返回 读取缓存指针地址
 */
NVOID* niu_data_read_value_loc(NIU_DATA_ADDR_BASE base, NUINT32 id, NVOID* data, NUINT32 len)
{
    NVOID* temp = NNULL;
    NUINT32 cpylen = 0;

    if(niu_eqp_id_is_vaild_loc(base, id) == NFALSE)
    {
        return NNULL;
    }


    temp = niu_data_get_pdata_loc(base, id);
    cpylen = niu_data_get_datalen_loc(base, id);
	TAKE_WRITE_KEY_LOC();
    if (temp)
    {
        cpylen = (cpylen > len) ? len : cpylen;

		
        nmemcpy(data, temp, cpylen);
    }

	GIVE_WRITE_KEY_LOC();

    return data;
}


/**
 * @brief 数据表本地更新
 * @param base 设备类型
 * @param id 字段号
 * @return NULL
 */
static NVOID niu_data_localupdate_loc( NIU_DATA_ADDR_BASE base, NUINT32 id )
{

}


/**
 * @brief 获取设备字段数据指针
 * @param base 设备类型
 * @param id 字段号
 * @return 返回void*指针
 */
NVOID* niu_data_get_pdata_loc(NIU_DATA_ADDR_BASE base, NUINT32 id)
{
    NVOID* result = 0;

    if(niu_eqp_id_is_vaild_loc(base, id) == NFALSE)
    {
        ITE_TRACE("niu_data_get_pdata,niu_eqp_id_is_vaild(%X,%d).\n", base, id);
        return NNULL;
    }

    niu_data_localupdate_loc(base, id);

	if(base==NIU_DB)
	{
		result = (niu_data_value_db_ite[id].addr);
	}
			
    return result;
}

void DeviceDataSheetInit(void)
{		
	unsigned short uint16_config = 0;
	unsigned char uint8_config = 0;
	// unsigned int uint32_config = 0;
	memset(local_niu_value_loc.db_loc.buff, 0, sizeof(NIU_DATA_BODY_DB_LOC));
	uint16_config = (unsigned short)0x2000;
	niu_data_write_data_loc(NIU_DB, NIU_ID_DB_LOC_DB_CMD, &uint16_config);
	uint16_config = (unsigned short)0x0000;
	niu_data_write_data_loc(NIU_DB, NIU_ID_DB_LOC_99_TIMEOUT, &uint16_config);
	niu_data_write_data_loc(NIU_DB, NIU_ID_DB_LOC_MODULE_VER, APP_VERSION);
}

#if 0
NUINT8 ReadAck_Error[] = {0x68,0x16,0xE9,0x68,0x82};


NVOID niu_create_respone_loc(NUINT8 start_id,NUINT8 id_cnt,NVOID* dst_str)
{
	NUINT8 *local_data = NULL;
	NUINT8 i = 0, j =0 ,loc_start_id = 0, loc_cnt_id = 0,status_temp = 0;
	NUINT16 id_data_len = 0;
	local_data = niu_data_get_pdata_loc(NIU_DB, start_id);
	id_data_len = niu_data_get_datalen_loc(NIU_DB, start_id);
	loc_start_id = start_id;
	loc_cnt_id = id_cnt;
	if(id_cnt>=1)
	{
		if(start_id>=0&&start_id<=1)
		{
			loc_start_id += (id_cnt-1);
			if(loc_start_id>1)
			{
				// status_temp = 2;
				loc_cnt_id = 1-0+1;
			}
		}
		else if(start_id>=14&&start_id<=30)
		{
			loc_start_id += (id_cnt-1);
			if(loc_start_id>30)
			{
				// status_temp = 2;
				loc_cnt_id = 30-14 +1;
			}
		}
		else
		{
			status_temp = 2;
		}		
	}
	else
	{
		status_temp = 3;
	}
	if(status_temp==0)
	{
		for (j = 0; j < loc_cnt_id;j++)
		{
			local_data = niu_data_get_pdata_loc(NIU_DB, start_id+j);
			id_data_len = niu_data_get_datalen_loc(NIU_DB, start_id+j);
			for (i = 0; i < id_data_len;i++)
			{
				*(dst_str++) = *(local_data++);
			}
		}
	}
	else if(status_temp==2)
	{

	}
	else if(status_temp==3)
	{

	}
	
	

}


NVOID niu_j1939_creat_respone(NIU_DATA_ADDR_BASE base,NUINT16 addr_offset,NUINT16 len,NUINT32 PGN,void(*update_callback)())
{
	NUINT8 *local_data;
	NUINT16 dev_data_len;
	if(base==NIU_DB)
	{
		local_data=local_niu_value.db.buff+addr_offset;
		dev_data_len=sizeof(NDATA_VALUE_DB);
		if(addr_offset+1>dev_data_len)
		{
			ITE_TRACE("%s:%s err!\n",__func__,__LINE__);
		}
		if(local_data+len>local_niu_value.db.buff+dev_data_len)
		{

			len=local_niu_value.db.buff+dev_data_len-local_data;
		}	
	}
	else
	{
		ITE_TRACE("%s:base err!\n",__func__);
		return; 
	}

	TASK_SUSPEND();
	//J1939_Create_Response(local_data,len,PGN,update_callback);
	TASK_RESUME();

}



/* 
*@notice	！！！本函数未对数据越界进行处理，编程人员请不要调用次函数，可用niu_data_write_data函数对数据表写操作，本函数用于C模板导出函数里调用，
*/
NVOID write_local_niu_value(NIU_DATA_ADDR_BASE base,NUINT16 addr_offset,NUINT16 byte_offset,void *databuff,NUINT16 len)
{

	NUINT8 *local_data;
	NUINT16 dev_data_len;
	if(base==NIU_DB)
	{
		local_data=local_niu_value.db.buff+addr_offset+byte_offset;
			dev_data_len=sizeof(NDATA_VALUE_DB);
			if(addr_offset+1>dev_data_len)
			{
				ITE_TRACE("%s:%s err!\n",__func__,__LINE__);
			}
			if(local_data+len>local_niu_value.db.buff+dev_data_len)
			{

				len=local_niu_value.db.buff+dev_data_len-local_data;
			}	

	}
	else
	{
			ITE_TRACE("%s:base err!\n",__func__);
			return; 
	}
	
	GIVE_WRITE_KEY_LOC();
	nmemcpy(local_data, databuff, len);
    GIVE_WRITE_KEY_LOC();
}
#endif
