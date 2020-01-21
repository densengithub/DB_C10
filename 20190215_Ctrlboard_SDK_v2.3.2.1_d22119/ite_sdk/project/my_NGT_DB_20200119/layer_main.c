#include <assert.h>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SDL/SDL.h"
#include "scene.h"
#include "stdint.h"
#include "project.h"

/* widgets:
mainLayer
mainSportBackground
mainEsaveBackground
mainDynamicBackground
mainBackground0
*/

bool curve_display_status = false;
int curve_display_Count = 0;
bool testmode_status = false;
bool sidestand_temple_status = false;
int delay_count = 0;
int delay_count_signal = 0;
bool side_isvisible = true;

bool signal_isvisible = true;

ITUBackground* Current_modeBackground = NULL;
ITUSprite* Current_speedSprite = NULL;
ITUSprite* Current_powerSprite = NULL;
// ITUContainer* Current_powerContainer = NULL;
ITUContainer* Current_modeContainer = NULL;

//mainLayer
ITULayer* mainLayer = NULL;
static ITUBackground* mainSportBackground = NULL;
static ITUBackground* mainEsaveBackground = NULL;
static ITUBackground* mainDynamicBackground = NULL;
static ITUIcon* mainSportIcon = NULL;

//curveLayer
ITULayer* curveLayer = NULL;
static ITUCurve *curveCurve = NULL;
static ITUSprite* curveStarSprite = NULL;
static ITUContainer* curveContainer = NULL;
// static ITUContainer* curveXContainer = NULL;
static ITUText* curveAvgPowerText = NULL;
static ITUText* curveAvgPowerUnitsText = NULL;
static ITUSprite* curveAxisSprite = NULL;
static ITUIcon* curveStarIcon = NULL;
static ITUIcon* curveEsaveStarIcon = NULL;

//timeLayer
// timeScoErrSprite
// timeErrorContainer
// timeErrorText
// timeErrorIcon
// timeScoreContainer
// timeScoreText
// timeScoreIcon

// timeSideIcon
// timeTotalContainer
// timeTotalUnitsText
// timeTotalValueText
// timeTotalTagText
// timeSignalContainer
// timeGPSSignalIcon0
// timeGPSSignalIcon1
// timeGPSSignalIcon2
// timeGPSSignalIcon3
// timeGSMSignalIcon1
// timeGSMSignalIcon2
// timeGSMSignalIcon3
// timeGSMSignalIcon0
ITULayer* timeLayer = NULL;
static ITUIcon* timeAirtemperatureUnitsIcon = NULL;
static ITUText* timeAirtemperatureUnitsText = NULL;
static ITUText* timeAirtemperatureText = NULL;
static ITUSprite* timeWeatherSprite = NULL;

static ITUIcon* timeCruiseControlIcon = NULL;
static ITUIcon* timeEnergyRecoveryIcon = NULL;
static ITUIcon* timeEnergyConservationIcon = NULL;
static ITUIcon* timeAutoLightIcon = NULL;
static ITUIcon* timeCloudConnectIcon = NULL;

static ITUIcon* timeTitleContainer = NULL;
static ITUSprite* timeModeSprite = NULL;
// timeSportText
// timeDynamicText
// timeEsaveText
static ITUText* timeMinuteText = NULL;
static ITUText* timeHourText = NULL;
// static ITUText* timeColonText = NULL;

static ITUIcon* timeSideIcon = NULL;
static ITUSprite* timeScoErrSprite = NULL;
static ITUText* timeErrorText = NULL;
static ITUText* timeScoreText = NULL;

static ITUContainer* timeTotalContainer = NULL;
static ITUText* timeTotalUnitsText = NULL;
static ITUText* timeTotalValueText = NULL;
static ITUText* timeTotalTagText = NULL;
static ITUContainer* timeSignalContainer = NULL;
static ITUIcon* timeGPSSignalIcon0 = NULL;
static ITUIcon* timeGPSSignalIcon1 = NULL;
static ITUIcon* timeGPSSignalIcon2 = NULL;
static ITUIcon* timeGPSSignalIcon3 = NULL;
static ITUIcon* timeGSMSignalIcon1 = NULL;
static ITUIcon* timeGSMSignalIcon2 = NULL;
static ITUIcon* timeGSMSignalIcon3 = NULL;
static ITUIcon* timeGSMSignalIcon0 = NULL;

//standLayer
ITULayer* standLayer = NULL;
static ITUContainer* standallContainer = NULL;
static ITUContainer* standContainer = NULL;
static ITUIcon* standsideIcon = NULL;
static ITUText* standTimesText = NULL;
static ITUText* standAvgSpeedText = NULL;
static ITUText* standRangeText = NULL;
static ITUText* standSpeedUnitsText = NULL;
static ITUText* standTimerTagText = NULL;
static ITUText* standRangeUnitsText = NULL;
static ITUIcon* standIcon = NULL;
static ITUText* standLastTripTagText = NULL;

//recoveryprogressLayer
ITULayer* recoveryprogressLayer = NULL;
static ITUContainer* progressRecoveryPowerContainer = NULL;
static ITUSprite* progressRecoveryPowerSprite = NULL;

// speedpowerLayer
ITULayer* speedpowerLayer = NULL;
static ITUContainer* sp_TotalContainer = NULL;
static ITUText* sp_TotalUnitsText = NULL;
static ITUText* sp_TotalValueText = NULL;
static ITUText* sp_TotalTagText = NULL;
static ITUIcon* sp_GSMSignalIcon[4] = {NULL};
static ITUIcon* sp_GPSSignalIcon[4] = {NULL};

static ITUText* sp_SpeedValueText = NULL;
static ITUText* sp_PowerValueText = NULL;
static ITUText* sp_PowerSignedText = NULL;
static ITUText* sp_PowerUnitsText = NULL;
static ITUText* sp_SpeedUnitsText = NULL;
static ITUIcon* sp_SpeedFrameNumberIcon = NULL;
static ITUIcon* sp_PowerFrameNumberIcon = NULL;
static ITUIcon* sp_SpeedFrameLinerIcon = NULL;
static ITUIcon* sp_PowerFrameLineIcon = NULL;

//sportprogressLayer
ITULayer* sportprogressLayer = NULL;
static ITUContainer* sportprogressContainer = NULL;
static ITUSprite* progressSportPowerSprite = NULL;
static ITUSprite* progressSportSpeedSprite = NULL;
static ITUText* modeSportText = NULL;

//esaveprogressLayer
ITULayer* esaveprogressLayer = NULL;
static ITUContainer* esaveprogressContainer = NULL;
static ITUSprite* progressEsavePowerSprite = NULL;
static ITUSprite* progressEsaveSpeedSprite = NULL;
static ITUText* modeEsaveText = NULL;

//dynamicprogressLayer
ITULayer* dynamicprogressLayer = NULL;
static ITUContainer* dynamicprogressContainer = NULL;
static ITUSprite* progressDynamicPowerSprite = NULL;
static ITUSprite* progressDynamicSpeedSprite = NULL;
static ITUText* modeDynamicText = NULL;

//bottomLayer
ITULayer* bottomLayer = NULL;
static ITUContainer* bottomBatContainer = NULL;
static ITUSprite* bottomBatSprite = NULL;
static ITUContainer* bottomDualContainer = NULL;
static ITUContainer* bottomSingleContainer = NULL;

static ITUBackground* bottomBatAFrameBackground = NULL;
static ITUBackground* bottomBatABackground = NULL;
static ITUBackground* bottomBatBFrameBackground = NULL;
static ITUBackground* bottomBatBBackground = NULL;
static ITUBackground* bottomSingleFrameBackground = NULL;
static ITUBackground* bottomSingleProgressBackground = NULL;
static ITUIcon* bottomBatRangeTagIcon = NULL;
// static ITUIcon* bottomBatBalanceTagIcon = NULL;
// static ITUIcon* bottomBatAloneTagIcon = NULL;
static ITUSprite* bottomBatTagSprite = NULL;

static ITUText* bottomMieagePreestimateText = NULL;
static ITUText* bottomMieageUintsText = NULL;
static ITUText* bottomPercentMarkText = NULL;
static ITUText* bottomBatCapacityText = NULL;
// static ITUIcon* bottomBatTagIcon = NULL;


static ITUSprite* curveMaskSprite = NULL;
static ITUBackground* curveMask0Background = NULL;
static ITUBackground* curveMask1Background = NULL;
static ITUBackground* curveMask2Background = NULL;
// static ITUIcon* curveLineIcon1 = NULL;
// static ITUIcon* curveLineIcon2 = NULL;
// static ITUIcon* curveTestIcon = NULL;
static ITUIcon* curveLineIcon[2] = {NULL};

// ITUSurface* TestIconSurface;
// ITUSurface* TestSurface_temp;
// const char *buffer_png[] = {
// 		"A:/spcurve.png",
// 		"A:/escurve.png"							
// 	};
// int curve_display_value = 0;
// int layer_status = 0;

int signal_status = 0;

bool Layer_time_Init(void)
{
	int i = 0;
    char buff_name[50] = {0};
	if(!timeTitleContainer)
    {
        timeTitleContainer = ituSceneFindWidget(&theScene, "timeTitleContainer");
        assert(timeTitleContainer);
        timeAirtemperatureText = ituSceneFindWidget(&theScene, "timeAirtemperatureText");
        assert(timeAirtemperatureText);
        timeWeatherSprite = ituSceneFindWidget(&theScene, "timeWeatherSprite");
        assert(timeWeatherSprite);
        timeAirtemperatureUnitsIcon = ituSceneFindWidget(&theScene, "timeAirtemperatureUnitsIcon");
        assert(timeAirtemperatureUnitsIcon);

        timeCruiseControlIcon = ituSceneFindWidget(&theScene, "timeCruiseControlIcon");
        assert(timeCruiseControlIcon);
        timeEnergyRecoveryIcon = ituSceneFindWidget(&theScene, "timeEnergyRecoveryIcon");
        assert(timeEnergyRecoveryIcon);
        timeEnergyConservationIcon = ituSceneFindWidget(&theScene, "timeEnergyConservationIcon");
        assert(timeEnergyConservationIcon);
        timeAutoLightIcon = ituSceneFindWidget(&theScene, "timeAutoLightIcon");
        assert(timeAutoLightIcon);
         timeCloudConnectIcon = ituSceneFindWidget(&theScene, "timeCloudConnectIcon");
        assert(timeCloudConnectIcon);
		ituWidgetSetVisible(timeCruiseControlIcon, false);
		ituWidgetSetVisible(timeEnergyRecoveryIcon, false);
		ituWidgetSetVisible(timeEnergyConservationIcon, false);
		ituWidgetSetVisible(timeAutoLightIcon, false);
		ituWidgetSetVisible(timeCloudConnectIcon, false);

        timeModeSprite = ituSceneFindWidget(&theScene, "timeModeSprite");
        assert(timeModeSprite);		

        timeMinuteText = ituSceneFindWidget(&theScene, "timeMinuteText");
        assert(timeMinuteText);
        timeHourText = ituSceneFindWidget(&theScene, "timeHourText");
		assert(timeHourText);
		
		sp_TotalValueText = ituSceneFindWidget(&theScene, "timeTotalValueText");
        assert(sp_TotalValueText);
        sp_TotalUnitsText = ituSceneFindWidget(&theScene, "timeTotalUnitsText");
        assert(sp_TotalUnitsText);
        sp_TotalTagText = ituSceneFindWidget(&theScene, "timeTotalTagText");
        assert(sp_TotalTagText);
		sp_TotalContainer = ituSceneFindWidget(&theScene, "timeTotalContainer");
		assert(sp_TotalContainer);

		// for (i = 0; i < 4;i++)
		for (i = 0; i < 2; i++)
		{
			snprintf(buff_name, sizeof(buff_name), "timeGSMSignalIcon%d", i);
            sp_GSMSignalIcon[i] = ituSceneFindWidget(&theScene, buff_name);
            assert(sp_GSMSignalIcon[i]);
            snprintf(buff_name, sizeof(buff_name), "timeGPSSignalIcon%d", i);
            sp_GPSSignalIcon[i] = ituSceneFindWidget(&theScene, buff_name);
			assert(sp_GPSSignalIcon[i]);
			ituWidgetSetVisible(sp_GSMSignalIcon[i], false);
			ituWidgetSetVisible(sp_GPSSignalIcon[i], false);
        }       

		timeWeatherSprite->delay = 0;	
		ituSpriteStop(timeWeatherSprite);	
		ituSpriteGoto(timeWeatherSprite, 0);	
		
		timeSideIcon = ituSceneFindWidget(&theScene, "timeSideIcon");
		assert(timeSideIcon);
		ituWidgetSetVisible(timeSideIcon, false);		
        timeScoErrSprite = ituSceneFindWidget(&theScene, "timeScoErrSprite");
        assert(timeScoErrSprite);
        timeErrorText = ituSceneFindWidget(&theScene, "timeErrorText");
		assert(timeErrorText);
		timeScoreText = ituSceneFindWidget(&theScene, "timeScoreText");
        assert(timeScoreText);

		timeScoErrSprite->delay = 0;
		ituSpriteStop(timeScoErrSprite);
		ituSpriteGoto(timeScoErrSprite, 1);
		ituWidgetSetVisible(timeScoErrSprite, false);

		timeModeSprite->delay = 0;
		ituSpriteStop(timeModeSprite);
		ituSpriteGoto(timeModeSprite, 1);		

		ituTextSetString(timeMinuteText, "00");
		ituTextSetString(timeHourText, "00");
		ituTextSetString(timeAirtemperatureText, "0");

		ituWidgetSetPosition(timeAirtemperatureUnitsIcon, 401 - 22, 55 - 8);
		ituWidgetSetX(timeWeatherSprite, 300-5);//hundreds

		ituWidgetSetVisible(timeModeSprite, false);
		ituWidgetSetVisible(timeWeatherSprite, false);
		ituWidgetSetVisible(timeAirtemperatureText, false);
		ituWidgetSetVisible(timeAirtemperatureUnitsIcon, false);
		Current_speedSprite = NULL;
		Current_powerSprite = NULL;
		Current_modeContainer = NULL;
	}
	// ituWidgetSetVisible(sp_TotalContainer, false);	
	// ituWidgetSetVisible(timeTitleContainer, true);
    return false;
}

