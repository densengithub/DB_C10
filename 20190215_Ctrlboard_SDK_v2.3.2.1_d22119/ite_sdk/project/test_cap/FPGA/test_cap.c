#include <sys/ioctl.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include "ite/ith.h"
#include "ite/itp.h"
#include "capture/capture_types_it970.h"
#include "capture/mmp_capture_it970.h"
#include "ite/itv.h"       //for VideoInit()
#include "ite/ith_video.h" //for VideoInit()

#define onfly_mode_test 0
#define dual_cap_intr_test 0
#define onfly_and_mem_mode_test 0

static CAP_DEVICE *gCapDev0 = 0,*gCapDev1 = 0;
static bool       cap0_get_err = true, cap1_get_err = true;
static bool       cap0_frame_end = false, cap1_frame_end = false;

static void
cap_isr(
    void* arg)
{
	uint32_t          capture0state = 0, capture1state = 0; 
	CAP_DEVICE *ptDev    = (CAP_DEVICE *) arg;
	CAP_CONTEXT *Capctxt = (CAP_CONTEXT *)ptDev;

	capture0state = ithCapGetEngineErrorStatus(gCapDev0,MMP_CAP_LANE0_STATUS);
	//ithPrintf("cap0_isr 0x%x\n", capture0state);

	if(capture0state >> 31)
	{
		if((capture0state >> 8) & 0xF)
		{
			//ithPrintf("cap0_isr err\n");
		    cap0_get_err = true;	
			//clear cap0 interrupt and reset error status
		    ithCapClearInterrupt(gCapDev0, MMP_TRUE);
		}
	    else
	    {
	    	//ithPrintf("cap0_isr frame end\n");
	        cap0_frame_end = true;
	    	//clear cap0 interrupt
		    ithCapClearInterrupt(gCapDev0, MMP_FALSE);

	    }
	}else
	{
		capture1state = ithCapGetEngineErrorStatus(gCapDev1,MMP_CAP_LANE0_STATUS);	
		if((capture1state >> 8) & 0xF)
		{
			//ithPrintf("cap1_isr err\n");
		    cap1_get_err = true;	
			//clear cap1 interrupt and reset error status
		    ithCapClearInterrupt(gCapDev1, MMP_TRUE);
		}
	    else
	    {
	    	//ithPrintf("cap1_isr frame end\n");
	        cap1_frame_end = true;
	    	//clear cap1 interrupt
		    ithCapClearInterrupt(gCapDev1, MMP_FALSE);
	    }
	}


	return;
	
}

static int
check_and_fire(
    CAP_DEVICE* ptDev)
{
	uint32_t 	OutWidth=0,OutHeight =0,ROI_Flag=0, timeout = 0;
	CAP_CONTEXT *Capctxt = (CAP_CONTEXT *)ptDev;

	//Detected Hsync.
	while(!(ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS) & 0x1))
	{
		if(++timeout > 50)
			return 1;
		printf("Hsync not stable!\n", ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS));
		usleep(1000);
	}
	//wait for getting interleave
 	usleep(30000);

	//Detected Interleave after Hstable. 
	//if(Capctxt->ininfo.Interleave != ithCapGetDetectedInterleave(ptDev))
	{
		Capctxt->ininfo.Interleave = ithCapGetDetectedInterleave(ptDev);
		printf("Get interleave=%d\n",Capctxt->ininfo.Interleave);
		ithCapSetInterleave(ptDev,Capctxt->ininfo.Interleave);
	}
	
	while((ithCapGetEngineErrorStatus(ptDev,MMP_CAP_LANE0_STATUS)&0x3) != 0x3)
	{
		printf("Hsync or Vsync not stable!\n");
		usleep(1000);
	}
	//Detected Height and Width.
	//if( (Capctxt->ininfo.capwidth != ithCapGetDetectedWidth(ptDev)) || (Capctxt->ininfo.capheight !=ithCapGetDetectedHeight(ptDev)))
	{
		Capctxt->ininfo.capwidth = Capctxt->ininfo.PitchY = Capctxt->ininfo.PitchUV = ithCapGetDetectedWidth(ptDev);
		Capctxt->ininfo.capheight = ithCapGetDetectedHeight(ptDev);
		printf("Ctxt_Width=%d(%d, %d),OCtxt_Height=%d\n",Capctxt->ininfo.capwidth,Capctxt->ininfo.PitchY,Capctxt->ininfo.PitchUV,Capctxt->ininfo.capheight);
		ithCapSetWidthHeight(ptDev,Capctxt->ininfo.capwidth,Capctxt->ininfo.capheight);
        ithCapFire(ptDev, MMP_TRUE);
        printf("Set WidthHeight and fire!\n");
		
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
	}
	return 0;
}

