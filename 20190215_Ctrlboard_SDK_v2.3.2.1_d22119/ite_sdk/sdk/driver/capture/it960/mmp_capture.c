#include <string.h> 
#include "capture_config.h"
#include "capture_hw.h"
#include "capture.h"
#include "mmp_capture.h"


//=============================================================================
//                Constant Definition
//=============================================================================
#define CAPCONFIGNUM 64
//=============================================================================
//                Macro Definition
//=============================================================================

//=============================================================================
//                Structure Definition
//=============================================================================

//=============================================================================
//                Global Data Definition
//=============================================================================
MMP_UINT32 CAPConfigTable[] =
{
#include "defcap_config.h"
};

MMP_UINT16        gCapinitNum;                                    //record how many device opened
pthread_mutex_t   gCapnum_mutex;                                  //for gCapinitNum mutex protect
MMP_UINT32*       gCap0Config = CAPConfigTable;                   //cap0 config


//=============================================================================
//                Private Function Definition
//=============================================================================

//=============================================================================
//                Public Function Definition
//=============================================================================
//=============================================================================
/**
 * Cap context initialization.
 * @param   none.
 * @return  MMP_RESULT :init success or fail
 */
//=============================================================================

MMP_RESULT
ithCapInitialize(
    void)
{
    MMP_RESULT  result   = MMP_SUCCESS;

    ithCap_Set_Reset();
    //ithCap_Set_Register_Reset();
    //ithCapHWICInit();    
    gCapinitNum = 0;
    /* mutex lock init */
    pthread_mutex_init(&gCapnum_mutex, NULL);    

    if (result)
    {
        cap_msg_ex(CAP_MSG_TYPE_ERR, "%s error %d", __FUNCTION__, __LINE__);
    }
    return result;
}

//=============================================================================
/**
 * Cap connect source and capinfo default init.
 * @param[in]   *ptDev,points to capture_handle structure.
 * @param[in]   CAPTURE_SETTING info
 * @return      MMP_RESULT :connect success or fail.
 *
 * Connect will be  use cap0 hw , can use Onfly mode,MEM mode,Onfly with MEM mode 
 */
//=============================================================================
MMP_RESULT ithCapConnect(
    CAPTURE_HANDLE *ptDev, CAPTURE_SETTING info)
{
    MMP_RESULT result = MMP_SUCCESS;
 
    pthread_mutex_lock(&gCapnum_mutex);
 
    if (ptDev == NULL || gCapinitNum >= CAP_DEVICE_ID_MAX)
    {
        result = MMP_RESULT_ERROR;
        goto end;
    }
 
    /* reset caphandle mem */
    memset((void *)ptDev, 0, sizeof(CAPTURE_HANDLE));
 
    /* count capture be init num */
    gCapinitNum++;
    
    /* cap id setting */
    ptDev->cap_id = gCapinitNum - 1;
 
    pthread_mutex_unlock(&gCapnum_mutex);
 
    /* default capinfo setting */
    Cap_Initialize(&ptDev->cap_info);
    
    /* capture set user config*/
    if(ptDev->cap_id == CAP_DEV_ID0)
        Cap_SetUserConfig(ptDev, gCap0Config);
    /* frontend source id setting */
    ptDev->source_id = info.inputsource;
    
    /* capture onflymode flag setting */
    if (info.OnflyMode_en)
    {   
        ptDev->cap_info.EnableOnflyMode = MMP_TRUE;//onfly mode
        if(info.Interrupt_en)
            Cap_Memory_Initialize(ptDev, info);// onfly with memory mode.
    }
    else
    {
        ptDev->cap_info.EnableOnflyMode = MMP_FALSE;
        Cap_Memory_Initialize(ptDev, info);//memory mode
    }
 
    /* capture interrupt flag setting */
    if (info.Interrupt_en)
        ptDev->cap_info.EnableInterrupt = MMP_TRUE;
    else
        ptDev->cap_info.EnableInterrupt = MMP_FALSE;
    
    /* setting mem pitchY pitchUV */
    ptDev->cap_info.ininfo.PitchY = info.Max_Width;
    ptDev->cap_info.ininfo.PitchUV = info.Max_Width;
    
end:
    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "%s error %d", __FUNCTION__, __LINE__);
    return result;
}

//=============================================================================
/**
 * Cap disconnect source include disable engine(unfire) ,free memory ,and reset cap_handle .
 * @param[in]   *ptDev,points to capture_handle structure.
 * @return      MMP_RESULT :disconnect success or fail.
 */
