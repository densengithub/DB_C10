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
#include "iic/mmp_iic.h"
#ifdef CFG_SENSOR_ENABLE
#include "sensor/mmp_sensor.h"
#endif

static CAPTURE_HANDLE gCapDev0 , gCapDev1;
static bool       cap0_get_err = true, cap1_get_err = true;
static bool       cap0_frame_end = false, cap1_frame_end = false;


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
    else
	{
	    if(gCapDev1.source_id == 0) return;
        
		capture1state = ithCapGetEngineErrorStatus(&gCapDev1,MMP_CAP_LANE0_STATUS);	
		if((capture1state >> 8) & 0xF)
		{
			//ithPrintf("cap1_isr err\n");
		    cap1_get_err = true;	
			//clear cap1 interrupt and reset error status
		    ithCapClearInterrupt(&gCapDev1, MMP_TRUE);
		}
	    else
	    {
	    	//ithPrintf("cap1_isr frame end\n");
	        cap1_frame_end = true;
	    	//clear cap1 interrupt
		    ithCapClearInterrupt(&gCapDev1, MMP_FALSE);
	    }
	}


	return;
	
}

static int
check_and_fire(
    CAPTURE_HANDLE* ptDev)
{
	uint32_t 	OutWidth=0,OutHeight =0,ROI_Flag=0, timeout = 0;
	CAP_CONTEXT *Capctxt = &ptDev->cap_info;

	//Detected Hsync.
	while(!(ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS) & 0x1))
	{
		if(++timeout > 50)
			return 1;
		printf("Hsync not stable!\n", ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS));
		usleep(1000);
	}

	//wait for getting interleave
 	sleep(1);

	//Detected Interleave after Hstable. 
	Capctxt->ininfo.Interleave = ithCapGetDetectedInterleave(ptDev);
	printf("Get interleave=%d\n",Capctxt->ininfo.Interleave);
	ithCapSetInterleave(ptDev,Capctxt->ininfo.Interleave);

	
	while((ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS)&0x3) != 0x3)
	{
		//printf("Hsync or Vsync not stable!\n");
		usleep(1000);
	}

	Capctxt->ininfo.capwidth = Capctxt->ininfo.PitchY = Capctxt->ininfo.PitchUV = ithCapGetDetectedWidth(ptDev);
	Capctxt->ininfo.capheight = ithCapGetDetectedHeight(ptDev);
	printf("Width=%d(%d, %d),Height=%d\n",Capctxt->ininfo.capwidth,Capctxt->ininfo.PitchY,Capctxt->ininfo.PitchUV,Capctxt->ininfo.capheight);
	ithCapSetWidthHeight(ptDev,Capctxt->ininfo.capwidth,Capctxt->ininfo.capheight);
    ithCapFire(ptDev, MMP_TRUE);
    printf("Set WidthHeight and fire! (%d)\n", ptDev->cap_id);
	
	if(Capctxt->ininfo.capwidth % 2)
	{
		OutWidth = Capctxt->ininfo.capwidth -1;
		ROI_Flag = 1;
	}
	else
		OutWidth  = Capctxt->ininfo.capwidth;

	if(Capctxt->ininfo.capheight % 2)
	{
		OutHeight = Capctxt->ininfo.capheight - 1;
		ROI_Flag = 1;
	}
	else
		OutHeight = Capctxt->ininfo.capheight;

	printf("OutWidth =%d,OutHeight=%d\n",OutWidth,OutHeight);
	if(ROI_Flag)
	{
		printf("ROI_Flag=%d\n",ROI_Flag);
		ithCapWaitEngineIdle(ptDev);
		ithCapSetROIWidthHeight(ptDev,OutWidth,OutHeight);
        ithCapFire(ptDev, MMP_TRUE);
		ROI_Flag = 0;
	}

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
    
#if 1
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
#endif	
    return NotDisplay;
}

