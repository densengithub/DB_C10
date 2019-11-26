/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-08-14 10:20:59
 * @LastEditTime: 2019-08-15 11:14:09
 * @LastEditors: Please set LastEditors
 */
#include <sys/time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "SDL/SDL.h"
#include "ite/itu.h"
#include "ite/itp.h"
#include "scene.h"
#include "project.h"

#define FPS_ENABLE
#define GESTURE_THRESHOLD 40
//#define DOUBLE_KEY_ENABLE

extern ITUActionFunction actionFunctions[];

// scene
ITUScene theScene;
static SDL_Window *window;
static ITUSurface *screenSurf;
static int periodPerFrame;
static bool isReady;
StorageAction action = STORAGE_USB_DEVICE_REMOVED;

void SceneInit(void)
{
    window = SDL_CreateWindow("Template", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, CFG_LCD_WIDTH, CFG_LCD_HEIGHT, 0);
    if (!window)
    {
        printf("Couldn't create window: %s\n", SDL_GetError());
        return;
    }

    // init itu
    ituLcdInit();

#ifdef CFG_M2D_ENABLE
    ituM2dInit();
    ituFrameFuncInit();
#else
    ituSWInit();
#endif
    //ituM2dInit();

    ituSceneInit(&theScene, NULL);

#ifdef CFG_ENABLE_ROTATE
    ituSceneSetRotation(&theScene, ITU_ROT_90, ithLcdGetWidth(), ithLcdGetHeight());
#endif

#ifdef CFG_PLAY_VIDEO_ON_BOOTING
#ifndef CFG_BOOT_VIDEO_ENABLE_WINDOW_MODE
#ifndef CFG_ENABLE_ROTATE
    PlayVideo(0, 0, ithLcdGetWidth(), ithLcdGetHeight(), CFG_BOOT_VIDEO_BGCOLOR, CFG_BOOT_VIDEO_VOLUME);
#else
    PlayVideo(0, 0, ithLcdGetHeight(), ithLcdGetWidth(), CFG_BOOT_VIDEO_BGCOLOR, CFG_BOOT_VIDEO_VOLUME);
#endif
#else
    PlayVideo(CFG_VIDEO_WINDOW_X_POS, CFG_VIDEO_WINDOW_Y_POS, CFG_VIDEO_WINDOW_WIDTH, CFG_VIDEO_WINDOW_HEIGHT, CFG_BOOT_VIDEO_BGCOLOR, CFG_BOOT_VIDEO_VOLUME);
#endif
    WaitPlayVideoFinish();
#endif

#ifdef CFG_PLAY_MJPEG_ON_BOOTING
#ifndef CFG_BOOT_VIDEO_ENABLE_WINDOW_MODE
#ifndef CFG_ENABLE_ROTATE
    PlayMjpeg(0, 0, ithLcdGetWidth(), ithLcdGetHeight(), CFG_BOOT_VIDEO_BGCOLOR, 0);
#else
    PlayMjpeg(0, 0, ithLcdGetHeight(), ithLcdGetWidth(), CFG_BOOT_VIDEO_BGCOLOR, 0);
#endif
#else
    PlayMjpeg(CFG_VIDEO_WINDOW_X_POS, CFG_VIDEO_WINDOW_Y_POS, CFG_VIDEO_WINDOW_WIDTH, CFG_VIDEO_WINDOW_HEIGHT, CFG_BOOT_VIDEO_BGCOLOR, 0);
#endif
    WaitPlayMjpegFinish();
#endif

    screenSurf = ituGetDisplaySurface();

    ituFtInit();
    // ituFtLoadFont(0, CFG_PRIVATE_DRIVE ":/font/" CFG_FONT_FILENAME, ITU_GLYPH_8BPP);
    ituFtLoadFont(0, CFG_PRIVATE_DRIVE ":/font/DINNextLTPro-Medium.ttf", ITU_GLYPH_8BPP);
    ituFtLoadFont(1, CFG_PRIVATE_DRIVE ":/font/DINNextLTPro-Regular.ttf", ITU_GLYPH_8BPP);
    ituFtLoadFont(2, CFG_PRIVATE_DRIVE ":/font/DINNextLTPro-BoldItalic.ttf", ITU_GLYPH_8BPP);
    ituFtLoadFont(3, CFG_PRIVATE_DRIVE ":/font/DINNextLTPro-Bold.ttf", ITU_GLYPH_8BPP);
    ituFtLoadFont(4, CFG_PRIVATE_DRIVE ":/font/DINNextLTPro-MediumItalic.ttf", ITU_GLYPH_8BPP);
    // ituFtLoadFont(5, CFG_PRIVATE_DRIVE ":/font/SourceHanSansCN-Medium.ttf", ITU_GLYPH_8BPP);

    // ituSceneInit(&theScene, NULL);
    ituSceneSetFunctionTable(&theScene, actionFunctions);

    theScene.leftKey = SDLK_LEFT;
    theScene.upKey = SDLK_UP;
    theScene.rightKey = SDLK_RIGHT;
    theScene.downKey = SDLK_DOWN;

    periodPerFrame = MS_PER_FRAME;

    isReady = false;
}

void SceneExit(void)
{
    if (theScene.root)
    {
        ituSceneExit(&theScene);
    }
    ituFtExit();

#ifdef CFG_M2D_ENABLE
    ituM2dExit();
#ifdef CFG_VIDEO_ENABLE
    ituFrameFuncExit();
#endif // CFG_VIDEO_ENABLE
#else
    ituSWExit();
#endif // CFG_M2D_ENABLE

    SDL_DestroyWindow(window);
}

void SceneLoad(void)
{
    uint32_t tick1, tick2;
    // load itu file
    tick1 = SDL_GetTicks();
    // load itu file
    ituSceneLoadFile(&theScene, CFG_PRIVATE_DRIVE ":/template.itu");

    tick2 = SDL_GetTicks();
    printf("itu loading time: %dms\n", tick2 - tick1);
}

