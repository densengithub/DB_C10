#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "ite/ith.h"
#include "ite/itp.h"
#include "test_itv.h"       //for VideoInit()
#include "ite/ith_video.h" //for VideoInit()
#include "capture_types.h"
#include "mmp_capture.h"
#include "hdmirx/mmp_hdmirx.h"
#include "i2s/i2s.h" 
#include "wavheader.h"
#include "audio_ctrl.h"                     

typedef enum state_control
{
    NO_SIGNAL = 0,
    NO_SIGNAL_TO_SIGNAL_STABLE = 1,
    SIGNAL_STABLE = 2,
} state_control;

static CAPTURE_HANDLE gCapDev0;
static bool       cap0_get_err = true;
static bool       cap0_frame_end = false;
state_control     state = NO_SIGNAL;

#if defined(CFG_I2S_ENABLE) && defined(CFG_AUDIO_ENABLE)

//========audio parameter==================
#define ADC_MEMORY_SIZE (256*1024 * 5) 
#define ADC_BUFFER_SIZE (256*1024)
#define DAC_BUFFER_SIZE (256*1024)
static STRC_I2S_SPEC spec_daI = {0};
static STRC_I2S_SPEC spec_adI = {0};
uint8_t *dac_buf;
uint8_t *adc_buf;
uint8_t *HDMI_Ibuf[4];
uint8_t *HDMI_Obuf[4];
unsigned char *recdata;
uint32_t sizecount = 0; 

//=====================================
//========audio function====================
void initAD(){
    /* init ADC */ 
    int i;
    adc_buf = (uint8_t*)malloc(ADC_BUFFER_SIZE);
    memset((uint8_t*) adc_buf, 0, ADC_BUFFER_SIZE);
    memset((void*)&spec_adI, 0, sizeof(STRC_I2S_SPEC));
    spec_adI.channels                   = 2;
    spec_adI.sample_rate                = 44100;
    spec_adI.buffer_size                = ADC_BUFFER_SIZE;

    spec_adI.num_hdmi_audio_buffer = 4;

    for (i = 0; i < 4; i++){
        if (i < spec_adI.num_hdmi_audio_buffer){
            HDMI_Ibuf[i] = (uint8_t*)malloc(ADC_BUFFER_SIZE);
            memset(HDMI_Ibuf[i], (i*10+10), ADC_BUFFER_SIZE);
        }
        else{
            HDMI_Ibuf[i] = NULL;
        }
    }
    spec_adI.base_hdmi[0]                    = HDMI_Ibuf[0];
    spec_adI.base_hdmi[1]                    = HDMI_Ibuf[1];
    spec_adI.base_hdmi[2]                    = HDMI_Ibuf[2];
    spec_adI.base_hdmi[3]                    = HDMI_Ibuf[3]; 
    
    spec_adI.is_big_endian              = 0;
    spec_adI.base_i2s                   = (uint8_t*) adc_buf;   
    spec_adI.sample_size                = 16;
    spec_adI.record_mode                = 1;
    spec_adI.from_LineIN                = 0;
    spec_adI.from_MIC_IN                = 1;
    //i2s_init_ADC(&spec_adI);
    audio_init_AD(&spec_adI);
    audio_pause_AD(1);
}

void initDA(){
	/* init DAC */
    dac_buf = (uint8_t*)malloc(DAC_BUFFER_SIZE);
	memset((uint8_t*) dac_buf, 0, DAC_BUFFER_SIZE);
	memset((void*)&spec_daI, 0, sizeof(STRC_I2S_SPEC));
    spec_daI.channels                 = 2;
    spec_daI.sample_rate              = 44100;
    spec_daI.buffer_size              = DAC_BUFFER_SIZE;
    spec_daI.is_big_endian            = 0;
    spec_daI.base_i2s                 = (uint8_t*) dac_buf;
    spec_daI.sample_size              = 16;
    spec_daI.num_hdmi_audio_buffer    = 0;
    spec_daI.is_dac_spdif_same_buffer = 1;
    spec_daI.enable_Speaker           = 1;
    spec_daI.enable_HeadPhone         = 1;
    spec_daI.postpone_audio_output    = 1;
    spec_daI.base_spdif                = (uint8_t*) dac_buf;
    //i2s_init_DAC(&spec_daI);
    audio_init_DA(&spec_daI);
    audio_pause_DA(1);

}