bool Layer_charge_time_init(void)
{
	Layer_time_Init();
	ituWidgetSetVisible(timeModeSprite, false);
	ituWidgetSetVisible(timeTitleContainer, true);
	return true;
}

bool Layer_bottom_Init(void)
{
    if(!bottomBatContainer)
    {
        bottomBatContainer = ituSceneFindWidget(&theScene, "bottomBatContainer");
        assert(bottomBatContainer);
        // bottomDualContainer = ituSceneFindWidget(&theScene, "bottomDualContainer");
        // assert(bottomDualContainer);
        // bottomSingleContainer = ituSceneFindWidget(&theScene, "bottomSingleContainer");
        // assert(bottomSingleContainer);

        // bottomBatAFrameBackground = ituSceneFindWidget(&theScene, "bottomBatAFrameBackground");
        // assert(bottomBatAFrameBackground);
        bottomBatABackground = ituSceneFindWidget(&theScene, "bottomBatABackground");
        assert(bottomBatABackground);

        // bottomBatBFrameBackground = ituSceneFindWidget(&theScene, "bottomBatBFrameBackground");
        // assert(bottomBatBFrameBackground);
        bottomBatBBackground = ituSceneFindWidget(&theScene, "bottomBatBBackground");
        assert(bottomBatBBackground);

        // bottomSingleFrameBackground = ituSceneFindWidget(&theScene, "bottomSingleFrameBackground");
        // assert(bottomSingleFrameBackground);
        bottomSingleProgressBackground = ituSceneFindWidget(&theScene, "bottomSingleProgressBackground");
        assert(bottomSingleProgressBackground);

        bottomBatRangeTagIcon = ituSceneFindWidget(&theScene, "bottomBatRangeTagIcon");
        assert(bottomBatRangeTagIcon);
        // bottomBatBalanceTagIcon = ituSceneFindWidget(&theScene, "bottomBatBalanceTagIcon");
		// assert(bottomBatBalanceTagIcon);
		bottomBatTagSprite = ituSceneFindWidget(&theScene, "bottomBatTagSprite");
		assert(bottomBatTagSprite); 
		// bottomBatAloneTagIcon = ituSceneFindWidget(&theScene, "bottomBatAloneTagIcon");
		// assert(bottomBatAloneTagIcon); 
		bottomBatTagSprite->delay = 0;
		ituSpriteStop(bottomBatTagSprite);
		ituSpriteGoto(bottomBatTagSprite, 0);


        bottomMieagePreestimateText = ituSceneFindWidget(&theScene, "bottomMieagePreestimateText");
        assert(bottomMieagePreestimateText);
        bottomMieageUintsText = ituSceneFindWidget(&theScene, "bottomMieageUintsText");
        assert(bottomMieageUintsText);
		ituTextSetString(bottomMieagePreestimateText, "--");
        // ituWidgetSetX(bottomMieageUintsText, 572);//hundreds
        ituWidgetSetX(bottomMieageUintsText, 547);//tens

        bottomPercentMarkText = ituSceneFindWidget(&theScene, "bottomPercentMarkText");
        assert(bottomPercentMarkText);
        bottomBatCapacityText = ituSceneFindWidget(&theScene, "bottomBatCapacityText");
        assert(bottomBatCapacityText);
        // bottomBatTagIcon = ituSceneFindWidget(&theScene, "bottomBatTagIcon");
        // assert(bottomBatTagIcon);

		bottomBatSprite = ituSceneFindWidget(&theScene, "bottomBatSprite");
		assert(bottomBatSprite);
		bottomBatSprite->delay = 0;
		ituSpriteStop(bottomBatSprite);
		ituSpriteGoto(bottomBatSprite, 0);
		ituTextSetString(bottomBatCapacityText, "100 ");		
        // return true;
    }
	// ituWidgetSetVisible(bottomBatBalanceTagIcon, true);
	ituWidgetSetVisible(bottomBatTagSprite, true);
	ituWidgetSetVisible(bottomBatRangeTagIcon, true);
	ituWidgetSetVisible(bottomBatCapacityText, true);
	ituWidgetSetVisible(bottomPercentMarkText, true);
	ituWidgetSetVisible(bottomMieagePreestimateText, true);
	ituWidgetSetVisible(bottomMieageUintsText, true);
    return false;
}

bool Layer_charge_bottom_init(void)
{
	Layer_bottom_Init();
	// ituWidgetSetVisible(bottomBatBalanceTagIcon, false);
	ituWidgetSetVisible(bottomBatTagSprite, false);
	ituWidgetSetVisible(bottomBatRangeTagIcon, false);
	ituWidgetSetVisible(bottomBatCapacityText, false);
	ituWidgetSetVisible(bottomPercentMarkText, false);
	ituWidgetSetVisible(bottomMieagePreestimateText, false);
	ituWidgetSetVisible(bottomMieageUintsText, false);
	return true;
}

bool Layer_upgrade_time_init(void)
{
	Layer_time_Init();
	// ituWidgetSetVisible(timeModeSprite, false);
	ituWidgetSetVisible(timeTitleContainer, false);
	ituWidgetSetVisible(sp_TotalContainer, false);
	ituWidgetSetVisible(sp_TotalContainer, false);
	return true;
}

bool Layer_stand_Init(void)
{
    if(!standallContainer)
    {
        standallContainer = ituSceneFindWidget(&theScene, "standallContainer");
        assert(standallContainer);
        // standContainer = ituSceneFindWidget(&theScene, "standContainer");
        // assert(standContainer);
        standsideIcon = ituSceneFindWidget(&theScene, "standsideIcon");
        assert(standsideIcon);

        standTimesText = ituSceneFindWidget(&theScene, "standTimesText");
        assert(standTimesText);
        standAvgSpeedText = ituSceneFindWidget(&theScene, "standAvgSpeedText");
        assert(standAvgSpeedText);
        standSpeedUnitsText = ituSceneFindWidget(&theScene, "standSpeedUnitsText");
        assert(standSpeedUnitsText);
        standRangeText = ituSceneFindWidget(&theScene, "standRangeText");
        assert(standRangeText);
        standRangeUnitsText = ituSceneFindWidget(&theScene, "standRangeUnitsText");
        assert(standRangeUnitsText);

        standIcon = ituSceneFindWidget(&theScene, "standIcon");
        assert(standIcon);
        standLastTripTagText = ituSceneFindWidget(&theScene, "standLastTripTagText");
        assert(standLastTripTagText);     

		ituWidgetSetVisible(standallContainer, true);
		// ituWidgetSetVisible(standContainer, true);
		ituWidgetSetVisible(standsideIcon, true);
		ituTextSetString(standTimesText, "00:00");
		ituTextSetString(standAvgSpeedText, "00.0 ");
		ituTextSetString(standRangeText, "0.0 ");
        return true;
    }
    return false;
}

bool Layer_recoveryprogress_Init(void)
{
    if(!progressRecoveryPowerContainer)
    {
        progressRecoveryPowerContainer = ituSceneFindWidget(&theScene, "progressRecoveryPowerContainer");
        assert(progressRecoveryPowerContainer);
        progressRecoveryPowerSprite = ituSceneFindWidget(&theScene, "progressRecoveryPowerSprite");
		assert(progressRecoveryPowerSprite);   
		
		ituWidgetSetVisible(progressRecoveryPowerContainer, false);
		progressRecoveryPowerSprite->delay = 0;
		ituSpriteStop(progressRecoveryPowerSprite);
		ituSpriteGoto(progressRecoveryPowerSprite, 0);
        return true;
    }
    return false;
}