//=============================================================================
MMP_RESULT ithCapDisConnect(CAPTURE_HANDLE *ptDev)
{
    MMP_RESULT result = MMP_SUCCESS;
 
    if (ptDev == NULL)
    {
        result = MMP_RESULT_ERROR;
        goto end;
    }
    
    if(ithCap_Get_IsFire(ptDev->cap_id))
    {
         ithCap_Set_UnFire(ptDev->cap_id);
         result = ithCap_Get_WaitEngineIdle(ptDev->cap_id);
         if (result)
         {
             cap_msg_ex(CAP_MSG_TYPE_ERR, " err 0x%x !\n", result);
             goto end;
         }
         ithCap_Set_Clean_Intr(ptDev->cap_id);
    }
    /*memory mode*/ 
    if(!ptDev->cap_info.EnableOnflyMode && ptDev->cap_info.EnableInterrupt)
    {
        Cap_Memory_Clear(ptDev);
    }
    /*onfly with memory mode*/ 
    if(ptDev->cap_info.EnableOnflyMode && ptDev->cap_info.EnableInterrupt)
    {
        Cap_Memory_Clear(ptDev);
    }
    
    /* reset caphandle mem */
    memset((void *)ptDev, 0, sizeof(CAPTURE_HANDLE));
    
 
    pthread_mutex_lock(&gCapnum_mutex);
    if (gCapinitNum > 0)
        gCapinitNum--;
    pthread_mutex_unlock(&gCapnum_mutex);
end:
    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "%s error %d", __FUNCTION__, __LINE__);
    
    return result;
}

//=============================================================================
/**
 * Cap terminate all engine,include disable engine(unfire)  and reset engine.
 * @param   none.
 * @return  MMP_RESULT,1 => fail 0 => success
 */
//=============================================================================

MMP_RESULT
ithCapTerminate(
    void)
{
    MMP_RESULT  result   = MMP_SUCCESS;
    MMP_UINT16  index   = 0;

    /*Disable Cap all engine*/
    for (index = 0; index < CAP_DEVICE_ID_MAX; index++)
    {
        if(ithCap_Get_IsFire(index))
        {
            ithCap_Set_UnFire(index);
            result = ithCap_Get_WaitEngineIdle(index);
            if (result)
            {
                cap_msg_ex(CAP_MSG_TYPE_ERR, " err 0x%x !\n", result);
                goto end;
            }
        }
        ithCap_Set_Clean_Intr(index);
    }
    
    ithCap_Set_Reset();
end:
    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, " %s() err 0x%x !\n", __FUNCTION__, result);

    return result;
}

//=============================================================================
/**
 * ithCapIsFire
 */
//=============================================================================
MMP_BOOL
ithCapIsFire(
    CAPTURE_HANDLE *ptDev)
{
    return ithCap_Get_IsFire(ptDev->cap_id);
}

//=============================================================================
/**
 * ithCapGetEngineErrorStatus
 */
//=============================================================================
MMP_UINT32
ithCapGetEngineErrorStatus(
    CAPTURE_HANDLE *ptDev, MMP_CAP_LANE_STATUS lanenum)
{
    return ithCap_Get_Lane_status(ptDev->cap_id, lanenum);
}
#if 0
//=============================================================================
/**
 * ithCapGetEngineErrorStatus
 */
//=============================================================================
MMP_UINT32
ithCapGetEngineErrorCodeStatus(
    CAPTURE_HANDLE *ptDev)
{
    return ithCap_Get_Error_Status(ptDev->cap_id);
}
#endif
//=============================================================================
/**
 * ithCapGetAllEngineStatus
 */
//=============================================================================
void
ithCapGetAllEngineStatus(
    CAPTURE_HANDLE *ptDev)
{
    ithCap_Get_Detected_Region(ptDev->cap_id);
}
#if 0
//=============================================================================
/**
 * ithCapOnflyResetAllEngine
 */
//=============================================================================
MMP_RESULT
ithCapOnflyResetAllEngine(
    CAPTURE_HANDLE *ptDev)
{
    MMP_RESULT  result   = MMP_SUCCESS;
    MMP_UINT16  status;

    result = ithCap_Get_WaitEngineIdle(ptDev->cap_id);
    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "!!!! capture not idle !!!!\n");


    ithCap_Set_UnFire(ptDev->cap_id);
    ithCap_Set_Reset();
    ithCap_Set_Clean_Intr(ptDev->cap_id);

    return result;
}

//=============================================================================
/**
 * ithCapResetEngine
 */
//=============================================================================
MMP_RESULT
ithCapResetEngine(
    CAPTURE_HANDLE *ptDev)
{
    MMP_RESULT  result   = MMP_SUCCESS;

    ithCap_Set_UnFire(ptDev->cap_id);
    ithCap_Set_Reset();
    ithCap_Set_Clean_Intr(ptDev->cap_id);

    return result;
}
#endif
//=============================================================================
/**
 * ithCapParameterSetting
 */