void* AudioThread(void* arg)
{
    initDA();
    initAD();    
    audio_AD_volume_set(60);//set rec 0~100 (faraday : level 60= 0db )
    audio_DA_volume_set(100);//set volume 0~100 (faraday : level 100= 0db )
    printf("audio init done\n");

    uint16_t bsize=audio_get_bsize_from_bps(&spec_daI);
    uint8_t *psrc=NULL;           
    psrc = (uint8_t*)malloc(bsize);  
    recdata = (uint8_t*)malloc(ADC_MEMORY_SIZE);
    audio_pause_AD(0);

    char *filename = "a:/record.wav";
    playdata *d;
    d = INITplaydata();
    openfilewb(d,filename);

    #if 0
    while(1)
    {
        if(audio_HDMIRX_Data_Recv(&spec_adI,psrc,bsize, 0))
        {
            audio_TX_Data_Send(&spec_daI,psrc,bsize);
        }
        usleep(10);
    }
    #else
    while(sizecount < ADC_MEMORY_SIZE){
        
        if((sizecount+bsize) > ADC_MEMORY_SIZE) break;
        if(audio_HDMIRX_Data_Recv(&spec_adI,psrc,bsize, 0)){
            memcpy(recdata+sizecount,psrc,bsize);
            sizecount += bsize;
        }

        audio_AD_wait_time(&spec_adI,bsize);
    }
    audio_pause_AD(1);
    if(d->fd)
    {
        write_wav_header(d,&spec_adI,ADC_MEMORY_SIZE);
        fwrite(recdata,ADC_MEMORY_SIZE,1,d->fd);
        fclose(d->fd);
        printf("save file in a:/record.wav\n");
        printf("now check USB disk file\n"); 
    }
    else
        printf("open file fail.\n");
    #endif
}
#endif
//=====================================

static void
FSM_STATE(BOOL signal)
{
    if(signal)
    {
        if(state == NO_SIGNAL)
            state = NO_SIGNAL_TO_SIGNAL_STABLE;
        else if(state == NO_SIGNAL_TO_SIGNAL_STABLE)
            state = SIGNAL_STABLE;
        else if(state == SIGNAL_STABLE)
            state = SIGNAL_STABLE;
    }
    else
    {
        state = NO_SIGNAL;
    }
}

static void
cap_isr(
    void* arg)
{
	uint32_t          capture0state = 0, capture1state = 0; 
	CAPTURE_HANDLE *ptDev    = (CAPTURE_HANDLE *) arg;

	capture0state = ithCapGetEngineErrorStatus(&gCapDev0,MMP_CAP_LANE0_STATUS);
	//ithPrintf("cap0_isr 0x%x\n", capture0state);

	if(capture0state >> 31)
	{
		if((capture0state >> 8) & 0xF)
		{
			//ithPrintf("cap0_isr err\n");
			ithPrintf("cap0_isr 0x%x\n", capture0state);
		    cap0_get_err = true;	
			//clear cap0 interrupt and reset error status
		    ithCapClearInterrupt(&gCapDev0, MMP_TRUE);
		}
	    else
	    {
	    	//ithPrintf("cap0_isr frame end\n");
	        cap0_frame_end = true;
	    	//clear cap0 interrupt
		    ithCapClearInterrupt(&gCapDev0, MMP_FALSE);
	    }
	}

	return;
	
}