bool Layer_speedpower_Init(void)
{
    // int i = 0;
    // char buff_name[50] = {0};
    if(!sp_SpeedValueText)
    {
        // sp_TotalValueText = ituSceneFindWidget(&theScene, "sp_TotalValueText");
        // assert(sp_TotalValueText);
        // sp_TotalUnitsText = ituSceneFindWidget(&theScene, "sp_TotalUnitsText");
        // assert(sp_TotalUnitsText);
        // sp_TotalTagText = ituSceneFindWidget(&theScene, "sp_TotalTagText");
        // assert(sp_TotalTagText);
		// sp_TotalContainer = ituSceneFindWidget(&theScene, "sp_TotalContainer");
		// assert(sp_TotalContainer);

        sp_SpeedValueText = ituSceneFindWidget(&theScene, "sp_SpeedValueText");
        assert(sp_SpeedValueText);
        sp_SpeedUnitsText = ituSceneFindWidget(&theScene, "sp_SpeedUnitsText");
        assert(sp_SpeedUnitsText);

        sp_PowerSignedText = ituSceneFindWidget(&theScene, "sp_PowerSignedText");
        assert(sp_PowerSignedText);
        sp_PowerValueText = ituSceneFindWidget(&theScene, "sp_PowerValueText");
        assert(sp_PowerValueText);
        sp_PowerUnitsText = ituSceneFindWidget(&theScene, "sp_PowerUnitsText");
        assert(sp_PowerUnitsText);	

		// ituTextSetFontSize(sp_SpeedValueText,10);
		// ituTextSetFontSize(sp_PowerSignedText,50);
		// ituTextSetFontSize(sp_PowerValueText,80);
		
		// ituWidgetSetVisible(sp_SpeedValueText, false);
		// ituWidgetSetVisible(sp_PowerSignedText, false);
		// ituWidgetSetVisible(sp_PowerValueText, false);

        sp_SpeedFrameNumberIcon = ituSceneFindWidget(&theScene, "sp_SpeedFrameNumberIcon");
        assert(sp_SpeedFrameNumberIcon);
        sp_PowerFrameNumberIcon = ituSceneFindWidget(&theScene, "sp_PowerFrameNumberIcon");
        assert(sp_PowerFrameNumberIcon);
		ituWidgetSetVisible(sp_SpeedFrameNumberIcon, false);
		ituWidgetSetVisible(sp_PowerFrameNumberIcon, false);
        // for (i = 0; i < 4;i++)
        // {
        //     snprintf(buff_name, sizeof(buff_name), "sp_GSMSignalIcon%d", i);
        //     sp_GSMSignalIcon[i] = ituSceneFindWidget(&theScene, buff_name);
        //     assert(sp_GSMSignalIcon[i]);
        //     snprintf(buff_name, sizeof(buff_name), "sp_GPSSignalIcon%d", i);
        //     sp_GPSSignalIcon[i] = ituSceneFindWidget(&theScene, buff_name);
        //     assert(sp_GPSSignalIcon[i]);
        // }       

        sp_SpeedFrameLinerIcon = ituSceneFindWidget(&theScene, "sp_SpeedFrameLinerIcon");
        assert(sp_SpeedFrameLinerIcon);
        sp_PowerFrameLineIcon = ituSceneFindWidget(&theScene, "sp_PowerFrameLineIcon");
        assert(sp_PowerFrameLineIcon);

		// ituTextSetString(sp_TotalValueText, "--");
    }
	ituTextSetString(sp_PowerUnitsText, "kW");
	// ituTextSetString(sp_PowerUnitsText, "W");
	ituTextSetString(sp_SpeedUnitsText, "km/h");

	ituWidgetSetX(sp_SpeedValueText, -7);
	ituWidgetSetX(sp_PowerValueText, 78);
	ituWidgetSetWidth(sp_PowerValueText, 202 + 3); //205
	// ituWidgetSetPosition(sp_PowerValueText, 78, 55);
	// ituWidgetSetDimension(sp_PowerValueText, 205, 202);
    return false;
}

bool Layer_charge_speedpower_init(void)
{
	Layer_speedpower_Init();	
	ituTextSetString(sp_PowerValueText, "0"); 
	ituTextSetString(sp_PowerUnitsText, "km");
	ituTextSetString(sp_SpeedValueText, "0");
	ituTextSetString(sp_SpeedUnitsText, "%");
	ituWidgetSetX(sp_SpeedValueText, 10);
	ituWidgetSetX(sp_PowerValueText, 78 - 20);
	ituWidgetSetWidth(sp_PowerValueText, 205 + 40);

	ituWidgetSetVisible(sp_SpeedFrameNumberIcon, false);
	ituWidgetSetVisible(sp_PowerFrameNumberIcon, false);	
	return true;
}

bool Layer_curve_Init(void)
{
	int i = 0;
	if(!curveContainer)
    {
        curveCurve = ituSceneFindWidget(&theScene, "curveCurve");
        assert(curveCurve);
		curveStarSprite = ituSceneFindWidget(&theScene, "curveStarSprite");
		assert(curveStarSprite);
        curveContainer = ituSceneFindWidget(&theScene, "curveContainer");
		assert(curveContainer);
		// curveXContainer = ituSceneFindWidget(&theScene, "curveYContainer");
		// assert(curveXContainer);
        curveAvgPowerText = ituSceneFindWidget(&theScene, "curveAvgPowerText");
        assert(curveAvgPowerText);
        curveAvgPowerUnitsText = ituSceneFindWidget(&theScene, "curveAvgPowerUnitsText");
        assert(curveAvgPowerUnitsText);
        curveAxisSprite = ituSceneFindWidget(&theScene, "curveAxisSprite");
        assert(curveAxisSprite);
        // curveStarIcon = ituSceneFindWidget(&theScene, "curveStarIcon");
        // assert(curveStarIcon);
        // curveEsaveStarIcon = ituSceneFindWidget(&theScene, "curveEsaveStarIcon");
        // assert(curveEsaveStarIcon);
		curveMask0Background = ituSceneFindWidget(&theScene, "curveMask0Background");
        assert(curveMask0Background);
		curveMask1Background = ituSceneFindWidget(&theScene, "curveMask1Background");
        assert(curveMask1Background);
		curveMask2Background = ituSceneFindWidget(&theScene, "curveMask2Background");
        assert(curveMask2Background);
		curveMaskSprite = ituSceneFindWidget(&theScene, "curveMaskSprite");
        assert(curveMaskSprite);
		ituWidgetSetVisible(curveMaskSprite, false);

		curveLineIcon[0] = ituSceneFindWidget(&theScene, "curveLineIcon1");
        assert(curveLineIcon[0]);
		curveLineIcon[1] = ituSceneFindWidget(&theScene, "curveLineIcon2");
        assert(curveLineIcon[1]);

		// curveTestIcon = ituSceneFindWidget(&theScene, "curveTestIcon");
        // assert(curveTestIcon);
		// ituWidgetSetVisible(curveTestIcon, true);

		// ituWidgetSetHeight(curveMaskSprite, 142);
		// ituWidgetSetHeight(curveMask0Background, 142);
		// ituWidgetSetHeight(curveMask1Background, 142);
		// ituWidgetSetHeight(curveMask2Background, 142);
		ituWidgetSetPosition(curveMaskSprite, 46, 0);
		ituWidgetSetDimension(curveMaskSprite, 448, 142);		
		ituWidgetSetDimension(curveMask0Background, 448, 142);
		ituWidgetSetDimension(curveMask1Background, 448, 142);
		ituWidgetSetDimension(curveMask2Background, 448, 142);

		
		// ituSpriteGoto(curveMaskSprite, 1);		
		ituWidgetSetVisible(curveContainer, false);
		// ituWidgetSetVisible(curveXContainer, false);
		ituTextSetString(curveAvgPowerText, "0.0");
		ituWidgetSetVisible(curveCurve, true);
		ituWidgetSetDimension(curveCurve, 540, 124 + 26 + 10 + 10);
		ituWidgetSetPosition(curveCurve, 0, 0);

		// ituWidgetSetHeight(curveCurve, 124 + 10);
		// curveCurve->bg.icon.widget.color = spmode_bgColor;
		// curveCurve->bg.graidentColor = spmode_graidentColor;
		// curveCurve->lineWidth = 6;
		// curveCurve->pointCount = 13;
		// for (i = 0; i < 13; i++)
		// {
		// 	curveCurve->points[i].y = 143; 
		// 	curveCurve->points[i].x = 96 + 29 * i;
		// }

		// typedef struct
		// {
		// 	ITUBackground bg;						///< Base background widget definition
		// 	unsigned int curveFlags;				///< The flags to indicate the status of the curve.
		// 	int pointCount;							///< Total count of points
		// 	int xPoints[ITU_CURVE_MAX_POINT_COUNT]; ///< The x coordinate of points
		// 	int yPoints[ITU_CURVE_MAX_POINT_COUNT]; ///< The y coordinate of points
		// 	ITUSurface *cacheSurf;					///< The cache surface.

		// } ITUCurve;
		#if 1
		curveCurve->pointCount = 11;
		curveCurve->xPoints[curveCurve->pointCount] = 0x2ff;
		curveCurve->yPoints[curveCurve->pointCount] = 500;
		for (i = 0; i < curveCurve->pointCount; i++)
		{
			curveCurve->yPoints[i] = 143; 
			// curveCurve->points[i].x = 95 + 35 * i;
			curveCurve->xPoints[i] = 90 + 36 * i;
			curveCurve->xPoints[curveCurve->pointCount + i + 1] = i;
			curveCurve->yPoints[curveCurve->pointCount + i + 1] = 0;
		}
		// ituWidgetSetPosition(curveStarSprite, curveCurve->points[curveCurve->pointCount - 1].x - 27, curveCurve->points[curveCurve->pointCount - 1].y - 29);
		ituWidgetSetPosition(curveStarSprite, curveCurve->xPoints[curveCurve->pointCount - 1] - 29, curveCurve->yPoints[curveCurve->pointCount - 1] - 29);
		// ituIconLinkSurface(&curveCurve->bg.icon,curveLineIcon[1]);
		// ituWidgetUpdate(curveCurve, ITU_EVENT_LAYOUT, 0, 0, 0);
		// ituWidgetUpdate(curveCurve, ITU_EVENT_LOAD, 0, 0, 0);

		#else

		curveCurve->pointCount = 11;
		curveCurve->points[curveCurve->pointCount].x = 0x2ff;
		curveCurve->points[curveCurve->pointCount].y = 500;
		for (i = 0; i < curveCurve->pointCount; i++)
		{
			curveCurve->points[i].y = 143; 
			// curveCurve->points[i].x = 95 + 35 * i;
			curveCurve->points[i].x = 90 + 36 * i;
			curveCurve->points[curveCurve->pointCount + i + 1].x = i;
			curveCurve->points[curveCurve->pointCount + i + 1].y = 0;
		}
		// ituWidgetSetPosition(curveStarSprite, curveCurve->points[curveCurve->pointCount - 1].x - 27, curveCurve->points[curveCurve->pointCount - 1].y - 29);
		ituWidgetSetPosition(curveStarSprite, curveCurve->points[curveCurve->pointCount - 1].x - 30, curveCurve->points[curveCurve->pointCount - 1].y - 29);
		ituWidgetUpdate(curveCurve, ITU_EVENT_LAYOUT, 0, 0, 0);

		#endif
		return true;
	} 
	
	return false;
}

bool Layer_dynamicprogress_Init(void)
{
    if(!dynamicprogressContainer)
    {
        dynamicprogressContainer = ituSceneFindWidget(&theScene, "dynamicprogressContainer");
        assert(dynamicprogressContainer);
        progressDynamicPowerSprite = ituSceneFindWidget(&theScene, "progressDynamicPowerSprite");
        assert(progressDynamicPowerSprite);
        progressDynamicSpeedSprite = ituSceneFindWidget(&theScene, "progressDynamicSpeedSprite");
        assert(progressDynamicSpeedSprite);

		progressDynamicPowerSprite->delay = 0;
		ituSpriteStop(progressDynamicPowerSprite);
		ituSpriteGoto(progressDynamicPowerSprite, 0);

		progressDynamicSpeedSprite->delay = 0;
		ituSpriteStop(progressDynamicSpeedSprite);
		ituSpriteGoto(progressDynamicSpeedSprite, 0);
        return true;
    }
    return false;
}

bool Layer_esaveprogress_Init(void)
{
    if(!esaveprogressContainer)
    {
        esaveprogressContainer = ituSceneFindWidget(&theScene, "esaveprogressContainer");
        assert(esaveprogressContainer);
        progressEsavePowerSprite = ituSceneFindWidget(&theScene, "progressEsavePowerSprite");
        assert(progressEsavePowerSprite);
        progressEsaveSpeedSprite = ituSceneFindWidget(&theScene, "progressEsaveSpeedSprite");
        assert(progressEsaveSpeedSprite);

		progressEsavePowerSprite->delay = 0;
		ituSpriteStop(progressEsavePowerSprite);
		ituSpriteGoto(progressEsavePowerSprite, 0);

		progressEsaveSpeedSprite->delay = 0;
		ituSpriteStop(progressEsaveSpeedSprite);
		ituSpriteGoto(progressEsaveSpeedSprite, 0);
        return true;
    }
    return false;
}