void SceneSetReady(bool ready)
{
    isReady = ready;
}

bool SceneReadReady(void)
{
    return isReady;
}

bool module_test = true;
bool board_test = false;
static unsigned short Check_temp = 0;
void Layer_TestMode_Task(ExternalEvent_layer *ev_testmode)
{
    unsigned short testmode = 0;
    unsigned char test_temp = 0;
    bool ret_check = false;

    // bool module_test = true;
    // printf("testmode_status=0x%02x..\t\n",testmode_status);
    printf(APP_VERSION "\t\n");
    printf(CFG_NIU_PANEL_VERSION "\t\n");
    printf("DATE:"__DATE__ "\t\n");
    testmode = ev_testmode->layer_displaydata.DashBoardInfo.test_mode;
    printf("test_mode=0x%04x..\t\n", testmode);
    // printf("test_psensor2_ad=0x%04x..\t\n", ev_testmode->layer_displaydata.DashBoardInfo.test_psensor2_ad);
    // printf("test_ntc_ad=0x%04x..\t\n", ev_testmode->layer_displaydata.DashBoardInfo.test_ntc_ad);
    // printf("test_temperature=0x%02x..\t\n", ev_testmode->layer_displaydata.DashBoardInfo.test_temperature);
    // printf("test_bt_flash_485=0x%02x..\t\n", ev_testmode->layer_displaydata.DashBoardInfo.test_bt_flash_485);
    // printf("test_input=0x%02x..\t\n", ev_testmode->layer_displaydata.DashBoardInfo.test_input);
    // printf("test_led=0x%04x..\t\n", ev_testmode->layer_displaydata.DashBoardInfo.test_led);
    // printf("test_psensor1_ad=0x%04x..\t\n", ev_testmode->layer_displaydata.DashBoardInfo.test_psensor1_ad);
    if (testmode & 0x4000)
    {
        module_test = true;
        board_test = true;
    }
    else
    {
        board_test = false;
        if (testmode & 0x8000)
        {
            module_test = true;
        }
        else
        {
            module_test = false;
        }
    }

    if (!testmode_status)
    {
        ituLayerGoto(testmodeLayer);
        return;
    }
    else
    {
        if (!board_test)
        {
            return;
        }
        testmodeLayer_module_Display(board_test, 0);
        if (testmode & 0x0001)
        {
            ret_check = testmodeLayer_psensor2_DisplayConfig(ev_testmode->layer_displaydata.DashBoardInfo.test_psensor2_ad);
            if (ret_check)
            {
                Check_temp |= ((unsigned short)1 << 3);
            }
            else
            {
                /* code */
                Check_temp &= ~((unsigned short)1 << 3);
            }
        }
        else
        {
            /* code */
            testmodeLayer_module_Display(false, 3 + 1);
        }

        if (testmode & 0x0002)
        {
            ret_check = testmodeLayer_ntc_DisplayConfig(ev_testmode->layer_displaydata.DashBoardInfo.test_ntc_ad, ev_testmode->layer_displaydata.DashBoardInfo.test_temperature);
            if (ret_check)
            {
                Check_temp |= ((unsigned short)1 << 4);
            }
            else
            {
                /* code */
                Check_temp &= ~((unsigned short)1 << 4);
            }
        }
        else
        {
            /* code */
            testmodeLayer_module_Display(false, 3 + 2);
        }

        test_temp = ev_testmode->layer_displaydata.DashBoardInfo.test_bt_flash_485 & 0x07;
        if (testmode & 0x0004)
        {
            if (test_temp & 0x04)
            {
                Check_temp |= ((unsigned short)1 << 5);
                testmodeLayer_bluetooth_DisplayConfig(2);
            }
            else
            {
                Check_temp &= ~((unsigned short)1 << 5);
                testmodeLayer_bluetooth_DisplayConfig(1);
            }
        }
        else
        {
            /* code */
            testmodeLayer_module_Display(false, 3 + 5);
        }

        if (testmode & 0x0008)
        {
            if (test_temp & 0x02)
            {
                Check_temp |= ((unsigned short)1 << 6);
                testmodeLayer_spiflash_DisplayConfig(2);
            }
            else
            {
                Check_temp &= ~((unsigned short)1 << 6);
                testmodeLayer_spiflash_DisplayConfig(1);
            }
        }
        else
        {
            testmodeLayer_module_Display(false, 3 + 4);
        }

        if (testmode & 0x0010)
        {
            if (test_temp & 0x01)
            {
                Check_temp |= ((unsigned short)1 << 7);
                testmodeLayer_rs485_DisplayConfig(2);
            }
            else
            {
                Check_temp &= ~((unsigned short)1 << 7);
                testmodeLayer_rs485_DisplayConfig(1);
            }
        }
        else
        {
            testmodeLayer_module_Display(false, 3 + 3);
        }

        if (testmode & 0x0020)
        {
            testmodeLayer_input_DisplayConfig((int)ev_testmode->layer_displaydata.DashBoardInfo.test_input);
        }
        else
        {
            testmodeLayer_module_Display(false, 3 + 6);
        }

        if (testmode & 0x0040)
        {
            testmodeLayer_led_DisplayConfig((int)ev_testmode->layer_displaydata.DashBoardInfo.test_led);
        }
        else
        {
            testmodeLayer_module_Display(false, 3 + 7);
        }
    }
}

