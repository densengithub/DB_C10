#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "ite/ith.h"
#include "ite/itp.h"
#include "test_itv.h"       //for VideoInit()
#include "capture_types.h"
#include "mmp_capture.h"               

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

//=====================================

static void
FSM_STATE(bool signal)
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
        Outdata->DisplayAddrOldY = (uint8_t*)Capctxt->OutAddrY[2];
        break;

    case 1:
        Outdata->DisplayAddrY = (uint8_t*)Capctxt->OutAddrY[1];
        Outdata->DisplayAddrU = (uint8_t*)Capctxt->OutAddrUV[1];
        Outdata->DisplayAddrV = (uint8_t*)Capctxt->OutAddrUV[1];
        Outdata->DisplayAddrOldY = (uint8_t*)Capctxt->OutAddrY[0];
        break;

    case 2:
        Outdata->DisplayAddrY = (uint8_t*)Capctxt->OutAddrY[2];
        Outdata->DisplayAddrU = (uint8_t*)Capctxt->OutAddrUV[2];
        Outdata->DisplayAddrV = (uint8_t*)Capctxt->OutAddrUV[2];
        Outdata->DisplayAddrOldY = (uint8_t*)Capctxt->OutAddrY[1];
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
    {   
		itp_itv_enable_isp_feature(MMP_ISP_DEINTERLACE); 
        itp_itv_enable_3d_deinterlance(true);
    }
    dbufprop.src_w    = outdata.OutWidth;
    dbufprop.src_h    = outdata.OutHeight;
    dbufprop.pitch_y  = outdata.PitchY;
    dbufprop.pitch_uv = outdata.PitchUV;
	
	dbufprop.format   = MMP_ISP_IN_NV12;
    dbufprop.ya       = outdata.DisplayAddrY;
    dbufprop.ua       = outdata.DisplayAddrU;
    dbufprop.va       = outdata.DisplayAddrV;
    dbufprop.old_y    = outdata.DisplayAddrOldY;
    //ithPrintf("to ISP outwidth = %d, out height = %d\n", outdata.OutWidth, outdata.OutHeight);
    //ithPrintf("to ISP PitchY = %d, out PitchUV = %d\n", outdata.PitchY, outdata.PitchUV);
	itp_itv_update_dbuf_anchor(&dbufprop);


    return;

}

//hdmi test Run
void* TestFunc(void* arg)
{
	printf("modules test cap start\n");
	itpInit();

#ifdef CFG_LCD_ENABLE
    /*lcd panel int*/
	ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL);
    /*itv int*/
	itp_itv_init();
#endif
        
    uint32_t i;

    ithGpioSetMode(ITH_GPIO_PIN64, ITH_GPIO_MODE2);
    ithGpioSetIn(ITH_GPIO_PIN64);
    ithGpioSetFFIEn(ITH_GPIO_PIN64);
    
    ithGpioSetMode(ITH_GPIO_PIN68, ITH_GPIO_MODE2);
    ithGpioSetIn(ITH_GPIO_PIN68);
    ithGpioSetFFIEn(ITH_GPIO_PIN68);
    
    ithGpioSetMode(ITH_GPIO_PIN69, ITH_GPIO_MODE2);
    ithGpioSetIn(ITH_GPIO_PIN69);
    ithGpioSetFFIEn(ITH_GPIO_PIN69);

    /*input pin , mode selection*/
    for (i = ITH_GPIO_PIN48; i <= ITH_GPIO_PIN55; i++){            
        ithGpioSetMode(i, ITH_GPIO_MODE2);
        ithGpioSetIn(i);
        ithGpioSetFFIEn(i);
    }

    ithCapPowerUp();
    usleep(5*1000);
    /*modules init*/
    ithCapDeviceInitialize();

    /*capture init*/
    ithCapInitialize();
    CAPTURE_SETTING mem_modeset = {MMP_CAP_DEV_ADV7180, MMP_FALSE,  MMP_TRUE, 720, 600};
    ithCapConnect(&gCapDev0,mem_modeset);
    ithCapRegisterIRQ(cap_isr, &gCapDev0);


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
        usleep(15*1000);

    }

}