bool Layer_sportprogress_Init(void)
{
    if(!sportprogressContainer)
    {
        sportprogressContainer = ituSceneFindWidget(&theScene, "sportprogressContainer");
        assert(sportprogressContainer);
        progressSportPowerSprite = ituSceneFindWidget(&theScene, "progressSportPowerSprite");
        assert(progressSportPowerSprite);
        progressSportSpeedSprite = ituSceneFindWidget(&theScene, "progressSportSpeedSprite");
        assert(progressSportSpeedSprite);

		progressSportPowerSprite->delay = 0;
		ituSpriteStop(progressSportPowerSprite);
		ituSpriteGoto(progressSportPowerSprite, 0);

		progressSportSpeedSprite->delay = 0;
		ituSpriteStop(progressSportSpeedSprite);
		ituSpriteGoto(progressSportSpeedSprite, 0);
        return true;
    }
    return false;
}

bool Layer_main_Init(void)
{
    if(!mainSportBackground)
    {
        mainSportBackground = ituSceneFindWidget(&theScene, "mainSportBackground");
        assert(mainSportBackground);
        mainEsaveBackground = ituSceneFindWidget(&theScene, "mainEsaveBackground");
        assert(mainEsaveBackground);
        mainDynamicBackground = ituSceneFindWidget(&theScene, "mainDynamicBackground");
        assert(mainDynamicBackground);

		ituWidgetSetVisible(mainSportBackground, false);
		ituWidgetSetVisible(mainEsaveBackground, false);
		ituWidgetSetVisible(mainDynamicBackground, false);
		Current_modeBackground = NULL;
		return true;
    }
    return false;
}


bool MainOnEnter(ITUWidget* widget, char* param)
{
	static int ui_delayset = 0;
    // if(!mainSportBackground)
    // {
        Layer_main_Init();
        Layer_sportprogress_Init();
        Layer_esaveprogress_Init();
        Layer_dynamicprogress_Init();
		Layer_time_Init();
		Layer_bottom_Init();
		Layer_curve_Init();
		Layer_recoveryprogress_Init();
		Layer_speedpower_Init();
		Layer_stand_Init();

        // curveLayer = ituSceneFindWidget(&theScene, "curveLayer");
        // assert(curveLayer);
        // standLayer = ituSceneFindWidget(&theScene, "standLayer");
        // assert(standLayer); 
        // bottomLayer = ituSceneFindWidget(&theScene, "bottomLayer");
        // assert(bottomLayer);
        // speedpowerLayer = ituSceneFindWidget(&theScene, "speedpowerLayer");
        // assert(speedpowerLayer); 
        // timeLayer = ituSceneFindWidget(&theScene, "timeLayer");
        // assert(timeLayer); 
		
		ituWidgetSetVisible(sportprogressContainer, false);
		ituWidgetSetVisible(esaveprogressContainer, false);
		ituWidgetSetVisible(dynamicprogressContainer, false);
		// Current_modeBackground = mainDynamicBackground;
		// Current_speedSprite = progressDynamicSpeedSprite;
		// Current_powerSprite = progressDynamicPowerSprite;
		// Current_powerContainer = dynamicprogressContainer;
		// Current_modeContainer = dynamicprogressContainer;
		
		// TestIconSurface = ituPngLoadFile(16, 16, buffer_png[0]);
		SceneSetReady(true);
	// }
	// ituWidgetSetVisible(mainSportBackground, false);
	// ituWidgetSetVisible(mainEsaveBackground, false);
	// ituWidgetSetVisible(mainDynamicBackground, true);

	// ituWidgetSetVisible(sp_TotalContainer, false);	
	ituWidgetSetVisible(timeTitleContainer, true);
	if(Current_modeBackground)
	{
		// ituWidgetSetVisible(Current_powerContainer, true);
		ituWidgetSetVisible(Current_modeContainer, true);
		ituWidgetSetVisible(Current_modeBackground, true);
		ituWidgetSetVisible(Current_speedSprite, true);
		ituWidgetSetVisible(Current_powerSprite, true);	
	}

	// ituWidgetSetVisible(sp_SpeedFrameNumberIcon, !charge_status);
	// ituWidgetSetVisible(sp_PowerFrameNumberIcon, !charge_status);
	
	// ituWidgetSetVisible(bottomBatRangeTagIcon, true);
	// ituWidgetSetVisible(bottomBatTagSprite, true);
	// ituWidgetSetVisible(bottomBatBalanceTagIcon, true);
	
	// ituWidgetSetDimension(mainDynamicBackground, 1246, 855);//source sidestand
	// ituWidgetSetPosition(mainDynamicBackground, 17, -170);//source sidestand
    
	// ituWidgetSetDimension(mainDynamicBackground, 760, 521);//ready
	// ituWidgetSetPosition(mainDynamicBackground, 260, -19);//ready

	// mainLayer_Ready_DisplayConfig(false,true);
	
	charge_status = false;		
	testmode_status = false;
	upgrade_status = false;
	sidestand_temple_status = false;
	ui_delayset = 0;
	// while (0 == ev_layer2.layer_displaydata.ScooterInfo.car_status1)
	while (0 == ev_layer2.layer_displaydata.ScooterInfo.car_gear)
	{
		usleep(10 * 1000);
		ui_delayset++;
		if (ui_delayset > 200)
		{
			break;
		}
	}
	// printf("ui_delayset=%d\t\n",ui_delayset);
	// if (ui_delayset <= 200)
	if (ev_layer2.layer_displaydata.ScooterInfo.car_gear > 0)
	{
		// ui_delayset = 0;
		Layer_MainMode_Task(&ev_layer2);
	}
	return true;
}


bool MainOnTimer(ITUWidget* widget, char* param)
{
	int pointx = 0;
	// if(local_niu_value_loc.db_loc.data.db_status2&(CAR_EX_BT1_INCHARGE|CAR_EX_BT2_INCHARGE))
	
	// if((!board_test)&&(!module_test))
	// {
	// 	if((local_niu_value_loc.db_loc.data.db_status&(CAR_INCHG)))	
	// 	{
	// 		ituLayerGoto(chargeLayer);
	// 		return true;
	// 	}
	// 	else if((local_niu_value_loc.db_loc.data.db_status&(CAR_VCU_UPDATE)))
	// 	{
	// 		ituLayerGoto(upgradeLayer);
	// 		return true;
	// 	}
	// }

	// if((local_niu_value_loc.db_loc.data.db_status&(CAR_INCHG))&&((!board_test)&&(!module_test)))	
	// {
	// 	ituLayerGoto(chargeLayer);
	// 	return true;
	// }	
	// layer_SpeedPower_Display();
	layer_SideStand_Display();
	layer_Signal_Display();
    return false;
}

bool mainLayer_Time_DisplayConfig(user_time_type timetype, unsigned char dis_param)
{
    ITUText* tempText = NULL;
	char buffer[8] = {0};	
	switch(timetype)
	{
		case TEXT_TIME_HOUR:			
			tempText = timeHourText;
			break;
		case TEXT_TIME_MINUTE:			
			tempText = timeMinuteText;
			break;
		default:			
			return false;
			break;
	}
	sprintf(buffer, "%02d", dis_param);
	if(0 != strcmp(ituTextGetString(tempText),buffer))
	{
		ituTextSetString(tempText, buffer);
	}
	return true;	
}

bool mainLayer_DriveMode_DisplayConfig(user_drivemode_type drivemode)
{
	bool isVisible = false;
	ITUSprite* spSprite[3][2] = { NULL };
	ITUBackground* modeBackground[3] = { NULL };
	ITUContainer* modeContainer[3] = { NULL };
	int mode = 0, i = 0;
	// if(!timeModeSprite||!curveAxisSprite)
	// {
	// 	return false;
	// }

	modeBackground[0] = mainSportBackground;
	modeBackground[1] = mainDynamicBackground;
	modeBackground[2] = mainEsaveBackground;

	modeContainer[0] = sportprogressContainer;
	modeContainer[1] = dynamicprogressContainer;
	modeContainer[2] = esaveprogressContainer;

	spSprite[0][0] = progressSportSpeedSprite;
    spSprite[0][1] = progressSportPowerSprite;
    spSprite[1][0] = progressDynamicSpeedSprite;
    spSprite[1][1] = progressDynamicPowerSprite;
    spSprite[2][0] = progressEsaveSpeedSprite;
    spSprite[2][1] = progressEsavePowerSprite;
    for (i = 0; i < 3; i++)
    {
        ituWidgetSetVisible(spSprite[i][0], isVisible);
        ituWidgetSetVisible(spSprite[i][1], isVisible);
		ituWidgetSetVisible(modeBackground[i], isVisible);
		ituWidgetSetVisible(modeContainer[i], isVisible);
	}    
	
	switch(drivemode)
	{
		case NO_DRIVER_MODE:
			// isVisible = false;
			// return false;
			break;
		case ICON_DRIVER_MODE_E:
            isVisible = true;	
			mode = 2;			
			break;
		case ICON_DRIVER_MODE_D:
            isVisible = true;	
			mode = 1;							
			break;
		case ICON_DRIVER_MODE_S:
			isVisible = true;	
			mode = 0;						
			break;
		default:
			return false;
			break;
	}	
	if (isVisible)
	{		
		// printf("mode=%d..\t\n", mode);
		ituSpriteGoto(timeModeSprite, mode);
		ituWidgetSetVisible(spSprite[mode][0], isVisible);
		if(!ituWidgetIsVisible(progressRecoveryPowerContainer))
		{
			ituWidgetSetVisible(spSprite[mode][1], isVisible);
		}
		// ituWidgetSetVisible(modeBackground[mode], isVisible);
		ituSpriteGoto(curveAxisSprite, ((mode == 2)?(1):(0)));
		ituSpriteGoto(curveStarSprite, ((mode == 2)?(1):(0)));
		ituSpriteGoto(curveMaskSprite, mode);
		ituWidgetSetVisible(curveMaskSprite, true);

		if (Current_modeContainer != modeContainer[mode])
		{
			Current_modeContainer = modeContainer[mode];
			Current_modeBackground = modeBackground[mode];
			Current_speedSprite = spSprite[mode][0];
			Current_powerSprite = spSprite[mode][1];
			// curveCurve->bg.icon.widget.color = modeColor[mode][0];
			// curveCurve->bg.graidentColor = modeColor[mode][1];

			// TestIconSurface = ituPngLoadFile(16, 16, buffer_png[(mode == 2)]);
			// curveTestIcon->surf = TestIconSurface;

			// ituIconLoadPngFile(curveTestIcon,buffer_png[(mode == 2)]);

			// TestSurface_temp = ituCreateSurface(TestIconSurface->width, TestIconSurface->height, TestIconSurface->pitch, TestIconSurface->format, NULL, TestIconSurface->flags);
			// ituBitBlt(TestSurface_temp, 0, 0, TestIconSurface->width, TestIconSurface->height, TestIconSurface, 0, 0);
			// ituStretchBlt(TestSurface_temp, 0, 0, TestIconSurface->width, TestIconSurface->height, TestIconSurface, 0, 0, 16, 16);
			// ituAlphaBlend(TestSurface_temp, 0, 0, TestIconSurface->width, TestIconSurface->height, TestIconSurface, 0, 0, 255);
			// curveTestIcon->surf = TestSurface_temp;
		
			// curveCurve->bg.icon.surf = TestSurface_temp;

			// curveCurve->bg.icon.surf = curveLineIcon[((mode == 2)?1:0)]->surf;
			// curveCurve->bg.icon.staticSurf = curveLineIcon[((mode == 2)?1:0)]->staticSurf;
			// ituIconLinkSurface(&curveCurve->bg.icon, curveLineIcon[((mode == 2) ? 1 : 0)]);
			// ituWidgetUpdate(curveCurve, ITU_EVENT_LAYOUT, 0, 0, 0);
			// Icon3->surf = testSurface1;
		}
		ituIconLinkSurface(&curveCurve->bg.icon, curveLineIcon[((mode == 2) ? 1 : 0)]);
		ituWidgetUpdate(curveCurve, ITU_EVENT_LAYOUT, 0, 0, 0);
	}
	ituWidgetSetVisible(timeModeSprite, isVisible);
	if(Current_modeBackground)
	{
		ituWidgetSetVisible(Current_modeBackground, isVisible);
		ituWidgetSetVisible(Current_modeContainer, isVisible);
	}

	return true;
}