void Layer_MainMode_Task(ExternalEvent_layer *ev_testmode)
{
    int ret = 0, num = 0, j = 0, i = 0;
    int soc_bat = 0;
    unsigned short status_menu = 0;
    int type_action = (int)STATUS_SHOW;
    unsigned short status_temp1 = 0;
    unsigned short status_temp2 = 0;
    unsigned short status_new_car = 0, status_warn_code = 0, status_func_code = 0;
    status_temp1 = ev_layer2.layer_displaydata.ScooterInfo.car_status1;
    status_temp2 = ev_layer2.layer_displaydata.ScooterInfo.car_status2;
    // printf("sta1=0x%04x\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_status1);
    // printf("car_gear=0x%02x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_gear);
    // printf("car_speed=%d..\r\n", ev_layer2.layer_displaydata.ScooterInfo.car_speed);
    // printf("car_mileage=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_mileage);
    // printf("car_chargecurrent=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_chargecurrent);
    // printf("car_dischargecurrent=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_dischargecurrent);
    // printf("car_bat1_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc);
    // printf("car_full_c_t=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_full_c_t);
    // printf("time_hour=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.time_hour);
    // printf("time_minute=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.time_minute);
    // printf("car_fault_code=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_fault_code);
    // printf("car_bat2_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc);
    // printf("sta2=0x%04x\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_status2);
    // printf("car_total_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_total_soc);

    // printf("weather_code=0x%04x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.weather_code);
    // printf("air_temperature=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.air_temperature);
    // printf("rtpor=%d\t\n", ev_layer2.layer_displaydata.ScooterInfo.realtime_input_power);

    // printf("estimated_remaining_mileage=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.estimated_remaining_mileage);
    // printf("car_dischargecurrent=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_dischargecurrent);
    // printf("car_bat1_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc);
    // printf("car_full_c_t=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_full_c_t);
    // printf("time_hour=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.time_hour);
    // printf("time_minute=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.time_minute);
    // printf("car_fault_code=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_fault_code);
    // printf("car_bat2_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc);
    // printf("car_status2=0x%04x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_status2);
    // printf("realtime_input_power=%d..\r\n", ev_layer2.layer_displaydata.ScooterInfo.realtime_input_power);
    // printf("est=%d\t\n", ev_layer2.layer_displaydata.ScooterInfo.remaining_mileage);

    // for (i = 0; i < sizeof(ScooterInfo_Typedef); i++)
    // {
    //     printf("Disp[%d]=0x%02x\t\n", i, ev_layer2.layer_displaydata.DisplayDatabuffer[i]);
    // }

    status_menu = status_temp2 & CAR_EX_TEMPLE_STATUS;
    if (!charge_status)
    {
        // printf("car_gear=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_gear);
        mainLayer_DriveMode_DisplayConfig((user_drivemode_type)ev_layer2.layer_displaydata.ScooterInfo.car_gear);

        // mainLayer_Sidestand_DisplayConfig(!status_menu);
        mainLayer_SideMistake_DisplayConfig(((status_temp2 & CAR_EX_START_MISTAKENLY) ? (true) : (false)));
        status_menu = status_temp1 & CAR_PARKING;
        // printf("status_menu=0x%04x..\t\n", status_menu);
        mainLayer_Ready_DisplayConfig(!status_menu, ((status_temp2 & CAR_EX_TEMPLE_STATUS) ? (true) : (false)));

        mainLayer_Workstatus_DisplayConfig(ICON_CruiseControl, (status_temp1 & CAR_CONSTANT) ? STATUS_SHOW : STATUS_HIDDEN);
        mainLayer_Workstatus_DisplayConfig(ICON_Autolight, (status_temp1 & CAR_AUTO_HEADLIGHT) ? STATUS_SHOW : STATUS_HIDDEN);
        mainLayer_Workstatus_DisplayConfig(ICON_EnergyConservation, (status_temp1 & CAR_ECO) ? STATUS_SHOW : STATUS_HIDDEN);
        mainLayer_Workstatus_DisplayConfig(ICON_CloudService, (status_temp2 & CAR_EX_CLOUD_CONNECTED) ? STATUS_SHOW : STATUS_HIDDEN);
        mainLayer_Workstatus_DisplayConfig(ICON_EnergyRecovery, (status_temp1 & CAR_RECOVERY) ? STATUS_SHOW : STATUS_HIDDEN);

        status_menu = status_temp1 & CAR_UNIT;
        type_action = (int)((status_menu) ? (STATUS_IMPERIAL_UNIT) : (STATUS_METRIC_UNIT));
        if (status_temp1 & CAR_PARKING)
        {
            mainLayer_LastTripTime_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.last_cycling_time);
            mainLayer_LastTripAvgSpeed_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.last_avg_speed, (user_unitlist_type)type_action);
            mainLayer_LastTripMileage_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.last_trip_mileage, (user_unitlist_type)type_action);
        }
        else
        {
            mainLayer_AverageEnergy_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.average_power_trip, (user_unitlist_type)type_action);
            // mainLayer_RealtimeEnergyCurve_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_speed);
        }

        // status_menu = status_temp1 & CAR_UNIT;
        // type_action = (int)((status_menu) ? (STATUS_IMPERIAL_UNIT) : (STATUS_METRIC_UNIT));
        
        speed_step_p = ev_layer2.layer_displaydata.ScooterInfo.car_speed;
        // mainLayer_SpeedValue_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_speed, (user_unitlist_type)type_action);
        mainLayer_SpeedUnit_DisplayConfig((user_unitlist_type)type_action, STATUS_SHOW);

        if (!(status_temp1 & CAR_PARKING))
        {
            if (status_temp2 & CAR_EX_AVGPOWER_FLAG)
            {
                mainLayer_RealtimeEnergyCurve_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.average_power);
                // mainLayer_RealtimeEnergyCurve_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_speed);
            }
            // mainLayer_RealtimeEnergy_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.realtime_input_power);
            // mainLayer_RealtimeEnergy_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.realtime_input_power, status_temp1 & CAR_RECOVERY);
            power_step_p = ev_layer2.layer_displaydata.ScooterInfo.realtime_input_power;
        }
    }
    else
    {
        chargeLayer_SideMistake_DisplayConfig(((status_temp2 & CAR_EX_START_MISTAKENLY) ? (true) : (false)));
        chargeLayer_Sidestand_DisplayConfig(!status_menu, ((status_temp2 & CAR_EX_TEMPLE_STATUS) ? (true) : (false)));
        // chargeLayer_Sidestand_DisplayConfig(!status_menu);
        chargeLayer_ChargeTime_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.charge_time);
    }

    mainLayer_Time_DisplayConfig(TEXT_TIME_HOUR, ev_layer2.layer_displaydata.ScooterInfo.time_hour);
    mainLayer_Time_DisplayConfig(TEXT_TIME_MINUTE, ev_layer2.layer_displaydata.ScooterInfo.time_minute);
    // mainLayer_Temperature_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.air_temperature);
    // mainLayer_Weather_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.weather_code, STATUS_SHOW);
    mainLayer_WeatherTemperature_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.weather_code,ev_layer2.layer_displaydata.ScooterInfo.air_temperature);
    
    status_menu = status_temp2 & CAR_EX_START_MISTAKENLY;
    // ScreenSetBrightness(ev_layer2.layer_displaydata.ScooterInfo.lightsenor);
    switch (status_temp1 & CAR_GPS_MASK)
    {
    case CAR_GPS_NO:
        type_action = (int)STATUS_SIGNAL_LEVEL1;
        break;
    case CAR_GPS_WEAK:
        type_action = (int)STATUS_SIGNAL_LEVEL2;
        break;
    case CAR_GPS_STRONG:
        type_action = (int)STATUS_SIGNAL_LEVEL3;
        break;
    case CAR_GPS_NULL:
        type_action = (int)STATUS_SIGNAL_LEVEL0;
        break;
    default:
        type_action = 0;
        break;
    }
    mainLayer_NetworksSignal_DisplayConfig(ICON_SatelliteSignal, (user_indicationaction_type)type_action);

    switch (status_temp1 & CAR_GSM_MASK)
    {
    case CAR_GSM_NO:
        type_action = (int)STATUS_SIGNAL_LEVEL1;
        break;
    case CAR_GSM_WEAK:
        type_action = (int)STATUS_SIGNAL_LEVEL2;
        break;
    case CAR_GSM_STRONG:
        type_action = (int)STATUS_SIGNAL_LEVEL3;
        break;
    case CAR_GSM_NULL:
        type_action = (int)STATUS_SIGNAL_LEVEL0;
        break;
    default:
        type_action = 0;
        break;
    }
    mainLayer_NetworksSignal_DisplayConfig(ICON_MobileNetSignal, (user_indicationaction_type)type_action);