static void
dump_YUV(
	void)
{

	FILE  *fout = 0;
	char  out_path[256] = {0};
	uint32_t addr = 0;
	uint8_t *pYUV=0,*pTmp=0;
	uint8_t *mappedSysRam = NULL;

	ITE_CAP_VIDEO_INFO outdata = {0};
	CaptureGetNewFrame(&gCapDev0, &outdata);

	pYUV = (uint8_t *)malloc(outdata.OutWidth * outdata.OutHeight * 3) ;
	if(!pYUV) printf("open memeory fail\n");
	memset(pYUV, 0x0, outdata.OutWidth * outdata.OutHeight * 3);
	pTmp = pYUV;

	// dump Y
	addr = (uint32_t)outdata.DisplayAddrY;  //Dst Y0
	printf("Y addr=0x%x\n",addr);
	mappedSysRam = ithMapVram((uint32_t)addr, (outdata.OutWidth * outdata.OutHeight) , ITH_VRAM_READ);
	memcpy(pYUV, mappedSysRam, (outdata.OutWidth * outdata.OutHeight));
	ithUnmapVram(mappedSysRam ,(outdata.OutWidth * outdata.OutHeight));

	snprintf(out_path, 256, "D:/dump_y_%ux%u.raw", outdata.OutWidth, outdata.OutHeight);
	if( !(fout = fopen(out_path, "w")) )
	{
		 printf("open dump_y fail !\n");
		 while(1);
	}
	fwrite(pYUV, 1, (outdata.OutWidth * outdata.OutHeight), fout);
	fclose(fout); 

	pYUV += (outdata.OutWidth * outdata.OutHeight);

	// dump UV
	addr = (uint32_t)outdata.DisplayAddrU;  //Dst UV0
	printf("UV addr=0x%x\n",addr);
	mappedSysRam = ithMapVram((uint32_t)addr, (outdata.OutWidth * outdata.OutHeight) , ITH_VRAM_READ);
	memcpy(pYUV, mappedSysRam, (outdata.OutWidth * outdata.OutHeight));
	ithUnmapVram(mappedSysRam ,(outdata.OutWidth * outdata.OutHeight));

	snprintf(out_path, 256, "D:/dump_UV_%ux%u.raw", outdata.OutWidth, outdata.OutHeight);
	if( !(fout = fopen(out_path, "w")) )
	{
		 printf("open dump_y fail !\n");
		 while(1);
	}
	fwrite(pYUV, 1, (outdata.OutWidth * outdata.OutHeight), fout);
	fclose(fout); 

	pYUV += (outdata.OutWidth * outdata.OutHeight);
	pYUV = pTmp;
	
	//dump Yuv
	snprintf(out_path, 256, "D:/dump_YUV_%ux%u.raw", outdata.OutWidth, outdata.OutHeight);
	if( !(fout = fopen(out_path, "w")) )
	{
		 printf("open dump_y fail !\n");
		 while(1);
	}
	fwrite(pYUV, 1, (outdata.OutWidth * outdata.OutHeight *2), fout);
	fclose(fout); 


	if(pYUV) free(pYUV);
	if(mappedSysRam) 
	{
		mappedSysRam = 0;
	}
	
	printf("dump OK!\n");
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
        //printf("no buf\n");
        return;
    }
	if(ptDev->cap_id == CAP_DEV_ID0)
	{
    	cap0_frame_end = false;
	}else
	{
		cap1_frame_end = false;
	}
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

	itp_itv_update_dbuf_anchor(&dbufprop);
	//printf("FilpLCD(%d)\n", ptDev->cap_id);

	
    return;

}

void SENSOR_GPIO_SETTING()
{
    uint32_t i;

#if defined(SENSOR_PIXELPLUS_PO3100)    
    printf("gpio input 16bit\n");
    /*sensor gpio pin share , mode selection*/
    for (i = ITH_GPIO_PIN43; i <= ITH_GPIO_PIN62; i++){            
        ithGpioSetMode(i, ITH_GPIO_MODE2);
        if(i == ITH_GPIO_PIN60)
            ithGpioSetOut(i);
        else
        {
            ithGpioSetIn(i);
            ithGpioSetFFIEn(i);
        }
    }
#else
    printf("gpio input 8bit\n");
    /*sensor gpio pin share , mode selection*/
    for (i = ITH_GPIO_PIN40; i <= ITH_GPIO_PIN51; i++){            
        ithGpioSetMode(i, ITH_GPIO_MODE2);
        if(i == ITH_GPIO_PIN43)
            ithGpioSetOut(i);
        else
        {
            ithGpioSetIn(i);
            ithGpioSetFFIEn(i);
        }
    }
#endif
}

MMP_RESULT
CaptureTerminate(
    void)
{
    MMP_RESULT result = MMP_SUCCESS;

    ithCapDeviceTerminate(&gCapDev0);
    
    ithCapTerminate();    

    return result;
}
//Sensor Test Run
void* TestFunc(void* arg)
{
	printf("sensor test cap start\n");
	itpInit();
    /*lcd panel int*/
#ifdef CFG_LCD_MULTIPLE
    ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_RESET, (void*)0);
#endif    
	ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL);
    ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_ON, NULL);
    
    SENSOR_GPIO_SETTING();

    /*itv int*/
	itp_itv_init();
	usleep(1000*500);   

    /*NT99141 Power On Sequence , need valid clk from capture*/
    ithCapPowerUp();

#if defined(CFG_SENSOR_ENABLE)
    /*sensor setting*/
    ithSensorRegsisterCallback();
    ithSensorPowerOn(MMP_TRUE, MMP_TRUE);
    ithSensorInitialize();
    printf("ithSensorInitialize finish \n");
#endif

    /*capture setting*/
    ithCapInitialize();
    CAPTURE_SETTING mem_modeset = {MMP_CAP_DEV_SENSOR, MMP_FALSE,  MMP_TRUE, 1280, 720};
    ithCapConnect(&gCapDev0,mem_modeset);
    ithCapRegisterIRQ(cap_isr, &gCapDev0);
    ithCapGetDeviceInfo(&gCapDev0);
    ithCapParameterSetting(&gCapDev0);

    printf("cap pre setting finish \n");
  
    /*main loop*/
	while(1)
	{    
	    if(cap0_get_err == true){
        	ithCapWaitEngineIdle(&gCapDev0);
            if(!check_and_fire(&gCapDev0))
            	cap0_get_err = false;            
        }
        
        if(cap0_frame_end == true){
	        SettingISPAnd_FilpLCD(&gCapDev0);
        }
              
        usleep(10);
    }

}