bool mainLayer_SpeedValue_DisplayConfig(int dis_speed,user_unitlist_type dis_unit)
{	
	char buffer[8] = {0};	
	int speed_dis = dis_speed;
	int goto_spriteindex = dis_speed;
	// if (!sp_SpeedValueText || !sp_SpeedUnitsText||!Current_speedSprite)
	// {
	// 	return false;
	// }
	if(!Current_speedSprite)
	{
		return false;
	}
	goto_spriteindex = (dis_speed > 79) ? 79 : dis_speed;
	if (dis_unit == STATUS_IMPERIAL_UNIT)
	{
		speed_dis = (int)((float)dis_speed * 0.62137 + 0.5);
	}

	sprintf(buffer, "%02d", speed_dis);
	buffer[3] = '\0';
	if(0 != strcmp(ituTextGetString(sp_SpeedValueText),buffer))
	{
		ituTextSetString(sp_SpeedValueText, buffer);
	}

	ituSpriteGoto(Current_speedSprite, goto_spriteindex);
	return true;	
}

bool mainLayer_BatteryACapacity_DisplayConfig(int dis_capacity)
{
	int dw = 0;
	// if (!bottomBatCapacityText || !bottomBatABackground || !bottomBatSprite)
	// {
	// 	return false;
	// }

	// ituSpriteGoto(bottomBatSprite, 1);
	dw = (int)(1.24 * dis_capacity);
	if (ituWidgetGetWidth(bottomBatABackground) != dw)
	{
		ituWidgetSetWidth(bottomBatABackground, dw); 
	}
	return true;
}

bool mainLayer_BatteryBCapacity_DisplayConfig(int dis_capacity)
{
	int dw = 0;
	// if (!bottomBatCapacityText || !bottomBatBBackground || !bottomBatSprite)
	// {
	// 	return false;
	// }

	// ituSpriteGoto(bottomBatSprite, 1);
	dw = (int)(1.24 * dis_capacity);
	if (ituWidgetGetWidth(bottomBatBBackground) != dw)
	{
		ituWidgetSetWidth(bottomBatBBackground, dw);
	}
	return true;
}

bool mainLayer_BatteryDual_DisplayConfig(int dis_capacity_a, int dis_capacity_b)
{
	// if (!bottomBatCapacityText || !bottomBatBBackground || !bottomBatSprite)
	// {
	// 	return false;
	// }
	mainLayer_BatteryACapacity_DisplayConfig(dis_capacity_a);
	mainLayer_BatteryBCapacity_DisplayConfig(dis_capacity_b);
	ituSpriteGoto(bottomBatSprite, 1);
	return true;
}

bool mainLayer_BatteryBalance_DisplayConfig(bool dis_balanced)
{
	int index = 0;
	// if (!bottomBatTagSprite)
	// {
	// 	return false;
	// }
	if(dis_balanced)
	{
		index = 1;
	}	
	ituSpriteGoto(bottomBatTagSprite, index);
	return true;
}

bool mainLayer_BatterySingle_DisplayConfig(int dis_capacity)
{
	int dw = 0;
	// if (!bottomBatCapacityText || !bottomSingleProgressBackground || !bottomBatSprite)
	// {
	// 	return false;
	// }
	if (upgrade_status)
	{
		return false;
	}
	dw = (int)(2.68 * dis_capacity);
	if (ituWidgetGetWidth(bottomSingleProgressBackground) != dw)
	{
		ituWidgetSetWidth(bottomSingleProgressBackground, dw);
	}
	ituSpriteGoto(bottomBatSprite, 0);
	return true;
}

bool mainLayer_BatteryCapacity_DisplayConfig(int dis_capacity_bat)
{
	char buffer[8] = {0};
	char buffer_unit[3] = {0};
	ITUText* bufferText = NULL;
	bool isVisible = true;
	// int dis_capacity = dis_capacity_bat;	
	// if(!bottomBatCapacityText||!sp_SpeedValueText)
	// {
	// 	return false;
	// }
	if (upgrade_status)
	{
		return false;
	}
	bufferText = bottomBatCapacityText;	
	if(charge_status)
	{
		bufferText = sp_SpeedValueText;
		buffer_unit[0] = '%';
		buffer_unit[1] = 0;
		if(0 != strcmp(ituTextGetString(sp_SpeedUnitsText),buffer_unit))
		{
			ituTextSetString(sp_SpeedUnitsText, buffer_unit);
		}
		isVisible = false;
	}
	// ituWidgetSetVisible(bottomBatBalanceTagIcon, isVisible);
	ituWidgetSetVisible(bottomBatTagSprite, isVisible);
	ituWidgetSetVisible(bottomBatRangeTagIcon, isVisible);	
	ituWidgetSetVisible(bottomBatCapacityText, isVisible);
	ituWidgetSetVisible(bottomPercentMarkText, isVisible);
	ituWidgetSetVisible(sp_SpeedUnitsText, true);
	snprintf(buffer, sizeof(buffer), "%02d ", dis_capacity_bat);
	if(0 != strcmp(ituTextGetString(bufferText),buffer))
	{
		ituTextSetString(bufferText, buffer);	
	}
	return true;
}


bool mainLayer_Battery_DisplayConfig(int dis_capacity_bata, int dis_capacity_batb, user_bat_type bat_type)
{
	switch (bat_type)
	{
	case STATUS_SINGLE_BAT:
		mainLayer_BatterySingle_DisplayConfig(dis_capacity_bata);
		break;
	case STATUS_DUAL_BAT:
		mainLayer_BatteryDual_DisplayConfig(dis_capacity_bata, dis_capacity_batb);
		break;
	default:
		return false;
		break;
	}	
	return true;
}

bool mainLayer_Temperature_DisplayConfig(int dis_temperature)
{
	char buffer[8] = {0};
	int dw_s = 45, px_s = 401;
	int temperature_dis = dis_temperature;
	// if (!timeAirtemperatureText ||!timeAirtemperatureUnitsIcon)
	// {
	// 	return false;
	// }
	// temperature_dis -= 20;
	sprintf(buffer, "%d", temperature_dis);
	if (temperature_dis < 0)
	{
		dw_s += 22;
		px_s += 22;		
	}
	else if(temperature_dis < 10)
	{
		dw_s -= 22;
		px_s -= 22;
	}
	if(ituWidgetGetWidth(timeAirtemperatureText) != dw_s)
	{
		ituWidgetSetWidth(timeAirtemperatureText, dw_s);
	}

	if(ituWidgetGetX(timeAirtemperatureText) != px_s)
	{
		ituWidgetSetX(timeAirtemperatureUnitsIcon, px_s);
	}

	if(0 != strcmp(ituTextGetString(timeAirtemperatureText),buffer))
	{
		ituTextSetString(timeAirtemperatureText, buffer);
	}
	return true;
}

bool mainLayer_RemainMileage_DisplayConfig(int dis_range,user_unitlist_type dis_unit)
{
	char buffer[8] = {0};
	char buffer_km[3] = {0};
	ITUText *bufferText[2] = {NULL};
	int range_dis = dis_range/1000, px_s = 0;
	// if (!bottomMieagePreestimateText || !bottomMieageUintsText)
	// {
	// 	return false;
	// }
	if (upgrade_status)
	{
		return false;
	}
	bufferText[0] = bottomMieagePreestimateText;
	bufferText[1] = bottomMieageUintsText;

	if (charge_status)
	{
		ituWidgetSetVisible(sp_PowerSignedText, false);
		bufferText[0] = sp_PowerValueText;
		bufferText[1] = sp_PowerUnitsText;
	}
	else
	{
		ituWidgetSetVisible(bottomMieagePreestimateText, true);
		ituWidgetSetVisible(bottomMieageUintsText, true);
		if (range_dis < 100)
		{
			px_s = 547;
		}
		else
		{
			px_s = 572;
		}
		if(ituWidgetGetX(bufferText[1]) != px_s)
		{
			ituWidgetSetX(bufferText[1], px_s);
		}
	}

	buffer_km[0] = 'k';
	buffer_km[1] = 'm';
	if (dis_unit == STATUS_IMPERIAL_UNIT)
	{
		buffer_km[0] = 'm';
		buffer_km[1] = 'i';
		range_dis = (int)((float)dis_range * 0.62137 + 0.5);
	}

	if (dis_range >= 0)
	{
		sprintf(buffer, "%02d", range_dis);
	}
	else
	{
		strcpy(buffer, "--");
	}
	// sprintf(buffer, "%02d", range_dis);

	if(0 != strcmp(ituTextGetString(bufferText[0]),buffer))
	{
		ituTextSetString(bufferText[0], buffer);
	}

	if(0 != strcmp(ituTextGetString(bufferText[1]),buffer_km))
	{
		ituTextSetString(bufferText[1], buffer_km);
	}

	return true;
}

bool mainLayer_AverageEnergy_DisplayConfig(int dis_avg_energy,user_unitlist_type dis_unit)
{
	char buffer[20] = {0};
	char buffer1[8] = {0};
	char buffer2[8] = {0};
	int temp_a = 0, temp_b = 0;
	char buffer_unit[6] = {'W','h','/','k','m','\0'};
	temp_a = abs(dis_avg_energy) / 100;
	temp_b = (abs(dis_avg_energy) % 100);
	// if (!curveAvgPowerText || !curveAvgPowerUnitsText)
	// {
	// 	return false;
	// }
	
	buffer_unit[3] = 'k';
	buffer_unit[4] = 'm';
	if (dis_unit == STATUS_IMPERIAL_UNIT)
	{
		buffer_unit[3] = 'm';
		buffer_unit[4] = 'i';
	}
	buffer_unit[5] = 0;
	// sprintf(buffer, "%.1f", avg_energy_dis);
	// sprintf(buffer1, "%d", temp_a);
	// sprintf(buffer2, "%d", temp_b);
	// sprintf(buffer, "%d.%d", temp_a, temp_b);
	// buffer[strlen(buffer1) + 2] = '\0';
	if (abs(temp_a) > 0)
	{
		sprintf(buffer, "%d", temp_a);
	}
	else
	{
		sprintf(buffer, "%d.%d", temp_a,temp_b);		
		// snprintf(buffer, sizeof(buffer), "%d.%d", temp_a,temp_b);
	}
	buffer[3] = '\0';
	if (0 != strcmp(ituTextGetString(curveAvgPowerText), buffer))
	{
		ituTextSetString(curveAvgPowerText, buffer);
	}

	if(0 != strcmp(ituTextGetString(curveAvgPowerUnitsText),buffer_unit))
	{
		ituTextSetString(curveAvgPowerUnitsText, buffer_unit);
	}
	return true;
}

