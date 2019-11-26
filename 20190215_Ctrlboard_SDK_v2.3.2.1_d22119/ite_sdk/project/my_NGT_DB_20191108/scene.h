/** @file
 * Template scene definition.
 *
 */
/** @defgroup template
 *  @{
 */
#ifndef SCENE_H
#define SCENE_H

#include "ite/itu.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @defgroup template_scene
 *  @{
 */

#define MS_PER_FRAME    17  ///< Drawing delay per frame


    typedef enum
    {
       STORAGE_NONE = -1,
       STORAGE_USB,
       STORAGE_SD,
       STORAGE_INTERNAL,

       STORAGE_MAX_COUNT
    } StorageType;

    typedef enum
    {
       STORAGE_UNKNOWN,
       STORAGE_SD_INSERTED,
       STORAGE_SD_REMOVED,
       STORAGE_USB_INSERTED,
       STORAGE_USB_REMOVED,
       STORAGE_USB_DEVICE_INSERTED,
       STORAGE_USB_DEVICE_REMOVED
    } StorageAction;

    void StorageInit(void);

    StorageAction StorageCheck_customized(void);
    
    StorageAction StorageCheck(void);
    StorageType StorageGetCurrType(void);
    void StorageSetCurrType(StorageType type);
    char *StorageGetDrivePath(StorageType type);
    bool StorageIsInUsbDeviceMode(void);
/**
 * Global instance variable of scene.
 */
extern ITUScene theScene;
extern ITULayer* mainLayer;
extern ITULayer* chargeLayer;
extern ITULayer* upgradeLayer;
extern ITULayer* testmodeLayer;
extern ITUIcon* chargeSideIcon;

extern StorageAction action;
extern bool module_test;
extern bool board_test;

// scene
/**
 * Initializes scene module.
 */
void SceneInit(void);

/**
 * Exits scene module.
 */
void SceneExit(void);

/**
 * Loads ITU file.
 */
void SceneLoad(void);

/**
 * Runs the main loop to receive events, update and draw scene.
 *
 * @return The QuitValue.
 */
int SceneRun(void);

/** @} */ // end of template_scene

void ScreenSetBrightness(int value);

void SceneSetReady(bool ready);
bool SceneReadReady(void);

void ScreenClear(void);

#ifdef __cplusplus
}
#endif

#endif /* SCENE_H */

/** @} */ // end of template
