/** @file
 * Template project definition.
 *
 */
/** @defgroup template
 *  @{
 */
#ifndef _PROTOCOL_STM_H
#define _PROTOCOL_STM_H

#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

//#define EUROPE_UNIT  //if the unit ship to europe, please enable the macro define

/*******************************CAR STATUS1 DEFINE*******************************/
#define CAR_INCHG                       (0x0001) //充电状态
#define CAR_RUN                         (0x0002) //行驶状态
#define CAR_PARKING                     (0x0004) //驻车状态 停车状态
#define CAR_EBS                         (0x0008) //EBS状态(能量回收功能开关)

#define CAR_RECOVERY                    (0x0010) //能量回收状态
#define CAR_ECO                         (0x0020) //ECO状态(最佳巡航)
#define CAR_UNIT                        (0x0040) //公英制（1：英制；0：公制）
#define CAR_FAULT_CODE                  (0x0080) //故障状态

#define CAR_CONSTANT                    (0x0100) //CCS定速巡航
#define CAR_VCU_UPDATE                  (0x0200) //中控固件升级
#define CAR_REMOTE_LOCK                 (0x0400) //远程锁车
#define CAR_AUTO_HEADLIGHT              (0x0800) //自动大灯

#define CAR_GPS_STRONG                  (0x1000) //GPS信号强    良
#define CAR_GPS_WEAK                    (0x2000) //GPS信号弱    一般
#define CAR_GPS_NO                      (0x3000) //GPS信号无    不良
#define CAR_GPS_MASK                    (0x3000) //            未定位

#define CAR_GSM_STRONG                  (0x4000) //GSM信号强    良
#define CAR_GSM_WEAK                    (0x8000) //GSM信号弱    一般
#define CAR_GSM_NO                      (0xC000) //GSM信号无    不良
#define CAR_GSM_MASK                    (0xC000) //            未联网

#define CAR_GSM_NULL                    (0x0000) //            未联网
#define CAR_GPS_NULL                    (0x0000) //            未定位    

/*******************************CAR STATUS2 DEFINE*******************************/
#define CAR_EX_BT1_ON                   (0x0001) //电池1存在
#define CAR_EX_BT2_ON                   (0x0002) //电池2存在
#define CAR_EX_FULLSPEED                (0x0004) //全速模式 控制器全速状态 双电均衡
#define CAR_EX_BT1_INCHARGE             (0x0008) //电池1充电中
#define CAR_EX_BT2_INCHARGE             (0x0010) //电池2充电中
#define CAR_EX_CLOUD_CONNECTED          (0x0020) //云连接
#define CAR_EX_HEALTH_SCORE    	        (0x0040) //健康评分
#define CAR_EX_AVGPOWER_FLAG   	        (0x0080) //平均功率更新标记

#define CAR_EX_BT_FAULT    	            (0x0100) //动力蓄电池故障
#define CAR_EX_MOTOR_OVERHEAT           (0x0200) //电动机及控制器过热
// #define CAR_EX_DANGER_WARNING		(0x0400) //危险警告
#define CAR_EX_DOUBLE_FLASH             (0x0400) //双闪状态
#define CAR_EX_SYSTEM_FAULT             (0x0800) //系统故障
#define CAR_EX_TEMPLE_STATUS            (0x1000) //边撑落下状态
#define CAR_EX_START_MISTAKENLY         (0x2000) //启动键误触
#define CAR_EX_BAT_LOW			        (0x4000) //电池电量低


/*******************************CAR CMD DEFINE*******************************/
#define CAR_CMD_IMPERIAL_UNIT           (0x00000002) //英制
#define CAR_CMD_METRIC_UNIT             (0x00000001) //公制


/********************************WEATHER*************************************/