static int
check_and_fire(
    CAPTURE_HANDLE* ptDev)
{
	uint32_t 	OutWidth=0,OutHeight =0,hsync_timeout = 0, vsync_timeout = 0;
	CAP_CONTEXT *Capctxt = &ptDev->cap_info;

    //Detected Hsync.
	while(!(ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS) & 0x1))
	{
		if(++hsync_timeout > 50)
			return 1;
		printf("Hsync not stable!\n");
		usleep(1000);
	}

	//wait for getting interleave
 	sleep(1);
    
    Capctxt->ininfo.Interleave = ithCapGetDetectedInterleave(ptDev);
    printf("Get interleave=%d\n",Capctxt->ininfo.Interleave);
	ithCapSetInterleave(ptDev,Capctxt->ininfo.Interleave);

	while((ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS)&0x3) != 0x3)
	{
		if(++vsync_timeout > 50)
			return 1;
		printf("Hsync or Vsync not stable!\n");
		usleep(1000);
	}

    ithCapFire(ptDev, MMP_TRUE);
    printf("Set WidthHeight and fire! (%d)\n", ptDev->cap_id);

	return 0;
}

static int 
CaptureGetNewFrame(
    CAPTURE_HANDLE *ptDev ,ITE_CAP_VIDEO_INFO *Outdata)
{
    static int      cap_idx;
    static int      NewBufferIdx      = 0;
    static int      OldBufferIdx      = 0;
    static bool 	GetFirstBufferIdx = false;
    uint16_t      	timeOut = 0,FirstBufferIdx = 1;
	int			    NotDisplay = 0;
	CAP_CONTEXT *Capctxt = &ptDev->cap_info;
    
    Outdata->OutHeight    = Capctxt->outinfo.OutHeight;
    Outdata->OutWidth     = Capctxt->outinfo.OutWidth;
    Outdata->IsInterlaced = Capctxt->ininfo.Interleave;
    Outdata->PitchY       = Capctxt->ininfo.PitchY;
    Outdata->PitchUV      = Capctxt->ininfo.PitchUV;
	Outdata->OutMemFormat = Capctxt->outinfo.OutMemFormat;
    
    if (!GetFirstBufferIdx)
    {
        OldBufferIdx      = ithCapReturnWrBufIndex(ptDev);
        GetFirstBufferIdx = true;
    }
	
    while (NewBufferIdx == OldBufferIdx)
    {
        NewBufferIdx = ithCapReturnWrBufIndex(ptDev);
	    usleep(1000);
        timeOut++;
        if (timeOut > 100)
        {
	        printf("Capture controller not moving!!\n");
            break;
        }
    }
	
    OldBufferIdx          = NewBufferIdx;
	
    if (!OldBufferIdx)
        cap_idx = 2;
    else
        cap_idx = OldBufferIdx - 1;
	
    switch (cap_idx)
    {
    case 0:
        Outdata->DisplayAddrY = (uint8_t*)Capctxt->OutAddrY[0];
        Outdata->DisplayAddrU = (uint8_t*)Capctxt->OutAddrUV[0];
        Outdata->DisplayAddrV = (uint8_t*)Capctxt->OutAddrUV[0];
        break;

    case 1:
        Outdata->DisplayAddrY = (uint8_t*)Capctxt->OutAddrY[1];
        Outdata->DisplayAddrU = (uint8_t*)Capctxt->OutAddrUV[1];
        Outdata->DisplayAddrV = (uint8_t*)Capctxt->OutAddrUV[1];
        break;

    case 2:
        Outdata->DisplayAddrY = (uint8_t*)Capctxt->OutAddrY[2];
        Outdata->DisplayAddrU = (uint8_t*)Capctxt->OutAddrUV[2];
        Outdata->DisplayAddrV = (uint8_t*)Capctxt->OutAddrUV[2];
        break;
    }

    //printf("[%d] AddrY = %x ,ADDU = %x\n",cap_idx,Outdata->DisplayAddrY,Outdata->DisplayAddrU);
    return NotDisplay;
}