//=============================================================================
MMP_RESULT
ithCapParameterSetting(
    CAPTURE_HANDLE *ptDev)
{
    MMP_RESULT  result   = MMP_SUCCESS;

    if (ptDev == MMP_NULL)
    {
        cap_msg_ex(CAP_MSG_TYPE_ERR, "Capture not initialize\n");
        result = MMP_RESULT_ERROR;
        goto end;
    }

    //Update parameter
    result = Cap_Update_Reg(ptDev);

end:
    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "%s (%d) ERROR !!!!\n", __FUNCTION__, __LINE__);

    return result;
}

//=============================================================================
/**
 * ithCapFire
 */
//=============================================================================
void
ithCapFire(
    CAPTURE_HANDLE *ptDev, MMP_BOOL enable)
{
    if(enable)
    {
        ithCap_Set_MemThreshold(ptDev->cap_id, ptDev->cap_info.ininfo.WrMergeThresld);
        // Update Error Handle mode
#ifdef SENSOR_DEV
        ithCap_Set_Error_Handleing(ptDev->cap_id,0xFF7F); //just turn off frame rate change error.
#else
        ithCap_Set_Error_Handleing(ptDev->cap_id,0xFFFF);
#endif
        ithCap_Set_Wait_Error_Reset(ptDev->cap_id);

        ithCap_Set_Fire(ptDev->cap_id);
    }
    else
    {
        ithCap_Set_UnFire(ptDev->cap_id);
    }
}
#if 0
//=============================================================================
/**
 * ithCapMemoryInitialize
 */
//=============================================================================
MMP_RESULT
ithCapMemoryInitialize(
    CAPTURE_HANDLE *ptDev)
{
    return Cap_Memory_Initialize(ptDev);
}

//=============================================================================
/**
 * ithCapMemoryClear
 */
//=============================================================================
MMP_RESULT
ithCapMemoryClear(
    CAPTURE_HANDLE *ptDev)
{
    return Cap_Memory_Clear(ptDev);
}
#endif
//=============================================================================
/**
 * Register  an interrupt handler[Only OPENRTOS]
 * @param[in]   user define caphandler
 * @param[in]   *ptDev,points to capture_handle structure.
 * @return      none.
 */
//=============================================================================
void
ithCapRegisterIRQ(
    ITHIntrHandler caphandler, CAPTURE_HANDLE *ptDev)
{
    // Initialize Capture IRQ
    ithIntrDisableIrq(ITH_INTR_CAPTURE);
    ithIntrClearIrq(ITH_INTR_CAPTURE);
#if defined (__OPENRTOS__)
    // register NAND Handler to IRQ
    ithIntrRegisterHandlerIrq(ITH_INTR_CAPTURE, caphandler, (void *)ptDev);
#endif     // defined (__OPENRTOS__)

    // set IRQ to edge trigger
    ithIntrSetTriggerModeIrq(ITH_INTR_CAPTURE, ITH_INTR_EDGE);

    // set IRQ to detect rising edge
    ithIntrSetTriggerLevelIrq(ITH_INTR_CAPTURE, ITH_INTR_HIGH_RISING);

    // Enable IRQ
    ithIntrEnableIrq(ITH_INTR_CAPTURE);
}

//=============================================================================
/**
 * ithCapDisableIRQ
 */
//=============================================================================
void
ithCapDisableIRQ(
    void)
{
    // Initialize Capture IRQ
    ithIntrDisableIrq(ITH_INTR_CAPTURE);
    ithIntrClearIrq(ITH_INTR_CAPTURE);
}

//=============================================================================
/**
 * ithCapClearInterrupt
 */
//=============================================================================
MMP_UINT16
ithCapClearInterrupt(
    CAPTURE_HANDLE *ptDev, MMP_BOOL get_err)
{
    if (get_err)
    {
        ithCap_Set_ErrReset(ptDev->cap_id);
        //ithCap_Set_Error_Handleing(ptDev->cap_id, 0x0);
    }

    ithCap_Set_Clean_Intr(ptDev->cap_id);
    
    return 0;
}

//=============================================================================
/**
 * Get current number of memory ring buffer
 * @param[in]   *ptDev,points to capture_handle structure.
 * @return      current buffer number.
 */
//=============================================================================