typedef enum
{
    W000 = 0,             //"未知":      
    W001,                 //"晴":        
    W002,                 //"少云":      
    W003,                 //"晴间多云":  
    W004,                 //"多云":      
    W005,                 //"阴":        
    W006,                 //"有风":      
    W007,                 //"平静":      
    W008,                 //"微风":      
    W009,                 //"和风":      
    W010,                 //"清风":      
    W011,                 //"强风/劲风": 
    W012,                 //"疾风": 
	W013,                 //"大风":  "烈风":    
    W014,                 //"风暴":      
    W015,                 //"狂爆风":    
    W016,                 //"飓风":      
    W017,                 //"热带风暴":  
    W018,                 //"阵雨":      
    W019,                 //"雷阵雨":    
    W020,                 //"雷阵雨并伴有冰雹":
    W021,                 //"小雨":       
    W022,                 //"中雨":       
    W023,                 //"大雨":       
    W024,                 //"暴雨":       
    W025,                 //"大暴雨":     
    W026,                 //"特大暴雨":   
    W027,                 //"强阵雨":     
    W028,                 //"强雷阵雨":   
    W029,                 //"极端降雨":   
    W030,                 //"毛毛雨/细雨":
    W031,                 //"雨":        
    W032,                 //"小雨-中雨":    
    W033,                 //"中雨-大雨":    
    W034,                 //"大雨-暴雨":    
    W035,                 //"暴雨-大暴雨":  
    W036,                 //"大暴雨-特大暴雨": 
    W037,                 //"雨雪天气":  
    W038,                 //"雨夹雪":    
    W039,                 //"阵雨夹雪":  
    W040,                 //"冻雨":      
    W041,                 //"雪":        
    W042,                 //"阵雪":      
    W043,                 //"小雪":      
    W044,                 //"中雪":      
    W045,                 //"大雪":      
    W046,                 //"暴雪":      
    W047,                 //"小雪-中雪": 
    W048,                 //"中雪-大雪": 
    W049,                 //"大雪-暴雪": 
    W050,                 //"浮尘":      
    W051,                 //"扬沙":      
    W052,                 //"沙尘暴":    
    W053,                 //"强沙尘暴":  
    W054,                 //"龙卷风":    
    W055,                 //"雾":        
    W056,                 //"浓雾":      
    W057,                 //"强浓雾":    
    W058,                 //"轻雾":      
    W059,                 //"大雾":      
    W060,                 //"特强浓雾":  
    W061,                 //"霾":        
    W062,                 //"中度霾":    
    W063,                 //"重度霾":    
    W064,                 //"严重霾":    
    W065,                 //"热":        
    W066,                 //"冷":        
    WMAX                         
} IOT_WEATHER_CODE;

typedef enum
{
    NIUW000 = 0,             //多云 			
    NIUW001,                 //晴				
    NIUW002,                 //阴				
    NIUW003,                 //小雨			
    NIUW004,                 //中雨			
    NIUW005,                 //大雨			
    NIUW006,                 //暴雨			
    NIUW007,                 //大暴雨			
    NIUW008,                 //特大暴雨		
    NIUW009,                 //阵雨			
    NIUW010,                 //雷阵雨		
    NIUW011,                 //小雨-中雨		
    NIUW012,                 //中雨-大雨		
	NIUW013,                 //大雨-暴雨		
    NIUW014,                 //暴雨-大暴雨	
    NIUW015,                 //大暴雨-特大暴雨
    NIUW016,                 //小雪			
    NIUW017,                 //中雪			
    NIUW018,                 //大雪			
    NIUW019,                 //暴雪			
    NIUW020,                 //小雪-中雪		
    NIUW021,                 //中雪-大雪		
    NIUW022,                 //大雪-暴雪		
    NIUW023,                 //阵雪			
    NIUW024,                 //雨夹雪		
    NIUW025,                 //冻雨			
    NIUW026,                 //雷阵雨伴有冰雹
    NIUW027,                 //雾			
    NIUW028,                 //霾			
    NIUW029,                 //轻霾			
    NIUW030,                 //龙卷风		
    NIUW031,                 //沙尘暴		
    NIUW032,                 //强沙尘暴		
    NIUW033,                 //扬沙			
    NIUW034,                 //浮尘			
    NIUW035,                 //飑			
    NIUW036,                 //弱高吹雪		
    NIUWMAX                         
} NIU_WEATHER_CODE;  
        
/****************************************************************************** */
         
/*******************************CAR TEST MODE DEFINE*******************************/
         
/*******************************CAR TEST MODE DEFINE*******************************/
   
         
#ifdef __cplusplus
}        
#endif   
         
#endif /* _PROTOCOL_STM_H */
         
/** @} */ // end of template
         