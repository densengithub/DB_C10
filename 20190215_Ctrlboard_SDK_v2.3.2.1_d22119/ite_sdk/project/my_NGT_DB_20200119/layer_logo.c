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
logoLayer
logoBackground5
logoBackground4
logoBackground3
logoBackground2
logoBackground1
*/

//#define ITU_PLAY_VIDEO_ON_BOOTING

static ITUVideo* logoVideo = NULL;
static ITUBackground* logoBackground1 = NULL;
static ITUBackground* logoBackground2 = NULL;
static ITUBackground* logoBackground3 = NULL;
static ITUBackground* logoBackground4 = NULL;
static ITUBackground* logoBackground5 = NULL;
static ITUBackground* logoBackground_a[5] = {NULL};
static int index_ground = 0;
static int tempx = 0;

static void LogoVideoOnStop(ITUVideo* video)
{
	// ituLayerGoto(mainLayer);
    if ((local_niu_value_loc.db_loc.data.db_status2 & (CAR_EX_BT1_INCHARGE | CAR_EX_BT2_INCHARGE)) || (local_niu_value_loc.db_loc.data.db_status & (CAR_INCHG)))
    {
        ituLayerGoto(chargeLayer);
    }
    else if((local_niu_value_loc.db_loc.data.db_status & (CAR_VCU_UPDATE)))
    {
        ituLayerGoto(upgradeLayer);
    }
    else
	{
		ituLayerGoto( mainLayer );
	}
}

bool Layer_logo_Init(void)
{
    int i = 0;
    if(!logoBackground5)
    {
        logoBackground1 = ituSceneFindWidget(&theScene, "logoBackground1");    
        logoBackground2 = ituSceneFindWidget(&theScene, "logoBackground2");  
        logoBackground3 = ituSceneFindWidget(&theScene, "logoBackground3");    
        logoBackground4 = ituSceneFindWidget(&theScene, "logoBackground4"); 
        logoBackground5 = ituSceneFindWidget(&theScene, "logoBackground5");
        assert(logoBackground1);
		assert(logoBackground2);
		assert(logoBackground3);
		assert(logoBackground4);
		assert(logoBackground5);
        logoBackground_a[i++] = logoBackground1;
        logoBackground_a[i++] = logoBackground2;
        logoBackground_a[i++] = logoBackground3;
        logoBackground_a[i++] = logoBackground4;
        logoBackground_a[i++] = logoBackground5;
        return true;
    }
    return false;
}

bool LogoOnEnter(ITUWidget* widget, char* param)
{
    //if (!logoVideo)
	if (!logoBackground5)
    {
        // logoVideo = ituSceneFindWidget(&theScene, "logoVideo");
        // assert(logoVideo);

        mainLayer = ituSceneFindWidget(&theScene, "mainLayer");        
        assert(mainLayer);

        chargeLayer = ituSceneFindWidget(&theScene, "chargeLayer");        
        assert(chargeLayer);

        upgradeLayer = ituSceneFindWidget(&theScene, "upgradeLayer");        
        assert(upgradeLayer);
        
        testmodeLayer = ituSceneFindWidget(&theScene, "testmodeLayer");        
        assert(testmodeLayer);

        // Layer_logo_Init();
    }

#ifdef ITU_PLAY_VIDEO_ON_BOOTING
        ituVideoPlay(logoVideo, 0);
        ituVideoSetOnStop(logoVideo, LogoVideoOnStop);
    #else    
    
    if ((local_niu_value_loc.db_loc.data.db_status2 & (CAR_EX_BT1_INCHARGE | CAR_EX_BT2_INCHARGE)) || (local_niu_value_loc.db_loc.data.db_status & (CAR_INCHG)))
    {
        ituLayerGoto(chargeLayer);
    }
    else if((local_niu_value_loc.db_loc.data.db_status & (CAR_VCU_UPDATE)))
    {
        ituLayerGoto(upgradeLayer);
    }
    else
    {
        ituLayerGoto(mainLayer);
    }    
    #endif
    
    return true;
}

bool LogoOnTimer(ITUWidget* widget, char* param)
{
    int j = 0;
    tempx++;
    if(tempx>30)
    {
		tempx = 0;
        index_ground++;
        index_ground %= 5;
		
        for (j = 0; j < 5;j++)
        {
			if (!logoBackground_a[j])
			{
				return false;
			}
            ituWidgetSetVisible(logoBackground_a[j], false);
        }
        ituWidgetSetVisible(logoBackground_a[index_ground], true);
        return true;
    }
    return false;
}
