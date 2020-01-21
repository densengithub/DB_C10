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
chargeLayer
chargeBlackBackground
chargeSideIcon
chargeContainer
chargeStarIcon
chargeLineIcon
chargeTimeText
chargeTimeTagText1
chargeTimeTagText0
chargeBackground
chargeSimpleAnimation
chargeButton2
chargeButton1
*/

ITULayer* chargeLayer = NULL;
bool  charge_status = false;
static ITUBackground* chargeBackground = NULL;
static ITUContainer* chargeContainer = NULL;
static ITUText* chargeTimeText = NULL;
static ITUText* chargeTimeTagText1 = NULL;
static ITUText* chargeTimeTagText0 = NULL;
// static ITUIcon* chargeSideIcon = NULL;
ITUIcon* chargeSideIcon = NULL;
static ITUIcon* chargeStarIcon = NULL;

static ITUAnimation* chargeSimpleAnimation = NULL;
static ITUButton* chargeButton1 = NULL;
static ITUButton* chargeButton2 = NULL;

bool Layer_charge_Init(void)
{
    if(!chargeBackground)
    {
        chargeLayer = ituSceneFindWidget(&theScene, "chargeLayer");
        assert(chargeLayer);
        
        chargeBackground = ituSceneFindWidget(&theScene, "chargeBackground");
        assert(chargeBackground);
        chargeTimeText = ituSceneFindWidget(&theScene, "chargeTimeText");
        assert(chargeTimeText);
        chargeSideIcon = ituSceneFindWidget(&theScene, "chargeSideIcon");
        assert(chargeSideIcon);
        chargeStarIcon = ituSceneFindWidget(&theScene, "chargeStarIcon");
        assert(chargeStarIcon);
		chargeTimeTagText1 = ituSceneFindWidget(&theScene, "chargeTimeTagText1");
		assert(chargeTimeTagText1);
		chargeTimeTagText0 = ituSceneFindWidget(&theScene, "chargeTimeTagText0");
		assert(chargeTimeTagText0);
        
        chargeSimpleAnimation = ituSceneFindWidget(&theScene, "chargeSimpleAnimation");
        assert(chargeSimpleAnimation);
        chargeButton1 = ituSceneFindWidget(&theScene, "chargeButton1");
        assert(chargeButton1);
        chargeButton2 = ituSceneFindWidget(&theScene, "chargeButton2");
        assert(chargeButton2);

		ituWidgetSetVisible(chargeTimeTagText0, true);
		ituWidgetSetVisible(chargeTimeTagText1, true);

        ituTextSetString(chargeTimeText, "00:00");
        // ituWidgetSetPosition(chargeStarIcon, 46, 113);
        ituWidgetSetX(chargeStarIcon, 46);//left:46,right:465
        
        chargeSimpleAnimation->totalframe = 152;//180
        chargeSimpleAnimation->delay = 1;
        return true;
    }
    return false;
}

bool chargeLayer_Sidestand_DisplayConfig(bool status_ready,bool status_sidestand)
{	
	// if(!standsideIcon)
	// {
	// 	return false;
    // }
	if (status_ready)
	{
        // printf("ready true\t\n");	
		if (status_sidestand && (!sidestand_temple_status))
		{
			sidestand_temple_status = true;
		}
	}	
	if(!sidestand_temple_status)
	{
		ituWidgetSetVisible(chargeSideIcon, status_sidestand);
	}
	return true;
}

bool chargeLayer_ChargeTime_DisplayConfig(int dis_charge)
{
	char buffer[10] = {0};
	int hour_dis = 0, minute_dis = 0;
	int charge_dis = dis_charge;
	// if (!timeAirtemperatureText ||!timeAirtemperatureUnitsIcon)
	// {
	// 	return false;
	// }
    hour_dis = charge_dis / 60;
    minute_dis = charge_dis % 60;
    sprintf(buffer, "%d:%02d", hour_dis, minute_dis); 
    if(0 != strcmp(ituTextGetString(chargeTimeText),buffer))
	{
		ituTextSetString(chargeTimeText, buffer);
	}  
    return true;
}

bool ChargeOnEnter(ITUWidget* widget, char* param)
{
    Layer_charge_Init();
    Layer_charge_speedpower_init();
    Layer_charge_bottom_init();
    Layer_charge_time_init();
    SceneSetReady(true);
    charge_status = true;
    testmode_status = false;
    upgrade_status = false;
    Layer_MainMode_Task(&ev_layer2);
    return true;
}

bool ChargeOnTimer(ITUWidget* widget, char* param)
{
    if(charge_status)
    {  
        layer_SideStand_Display();
        layer_Signal_Display();        
    }
    return false;
}