bool mainLayer_RealtimeEnergyCurve_DisplayConfig(int dis_realtime_energy)
{
	int realtime_energy_dis = abs(dis_realtime_energy);

	#if	0
	curve_display_status = true;
	realtime_energy_dis *= 143;
	realtime_energy_dis /= 900;
	curve_display_value = 143 - realtime_energy_dis - 4;
	if(curve_display_value < 20)
	{
		curve_display_value = 20;
	}
	else
	{
		if (curve_display_value >= (143 - 4))
		{
			curve_display_value = 139;
		}
	}	
	#endif
		// typedef struct
		// {
		// 	ITUBackground bg;						///< Base background widget definition
		// 	unsigned int curveFlags;				///< The flags to indicate the status of the curve.
		// 	int pointCount;							///< Total count of points
		// 	int xPoints[ITU_CURVE_MAX_POINT_COUNT]; ///< The x coordinate of points
		// 	int yPoints[ITU_CURVE_MAX_POINT_COUNT]; ///< The y coordinate of points
		// 	ITUSurface *cacheSurf;					///< The cache surface.

		// } ITUCurve;
	#if 1
	int pointx = 0;
	int i = 0;
	curve_display_status = true;
	for (i = 0; i < curveCurve->pointCount; i++)
	{
		pointx = curveCurve->pointCount + i + 1;
		if (i == (curveCurve->pointCount - 1))
		{
			curveCurve->yPoints[pointx] = realtime_energy_dis;
		}
		else
		{
			curveCurve->yPoints[pointx] = curveCurve->yPoints[pointx + 1];
		}	
	}
	curveCurve->yPoints[curveCurve->pointCount] = curveCurve->yPoints[curveCurve->pointCount + 1];
	for (i = 0; i < curveCurve->pointCount; i++)
	{
		pointx = curveCurve->pointCount + i + 1;
		if (curveCurve->yPoints[curveCurve->pointCount] < curveCurve->yPoints[pointx])
		{
			curveCurve->yPoints[curveCurve->pointCount] = curveCurve->yPoints[pointx];			
		}
	}
	if (curveCurve->yPoints[curveCurve->pointCount] <= 800)
	{
		curveCurve->yPoints[curveCurve->pointCount] += 200;
	}
	else
	{
		curveCurve->yPoints[curveCurve->pointCount] *= 5;
		curveCurve->yPoints[curveCurve->pointCount] >>= 2;
	}	

	// printf("value=%d,y=%d\t\n", dis_realtime_energy, curveCurve->points[curveCurve->pointCount].y);
	for (i = 0; i < curveCurve->pointCount; i++)
	{
		pointx = curveCurve->pointCount + i + 1;
		realtime_energy_dis = curveCurve->yPoints[pointx];
		realtime_energy_dis *= 143;
		realtime_energy_dis /= curveCurve->yPoints[curveCurve->pointCount];
		//curve_display_value = 143 - realtime_energy_dis;		
		curveCurve->yPoints[i]= 143 - realtime_energy_dis;
	}

	ituWidgetSetPosition(curveStarSprite, curveCurve->xPoints[curveCurve->pointCount - 1] - 29, curveCurve->yPoints[curveCurve->pointCount - 1] - 29);
	ituWidgetUpdate(curveCurve, ITU_EVENT_LAYOUT, 0, 0, 0);
	#else

	int pointx = 0;
	int i = 0;
	curve_display_status = true;
	for (i = 0; i < curveCurve->pointCount; i++)
	{
		pointx = curveCurve->pointCount + i + 1;
		if (i == (curveCurve->pointCount - 1))
		{
			curveCurve->points[pointx].y = realtime_energy_dis;
		}
		else
		{
			curveCurve->points[pointx].y = curveCurve->points[pointx + 1].y;
		}	
	}
	curveCurve->points[curveCurve->pointCount].y = curveCurve->points[curveCurve->pointCount + 1].y;
	for (i = 0; i < curveCurve->pointCount; i++)
	{
		pointx = curveCurve->pointCount + i + 1;
		if (curveCurve->points[curveCurve->pointCount].y < curveCurve->points[pointx].y)
		{
			curveCurve->points[curveCurve->pointCount].y = curveCurve->points[pointx].y;			
		}
	}
	if (curveCurve->points[curveCurve->pointCount].y <= 800)
	{
		curveCurve->points[curveCurve->pointCount].y += 200;
	}
	else
	{
		curveCurve->points[curveCurve->pointCount].y *= 5;
		curveCurve->points[curveCurve->pointCount].y >>= 2;
	}	

	// printf("value=%d,y=%d\t\n", dis_realtime_energy, curveCurve->points[curveCurve->pointCount].y);
	for (i = 0; i < curveCurve->pointCount; i++)
	{
		pointx = curveCurve->pointCount + i + 1;
		realtime_energy_dis = curveCurve->points[pointx].y;
		realtime_energy_dis *= 143;
		realtime_energy_dis /= curveCurve->points[curveCurve->pointCount].y;
		//curve_display_value = 143 - realtime_energy_dis;		
		curveCurve->points[i].y = 143 - realtime_energy_dis;
	}

	ituWidgetSetPosition(curveStarSprite, curveCurve->points[curveCurve->pointCount - 1].x - 30, curveCurve->points[curveCurve->pointCount - 1].y - 29);
	ituWidgetUpdate(curveCurve, ITU_EVENT_LAYOUT, 0, 0, 0);

	#endif
	return true;	
}

bool mainLayer_Total_DisplayConfig(int dis_total,user_unitlist_type dis_unit)
{
	char buffer[32] = {0};
	char buffer_km[3] = {'k','m','\0'};
	int length = 0,px_s = 0,dw_s = 0,ptx_s = 0;
	int total_dis = dis_total;
	// if (!sp_TotalValueText || !sp_TotalUnitsText || !sp_TotalTagText || !sp_TotalContainer)
	// {
	// 	return false;
	// }
	if (upgrade_status)
	{
		return false;
	}
	buffer_km[0] = 'k';
	buffer_km[1] = 'm';
    if (dis_unit == STATUS_IMPERIAL_UNIT)
    {
        buffer_km[0] = 'm';
		buffer_km[1] = 'i';
		total_dis = (int)((float)dis_total * 0.62137 + 0.5);
	}	
	if(0 != strcmp(ituTextGetString(sp_TotalUnitsText),buffer_km))
	{
		ituTextSetString(sp_TotalUnitsText, buffer_km);
	}
	
	sprintf(buffer, "%04d", total_dis);
	// sprintf(buffer, "%d", total_dis);
	length = strlen(buffer);
    if (length > 4)
    {
		px_s = 185 - (length - 4) * 19;
		dw_s = 77 + (length - 4) * 19;
		ptx_s = 134 - (length - 4) * 19;
    }
    else
    {
		px_s = 185;
		dw_s = 77;
		ptx_s = 134;
	}
	if(ituWidgetGetX(sp_TotalValueText) != px_s)
	{
		ituWidgetSetX(sp_TotalValueText, px_s);
	}

	if(ituWidgetGetWidth(sp_TotalValueText) != dw_s)
	{
		ituWidgetSetWidth(sp_TotalValueText, dw_s);
	}

	if(ituWidgetGetX(sp_TotalTagText) != ptx_s)
	{
		ituWidgetSetX(sp_TotalTagText, ptx_s);
	}

	if(0 != strcmp(ituTextGetString(sp_TotalValueText),buffer))
	{
		ituTextSetString(sp_TotalValueText, buffer);
	}
    return true;
}

bool mainLayer_RealtimeEnergy_DisplayConfig(int dis_realtime_energy,int dis_rtenergy)
{
	#if 0
	char buffer[10] = {0};
	int length = 0;
	int temp_realtime_energy = dis_realtime_energy/10;	
	int goto_spriteindex = 0;
	float temp_f = 0, pw_f = 0,ph_f = 0;
	int px_f = 0, py_f = 0;

	if (!Current_modeBackground||!Current_powerSprite)
	{
		return false;
	}

	#if 1
    temp_realtime_energy = abs(temp_realtime_energy);
    goto_spriteindex = (temp_realtime_energy > 39) ? 79 : (temp_realtime_energy<<1);
	if (temp_realtime_energy > 9)
	{
		sprintf(buffer, "%d.%d", (temp_realtime_energy / 10), (temp_realtime_energy % 10));
	}
	else
	{
		if (dis_realtime_energy)
		{
			sprintf(buffer, ".%d%d", temp_realtime_energy, abs(dis_realtime_energy % 10));
		}
		else
		{
			buffer[0] = '0';
			buffer[1] = '.';
			buffer[2] = '0';
		}
	}	
	buffer[3] = '\0';
	#endif

	if(0 != strcmp(ituTextGetString(sp_PowerValueText),buffer))
	{
		ituTextSetString(sp_PowerValueText, buffer);
	}
	
	if (dis_realtime_energy < 0 )
	{
		ituWidgetSetVisible(sp_PowerSignedText, true);
		if(goto_spriteindex > 0)
		{
			ituSpriteGoto(progressRecoveryPowerSprite, goto_spriteindex - 1);
			ituWidgetSetVisible(Current_powerSprite, false);
			ituWidgetSetVisible(progressRecoveryPowerContainer, true);
		}		
    }		
    else
    {
        ituWidgetSetVisible(sp_PowerSignedText, false);		
		ituSpriteGoto(Current_powerSprite, goto_spriteindex);		
		ituWidgetSetVisible(progressRecoveryPowerContainer, false);
		ituWidgetSetVisible(Current_powerSprite, true);	
    }	

	length = (temp_realtime_energy > 39) ? 80 : (temp_realtime_energy << 1);
	if (dis_realtime_energy < 0)
	{
		temp_f = (1.0 - (length * 3) / 1000.0);
	}
	else
	{
		temp_f = (1.0 + (length * 5) / 1000.0);
	}
	pw_f = 760 * temp_f;
	ph_f = 521 * temp_f;
	if((ituWidgetGetWidth(Current_modeBackground) != pw_f)||(ituWidgetGetHeight(Current_modeBackground) != ph_f))
	{
		ituWidgetSetDimension(Current_modeBackground, (int)pw_f, (int)ph_f);
	}
	px_f = 1280 - (int)pw_f;
	px_f >>= 1;
	temp_f = ph_f - 480;
	temp_f *= (-170.0 / 187.5);
	py_f = (int)temp_f;
	py_f >>= 1;
	if((ituWidgetGetX(Current_modeBackground) != px_f)||(ituWidgetGetY(Current_modeBackground) != py_f))
	{
		ituWidgetSetPosition(Current_modeBackground, px_f, py_f);
	}
	#else
	
	char buffer[10] = {0};
	int length = 0;
	int temp_realtime_energy = dis_realtime_energy / 10;
	int tp_realtime_energy = ((dis_realtime_energy << 1) / 10);
	int goto_spriteindex = 0;
	float temp_f = 0, pw_f = 0,ph_f = 0;
	int px_f = 0, py_f = 0;

	// if (!sp_PowerValueText || !sp_PowerSignedText || !Current_modeBackground)
	if (!Current_modeBackground||!Current_powerSprite)
	{
		return false;
	}

	#if 1
    // printf("dis_realtime_energy=%d...\t\n", dis_realtime_energy);
	temp_realtime_energy = abs(temp_realtime_energy);
	tp_realtime_energy = abs(tp_realtime_energy);
	goto_spriteindex = (tp_realtime_energy > 79) ? 79 : (tp_realtime_energy);
	if (temp_realtime_energy > 9)
	{
		sprintf(buffer, "%d.%d", (temp_realtime_energy / 10), (temp_realtime_energy % 10));
	}
	else
	{
		if (dis_realtime_energy)
		{
			sprintf(buffer, ".%d%d", temp_realtime_energy, abs(dis_realtime_energy % 10));
		}
		else
		{
			buffer[0] = '0';
			buffer[1] = '.';
			buffer[2] = '0';
		}
	}	
	buffer[3] = '\0';
	#endif
  
	if(0 != strcmp(ituTextGetString(sp_PowerValueText),buffer))
	{
		ituTextSetString(sp_PowerValueText, buffer);
	}	
	
	if (dis_realtime_energy < 0 )
	{
		ituWidgetSetVisible(sp_PowerSignedText, true);
		if(goto_spriteindex > 0)
		{
			ituSpriteGoto(progressRecoveryPowerSprite, goto_spriteindex - 1);
			ituWidgetSetVisible(Current_powerSprite, false);
			ituWidgetSetVisible(progressRecoveryPowerContainer, true);
		}		
    }		
    else
    {
        ituWidgetSetVisible(sp_PowerSignedText, false);		
		ituSpriteGoto(Current_powerSprite, goto_spriteindex);		
		ituWidgetSetVisible(progressRecoveryPowerContainer, false);
		ituWidgetSetVisible(Current_powerSprite, true);	
    }	

	length = (tp_realtime_energy > 79) ? 80 : (tp_realtime_energy);
	if (dis_realtime_energy < 0)
	{
		temp_f = (1.0 - (length * 3) / 1000.0);
	}
	else
	{
		temp_f = (1.0 + (length * 5) / 1000.0);
	}
	pw_f = 760 * temp_f;
	ph_f = 521 * temp_f;
	if((ituWidgetGetWidth(Current_modeBackground) != pw_f)||(ituWidgetGetHeight(Current_modeBackground) != ph_f))
	{
		ituWidgetSetDimension(Current_modeBackground, pw_f, ph_f);
	}
	px_f = 1280 - (int)pw_f;
	px_f >>= 1;
	temp_f = ph_f - 480;
	temp_f *= (-170.0 / 187.5);
	py_f = (int)temp_f;
	py_f >>= 1;
	if((ituWidgetGetX(Current_modeBackground) != px_f)||(ituWidgetGetY(Current_modeBackground) != py_f))
	{
		ituWidgetSetPosition(Current_modeBackground, px_f, py_f);
	}

	#endif
	return true;	
}