#if 0
    // printf("status_temp2&BAT1_EXIST:0x%04x..\t\n", status_temp1 & BAT1_EXIST);
    switch (status_temp2 & (CAR_EX_BT1_ON | CAR_EX_BT2_ON))
    {
    case CAR_EX_BT1_ON:
        //    printf("CAR_EX_BT1_ON..\t\n");
        soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc;
        mainLayer_BatterySingle_DisplayConfig(soc_bat);
        break;
    case CAR_EX_BT2_ON:
        // printf("CAR_EX_BT2_ON..\t\n");
        soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc;
        mainLayer_BatterySingle_DisplayConfig(soc_bat);
        break;
    case (CAR_EX_BT1_ON | CAR_EX_BT2_ON): //0x0003:CAR_EX_BT1_ON|CAR_EX_BT2_ON
        // printf("CAR_EX_BT1_ON..CAR_EX_BT2_ON..\t\n");
        mainLayer_BatteryDual_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc, ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc);
        soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_total_soc;
        break;
    default:
        // printf("CAR_EX_BT DEFAULT..\t\n");
        soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_total_soc;
        mainLayer_BatterySingle_DisplayConfig(soc_bat);
        break;
    }
    mainLayer_BatteryCapacity_DisplayConfig(soc_bat);

#else
    status_menu = status_temp2 & (CAR_EX_BT1_ON | CAR_EX_BT2_ON);
    if (status_menu == (CAR_EX_BT1_ON | CAR_EX_BT2_ON))
    {
        mainLayer_BatteryBalance_DisplayConfig((status_temp2 & CAR_EX_FULLSPEED) ? (true) : (false));
        mainLayer_BatteryDual_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc, ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc);
        soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_total_soc;
    }
    else
    {
        switch (status_menu)
        {
        case CAR_EX_BT1_ON:
            soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc;
            break;
        case CAR_EX_BT2_ON:
            soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc;
            break;
        default:
            soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_total_soc;
            break;
        }
        mainLayer_BatterySingle_DisplayConfig(soc_bat);
    }
    mainLayer_BatteryCapacity_DisplayConfig(soc_bat);

#endif

    status_menu = status_temp1 & CAR_UNIT;
    type_action = (int)((status_menu) ? (STATUS_IMPERIAL_UNIT) : (STATUS_METRIC_UNIT));
    mainLayer_RemainMileage_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.remaining_mileage, (user_unitlist_type)type_action);
    // mainLayer_RemainMileage_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_dischargecurrent, (user_unitlist_type)type_action);
    mainLayer_Total_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_mileage, (user_unitlist_type)type_action);

    if (status_temp1 & CAR_FAULT_CODE)
    {
        type_action = 1;
    }
    else
    {
        if (status_temp2 & CAR_EX_HEALTH_SCORE)
        {
            type_action = 0; 
        }
        else
        {
            type_action = 2;
        }
    }
    mainLayer_ScoreError_DisplayConfig(type_action, ev_layer2.layer_displaydata.ScooterInfo.car_fault_code);
}