MMP_UINT16
ithCapReturnWrBufIndex(
    CAPTURE_HANDLE *ptDev)
{
    MMP_UINT16  CapWrBufIndex;
    MMP_UINT16  status   = ithCap_Get_Lane_status(ptDev->cap_id, CAP_LANE0_STATUS);

    CapWrBufIndex = ((status & 0x70) >> 4);

    return CapWrBufIndex;
}

//=============================================================================
/**
 * Get current input Source Frame Rate(Note: need wait capture 1 frame )
 * @param[in] *ptDev,points to capture_handle structure.     
 * @return      MMP_CAP_FRAMERATE type.
 */
//=============================================================================
MMP_CAP_FRAMERATE
ithCapGetInputFrameRate(
    CAPTURE_HANDLE *ptDev)
{
    MMP_UINT32  RawVTotal;
    MMP_UINT16  FrameRate_mode;
    MMP_UINT32  MCLK_Freq;
    MMP_UINT32  framerate;
    MMP_UINT32  div;

    RawVTotal = ithCap_Get_MRawVTotal(ptDev->cap_id);
    if(RawVTotal > 0)
    {
        div    = ithReadRegA(0xD8000014) & 0xF;
        MCLK_Freq = 972 / div;//PLL1_N1 792MHZ
        framerate = ((3906.25 * MCLK_Freq * 1000) /RawVTotal);
        //printf("RawVTotal = %d MCLK_Freq = %d framerate = %d\n", RawVTotal, MCLK_Freq, framerate);
    }

    if ((23988 > framerate) && (framerate > 23946))       // 23.976fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_23_97HZ;
    }
    else if ((24030 > framerate) && (framerate > 23987))  // 24fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_24HZ;
    }
    else if ((25030 > framerate) && (framerate > 24970))  // 25fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_25HZ;
    }
    else if ((29985 > framerate) && (framerate > 29940))  // 29.97fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_29_97HZ;
    }
    else if ((30030 > framerate) && (framerate > 29984))  // 30fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_30HZ;
    }
    else if ((50030 > framerate) && (framerate > 49970))  // 50fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_50HZ;
    }
    else if ((57000 > framerate) && (framerate > 55000))  // 56fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_56HZ;
    }
    else if ((59970 > framerate) && (framerate > 57001))  // 59.94fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_59_94HZ;
    }
    else if ((62030 > framerate) && (framerate > 59969))  // 60fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_60HZ;
    }
    else if ((70999 > framerate) && (framerate > 69000))  // 70fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_70HZ;
    }
    else if ((73000 > framerate) && (framerate > 71000))  // 72fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_72HZ;
    }
    else if ((76000 > framerate) && (framerate > 74000))  // 75fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_75HZ;
    }
    else if ((86000 > framerate) && (framerate > 84000))  // 85fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_85HZ;
    }
    else
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_UNKNOW;
    }
    //printf("frame rate mode = %d \n",FrameRate_mode);
    return FrameRate_mode;
}

//=============================================================================
/**
 * Get current output Frame Rate,according to skip mode
 * @param[in]    *ptDev,points to capture_handle structure.
 * @param[out]  input original frame rate.(unit:frames per millisecond)
 * @return         Capture output frame rate, according to skip mode
 */