static int 
CaptureGetNewFrame(
    CAP_DEVICE *ptDev ,ITE_CAP_VIDEO_INFO *Outdata)
{
    static int      cap_idx;
    static int      NewBufferIdx      = 0;
    static int      OldBufferIdx      = 0;
    static bool 	GetFirstBufferIdx = false;
    uint16_t      	timeOut = 0,FirstBufferIdx = 1;
	int			    NotDisplay = 0;
	CAP_CONTEXT *Capctxt = (CAP_CONTEXT *)ptDev;
    
    Outdata->OutHeight    = Capctxt->ininfo.capheight;
    Outdata->OutWidth     = Capctxt->ininfo.capwidth;
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
        Outdata->DisplayAddrY = Capctxt->OutAddrY[0];
        Outdata->DisplayAddrU = Capctxt->OutAddrUV[0];
        Outdata->DisplayAddrV = Capctxt->OutAddrUV[0];
        break;

    case 1:
        Outdata->DisplayAddrY = Capctxt->OutAddrY[1];
        Outdata->DisplayAddrU = Capctxt->OutAddrUV[1];
        Outdata->DisplayAddrV = Capctxt->OutAddrUV[1];
        break;

    case 2:
        Outdata->DisplayAddrY = Capctxt->OutAddrY[2];
        Outdata->DisplayAddrU = Capctxt->OutAddrUV[2];
        Outdata->DisplayAddrV = Capctxt->OutAddrUV[2];
        break;
    }
#endif	
    return NotDisplay;
}

static CAP_DEVICE*
CapInit(CAPTURE_DEV_ID DEV_ID)
{
	uint32_t i;
    CAP_DEVICE *ptDev = NULL;
	
	 for (i = ITH_GPIO_PIN38; i <= ITH_GPIO_PIN66; i++)   //GPIO70 ~ GPIO81  for capture setting
	 {
		 ithGpioSetMode(i, ITH_GPIO_MODE2);
		 ithGpioSetIn(i);
	 } 

	 ithCapPowerUp();

	 //1st Capture Init, switch IO direction  //move this function to be the first entry func().
	 ptDev = ithCapInitialize(DEV_ID);
	 
	 ithCapMemoryInitialize(ptDev);
	 
    return ptDev;
}

