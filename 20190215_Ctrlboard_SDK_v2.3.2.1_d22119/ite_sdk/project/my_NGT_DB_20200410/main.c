#include <unistd.h>
#include <sys/ioctl.h>
#include "SDL/SDL.h"
#include "ite/itp.h"
#include "project.h"
#include "scene.h"

#ifdef _WIN32
#include <crtdbg.h>
#else
#include "openrtos/FreeRTOS.h"
#include "openrtos/task.h"
#include "openrtos/queue.h"
#include "openrtos/semphr.h"
#endif

int	Update_Flag = 0;

#ifndef DEMO_BUG_RUN
// SemaphoreHandle_t write_data_key = NULL;
#endif

int SDL_main(int argc, char *argv[])
{
    int ret = 0;
	Update_Flag = 0;

#if !defined(CFG_LCD_INIT_ON_BOOTING) && !defined(CFG_BL_SHOW_LOGO)
    ScreenClear();
#endif

#ifdef CFG_CHECK_FILES_CRC_ON_BOOTING
    int restryCount = 0;
    BackupInit();
retry_backup:
    ret = UpgradeInit();
    if (ret)
    {
        if (++restryCount > 2)
        {
            printf("cannot recover from backup....\n");
            goto end;
        }
        BackupRestore();
        goto retry_backup;
    }
    BackupSyncFile();
#else
    ret = UpgradeInit();
    if (ret)
        goto end;
#endif

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());
    
    ConfigInit();

#ifdef CFG_NET_ENABLE
    NetworkInit();
#endif    

    SceneInit();
	ExternalInit();
    StorageInit();
       
#ifndef DEMO_BUG_RUN
    DeviceDataSheetInit();
    SendModuleVer_Handle();
    CreateAppComunicationTask();
#endif

    SceneLoad();
    ret = SceneRun();

	ExternalExit();
    SceneExit();	
    ConfigExit();
    SDL_Quit();

end:
    ret = UpgradeProcess(ret);
    exit(ret);
    return ret;
}