#if 0
bool mainLayer_NetworksSignal_DisplayConfig(user_indicationlist_type dis_signallist,user_indicationaction_type dis_action)
{
	int i = 0;
	int j = 0;
	ITUIcon* signal_icon[4] = {NULL};
	int alpha_set[4] = { 255, 102, 102, 102};
	// if(!sp_GSMSignalIcon[0]||!sp_GPSSignalIcon[0])
	// {
	// 	return false;
	// }
	switch(dis_action)
	{		
		case STATUS_SIGNAL_LEVEL3:
			alpha_set[3] = 255;						
		case STATUS_SIGNAL_LEVEL2:
            alpha_set[2] = 255;            
		case STATUS_SIGNAL_LEVEL1:
			alpha_set[1] = 255;
			break;
		case STATUS_SIGNAL_LEVEL0:
			j = 0x01;				
			break;
		default:
			return false;
			break;		
	}

	switch (dis_signallist)
	{
	case ICON_MobileNetSignal:
		for (i = 0; i < 4;i++)
		{
			signal_icon[i] = sp_GSMSignalIcon[i];
		}
		j |= 0x80;
		if (j == 0x81)
		{
			if ((signal_status & 0x10) == 0)
			{
				signal_status |= 0x10;
			}
		}
		else
		{
			signal_status &= ~0x10;
		}
		break;
	case ICON_SatelliteSignal:
		for (i = 0; i < 4; i++)
		{
			signal_icon[i] = sp_GPSSignalIcon[i];
		}
		j |= 0x08;
		if (j == 0x09)
		{
			if ((signal_status & 0x20) == 0)
			{
				signal_status |= 0x20;
			}
		}
		else
		{
			signal_status &= ~0x20;
		}
		break;
	default:
		return false;
		break;
	}	
	for (i = 1; i < 4; i++)
	{
		ituWidgetSetAlpha(signal_icon[i], alpha_set[i]);
	}
	return true;
}

void layer_Signal_Display(void)
{	
	if (signal_status & 0x30)
	{
		delay_count_signal++;
		if (delay_count_signal % 20 == 0)
		{
			signal_isvisible = !signal_isvisible;
		}

		if (signal_status & 0x10)
		{
			ituWidgetSetVisible(sp_GSMSignalIcon[0], signal_isvisible);
		}
		else
		{
			ituWidgetSetVisible(sp_GSMSignalIcon[0], true);
		}		

		if (signal_status & 0x20)
		{
			ituWidgetSetVisible(sp_GPSSignalIcon[0], signal_isvisible);
		}
		else
		{
			ituWidgetSetVisible(sp_GPSSignalIcon[0], true);
		}
		// return true;
	}
	else
	{
		delay_count_signal = 0;
		signal_isvisible = true;
		ituWidgetSetVisible(sp_GSMSignalIcon[0], signal_isvisible);
		ituWidgetSetVisible(sp_GPSSignalIcon[0], signal_isvisible);
	}
}
#else
bool mainLayer_NetworksSignal_DisplayConfig(user_indicationlist_type dis_signallist,user_indicationaction_type dis_action)
{
	int j = 0;
	bool isvisible[2] = { false,false };
	// if(!sp_GSMSignalIcon[0]||!sp_GPSSignalIcon[0])
	// {
	// 	return false;
	// }
	switch(dis_action)
	{		
		case STATUS_SIGNAL_LEVEL3:
			isvisible[0] = true;
			isvisible[1] = true;
			break;
		case STATUS_SIGNAL_LEVEL2:
			j = 2;
			isvisible[0] = true;			
			break;
		case STATUS_SIGNAL_LEVEL1:
			j = 1;			
			break;
		case STATUS_SIGNAL_LEVEL0:
			isvisible[0] = false;
			isvisible[1] = false;
			break;
		default:
			return false;
			break;		
	}

	switch (dis_signallist)
	{
	case ICON_MobileNetSignal:
		j |= 0x80;
		switch(j)
		{
			case 0x81:				
				signal_status |= 0x10;
				signal_status &= ~0x20;	
				ituWidgetSetVisible(sp_GSMSignalIcon[1], isvisible[1]);					
				break;
			case 0x82:				
				signal_status |= 0x20;
				signal_status &= ~0x10;	
				ituWidgetSetVisible(sp_GSMSignalIcon[0], isvisible[0]);						
				break;
			case 0x80:
				signal_status &= ~0x70;
				ituWidgetSetVisible(sp_GSMSignalIcon[0], isvisible[0]);
				ituWidgetSetVisible(sp_GSMSignalIcon[1], isvisible[1]);
				break;
			default:
				return false;
				break;
		}		
		break;
	case ICON_SatelliteSignal:
		j |= 0x40;
		switch(j)
		{
			case 0x41:				
				signal_status |= 0x01;
				signal_status &= ~0x02;	
				ituWidgetSetVisible(sp_GPSSignalIcon[1], isvisible[1]);				
				break;
			case 0x42:				
				signal_status |= 0x02;	
				signal_status &= ~0x01;
				ituWidgetSetVisible(sp_GPSSignalIcon[0], isvisible[0]);						
				break;
			case 0x40:				
				signal_status &= ~0x07;
				ituWidgetSetVisible(sp_GPSSignalIcon[0], isvisible[0]);
				ituWidgetSetVisible(sp_GPSSignalIcon[1], isvisible[1]);
				break;
			default:
				return false;
				break;
		}
		break;
	default:
		return false;
		break;
	}
	return true;
}

void layer_Signal_Display(void)
{	
	if (signal_status & 0x33)
	{
		delay_count_signal++;
		if (delay_count_signal % 30 == 0)
		{
			signal_isvisible = !signal_isvisible;
		}
		
		if(signal_status & 0x30)
		{
			if (signal_status & 0x10)
			{
				ituWidgetSetVisible(sp_GSMSignalIcon[0], signal_isvisible);
			}
			else if(signal_status & 0x20)
			{
				ituWidgetSetVisible(sp_GSMSignalIcon[1], signal_isvisible);
			}	
		}

		if(signal_status & 0x03)
		{
			if (signal_status & 0x01)
			{
				ituWidgetSetVisible(sp_GPSSignalIcon[0], signal_isvisible);
			}
			else if(signal_status & 0x02)
			{
				ituWidgetSetVisible(sp_GPSSignalIcon[1], signal_isvisible);
			}
		}
		// return true;
	}
	else
	{
		delay_count_signal = 0;
		signal_isvisible = true;
	}
}



#endif

void layer_SideStand_Display(void)
{	
	ITUIcon* bufferIcon = standsideIcon;
	if(charge_status)
	{
		bufferIcon = chargeSideIcon;
	}
	// bufferIcon = timeSideIcon;
	if (sidestand_temple_status)
	{
		if ((delay_count % 2) == 0)
		{
			side_isvisible = !side_isvisible;
		}
		delay_count++;
		if ((delay_count / 2) >= 6)
		{
			sidestand_temple_status = false;
			side_isvisible = true;
			delay_count = 0;
		}
		ituWidgetSetVisible(bufferIcon, side_isvisible);
	}
	else
	{
		delay_count = 0;
		side_isvisible = true;
	}
}

bool  mainLayer_Workstatus_DisplayConfig(user_indicationlist_type dis_worklist,user_indicationaction_type dis_action)
{
	ITUWidget* icon_temp = NULL;
	bool isVisible = false;
	// if(!timeCruiseControlIcon||!timeEnergyRecoveryIcon||!timeAutoLightIcon||!timeEnergyConservationIcon||!timeCloudConnectIcon)
	// {
	// 	return false;
	// }

	switch(dis_action)
	{
		case STATUS_SHOW:
			isVisible = true;
			break;
		case STATUS_HIDDEN:
			isVisible = false;
			break;
		default:
			return false;
			break;
	}

	switch (dis_worklist)
	{	
	case ICON_Autolight:
		icon_temp = (ITUWidget*)timeAutoLightIcon;		
		break;
	case ICON_CruiseControl:
		icon_temp = (ITUWidget*)timeCruiseControlIcon;		
		break;
	case ICON_EnergyConservation:
		icon_temp = (ITUWidget*)timeEnergyConservationIcon;		
		break;
	case ICON_EnergyRecovery:
		icon_temp = (ITUWidget*)timeEnergyRecoveryIcon;		
		break;	
	case ICON_CloudService:
		icon_temp = (ITUWidget*)timeCloudConnectIcon;		
		break;	
	default:
		return false;
		break;
	}	
	ituWidgetSetVisible(icon_temp, isVisible);
	return true;
}


bool mainLayer_SpeedUnit_DisplayConfig(user_unitlist_type dis_unit,user_indicationaction_type dis_action)
{	
	char buffer[8] = {0};
	bool isVisible = false;
	// if (!sp_SpeedUnitsText)
	// {
	// 	return false;
	// }
	switch (dis_action)
	{
		case STATUS_SHOW:
			isVisible = true;
			break;
		case STATUS_HIDDEN:
			isVisible = false;
			break;
		default:
			return false;
			break;
	}	
	switch(dis_unit)
	{
		case STATUS_METRIC_UNIT:
			// strcpy(buffer,"km/h");
			buffer[0] = 'k';
			buffer[1] = 'm';
			buffer[2] = '/';
			buffer[3] = 'h';
			break;
		case STATUS_IMPERIAL_UNIT:
			// strcpy(buffer, "mph");
			buffer[0] = 'm';
			buffer[1] = 'p';
			buffer[2] = 'h';			
			break;
		default:
			return false;
			break;
	}

	if(0 != strcmp(ituTextGetString(sp_SpeedUnitsText),buffer))
	{
		ituTextSetString(sp_SpeedUnitsText, buffer);
	}
	ituWidgetSetVisible(sp_SpeedUnitsText, isVisible);
	return true;	
}

bool mainLayer_LastTripMileage_DisplayConfig(int dis_lasttrip_mileage, user_unitlist_type dis_unit)
{
	char buffer[10] = { 0 };
	char buffer_km[3] = { 0 };
	int mileage_dis = dis_lasttrip_mileage/100;
	// if (!standRangeText || !standRangeUnitsText)
	// {
	// 	return false;
	// }
	buffer_km[0] = 'k';
	buffer_km[1] = 'm';
	if (dis_unit == STATUS_IMPERIAL_UNIT)
	{
		buffer_km[0] = 'm';
		buffer_km[1] = 'i';
		mileage_dis = (int)((float)mileage_dis * 0.62137 + 0.5);
	}
	sprintf(buffer, "%d.%d ", mileage_dis / 10, mileage_dis % 10);
	buffer[5] = '\0';

	if(0 != strcmp(ituTextGetString(standRangeText),buffer))
	{
		ituTextSetString(standRangeText, buffer);
	}

	if(0 != strcmp(ituTextGetString(standRangeUnitsText),buffer_km))
	{
		ituTextSetString(standRangeUnitsText, buffer_km);
	}
	return true;
}