//=============================================================================
MMP_CAP_FRAMERATE
ithCapGetOutputFrameRate(
    CAPTURE_HANDLE *ptDev, MMP_UINT32 *FramePeriod)
{
    MMP_UINT32        RawVTotal;
    MMP_UINT32        framerate;
    MMP_FLOAT         MCLK_Freq;
    MMP_UINT32        div;
    MMP_CAP_FRAMERATE FrameRate_mode = MMP_CAP_FRAMERATE_UNKNOW;
    CAP_CONTEXT       *Capctxt       = &ptDev->cap_info;

    RawVTotal = ithCap_Get_MRawVTotal(ptDev->cap_id);

    if(RawVTotal > 0)
    {
        div    = ithReadRegA(0xD8000014) & 0xF;
        MCLK_Freq = 972 / div;//PLL1_N1 792MHZ
        framerate = ((3906.25 * MCLK_Freq * 1000) /RawVTotal);
        //printf("RawVTotal = %d MCLK_Freq = %d framerate = %d\n", RawVTotal, MCLK_Freq, framerate);
    }

    if (ptDev->source_id == MMP_CAP_DEV_HDMIRX && ithCapGetResolutionIndex(MMP_CAP_DEV_HDMIRX) >= CAP_HDMI_INPUT_VESA)
    {
        if (Capctxt->skip_mode == CAPTURE_SKIP_BY_TWO)
            FrameRate_mode = MMP_CAP_FRAMERATE_VESA_30HZ;
        else
            FrameRate_mode = MMP_CAP_FRAMERATE_VESA_60HZ;
    }
    else if ((23988 > framerate) && (framerate > 23946))  // 23.976fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_23_97HZ;
    }
    else if ((24030 > framerate) && (framerate > 23987))  // 24fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_24HZ;
    }
    else if ((25030 > framerate) && (framerate > 24970))  // 25fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_25HZ;
    }
    else if ((29985 > framerate) && (framerate > 29940))  // 29.97fps
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_29_97HZ;
    }
    else if ((30030 > framerate) && (framerate > 29984))  // 30fps
    {
        if(Capctxt->skip_mode == CAPTURE_SKIP_30FPS_TO_25FPS)
            FrameRate_mode = MMP_CAP_FRAMERATE_25HZ;
        else
            FrameRate_mode = MMP_CAP_FRAMERATE_30HZ;
    }
    else if ((50030 > framerate) && (framerate > 49970))  // 50fps
    {
        if (Capctxt->ininfo.Interleave == MMP_TRUE || Capctxt->skip_mode == CAPTURE_SKIP_BY_TWO)
            FrameRate_mode = MMP_CAP_FRAMERATE_25HZ;
        else
            FrameRate_mode = MMP_CAP_FRAMERATE_50HZ;
    }
    else if ((59970 > framerate) && (framerate > 59910))  // 59.94fps
    {
        if (Capctxt->ininfo.Interleave == MMP_TRUE || Capctxt->skip_mode == CAPTURE_SKIP_BY_TWO)
            FrameRate_mode = MMP_CAP_FRAMERATE_29_97HZ;
        else
            FrameRate_mode = MMP_CAP_FRAMERATE_59_94HZ;
    }
    else if ((60030 > framerate) && (framerate > 59969))  // 60fps
    {
        if (Capctxt->ininfo.Interleave == MMP_TRUE || Capctxt->skip_mode == CAPTURE_SKIP_BY_TWO)
            FrameRate_mode = MMP_CAP_FRAMERATE_30HZ;
        else if(Capctxt->skip_mode == CAPTURE_SKIP_60FPS_TO_25FPS)
            FrameRate_mode = MMP_CAP_FRAMERATE_25HZ;
        else
            FrameRate_mode = MMP_CAP_FRAMERATE_60HZ;
    }
    else
    {
        FrameRate_mode = MMP_CAP_FRAMERATE_UNKNOW;
    }

    if (Capctxt->ininfo.Interleave == MMP_TRUE || Capctxt->skip_mode == CAPTURE_SKIP_BY_TWO)
        *FramePeriod = (framerate >> 1);
    else
        *FramePeriod = framerate;

    return FrameRate_mode;
}

//=============================================================================
/**
 * ithCapGetInputSrcInfo
 */