static void SettingISPAnd_FilpLCD(
	CAPTURE_HANDLE *ptDev)
{
	uint32_t Error_Status = 0,Get_Error_Frame = 0;
    uint8_t *dbuf = NULL;
    ITV_DBUF_PROPERTY dbufprop = {0};
	ITE_CAP_VIDEO_INFO outdata = {0};

	dbuf = itp_itv_get_dbuf_anchor();
    if (dbuf == NULL)
    {
        return;
    }

	if(ptDev->cap_id == CAP_DEV_ID0)
    	cap0_frame_end = false;


	Get_Error_Frame = CaptureGetNewFrame(ptDev, &outdata);
	
	if(Get_Error_Frame)
	{
		printf("Get Error Frame!\n");
		return;
	}
	
	if(outdata.IsInterlaced)
		itp_itv_enable_isp_feature(MMP_ISP_DEINTERLACE); 

    dbufprop.src_w    = outdata.OutWidth;
    dbufprop.src_h    = outdata.OutHeight;
    dbufprop.pitch_y  = outdata.PitchY;
    dbufprop.pitch_uv = outdata.PitchUV;
	
	dbufprop.format   = MMP_ISP_IN_NV12;
    dbufprop.ya       = outdata.DisplayAddrY;
    dbufprop.ua       = outdata.DisplayAddrU;
    dbufprop.va       = outdata.DisplayAddrV;
    //ithPrintf("to ISP outwidth = %d, out height = %d\n", outdata.OutWidth, outdata.OutHeight);
    //ithPrintf("to ISP PitchY = %d, out PitchUV = %d\n", outdata.PitchY, outdata.PitchUV);

	itp_itv_update_dbuf_anchor(&dbufprop);


    return;

}

//hdmi test Run
void* TestFunc(void* arg)
{
	printf("hdmi test cap start\n");
	itpInit();

#ifdef CFG_LCD_ENABLE
    /*lcd panel int*/
	ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL);
    /*itv int*/
	itp_itv_init();
#endif
        
    uint32_t i;

    /*hdmi RGB pin , mode selection*/
    for (i = ITH_GPIO_PIN44; i <= ITH_GPIO_PIN71; i++){            
        ithGpioSetMode(i, ITH_GPIO_MODE2);
        ithGpioSetIn(i);
        ithGpioSetFFIEn(i);
    }
    
    /*DECTECT pin*/
    ithGpioSetMode(ITH_GPIO_PIN42, ITH_GPIO_MODE0);
    ithGpioSetIn(ITH_GPIO_PIN42);
    
    /*INIT pin*/
    ithGpioSetMode(ITH_GPIO_PIN43, ITH_GPIO_MODE0);
    ithGpioSetIn(ITH_GPIO_PIN43); 

    ithCapPowerUp();
    
    /*Hdmi init*/
    ithCapDeviceInitialize();

    /*capture init*/
    ithCapInitialize();
    CAPTURE_SETTING mem_modeset = {MMP_CAP_DEV_HDMIRX, MMP_FALSE,  MMP_TRUE, 1920, 1080};
    ithCapConnect(&gCapDev0,mem_modeset);
    ithCapRegisterIRQ(cap_isr, &gCapDev0);

#if defined(CFG_I2S_ENABLE) && defined(CFG_AUDIO_ENABLE)
    /*========audio thread========*/
    sleep(2);//wait usb mount on system
    pthread_t task;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
	pthread_create(&task, &attr, AudioThread, NULL);
#endif

    /*main loop*/
    for(;;)
    {
        FSM_STATE(ithCapDeviceIsSignalStable());

        switch(state)
        {
            case NO_SIGNAL:
                if(cap0_get_err){
                    printf("###NO_SIGNAL###\n");
                    ithCapWaitEngineIdle(&gCapDev0);
                    ithCapEngineReset();
                    cap0_get_err = false;
                }
                break;
            case NO_SIGNAL_TO_SIGNAL_STABLE:
                printf("###NO_SIGNAL_TO_SIGNAL_STABLE###\n");
                ithCapGetDeviceInfo(&gCapDev0);
                ithCapParameterSetting(&gCapDev0);
                check_and_fire(&gCapDev0);
                
                break;
            case SIGNAL_STABLE:
                if(cap0_get_err){
                    printf("###CAPTURE ERROR###\n");
                    ithCapWaitEngineIdle(&gCapDev0);
                    ithCapEngineReset();
                    state = NO_SIGNAL;
                    cap0_get_err = false;
                }
                
                if(cap0_frame_end){
                    #ifdef CFG_LCD_ENABLE
                    SettingISPAnd_FilpLCD(&gCapDev0);
                    #else
                    cap0_frame_end = false;
                    #endif
                }
                break;
            default:
                break;
        }
        
        usleep(15*1000); // < 16 ms

    }

}



