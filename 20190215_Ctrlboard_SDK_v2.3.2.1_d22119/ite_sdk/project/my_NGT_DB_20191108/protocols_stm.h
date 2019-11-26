/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-08-14 10:20:59
 * @LastEditTime: 2019-08-14 15:13:30
 * @LastEditors: Please set LastEditors
 */
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


// 0-----多云 					Cloudy
// 1-----晴						Fine
// 2-----阴						Gloomy
// 3-----小雨					Drizzle
// 4-----中雨					Mod_rain
// 5-----大雨					Pour
// 6-----暴雨					Downpour
// 7-----大暴雨					Rainstorm
// 8-----特大暴雨				Tor_rain
// 9-----阵雨					Shower
// 10-----雷阵雨				ThunderShower
// 11-----小雨-中雨				ThunderShower
// 12-----中雨-大雨				ThunderShower
// 13-----大雨-暴雨				ThunderShower
// 14-----暴雨-大暴雨			ThunderShower
// 15-----大暴雨-特大暴雨       ThunderShower
// 16-----小雪					light snow
// 17-----中雪					ThunderShower
// 18-----大雪					heavy snow
// 19-----暴雪					blizzard
// 20-----小雪-中雪				ThunderShower
// 21-----中雪-大雪				ThunderShower
// 22-----大雪-暴雪				ThunderShower
// 23-----阵雪					snow flurries         
// 24-----雨夹雪				mixed rain and snow
// 25-----冻雨					freezing rain
// 26-----雷阵雨伴有冰雹		 ThunderShower and hail
// 27-----雾					foggy
// 28-----霾					haze
// 29-----轻霾					haze
// 30-----龙卷风				tornado
// 31-----沙尘暴				ThunderShower
// 32-----强沙尘暴				ThunderShower
// 33-----扬沙					ThunderShower
// 34-----浮尘					dust
// 35-----飑					ThunderShower
// 36-----弱高吹雪				blowing snow
         
/*       
         
   "未知":      
   "晴":        
   "少云":      
   "晴间多云":  
   "多云":      
   "阴":        
   "有风":      
   "平静":      
   "微风":      
   "和风":      
   "清风":      
   "强风/劲风": 
   "疾风":      
   "大风":      
   "烈风":      
   "风暴":      
   "狂爆风":    
   "飓风":      
   "热带风暴":  
   "阵雨":      
   "雷阵雨":    
   "雷阵雨并伴有冰雹": 
   "小雨":       
   "中雨":       
   "大雨":       
   "暴雨":       
   "大暴雨":     
   "特大暴雨":   
   "强阵雨":     
   "强雷阵雨":   
   "极端降雨":   
   "毛毛雨/细雨":
   "雨":        
   "小雨-中雨":    
   "中雨-大雨":    
   "大雨-暴雨":    
   "暴雨-大暴雨":  
   "大暴雨-特大暴雨": 
   "雨雪天气":  
   "雨夹雪":    
   "阵雨夹雪":  
   "冻雨":      
   "雪":        
   "阵雪":      
   "小雪":      
   "中雪":      
   "大雪":      
   "暴雪":      
   "小雪-中雪": 
   "中雪-大雪": 
   "大雪-暴雪": 
   "浮尘":      
   "扬沙":      
   "沙尘暴":    
   "强沙尘暴":  
   "龙卷风":    
   "雾":        
   "浓雾":      
   "强浓雾":    
   "轻雾":      
   "大雾":      
   "特强浓雾":  
   "霾":        
   "中度霾":    
   "重度霾":    
   "严重霾":    
   "热":        
   "冷":        
         
switch (weatherCode) {
    case "101": // 多云
    case "102": // 少云
    case "103": // 晴间多云
    case "200": //有风
    case "201": //平静
    case "202": //微风
    case "203": //和风
    case "204": //清风
        多云
        break;
    case "205": //强风
    case "206": //疾风
    case "207": //大风
    case "208": //烈风
    case "209": //风暴
    case "210": //狂暴风
    case "211": //飓风
    case "212": //龙卷风
    case "213": //热带风暴
        龙卷风
        break;
         
    case "305": //小雨
    case "309": //毛毛雨
    case "306": //中雨
    case "314": //小到中雨
        小雨
        break;
         
    case "300": //阵雨
        阵雨
        break;
    case "301": //强阵雨
    case "302": //雷阵雨
    case "315": //中到大雨
        雷阵雨
        break;
    case "304": //雷阵雨伴有冰雹
        雷阵雨伴有冰雹
        break;
    case "303": //强雷阵雨
    case "307": //大雨
    case "399": //雨
    case "308": //极端降雨
    case "310": //暴雨
    case "311": //大暴雨
    case "312": //特大暴雨
    case "316": //大到暴雨
    case "317": //暴雨到大暴雨
    case "318": //大暴雨到特大暴雨
        大暴雨到特大暴雨
        break;
    case "313": //冻雨
        冻雨
        break;
    case "400": //小雪
    case "401": //中雪
    case "402": //大雪
    case "403": //暴雪
    case "404": //雨夹雪
    case "406": //阵雨夹雪
        if (type == SMALL_TYPE) {
            resId = lightMode ? R.mipmap.yu_jia_xue_small : R.mipmap.yu_jia_xue_small_black;
        } else if (type == BIG_TYPE) {
            resId = R.mipmap.yu_jia_xue_big;
        }
        break;
    case "405": //雨雪天气
    case "407": //阵雪
        if (type == SMALL_TYPE) {
            resId = lightMode ? R.mipmap.zheng_xue_small : R.mipmap.zheng_xue_small_black;
        } else if (type == BIG_TYPE) {
            resId = R.mipmap.zheng_xue_big;
        }
        break;
    case "408": //小到中雪
    case "409": //中到大雪
    case "410": //大到暴雪
    case "499": //雪
        if (type == SMALL_TYPE) {
            resId = lightMode ? R.mipmap.xiao_xue_small : R.mipmap.xiao_xue_small_black;
        } else if (type == BIG_TYPE) {
            resId = R.mipmap.xiao_xue_big;
        }
        break;
    case "500": //薄雾
    case "501": //雾
    case "509": //浓雾
    case "510": //强浓雾
    case "502": //霾
        雾
        break;
    case "504": //浮尘
        浮尘
        break;
    case "503": //扬沙
        扬沙
        break;
    case "511": //中度霾
    case "512": //重度霾
    case "513": //严重霾
    case "514": //大雾
    case "515": //特强浓雾
        中度霾
        break;
    case "507": //沙尘暴
    case "508": //强沙尘暴
        沙尘暴
        break;
    case "104": //阴
    case "901": //冷
        阴
        break;
    case "900": //热
    case "100": //晴
        晴
        break;
}        
         
         
         
*/       
/****************************************************************************** */
         
/*******************************CAR TEST MODE DEFINE*******************************/
         
/*******************************CAR TEST MODE DEFINE*******************************/
         
typedef union
{        
    struct
    {    
        unsigned char _command : 5; //
        unsigned char _continuous : 1;
        unsigned char _ack : 1;
        unsigned char _direction : 1;
    } CtrlCommand;
    unsigned char car_command;
    } CMD_Typedef;
         
         
#ifdef __cplusplus
}        
#endif   
         
#endif /* _PROTOCOL_STM_H */
         
/** @} */ // end of template
         