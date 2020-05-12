/** @file
 * Template project definition.
 *
 */
/** @defgroup template
 *  @{
 */
#ifndef _PROJECT_H
#define _PROJECT_H

// #define DEMO_BUG_RUN

//default modify
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>


//userself modify
#include "user_typedef.h"
#include "user_config.h"
#include "external.h"
#include "ui_control.h"
#include "niu_db_data_ite.h"
#include "protocols_stm.h"

#ifdef __cplusplus
extern "C"
{
#endif

   extern bool light_set_flag;
   
   extern bool  testmode_status;
   extern bool  charge_status;
   extern bool  upgrade_status;
   extern int layer_status;
   extern ExternalEvent_layer ev_layer2;
   extern volatile bool sidestand_temple_status;
   extern int delay_count;
   extern bool side_isvisible;
   // extern volatile int speed_step_p;
   // extern volatile int speed_step_n;
   // extern volatile int power_step_p;
   // extern volatile int power_step_n;
   // extern volatile int speedmax_step;
   // extern volatile int powermax_step;

   // void BatteryChargeProcessExternalEvent(ExternalEvent_layer *ev);
   // void MainProcessExternalEvent(ExternalEvent_layer *ev);
   // void SettingProcessExternalEvent(ExternalEvent_layer *ev);

   unsigned char CRC8(unsigned char *ptr, unsigned int len);
   void SendModuleVer_Handle(void);
   
   /** @defgroup template_audio Audio Player
 *  @{
 */
   typedef int (*AudioPlayCallback)(int state);

   /**
 * Initializes audio module.
 */
   void AudioInit(void);

   /**
 * Exits audio module.
 */
   void AudioExit(void);

   /**
 * Plays the specified wav file.
 *
 * @param filename The specified wav file to play.
 * @param func The callback function.
 * @return 0 for success, otherwise failed.
 */
   int AudioPlay(char *filename, AudioPlayCallback func);

   /**
 * Stops playing sound.
 */
   void AudioStop(void);

   int AudioPlayMusic(char *filename, AudioPlayCallback func);

   /**
 * Plays keypad sound.
 */
   void AudioPlayKeySound(void);
   void AudioPauseKeySound(void);
   void AudioResumeKeySound(void);

   /**
 * Sets the volume of keypad sound.
 *
 * @param level the percentage of volume.
 */
   void AudioSetKeyLevel(int level);

   /**
 * Mutes all audio.
 */
   void AudioMute(void);

   /**
 * Un-mutes all audio.
 */
   void AudioUnMute(void);

   /**
 * Determines whether this audio is muted or not.
 *
 * @return true muted, false otherwise.
 */
   bool AudioIsMuted(void);

   bool AudioIsPlaying(void);

   void AudioSetVolume(int level);
   int AudioGetVolume(void);

   /** @} */ // end of template_audio

   /** @defgroup template_config Configuration
 *  @{
 */
   typedef struct
   {
      // network
      int dhcp;         ///< Enable DHCP or not
      char ipaddr[16];  ///< IP address
      char netmask[16]; ///< Netmask
      char gw[16];      ///< Gateway address
      char dns[16];     ///< DNS address

      // sound
      char keysound[PATH_MAX]; ///< Key sound file path
      int keylevel;            ///< Key volume percentage, range is 0~100
      int audiolevel;          ///< Audio volume percentage, range is 0~100

      // display
      int brightness;     ///< Brightness, the range is 0~9
      int lang;           ///< Language type @see LangType
      int speeduint_type; ///< speeduint type

      //
      char drivemode;      ///<
      int rangetrip;       ///< trip
      int totalodograph;   ///< odograph
      char batterystatus;  ///<
      int batterycapacity; ///<

      //
      char STM32_SOFT_VERSION[8];
      char PANEL_SOFT_VERSION[8];
      int Smode_speedmaxinum; ///< Smode_speedmaxinum
      int Dmode_speedmaxinum; ///< Smode_speedmaxinum
      int Mmode_speedmaxinum; ///< Smode_speedmaxinum      

    } Config;

    /**
 * Global instance variable of configuration.
 */
    extern Config theConfig;

    /**
 * Loads configuration file.
 */
    void ConfigInit(void);

    /**
 * Exits configuration.
 */
    void ConfigExit(void);

    /**
 * Updates CRC files.
 *
 * @param filepath The file path to update the CRC value. NULL for ini file on public drive.
 */
    void ConfigUpdateCrc(char *filepath);

    /**
 * Saves the public part of configuration to file.
 */
    void ConfigSave(void);

    /** @} */ // end of template_config

    /** @defgroup template_network Network
 *  @{
 */
    /**
 * Initializes network module.
 */
    void NetworkInit(void);

    /**
 * Resets network module.
 */
    void NetworkReset(void);

    /**
 * Determines whether the network is ready or not.
 *
 * @return true for ready; false for net ready yet.
 */
    bool NetworkIsReady(void);

    /** @} */ // end of template_network

    /** @defgroup template_upgrade Upgrade
 *  @{
 */
    /**
 * Quit value definition.
 */
    typedef enum
    {
       QUIT_NONE,             ///< Do not quit
       QUIT_DEFAULT,          ///< Quit for nothing
       QUIT_RESET_FACTORY,    ///< Quit to reset to factory setting
       QUIT_UPGRADE_FIRMWARE, ///< Quit to upgrade firmware
       QUIT_UPGRADE_WEB,      ///< Quit to wait web upgrading
       QUIT_RESET_NETWORK     ///< Quit to reset network
    } QuitValue;

    /**
 * Initializes upgrade module.
 *
 * @return 0 for initializing success, non-zero for initializing failed and the value will be the QuitValue.
 */
    int UpgradeInit(void);

    /**
 * Sets the CRC value of the specified file path.
 *
 * @param filepath The file path to update the CRC value.
 */
    void UpgradeSetFileCrc(char *filepath);

    /**
 * Sets the URL to upgrade.
 *
 * @param url The url to download and upgrade.
 */
    void UpgradeSetUrl(char *url);

    /**
 * Sets the stream to upgrade.
 *
 * @param stream The stream to upgrade.
 */
    void UpgradeSetStream(void *stream);

    /**
 * Processes the upgrade procedure by QuitValue.
 *
 * @param code The QuitValue.
 * @return 0 for process success; otherwise failed.
 */
    int UpgradeProcess(int code);

    /**
 * Is upgrading ready or not.
 *
 * @return true for ready; otherwise not ready yet.
 */
    bool UpgradeIsReady(void);

    /**
 * Is upgrading finished or not.
 *
 * @return true for finished; otherwise not finish yet.
 */
    bool UpgradeIsFinished(void);

    /**
 * Gets the upgrading result.
 *
 * @return 0 for success, failed otherwise.
 */
    int UpgradeGetResult(void);

    /** @} */ // end of template_upgrade

    /** @defgroup ctrlboard_storage Storage
*  @{
*/

   //  typedef enum
   //  {
   //     STORAGE_NONE = -1,
   //     STORAGE_USB,
   //     STORAGE_SD,
   //     STORAGE_INTERNAL,

   //     STORAGE_MAX_COUNT
   //  } StorageType;

   //  typedef enum
   //  {
   //     STORAGE_UNKNOWN,
   //     STORAGE_SD_INSERTED,
   //     STORAGE_SD_REMOVED,
   //     STORAGE_USB_INSERTED,
   //     STORAGE_USB_REMOVED,
   //     STORAGE_USB_DEVICE_INSERTED,
   //     STORAGE_USB_DEVICE_REMOVED
   //  } StorageAction;

    /**
* Initializes storage module.
*/

/** @} */ // end of ctrlboard_storage

#ifdef __cplusplus
}
#endif

#endif /* _PROJECT_H */

/** @} */ // end of template