bool mainLayer_LastTripTime_DisplayConfig(int dis_lasttrip_time)
{
	char buffer[10] = { 0 };
	int px_s = 0, dw_s = 0;
	int lasttrip_time_h_dis = dis_lasttrip_time / 3600;
	int lasttrip_time_m_dis = ((dis_lasttrip_time % 3600) / 60);
	int lasttrip_time_s_dis = ((dis_lasttrip_time % 3600) % 60);
	// if (!standTimesText)
	// {
	// 	return false;
	// }	
	
	if (lasttrip_time_h_dis)
	{
		if (lasttrip_time_h_dis > 999)
		{
			lasttrip_time_h_dis = 999;
		}
		px_s = 209 - 35;
		dw_s = 188;
		sprintf(buffer, "%02d:%02d:%02d", lasttrip_time_h_dis,lasttrip_time_m_dis, lasttrip_time_s_dis);
	}
	else
	{
		px_s = 209;
		dw_s = 118;
		sprintf(buffer, "%02d:%02d", lasttrip_time_m_dis, lasttrip_time_s_dis);
	}

	if (ituWidgetGetX(standTimesText) != px_s)
	{
		ituWidgetSetX(standTimesText, px_s);
	}

	if (ituWidgetGetWidth(standTimesText) != dw_s)
	{
		ituWidgetSetWidth(standTimesText, dw_s);
	}

	if (0 != strcmp(ituTextGetString(standTimesText), buffer))
	{
		ituTextSetString(standTimesText, buffer);
	}
	return true;
}

bool mainLayer_LastTripAvgSpeed_DisplayConfig(int dis_lasttrip_avgspeed, user_unitlist_type dis_unit)
{
	char buffer[10] = { 0 };
	char buffer_km[5] = { 0 };
	int lasttrip_avgspeed_dis = dis_lasttrip_avgspeed*36;
	// if (!standRangeText || !standRangeUnitsText)
	// {
	// 	return false;
	// }
	switch (dis_unit)
	{
	case STATUS_METRIC_UNIT:
		// strcpy(buffer,"km/h");
		buffer_km[0] = 'k';
		buffer_km[1] = 'm';
		buffer_km[2] = '/';
		buffer_km[3] = 'h';		
		break;
	case STATUS_IMPERIAL_UNIT:
		// strcpy(buffer, "mph");
		buffer_km[0] = 'm';
		buffer_km[1] = 'p';
		buffer_km[2] = 'h';	
		lasttrip_avgspeed_dis = (int)((float)lasttrip_avgspeed_dis * 0.62137 + 0.5);
		break;
	default:
		return false;
		break;
	}	
	sprintf(buffer, "%d.%d ", lasttrip_avgspeed_dis/10,lasttrip_avgspeed_dis%10);
	if (0 != strcmp(ituTextGetString(standAvgSpeedText), buffer))
	{
		ituTextSetString(standAvgSpeedText, buffer);
	}

	if (0 != strcmp(ituTextGetString(standSpeedUnitsText), buffer_km))
	{
		ituTextSetString(standSpeedUnitsText, buffer_km);
	}
	return true;
}

bool mainLayer_Sidestand_DisplayConfig(bool status_side)
{
	// if(!standsideIcon)
	// {
	// 	return false;
	// }
	ituWidgetSetVisible(standsideIcon, status_side);
	return true;
}

bool Layer_SideMistake_DisplayConfig(bool status_mistake)
{
	if (status_mistake && (!sidestand_temple_status))
	{
		sidestand_temple_status = true;
	}
	return true;
}

bool mainLayer_Ready_DisplayConfig(bool status_ready,bool status_sidestand)
{
	bool isVisible[2] = { false };
	int px_s = 0, py_s = 0, dw_s = 0, dh_s = 0;
	// if (!curveContainer || !standallContainer || !sp_SpeedFrameNumberIcon || !sp_PowerFrameNumberIcon)
	// {
	// 	return false;
	// }
	if (status_ready)
	{
		isVisible[0] = true;
		isVisible[1] = false;
		px_s = 260;
		py_s = -19;
		dw_s = 760;
		dh_s = 521;
		if (status_sidestand && (!sidestand_temple_status))
		{
			sidestand_temple_status = true;
			// ituWidgetSetVisible(standsideIcon, false);
		}
	}
	else
	{
		isVisible[0] = false;
		isVisible[1] = true;
		px_s = 17;
		py_s = -170;
		dw_s = 1246;
		dh_s = 855;
	}

	if (status_sidestand)
	{
		isVisible[0] = false;
		isVisible[1] = true;
		px_s = 17;
		py_s = -170;
		dw_s = 1246;
		dh_s = 855;
	}
	// else
	// {
	// 	printf("side false\t\n");
	// }

	if(!sidestand_temple_status)
	{
		ituWidgetSetVisible(standsideIcon, status_sidestand);
	}
	ituWidgetSetVisible(curveMaskSprite, isVisible[0]);
	ituWidgetSetVisible(standallContainer, isVisible[1]);
	ituWidgetSetVisible(curveContainer, isVisible[0]);
	// ituWidgetSetVisible(curveXContainer, isVisible[0]);
	ituWidgetSetVisible(sp_SpeedFrameNumberIcon, isVisible[0]);
	ituWidgetSetVisible(sp_PowerFrameNumberIcon, isVisible[0]);
	if(Current_modeBackground)
	{
		if((ituWidgetGetX(Current_modeBackground) != px_s)||(ituWidgetGetY(Current_modeBackground) != py_s))
		{
			ituWidgetSetPosition(Current_modeBackground, px_s, py_s);
		}
		if((ituWidgetGetWidth(Current_modeBackground) != dw_s)||(ituWidgetGetHeight(Current_modeBackground) != dh_s))
		{
			ituWidgetSetDimension(Current_modeBackground, (int)dw_s, (int)dh_s);
		}
	}
	
	return true;
}

bool mainLayer_WeatherTemperature_DisplayConfig(int status_weather,int dis_temperature)
{
	char buffer[8] = {0};
	int dw_s = 45, px_s = 401;
	int temperature_dis = dis_temperature;
	bool isVisible = false;	
	char weathercode_map[WMAX] = {
		NIUW000,		 		 //"未知":      
		NIUW001,                 //"晴":        
		NIUW001,                 //"少云":      
		NIUW001,                 //"晴间多云":  
		NIUW000,            	 //"多云":      
		NIUW002,                 //"阴":        
		NIUW001,                 //"有风":      
		NIUW001,                 //"平静":      
		NIUW001,                 //"微风":      
		NIUW001,                 //"和风":      
		NIUW001,                 //"清风":      
		NIUW030,                 //"强风/劲风": 
		NIUW030,                 //"疾风": 
		NIUW030,                 //"大风":  "烈风":    
		NIUW030,                 //"风暴":      
		NIUW030,                 //"狂爆风":    
		NIUW030,                 //"飓风":      
		NIUW030,                 //"热带风暴":  
		NIUW009,                 //"阵雨":      
		NIUW010,                 //"雷阵雨":    
		NIUW026,                 //"雷阵雨并伴有冰雹":
		NIUW003,                 //"小雨":       
		NIUW004,                 //"中雨":       
		NIUW005,                 //"大雨":       
		NIUW006,                 //"暴雨":       
		NIUW007,                 //"大暴雨":     
		NIUW008,                 //"特大暴雨":   
		NIUW010,                 //"强阵雨":     
		NIUW015,                 //"强雷阵雨":   
		NIUW015,                 //"极端降雨":   
		NIUW003,                 //"毛毛雨/细雨":
		NIUW015,                 //"雨":        
		NIUW011,                 //"小雨-中雨":    
		NIUW012,                 //"中雨-大雨":    
		NIUW013,                 //"大雨-暴雨":    
		NIUW014,                 //"暴雨-大暴雨":  
		NIUW015,                 //"大暴雨-特大暴雨": 
		NIUW023,                 //"雨雪天气":  
		NIUW024,                 //"雨夹雪":    
		NIUW024,                 //"阵雨夹雪":  
		NIUW025,                 //"冻雨":      
		NIUW018,                 //"雪":        
		NIUW023,                 //"阵雪":      
		NIUW016,                 //"小雪":      
		NIUW017,                 //"中雪":      
		NIUW018,                 //"大雪":      
		NIUW019,                 //"暴雪":      
		NIUW020,                 //"小雪-中雪": 
		NIUW021,                 //"中雪-大雪": 
		NIUW022,                 //"大雪-暴雪": 
		NIUW034,                 //"浮尘":      
		NIUW033,                 //"扬沙":      
		NIUW031,                 //"沙尘暴":    
    	NIUW032,                 //"强沙尘暴":  
		NIUW030,                 //"龙卷风":    
		NIUW027,                 //"雾":        
		NIUW027,                 //"浓雾":      
		NIUW027,                 //"强浓雾":    
		NIUW027,                 //"轻雾":      
		NIUW028,                 //"大雾":      
		NIUW028,                 //"特强浓雾":  
		NIUW029,                 //"霾":        
		NIUW028,                 //"中度霾":    
		NIUW028,                 //"重度霾":    
		NIUW028,                 //"严重霾":    
		NIUW001,                 //"热":        
		NIUW002,                 //"冷":
	};

	if ( (status_weather < WMAX) && (status_weather > 0) )
	{
		ituSpriteGoto(timeWeatherSprite, (int)weathercode_map[status_weather]);
		isVisible = true;

		sprintf(buffer, "%d", temperature_dis);
		if ((temperature_dis < -9) || (temperature_dis > 99))
		{
			dw_s += 22;
			px_s += 22;
			buffer[3] = 0;
		}
		else if ((temperature_dis >= 0) && (temperature_dis < 10))
		{
			dw_s -= 22;
			px_s -= 22;
		}

		
		if(ituWidgetGetWidth(timeAirtemperatureText) != dw_s)
		{
			ituWidgetSetWidth(timeAirtemperatureText, dw_s);
		}
		if(ituWidgetGetX(timeAirtemperatureUnitsIcon) != px_s)
		{
			ituWidgetSetX(timeAirtemperatureUnitsIcon, px_s);
		}
		if(0 != strcmp(ituTextGetString(timeAirtemperatureText),buffer))
		{
			ituTextSetString(timeAirtemperatureText, buffer);
		}	
	}	
	ituWidgetSetVisible(timeWeatherSprite, isVisible);
	ituWidgetSetVisible(timeAirtemperatureText, isVisible);
	ituWidgetSetVisible(timeAirtemperatureUnitsIcon, isVisible);
	return true;
}

bool  mainLayer_ScoreError_DisplayConfig(int dis_type,int dis_data)
{	
	char buffer[8] = {0};
	bool isVisible = false;
	ITUText* bufferText[2] = {NULL};
	// if(!timeErrorText||!timeErrorText||!timeScoErrSprite)
	// {
	// 	return false;
	// }
	bufferText[0] = timeScoreText;
	bufferText[1] = timeErrorText;
	if (dis_type < 2)
	{
		ituSpriteGoto(timeScoErrSprite,dis_type);
		isVisible = true;
		sprintf(buffer, "%d", dis_data);
		if(0 != strcmp(ituTextGetString(bufferText[dis_type]),buffer))
		{
			ituTextSetString(bufferText[dis_type], buffer);
		}			
	}	
	ituWidgetSetVisible(timeScoErrSprite, isVisible);	
	return true;
}