#ifndef DEMO_BUG_RUN
static void CheckExternal(void)
{
    bool ret_check = false;
    ExternalEvent ev_user = {0};
    int ret = 0, num = 0, j = 0, i = 0;
    static bool usb_status = false;
    int soc_bat = 0;
    unsigned short status_menu = 0;
    int type_action = (int)STATUS_SHOW;
    unsigned short status_temp1 = 0;
    unsigned short status_temp2 = 0;
    unsigned short status_new_car = 0, status_warn_code = 0, status_func_code = 0;
    ret = Layer_ExternalReceive(&ev_user);
    if (ret)
    {
        // printf("ev..!\t\n");
        // printf("ev_user.type = %d....\t\n", ev_user.type);
        // printf("testmode_status = %d....\t\n", testmode_status);
        if (ev_user.type == EXTERNAL_UPDATA)
        {
            if ((ev_layer2.layer_displaydata.ScooterInfo.car_status1 & (CAR_INCHG)))
            {
                if (!charge_status)
                {
                    testmode_status = false;
                    charge_status = true;
                    // upgrade_status = false;
                    ituLayerGoto(chargeLayer);
                    return;
                }
                // printf("charge_status=%d\t\n",charge_status);
            }
            // else if ((ev_layer2.layer_displaydata.ScooterInfo.car_status1 & (CAR_VCU_UPDATE)))
            // {
            //     if (!upgrade_status)
            //     {
            //         testmode_status = false;
            //         charge_status = false;
            //         upgrade_status = true;
            //         ituLayerGoto(upgradeLayer);
            //         return;
            //     }  
            //     // printf("upgrade_status=%d\t\n",upgrade_status);             
            // }
            else
            {
                // if (upgrade_status||charge_status||testmode_status)
                if (charge_status||testmode_status)
                {
                    testmode_status = false;
                    charge_status = false;
                    // upgrade_status = false;
                    ituLayerGoto(mainLayer);
                    return;
                } 
                // printf("mainLayer\t\n");                
            }
            Layer_MainMode_Task(&ev_layer2);

            // if (!testmode_status)
            // {
            //     module_test = board_test = false;
            //     if((local_niu_value_loc.db_loc.data.db_status&(CAR_INCHG))&&)	
            //     {
            //         ituLayerGoto(chargeLayer);                    
            //     }
            //     else if((local_niu_value_loc.db_loc.data.db_status&(CAR_VCU_UPDATE)))
            //     {
            //         ituLayerGoto(upgradeLayer);                    
            //     }
            //     Layer_MainMode_Task(&ev_layer2);
            // }
            // else
            // {
            //     ituLayerGoto(mainLayer);
            //     return;
            // }

#if 0                      
            status_temp1 = ev_layer2.layer_displaydata.ScooterInfo.car_status1;
            status_temp2 = ev_layer2.layer_displaydata.ScooterInfo.car_status2;           
            // printf("car_status1=0x%04x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_status1);
            // printf("car_gear=0x%02x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_gear);
            // printf("car_speed=%d..\r\n", ev_layer2.layer_displaydata.ScooterInfo.car_speed);
            // printf("car_mileage=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_mileage);
            // printf("car_chargecurrent=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_chargecurrent);
            // printf("car_dischargecurrent=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_dischargecurrent);
            // printf("car_bat1_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc);
            // printf("car_full_c_t=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_full_c_t);
            // printf("time_hour=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.time_hour);
            // printf("time_minute=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.time_minute);
            // printf("car_fault_code=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_fault_code);
            // printf("car_bat2_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc);
            // printf("car_status2=0x%04x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_status2);
            // printf("car_total_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_total_soc);
            
            // printf("weather_code=0x%04x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.weather_code);
            // printf("air_temperature=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.air_temperature);
            // printf("realtime_input_power=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.realtime_input_power);
           
            // printf("estimated_remaining_mileage=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.estimated_remaining_mileage);
            // printf("car_dischargecurrent=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_dischargecurrent);
            // printf("car_bat1_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc);
            // printf("car_full_c_t=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_full_c_t);
            // printf("time_hour=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.time_hour);
            // printf("time_minute=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.time_minute);
            // printf("car_fault_code=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_fault_code);
            // printf("car_bat2_soc=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc);
            // printf("car_status2=0x%04x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_status2);
            // printf("realtime_input_power=%d..\r\n", ev_layer2.layer_displaydata.ScooterInfo.realtime_input_power);
            // printf("estimated_remaining_mileage=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.estimated_remaining_mileage);

            // for (i = 0; i < sizeof(ScooterInfo_Typedef); i++)
            // {
            //     printf("DisplayDatabuffer[%d]=0x%02x....\n", i, ev_layer2.layer_displaydata.DisplayDatabuffer[i]);
            // }            
            
            status_menu = status_temp2 & CAR_EX_TEMPLE_STATUS; 
            if(!charge_status)
            {
                // printf("car_gear=%d..\t\n", ev_layer2.layer_displaydata.ScooterInfo.car_gear);
                mainLayer_DriveMode_DisplayConfig((user_drivemode_type)ev_layer2.layer_displaydata.ScooterInfo.car_gear);

                // mainLayer_Sidestand_DisplayConfig(!status_menu);

                status_menu = status_temp1 & CAR_PARKING;
                // printf("status_menu=0x%04x..\t\n", status_menu);
                mainLayer_Ready_DisplayConfig(!status_menu,((status_temp2 & CAR_EX_TEMPLE_STATUS) ? (true) : (false)));          
                
                mainLayer_Workstatus_DisplayConfig(ICON_CruiseControl,(status_temp1 & CAR_CONSTANT)? STATUS_SHOW:STATUS_HIDDEN);
                mainLayer_Workstatus_DisplayConfig(ICON_Autolight, (status_temp1 & CAR_AUTO_HEADLIGHT)? STATUS_SHOW:STATUS_HIDDEN);
                mainLayer_Workstatus_DisplayConfig(ICON_EnergyConservation, (status_temp1 & CAR_ECO)? STATUS_SHOW:STATUS_HIDDEN);
                mainLayer_Workstatus_DisplayConfig(ICON_CloudService, (status_temp2 & CAR_EX_CLOUD_CONNECTED)? STATUS_SHOW:STATUS_HIDDEN);
                mainLayer_Workstatus_DisplayConfig(ICON_EnergyRecovery, (status_temp1 & CAR_RECOVERY)? STATUS_SHOW:STATUS_HIDDEN);

                status_menu = status_temp1 & CAR_UNIT;
                type_action = (int)((status_menu) ? (STATUS_IMPERIAL_UNIT) : (STATUS_METRIC_UNIT));
                if (status_temp1 & CAR_PARKING)
                {
                    mainLayer_LastTripTime_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.last_cycling_time);
                    mainLayer_LastTripAvgSpeed_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.last_avg_speed, (user_unitlist_type)type_action);
                    mainLayer_LastTripMileage_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.last_trip_mileage, (user_unitlist_type)type_action);
                }
                else
                {
                    mainLayer_AverageEnergy_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.average_power_trip, (user_unitlist_type)type_action);
                    // mainLayer_RealtimeEnergyCurve_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_speed);
                }

                // status_menu = status_temp1 & CAR_UNIT;
                // type_action = (int)((status_menu) ? (STATUS_IMPERIAL_UNIT) : (STATUS_METRIC_UNIT));
                mainLayer_SpeedValue_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_speed, (user_unitlist_type)type_action);
                mainLayer_SpeedUnit_DisplayConfig((user_unitlist_type)type_action, STATUS_SHOW);
               
                if(!(status_temp1 & CAR_PARKING))
                {
                     if (status_temp2 & CAR_EX_AVGPOWER_FLAG)
                    {
                        printf("average_power=0x%02x..\t\n", ev_layer2.layer_displaydata.ScooterInfo.average_power);
                        mainLayer_RealtimeEnergyCurve_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.average_power);
                        // mainLayer_RealtimeEnergyCurve_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_speed);
                    }
					 mainLayer_RealtimeEnergy_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.realtime_input_power, status_temp1 & CAR_RECOVERY);
                }               
            }      
            else
            {
                chargeLayer_Sidestand_DisplayConfig(!status_menu);
                chargeLayer_ChargeTime_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.charge_time);
            }     
            // mainLayer_Sidestand_DisplayConfig(!status_menu); 
          
            mainLayer_Time_DisplayConfig(TEXT_TIME_HOUR, ev_layer2.layer_displaydata.ScooterInfo.time_hour);
			mainLayer_Time_DisplayConfig(TEXT_TIME_MINUTE, ev_layer2.layer_displaydata.ScooterInfo.time_minute);
            mainLayer_Temperature_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.air_temperature);
            mainLayer_Weather_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.weather_code,STATUS_SHOW);
            
            status_menu = status_temp2 & CAR_EX_START_MISTAKENLY;            
            // ScreenSetBrightness(ev_layer2.layer_displaydata.ScooterInfo.lightsenor);
            
            switch (status_temp1 & CAR_GPS_MASK)
            {
            case CAR_GPS_NO:
                type_action = (int)STATUS_SIGNAL_LEVEL0;
                break;
            case CAR_GPS_WEAK:
                type_action = (int)STATUS_SIGNAL_LEVEL1;
                break;
            case CAR_GPS_STRONG:
                type_action = (int)STATUS_SIGNAL_LEVEL3;
                break;
            case CAR_GPS_NULL:
                type_action = (int)STATUS_HIDDEN;
                break;
            default:
                type_action = 0;
                break;
            }
            mainLayer_NetworksSignal_DisplayConfig(ICON_SatelliteSignal, (user_indicationaction_type)type_action);

            switch (status_temp1 & CAR_GSM_MASK)
            {
            case CAR_GSM_NO:
                type_action = (int)STATUS_SIGNAL_LEVEL0;
                break;
            case CAR_GSM_WEAK:        
                type_action = (int)STATUS_SIGNAL_LEVEL1;    
                break;
            case CAR_GSM_STRONG:
                type_action = (int)STATUS_SIGNAL_LEVEL3;    
                break;
            case CAR_GSM_NULL:
                type_action = (int)STATUS_HIDDEN;  
                break;
            default:
                type_action = 0;  
                break;
            }
            mainLayer_NetworksSignal_DisplayConfig(ICON_MobileNetSignal, (user_indicationaction_type)type_action);
            
           switch(status_temp2 & (CAR_EX_BT1_ON|CAR_EX_BT2_ON))
           {
               case CAR_EX_BT1_ON:
                   //    printf("CAR_EX_BT1_ON..\t\n");
                   mainLayer_BatterySingle_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc);
                   soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc;
                   break;
               case CAR_EX_BT2_ON:
                    // printf("CAR_EX_BT2_ON..\t\n");   
                    mainLayer_BatterySingle_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc);
                    soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc;
                   break;
                case (CAR_EX_BT1_ON|CAR_EX_BT2_ON)://0x0003:CAR_EX_BT1_ON|CAR_EX_BT2_ON
                    // printf("CAR_EX_BT1_ON..CAR_EX_BT2_ON..\t\n");   
                    mainLayer_BatteryDual_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_bat1_soc,ev_layer2.layer_displaydata.ScooterInfo.car_bat2_soc);
                    soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_total_soc;
                    break;
                default:
                    // printf("CAR_EX_BT DEFAULT..\t\n");   
                    mainLayer_BatterySingle_DisplayConfig(0);
                    soc_bat = ev_layer2.layer_displaydata.ScooterInfo.car_total_soc;
                    break;
           }
           mainLayer_BatteryCapacity_DisplayConfig(soc_bat);

           status_menu = status_temp1 & CAR_UNIT;
           type_action = (int)((status_menu) ? (STATUS_IMPERIAL_UNIT) : (STATUS_METRIC_UNIT));
		   mainLayer_RemainMileage_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.remaining_mileage, (user_unitlist_type)type_action);
           mainLayer_Total_DisplayConfig(ev_layer2.layer_displaydata.ScooterInfo.car_mileage, (user_unitlist_type)type_action);