//=============================================================================
MMP_CAP_INPUT_INFO
ithCapGetInputSrcInfo(
    CAPTURE_HANDLE *ptDev)
{
    MMP_CAP_INPUT_INFO info;
    MMP_CAP_FRAMERATE  frameRateMode;
    CAP_CONTEXT        *Capctxt = &ptDev->cap_info;

    if (Capctxt->bMatchResolution == MMP_FALSE)
        return MMP_CAP_INPUT_INFO_UNKNOWN;

    if (ptDev->source_id == MMP_CAP_DEV_HDMIRX && ithCapGetResolutionIndex(MMP_CAP_DEV_HDMIRX) >= CAP_HDMI_INPUT_VESA )
    {
        ithCapVESATimingCheck(ptDev, Capctxt->ininfo.capwidth, Capctxt->ininfo.capheight, &info);
    }
    else
    {
        frameRateMode = ithCapGetInputFrameRate(ptDev);
        
        switch (frameRateMode)
        {
        case MMP_CAP_FRAMERATE_23_97HZ:
            if (Capctxt->ininfo.capwidth == 1920 && Capctxt->ininfo.capheight == 1080)
                info = MMP_CAP_INPUT_INFO_1920X1080_23P;
            else
                info = MMP_CAP_INPUT_INFO_UNKNOWN;
            break;

        case MMP_CAP_FRAMERATE_24HZ:
            if (Capctxt->ininfo.capwidth == 1920 && Capctxt->ininfo.capheight == 1080)
                info = MMP_CAP_INPUT_INFO_1920X1080_24P;
            else
                info = MMP_CAP_INPUT_INFO_UNKNOWN;
            break;

        case MMP_CAP_FRAMERATE_25HZ:
            if (Capctxt->ininfo.capwidth == 1920 && Capctxt->ininfo.capheight == 1080)
                info = MMP_CAP_INPUT_INFO_1920X1080_25P;
            else
                info = MMP_CAP_INPUT_INFO_UNKNOWN;
            break;

        case MMP_CAP_FRAMERATE_29_97HZ:
            if (Capctxt->ininfo.capwidth == 1920 && Capctxt->ininfo.capheight == 1080)
                info = MMP_CAP_INPUT_INFO_1920X1080_29P;
            else
                info = MMP_CAP_INPUT_INFO_UNKNOWN;
            break;

        case MMP_CAP_FRAMERATE_30HZ:
            if (Capctxt->ininfo.capwidth == 1920 && Capctxt->ininfo.capheight == 1080)
                info = MMP_CAP_INPUT_INFO_1920X1080_30P;
            else
                info = MMP_CAP_INPUT_INFO_UNKNOWN;
            break;

        case MMP_CAP_FRAMERATE_50HZ:
            if (Capctxt->ininfo.capwidth == 720 && Capctxt->ininfo.capheight == 576)
            {
                if (Capctxt->ininfo.Interleave)
                    info = MMP_CAP_INPUT_INFO_720X576_50I;
                else
                    info = MMP_CAP_INPUT_INFO_720X576_50P;
            }
            else if (Capctxt->ininfo.capwidth == 1280 && Capctxt->ininfo.capheight == 720)
            {
                info = MMP_CAP_INPUT_INFO_1280X720_50P;
            }
            else if (Capctxt->ininfo.capwidth == 1920 && Capctxt->ininfo.capheight == 1080)
            {
                if (Capctxt->ininfo.Interleave)
                    info = MMP_CAP_INPUT_INFO_1920X1080_50I;
                else
                    info = MMP_CAP_INPUT_INFO_1920X1080_50P;
            }
            else
            {
                info = MMP_CAP_INPUT_INFO_UNKNOWN;
            }
            break;

        case MMP_CAP_FRAMERATE_59_94HZ:
            if (Capctxt->ininfo.capwidth == 720 && Capctxt->ininfo.capheight == 480)
            {
                if (Capctxt->ininfo.Interleave)
                    info = MMP_CAP_INPUT_INFO_720X480_59I;
                else
                    info = MMP_CAP_INPUT_INFO_720X480_59P;
            }
            else if (Capctxt->ininfo.capwidth == 1280 && Capctxt->ininfo.capheight == 720)
            {
                info = MMP_CAP_INPUT_INFO_1280X720_59P;
            }
            else if (Capctxt->ininfo.capwidth == 1920 && Capctxt->ininfo.capheight == 1080)
            {
                if (Capctxt->ininfo.Interleave)
                    info = MMP_CAP_INPUT_INFO_1920X1080_59I;
                else
                    info = MMP_CAP_INPUT_INFO_1920X1080_59P;
            }
            else if (Capctxt->ininfo.capwidth == 640 && Capctxt->ininfo.capheight == 480)
            {
                info = MMP_CAP_INPUT_INFO_640X480_60P;
            }
            else
            {
                info = MMP_CAP_INPUT_INFO_UNKNOWN;
            }
            break;

        case MMP_CAP_FRAMERATE_60HZ:
            if (Capctxt->ininfo.capwidth == 720 && Capctxt->ininfo.capheight == 480)
            {
                if (Capctxt->ininfo.Interleave)
                    info = MMP_CAP_INPUT_INFO_720X480_60I;
                else
                    info = MMP_CAP_INPUT_INFO_720X480_60P;
            }
            else if (Capctxt->ininfo.capwidth == 1280 && Capctxt->ininfo.capheight == 720)
            {
                info = MMP_CAP_INPUT_INFO_1280X720_60P;
            }
            else if (Capctxt->ininfo.capwidth == 1920 && Capctxt->ininfo.capheight == 1080)
            {
                if (Capctxt->ininfo.Interleave)
                    info = MMP_CAP_INPUT_INFO_1920X1080_60I;
                else
                    info = MMP_CAP_INPUT_INFO_1920X1080_60P;
            }
            else if (Capctxt->ininfo.capwidth == 640 && Capctxt->ininfo.capheight == 480)
            {
                info = MMP_CAP_INPUT_INFO_640X480_60P;
            }
            else
            {
                info = MMP_CAP_INPUT_INFO_UNKNOWN;
            }
            break;

        default:
            info = MMP_CAP_INPUT_INFO_UNKNOWN;
            break;
        }
    }

    return info;
}

//=============================================================================
/**
 * ithAVSyncCounterInit
 */
//=============================================================================
void
ithAVSyncCounterCtrl(CAPTURE_HANDLE *ptDev, AV_SYNC_COUNTER_CTRL mode, MMP_UINT16 divider)
{
    ithAVSync_CounterCtrl(ptDev->cap_id, mode, divider);
}

