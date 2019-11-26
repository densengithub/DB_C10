#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "ite/itp.h"
#include "i2s/i2s.h" 
#include "wavheader.h"
#include "audio_ctrl.h"
#include "sdl/main_processor_message_queue.h"

//#define Volume_test
/**********************************************************/

static void *event_loop(void *arg)
{
    while(1)
    {
        uint16_t nAudioPluginRegister = 0;
        nAudioPluginRegister = getAudioPluginMessageStatus();
        if (((nAudioPluginRegister & 0xc000)>>14) == SMTK_AUDIO_PROCESSOR_ID)
        {
            // do audio api
            //printf("nAudioPluginRegister 0x%x \n",nAudioPluginRegister);
            smtkMainProcessorExecuteAudioPluginCmd(nAudioPluginRegister);
        }
        
        usleep(500000);
    } 
}

void* TestFunc(void* arg)
{
    pthread_t       task;    
    char *filename = "a:/test.mp3";
    itpInit();
    AudioInit();
    sleep(1);
    pthread_create(&task, NULL, event_loop, NULL);
    audio_DA_volume_set(20);
    AudioPlayMusic(filename,NULL);
    
    while(1){
        sleep(1);
    };
    
    
    return NULL;
}
