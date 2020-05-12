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
upgradeLayer
upgradeBlackBackground
upgradeTimeText
upgradeTimeTagText1
upgradeContainer
upgradePrecentText
upgradeTagText0
upgradeSimpleAnimation
upgradeButton2
upgradeButton1
upgradeLineIcon
upgradeBackground
*/
ITULayer *upgradeLayer = NULL;
bool upgrade_status = false;
static ITUBackground *upgradeBackground = NULL;
// static ITUContainer* upgradeContainer = NULL;
static ITUText *upgradeTimeText = NULL;
static ITUText *upgradePrecentText = NULL;
static ITUText *upgradeTimeTagText1 = NULL;
static ITUAnimation *upgradeSimpleAnimation = NULL;
// static ITUButton* upgradeButton1 = NULL;
// static ITUButton* upgradeButton2 = NULL;

bool Layer_upgrade_Init(void)
{
    if (!upgradeBackground)
    {
        upgradeBackground = ituSceneFindWidget(&theScene, "upgradeBackground");
        assert(upgradeBackground);
        upgradePrecentText = ituSceneFindWidget(&theScene, "upgradePrecentText");
        assert(upgradePrecentText);

        upgradeTimeText = ituSceneFindWidget(&theScene, "upgradeTimeText");
        assert(upgradeTimeText);
        upgradeTimeTagText1 = ituSceneFindWidget(&theScene, "upgradeTimeTagText1");
        assert(upgradeTimeTagText1);

        upgradeSimpleAnimation = ituSceneFindWidget(&theScene, "upgradeSimpleAnimation");
        assert(upgradeSimpleAnimation);

        upgradeSimpleAnimation->totalframe = 152; //180
        upgradeSimpleAnimation->delay = 1;
    }
    ituTextSetString(upgradePrecentText, "0%");
    return false;
}

bool UpgradeLayer_Precent_Display(int dis_pre)
{
    char buffer[10] = {0};
    // if(!upgradePrecentText)
    // {
    //     return false;
    // }
    // sprintf(buffer, "%02d%%", dis_pre);
    sprintf(buffer, "%d", dis_pre);
    strcat(buffer, "%");
    if (0 != strcmp(ituTextGetString(upgradePrecentText), buffer))
    {
        ituTextSetString(upgradePrecentText, buffer);
    }
    return false;
}

bool UpgradeLayer_Remaintime_Display(int dis_time)
{
    char buffer[50] = {0};
    char buffer_t[10] = {0};
    // if(!upgradeTimeTagText1)
    // {
    //     return false;
    // }
    strcpy(buffer, "Est.");
    sprintf(buffer_t, " %d ", dis_time);
    strcat(buffer, buffer_t);
    strcat(buffer, "min remaining");
    if (0 != strcmp(ituTextGetString(upgradeTimeTagText1), buffer))
    {
        ituTextSetString(upgradeTimeTagText1, buffer);
    }
    return false;
}

bool UpgradeOnEnter(ITUWidget *widget, char *param)
{
    Layer_upgrade_Init();
    Layer_upgrade_time_init();
    SceneSetReady(true);
    upgrade_status = true;
    testmode_status = false;
    charge_status = false;
    Layer_MainMode_Task(&ev_layer2);
    return true;
}

bool UpgradeOnTimer(ITUWidget *widget, char *param)
{
    layer_Signal_Display();
    return false;
}