//=============================================================================
/**
 * ithAVSyncCounterReset
 */
//=============================================================================
void
ithAVSyncCounterReset(CAPTURE_HANDLE *ptDev, AV_SYNC_COUNTER_CTRL mode)
{
    ithAVSync_CounterReset(ptDev->cap_id, mode);
}

//=============================================================================
/**
 * ithAVSyncCounterLatch
 */
//=============================================================================
MMP_UINT32
ithAVSyncCounterLatch(CAPTURE_HANDLE *ptDev, AV_SYNC_COUNTER_CTRL cntSel)
{
    ithAVSync_CounterLatch(ptDev->cap_id, cntSel);
}

//=============================================================================
/**
 * ithAVSyncCounterRead
 */
//=============================================================================
MMP_UINT32
ithAVSyncCounterRead(CAPTURE_HANDLE *ptDev, AV_SYNC_COUNTER_CTRL mode)
{
    return ithAVSync_CounterRead(ptDev->cap_id, mode);
}

//=============================================================================
/**
 * ithAVSyncMuteDetect
 */
//=============================================================================
MMP_BOOL
ithAVSyncMuteDetect(CAPTURE_HANDLE *ptDev)
{
    return ithAVSync_MuteDetect(ptDev->cap_id);
}

//=============================================================================
/**
 * ithCapSetSkipPattern
 */
//=============================================================================
MMP_RESULT
ithCapSetSkipMode(
    CAPTURE_HANDLE *ptDev, MMP_CAP_SKIP_MODE mode)
{
    MMP_RESULT  result   = MMP_SUCCESS;

    if (ptDev == MMP_NULL)
    {
        cap_msg_ex(CAP_MSG_TYPE_ERR, "Capture not initialize\n");
        result = MMP_RESULT_ERROR;
        goto end;
    }

    Cap_SetSkipMode(ptDev, (CAP_SKIP_MODE) mode);

end:
    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, "%s (%d) ERROR !!!!\n", __FUNCTION__, __LINE__);

    return result;
}

#if 0
//=============================================================================
/**
 * ithCapIsOnflyMode
 */
//=============================================================================
MMP_BOOL
ithCapIsOnflyMode(
    CAPTURE_HANDLE *ptDev)
{
    CAP_CONTEXT *Capctxt = &ptDev->cap_info;

    if (Capctxt->EnableOnflyMode)
        return MMP_TRUE;
    else
        return MMP_FALSE;
}
#endif
//=============================================================================
/**
 * Set color control value.
 */
//=============================================================================
void
ithCapSetColorCtrl(
    CAPTURE_HANDLE *ptDev, const MMP_CAP_COLOR_CTRL *data)
{
#if defined (CAP_USE_COLOR_EFFECT)
    CAP_CONTEXT *Capctxt = &ptDev->cap_info;

    if (data->brightness > 127)
        Capctxt->ColorCtrl.brightness = 127;
    else if (data->brightness < -128)
        Capctxt->ColorCtrl.brightness = -128;
    else
        Capctxt->ColorCtrl.brightness = data->brightness;

    if (data->contrast > 4.0)
        Capctxt->ColorCtrl.contrast = 4.0;
    else if (data->contrast < 0.0)
        Capctxt->ColorCtrl.contrast = 0.0;
    else
        Capctxt->ColorCtrl.contrast = data->contrast;

    if (data->hue > 359)
        Capctxt->ColorCtrl.hue = 359;
    else if (data->hue < 0)
        Capctxt->ColorCtrl.hue = 0;
    else
        Capctxt->ColorCtrl.hue = data->hue;

    if (data->saturation > 4.0)
        Capctxt->ColorCtrl.saturation = 4.0;
    else if (data->saturation < 0.0)
        Capctxt->ColorCtrl.saturation = 0.0;
    else
        Capctxt->ColorCtrl.saturation = data->saturation;

    Cap_SetColorCorrMatrix(
        &Capctxt->CCFun,
        Capctxt->ColorCtrl.brightness,
        Capctxt->ColorCtrl.contrast,
        Capctxt->ColorCtrl.hue,
        Capctxt->ColorCtrl.saturation,
        Capctxt->ColorCtrl.colorEffect);

    Capctxt->UpdateFlags |= CAP_FLAGS_UPDATE_CCMatrix;
#endif
}

//=============================================================================
/**
 * Get color control value.
 */