#endif
        }
        else if (ev_user.type == EXTERNAL_TESTMODE)
        {
            if (!testmode_status)
            {
                testmode_status = true;
                charge_status = false;               
                // upgrade_status = false;
                ituLayerGoto(testmodeLayer);                
                return;
            }
            // printf("ev_user.type = %d....\t\n",ev_user.type);
            Layer_TestMode_Task(&ev_layer2);
        }
    }
    // else
    // {
    //     printf("Layer_ExternalReceive no data!\n");
    // }

    if (testmode_status)
    {
        // for (i = 0; i < (sizeof(DashBoardInfo_typedef)); i++)
        // {
        //     printf("evDatabuffer[%d] = 0x%02x...\t\n", i, ev_layer2.layer_displaydata.DisplayDatabuffer[i]);
        // }
        if (module_test)
        {
            ret_check = testmodeLayer_psensor1_DisplayConfig(ev_layer2.layer_displaydata.DashBoardInfo.test_psensor1_ad);
            // if (ret_check)
            // {
            Check_temp |= ((unsigned short)1 << 2);
            // }
            // else
            // {
            //     /* code */
            //     Check_temp &= ~((unsigned short)1 << 2);
            // }
            testmodeLayer_module_Display(false, 3);
            testmodeLayer_uart_DisplayConfig(2);
            Check_temp |= 0x0001;

            // if (StorageIsInUsbDeviceMode())
            action = StorageCheck_customized();
            if (action)
            {
                // printf("action=%d..\t\n",action);
                if (action == STORAGE_USB_INSERTED)
                {
                    if (!usb_status)
                    {
                        testmodeLayer_usb_DisplayConfig(2);
                        Check_temp |= ((unsigned short)1 << 1);
                    }
                    usb_status = true;
                }
                else if (action == STORAGE_USB_REMOVED)
                {
                    if (usb_status)
                    {
                        testmodeLayer_usb_DisplayConfig(1);
                        Check_temp &= ~((unsigned short)1 << 1);
                    }
                    usb_status = false;
                }
            }

            status_menu = 0x0007;
            if (board_test)
            {
                status_menu = ev_layer2.layer_displaydata.DashBoardInfo.test_mode & 0x001F;
                status_menu <<= 3;
                status_menu |= 0x0007;
            }
            else
            {
                Check_temp &= 0x0007;
            }

            printf("Check_temp=0x%04x ", Check_temp);
            printf("status_menu=0x%04x\t\n", status_menu);
            if (Check_temp == status_menu)
            {
                testmodeLayer_pass_DisplayConfig(1);
            }
            else
            {
                testmodeLayer_pass_DisplayConfig(0);
            }
        }
    }
}
#endif