static int
CapSetting(
	CAP_DEVICE *ptDev,
	ITHIntrHandler cap_isr)
{
    MMP_RESULT result = MMP_SUCCESS;
	CAP_CONTEXT *Capctxt = (CAP_CONTEXT *)ptDev;

    //Capture setting
    //get device information
	ithCapGetDeviceInfo(ptDev);
    if (Capctxt->bMatchResolution == MMP_FALSE)
    {
        printf("MatchResolution fail!!\n");
        return MMP_RESULT_ERROR;
    }
	
	//Register IRQ, capture0/capture1 share the same ISR.
	if(Capctxt->Cap_ID == CAP_DEV_ID0)
	ithCapRegisterIRQ(cap_isr,ptDev);
	
#ifdef CAP_ONFLY_ENABLE
#if onfly_and_mem_mode_test
	ithCapFunEnable(ptDev,MMP_CAP_INTERRUPT);
	ithCapFunEnable(ptDev,MMP_CAP_ONFLY_MODE);
#else
	ithCapFunDisable(ptDev,MMP_CAP_INTERRUPT);
	ithCapFunEnable(ptDev,MMP_CAP_ONFLY_MODE);
#endif
#else //Memory Mode
	ithCapFunEnable(ptDev,MMP_CAP_INTERRUPT);
	ithCapFunDisable(ptDev,MMP_CAP_ONFLY_MODE);
#endif
	 
    //set capture parameter
    //ithCapSetSkipMode(ptDev, MMP_CAPTURE_SKIP_BY_TWO);
	ithCapParameterSetting(ptDev);

	//usleep(1000);
	//Cap_Dump_Reg(CAP_DEV_ID0);

    return result;
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
	CaptureGetNewFrame(gCapDev0, &outdata);

	pYUV = malloc(outdata.OutWidth * outdata.OutHeight * 3) ;
	if(!pYUV) printf("open memeory fail\n");
	memset(pYUV, 0x0, outdata.OutWidth * outdata.OutHeight * 3);
	pTmp = pYUV;

	// dump Y
	addr = outdata.DisplayAddrY;  //Dst Y0
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
	addr = outdata.DisplayAddrU;  //Dst UV0
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


static void
VideoInit(
    void)
{
    ithVideoInit(NULL);
    itv_init();
}

static void
VideoExit(
    void)
{
    /* release dbuf & itv */
    itv_flush_dbuf();
    itv_deinit();

    /* release decoder stuff */
    printf("%s(%d)\n", __FUNCTION__, __LINE__);
    ithVideoExit();
    printf("%s(%d)\n", __FUNCTION__, __LINE__);
}

static void SettingISPAnd_FilpLCD(
	CAP_DEVICE *ptDev)
{
    static uint32_t GetNewWidth = 0,GetNewHeight = 0;
	uint32_t Error_Status = 0,Get_Error_Frame = 0,ReFlipLCD = 0;
    uint8_t *dbuf = NULL;
	CAP_CONTEXT *Capctxt = (CAP_CONTEXT *)ptDev;
    ITV_DBUF_PROPERTY dbufprop = {0};
	ITE_CAP_VIDEO_INFO outdata = {0};

	dbuf = itv_get_dbuf_anchor();
    if (dbuf == NULL)
    {
//        printf("no buf\n");
        return;
    }
	if(Capctxt->Cap_ID == CAP_DEV_ID0)
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
		itv_enable_isp_feature(MMP_ISP_DEINTERLACE); 
	
	//if((GetNewWidth != outdata.OutWidth) || GetNewHeight != outdata.OutHeight)
		ReFlipLCD = 1;
	//printf("+++ %d, %d. %d. %d+++\n", outdata.OutWidth, outdata.OutHeight, outdata.PitchY, outdata.PitchUV);
#if dual_cap_intr_test
	if(Capctxt->Cap_ID == CAP_DEV_ID1)
		dbufprop.src_w   = GetNewWidth  = 640;
	else
		dbufprop.src_w   = GetNewWidth  = outdata.OutWidth;
#else
    dbufprop.src_w   = GetNewWidth  = outdata.OutWidth;
#endif
    dbufprop.src_h   = GetNewHeight = outdata.OutHeight;
    dbufprop.pitch_y  = outdata.PitchY;
    dbufprop.pitch_uv = outdata.PitchUV;
	
	#if 0
		dbufprop.format    = MMP_ISP_IN_YUV444;
		dbufprop.ya = (uint8_t *)ithLcdGetBaseAddrA();
		dbufprop.ua = (uint8_t *)ithLcdGetBaseAddrA()+ (dbufprop.src_w * dbufprop.src_h);
		dbufprop.va = (uint8_t *)ithLcdGetBaseAddrA()+ (dbufprop.src_w * dbufprop.src_h);

		if(ReFlipLCD)
		{
			//printf("dbufprop.ya=0x%x,dbufprop.ua=0x%x,dbufprop.va=0x%x,dbufprop.src_w=%d,dbufprop.src_h=%d,dbufprop.pitch_y=%d,dbufprop.pitch_uv=%d\n",dbufprop.ya,dbufprop.ua,dbufprop.va,dbufprop.src_w,dbufprop.src_h,dbufprop.pitch_y,dbufprop.pitch_uv);
			itv_update_dbuf_anchor(&dbufprop);
		}
	#else
		dbufprop.format   = MMP_ISP_IN_NV12;
	    dbufprop.ya       = outdata.DisplayAddrY;
	    dbufprop.ua       = outdata.DisplayAddrU;
	    dbufprop.va       = outdata.DisplayAddrV;

		//printf("dbufprop.ya=0x%x,dbufprop.ua=0x%x,dbufprop.va=0x%x,dbufprop.src_w=%d,dbufprop.src_h=%d,dbufprop.pitch_y=%d,dbufprop.pitch_uv=%d\n",dbufprop.ya,dbufprop.ua,dbufprop.va,dbufprop.src_w,dbufprop.src_h,dbufprop.pitch_y,dbufprop.pitch_uv);
		itv_update_dbuf_anchor(&dbufprop);
		printf("--------------(%d)\n", Capctxt->Cap_ID);
	#endif	
	
    return;

}

MMP_RESULT
CaptureTerminate(
    void)
{
    MMP_RESULT result = MMP_SUCCESS;

    ithCapDeviceTerminate(gCapDev0);
    
    ithCapTerminate(gCapDev0);    

    return result;
}

void* TestFunc(void* arg)
{
	uint32_t i=0;
	uint32_t r_value=0;
	uint32_t data = 0;
	uint32_t Rwidth=0, Rheight=0,Interleave=1;
	uint32_t HsyncT=0,VsyncT=0,HsyncDet=0,VsyncDet=0;

	printf("test cap start\n");
	itpInit();
	ioctl(ITP_DEVICE_SCREEN, ITP_IOCTL_POST_RESET, NULL);
    ioctl(ITP_DEVICE_BACKLIGHT, ITP_IOCTL_ON, NULL);

	VideoInit();
	usleep(1000*1000*1);

#if defined(CFG_BUILD_ITV) && !defined(CFG_TEST_VIDEO) 
	itv_set_pb_mode(1);
#endif

	gCapDev0 = CapInit(CAP_DEV_ID0); 
	//only register ISR one time
	CapSetting(gCapDev0, cap_isr);
	//usleep(1000*1000*1);
#if dual_cap_intr_test
	gCapDev1 = CapInit(CAP_DEV_ID1); 
	CapSetting(gCapDev1, NULL);
#endif
	
	for (;;)
	{
	    if(cap0_get_err == true)
        {   
        	ithCapWaitEngineIdle(gCapDev0);
            if(!check_and_fire(gCapDev0))
            	cap0_get_err = false;
#if onfly_and_mem_mode_test			
			usleep (1000*100*1);
#endif			
        }
#if dual_cap_intr_test		
		else if(cap1_get_err == true)
        {   
        	ithCapWaitEngineIdle(gCapDev1);
            if(!check_and_fire(gCapDev1))
	            cap1_get_err = false;
        }
#endif		
#if !onfly_mode_test			
        if(cap0_frame_end == true)
        {
	    	SettingISPAnd_FilpLCD(gCapDev0);
        }
#endif		
#if (onfly_and_mem_mode_test || onfly_mode_test)
		{
			//itv_enable_isp_feature(MMP_ISP_DEINTERLACE); 
			uint8_t *dbuf = NULL;
			dbuf = itv_get_dbuf_anchor();
    		if (dbuf != NULL)
    		{
    			ITV_DBUF_PROPERTY dbufprop = {0};

				dbufprop.src_w   = 720;
    			dbufprop.src_h   = 480;
    			dbufprop.pitch_y  = 720;
    			dbufprop.pitch_uv = 720;
	
				dbufprop.format    = MMP_ISP_IN_YUV444;
				dbufprop.ya = (uint8_t *)ithLcdGetBaseAddrA();
				dbufprop.ua = (uint8_t *)ithLcdGetBaseAddrA()+ (dbufprop.src_w * dbufprop.src_h);
				dbufprop.va = (uint8_t *)ithLcdGetBaseAddrA()+ (dbufprop.src_w * dbufprop.src_h);
				dbufprop.src_w   = 640;
				itv_update_dbuf_anchor(&dbufprop);
    		}
		}
#endif
#if dual_cap_intr_test
		if(cap1_frame_end == true)
        {
	    	SettingISPAnd_FilpLCD(gCapDev1);
        }
#endif	
#if (onfly_mode_test && !onfly_and_mem_mode_test && !dual_cap_intr_test)
        usleep (10);
#endif
	    //dump_YUV();
	}
	
	printf("run end!\n");
	ithCapFire(gCapDev0, MMP_FALSE);
	//ithCapUnFire(gCapDev1);
	printf("ending, unfire\n");
}