//=============================================================================
void
ithCapGetColorCtrl(
    CAPTURE_HANDLE *ptDev, MMP_CAP_COLOR_CTRL *data)
{
#if defined (CAP_USE_COLOR_EFFECT)
    CAP_CONTEXT *Capctxt = &ptDev->cap_info;

    data->brightness = Capctxt->ColorCtrl.brightness;
    data->contrast   = Capctxt->ColorCtrl.contrast;
    data->hue        = Capctxt->ColorCtrl.hue;
    data->saturation = Capctxt->ColorCtrl.saturation;
#endif
}

//=============================================================================
/**
 * Update Color Matrix.
 */
//=============================================================================
void
ithCapUpdateColorMatrix(
    CAPTURE_HANDLE *ptDev)
{
#if defined (CAP_USE_COLOR_EFFECT)
    Cap_UpdateColorMatrix(ptDev);
#endif
}

//=============================================================================
/**
 * Capture Power Up.
 */
//=============================================================================
void
ithCapPowerUp(
    void)
{
    ithCap_Set_Reset();
    ithCapEnableClock();
}

//=============================================================================
/**
 * Capture Power Down.
 */
//=============================================================================
void
ithCapPowerDown(
    void)
{
    MMP_UINT16  index   = 0;
    MMP_RESULT  result   = MMP_SUCCESS;
    
    for (index = 0; index < CAP_DEVICE_ID_MAX; index++)
    {
        if(ithCap_Get_IsFire(index))
        {
            ithCap_Set_UnFire(index);
            result = ithCap_Get_WaitEngineIdle(index);
            if (result)
            {
                cap_msg_ex(CAP_MSG_TYPE_ERR, " err 0x%x !\n", result);
                goto end;
            }
        }
        ithCap_Set_Clean_Intr(index);
    }
    ithCapDisableClock();
    
end:
    if (result)
        cap_msg_ex(CAP_MSG_TYPE_ERR, " %s() err 0x%x !\n", __FUNCTION__, result);    
}

//=============================================================================
/**
 * ithCapWaitEngineIdle
 */
//=============================================================================
MMP_UINT32
ithCapWaitEngineIdle(
    CAPTURE_HANDLE *ptDev)
{
    ithCap_Set_UnFire(ptDev->cap_id);
    
    return ithCap_Get_WaitEngineIdle(ptDev->cap_id);
}

//=============================================================================
/**
 * ithCapEngineReset
 */
//=============================================================================
void
ithCapEngineReset(
    void)
{
    ithCap_Set_Reset();
}

//=============================================================================
/**
 * ithCapGetDetectedWidth
 */
//=============================================================================
MMP_UINT32
ithCapGetDetectedWidth(
    CAPTURE_HANDLE *ptDev)
{
    return ithCap_Get_Detected_Width(ptDev->cap_id);
}

//=============================================================================
/**
 * ithCapGetDetectedHeight
 */
//=============================================================================
MMP_UINT32
ithCapGetDetectedHeight(
    CAPTURE_HANDLE *ptDev)
{
    return ithCap_Get_Detected_Height(ptDev->cap_id);
}

//=============================================================================
/**
 * ithCapGetDetectedInterleave
 */
//=============================================================================
MMP_UINT32
ithCapGetDetectedInterleave(
    CAPTURE_HANDLE *ptDev)
{
    return ithCap_Get_Detected_Interleave(ptDev->cap_id);
}

//=============================================================================
/**
 * ithCapSetPolarity
 */
//=============================================================================
void
ithCapSetPolarity(
    CAPTURE_HANDLE *ptDev,
    MMP_UINT16   HPolarity,
    MMP_UINT16   VPolarity)
{

    /* Set Hsync & Vsync Porlarity */
    ithCap_Set_Hsync_Polarity(ptDev->cap_id, HPolarity);
    ithCap_Set_Vsync_Polarity(ptDev->cap_id, VPolarity);
}

//=============================================================================
/**
 * ithCapSetInterleave
 */
//=============================================================================
void
ithCapSetInterleave(
    CAPTURE_HANDLE *ptDev, MMP_UINT32 interleave)
{
    ithCap_Set_Interleave(ptDev->cap_id, interleave);
}

//=============================================================================
/**
 * ithCapSetWidthHeight
 */
//=============================================================================
void
ithCapSetWidthHeight(
    CAPTURE_HANDLE *ptDev, MMP_UINT32 width, MMP_UINT32 height)
{
    ithCap_Set_Width_Height(ptDev->cap_id, width, height);
}

//=============================================================================
/**
 * ithCapSetROIWidthHeight
 */
//=============================================================================
void
ithCapSetROIWidthHeight(
    CAPTURE_HANDLE *ptDev, MMP_UINT32 width, MMP_UINT32 height)
{
    ithCap_Set_ROI_Width_Height(ptDev->cap_id, width, height);
}