int SceneRun(void)
{
    SDL_Event ev, lastev;
    int done, delay, frames, lastx, lasty;
    uint32_t tick, dblclk, lasttick;

    /* Watch keystrokes */
    done = dblclk = frames = lasttick = lastx = lasty = 0;
#ifndef _WIN32
#endif
    while (!done)
    {
        bool result = false;

#ifndef DEMO_BUG_RUN
        CheckExternal();
#endif
        // //Maksim test usb device
        // StorageCheck();
        tick = SDL_GetTicks();

        frames++;

#ifdef FPS_ENABLE
        if (tick - lasttick >= 1000)
        {
            printf("fps: %d\n", frames);
            frames = 0;
            lasttick = tick;
        }
#endif

        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_KEYDOWN:
                result = ituSceneUpdate(&theScene, ITU_EVENT_KEYDOWN, ev.key.keysym.sym, 0, 0);
#ifndef _WIN32
#endif
                break;

            case SDL_KEYUP:
                result = ituSceneUpdate(&theScene, ITU_EVENT_KEYUP, ev.key.keysym.sym, 0, 0);
                break;

            case SDL_MOUSEMOTION:
                result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEMOVE, ev.button.button, ev.button.x, ev.button.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
            {
                uint32_t t = SDL_GetTicks();
                if (t - dblclk <= 300)
                {
                    result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOUBLECLICK, ev.button.button, ev.button.x, ev.button.y);
                    dblclk = 0;
                }
                else
                {
                    printf("mouse: down %d, %d\n", ev.button.x, ev.button.y);
                    result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOWN, ev.button.button, ev.button.x, ev.button.y);
                    dblclk = t;
                    lastx = ev.button.x;
                    lasty = ev.button.y;
                }
#ifndef _WIN32
#endif
            }
            break;

            case SDL_MOUSEBUTTONUP:
                if (SDL_GetTicks() - dblclk <= 300)
                {
                    int xdiff = abs(ev.button.x - lastx);
                    int ydiff = abs(ev.button.y - lasty);

                    if (xdiff >= GESTURE_THRESHOLD && xdiff > ydiff)
                    {
                        if (ev.button.x > lastx)
                        {
                            printf("mouse: slide to right\n");
                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDERIGHT, xdiff, ev.button.x, ev.button.y);
                        }
                        else
                        {
                            printf("mouse: slide to left\n");
                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDELEFT, xdiff, ev.button.x, ev.button.y);
                        }
                    }
                    else if (ydiff >= GESTURE_THRESHOLD)
                    {
                        if (ev.button.y > lasty)
                        {
                            printf("mouse: slide to down\n");
                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEDOWN, ydiff, ev.button.x, ev.button.y);
                        }
                        else
                        {
                            printf("mouse: slide to up\n");
                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEUP, ydiff, ev.button.x, ev.button.y);
                        }
                    }
                }

                printf("mouse: up %d, %d\n", ev.button.x, ev.button.y);
                result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEUP, ev.button.button, ev.button.x, ev.button.y);

                break;

            case SDL_FINGERDOWN:
                printf("touch: down %d, %d\n", ev.tfinger.x, ev.tfinger.y);
                {
                    uint32_t t = SDL_GetTicks();
#ifdef DOUBLE_KEY_ENABLE
                    if (t - dblclk <= 300)
                    {
                        result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOUBLECLICK, 1, ev.tfinger.x, ev.tfinger.y);
                        dblclk = 0;
                    }
                    else
#endif
                    {
                        result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEDOWN, 1, ev.tfinger.x, ev.tfinger.y);
                        dblclk = t;
                        lastx = ev.tfinger.x;
                        lasty = ev.tfinger.y;
                    }
#ifndef _WIN32
#endif
                }
                break;

            case SDL_FINGERMOTION:
                printf("touch: move %d, %d\n", ev.tfinger.x, ev.tfinger.y);
                result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEMOVE, 1, ev.tfinger.x, ev.tfinger.y);
                break;

            case SDL_FINGERUP:
                printf("touch: up %d, %d\n", ev.tfinger.x, ev.tfinger.y);
                if (SDL_GetTicks() - dblclk <= 300)
                {
                    int xdiff = abs(ev.tfinger.x - lastx);
                    int ydiff = abs(ev.tfinger.y - lasty);

                    if (xdiff >= GESTURE_THRESHOLD && xdiff > ydiff)
                    {
                        if (ev.tfinger.x > lastx)
                        {
                            printf("touch: slide to right %d %d\n", ev.tfinger.x, ev.tfinger.y);
                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDERIGHT, xdiff, ev.tfinger.x, ev.tfinger.y);
                        }
                        else
                        {
                            printf("touch: slide to left %d %d\n", ev.tfinger.x, ev.tfinger.y);
                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDELEFT, xdiff, ev.tfinger.x, ev.tfinger.y);
                        }
                    }
                    else if (ydiff >= GESTURE_THRESHOLD)
                    {
                        if (ev.tfinger.y > lasty)
                        {
                            printf("touch: slide to down %d %d\n", ev.tfinger.x, ev.tfinger.y);
                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEDOWN, ydiff, ev.tfinger.x, ev.tfinger.y);
                        }
                        else
                        {
                            printf("touch: slide to up %d %d\n", ev.tfinger.x, ev.tfinger.y);
                            result |= ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEUP, ydiff, ev.tfinger.x, ev.tfinger.y);
                        }
                    }
                }

                result = ituSceneUpdate(&theScene, ITU_EVENT_MOUSEUP, 1, ev.tfinger.x, ev.tfinger.y);

                break;
                /*
            case SDL_SLIDEGESTURE:
                switch (ev.dgesture.gestureId)
                {
                case SDL_TG_LEFT:
                    printf("touch: slide to left\n");
                    result = ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDELEFT, 1, ev.dgesture.x, ev.dgesture.y);
                    break;

                case SDL_TG_UP:
                    printf("touch: slide to up\n");
                    result = ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEUP, 1, ev.dgesture.x, ev.dgesture.y);
                    break;

                case SDL_TG_RIGHT:
                    printf("touch: slide to right\n");
                    result = ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDERIGHT, 1, ev.dgesture.x, ev.dgesture.y);
                    break;

                case SDL_TG_DOWN:
                    printf("touch: slide to down\n");
                    result = ituSceneUpdate(&theScene, ITU_EVENT_TOUCHSLIDEDOWN, 1, ev.dgesture.x, ev.dgesture.y);
                    break;
                }
                break;
				*/
            case SDL_QUIT:
                done = 1;
                break;
            }
            lastev = ev;
        }

        result |= ituSceneUpdate(&theScene, ITU_EVENT_TIMER, 0, 0, 0);
#ifndef _WIN32
        if (result)
#endif
        {
            ituSceneDraw(&theScene, screenSurf);
            ituFlip(screenSurf);
        }

        delay = periodPerFrame - (SDL_GetTicks() - tick);
        //printf("scene loop delay=%d\n", delay);
        if (delay > 0)
        {
            SDL_Delay(delay);
        }
        else
            sched_yield();

        //delay = 33 - (SDL_GetTicks() - tick);
        //if (delay > 0)
        //{
        //    SDL_Delay(delay);
        //}
    }

    //SDL_Quit();
    return 0;
}

void ScreenSetBrightness(int value)
{
    static int backlight_default = CFG_BACKLIGHT_DEFAULT_DUTY_CYCLE;
    // int test_v = 0;
    int backlightvalue = value;    
    if (value > 90)
    {
        backlightvalue = 90;
    }
    else if(value < 0)
    {
        backlightvalue = 0;
    }
    if (backlight_default != backlightvalue)
    {
        if (backlight_default < backlightvalue)
        {
            backlight_default++;
        }
        else //if (backlight_default > backlightvalue)
        {
            backlight_default--;
        }      
        // printf("setlight=%d\t\n",backlight_default);
        ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_CUSTOM_BRIGHTNESS, (void *)backlight_default);
        // backlight_default = backlightvalue;
    }
    else
    {
        light_set_flag = false;
    }
    
#if 0
    if (value != backlight_default)
    {
        // test_v = abs(value - backlight_default);
        // test_v >>= 1;
        if (value < backlight_default)
        {
            // backlight_default -= test_v;
            backlight_default--;
        }
        else
        {
            // backlight_default += test_v;
            backlight_default++;
        }
        // printf("duty=%d...\t\n",backlight_default);
        ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_CUSTOM_BRIGHTNESS, (void *)value);
        // ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_CUSTOM_BRIGHTNESS, (void *)backlight_default);
    }
#endif
}

void ScreenClear(void)
{
    uint16_t *addr = (uint16_t *)ithLcdGetBaseAddrA();
    int size = ithLcdGetPitch() * ithLcdGetHeight();
    uint16_t *base = ithMapVram((uint32_t)addr, size, ITH_VRAM_WRITE);

    memset(base, 0, size);
    ithUnmapVram(base, size);
}