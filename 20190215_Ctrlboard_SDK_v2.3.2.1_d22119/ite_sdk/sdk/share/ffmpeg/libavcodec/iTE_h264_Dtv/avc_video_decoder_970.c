/*
 * Copyright (c) 2010 ITE Technology Corp. All Rights Reserved.
 */
/** @file avc_video_decoder.c
 *
 * @author
 */

//=============================================================================
//                              Include Files
//=============================================================================
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core_config.h"
#include "ite/ith_video.h" //#include "mmp_video.h"
#include "isp/mmp_isp.h"
#include "video_decoder.h"
#include "avc_video_decoder.h"
#include "bitstream_kit.h"
#include "queue_mgr.h"
#include <math.h>
#include "avc.h"

#include "h264decapi.h"
#include "dwl.h"
#include "dwlthread.h"

/*
 * include files are needed for hook to ffmpeg decoder
 */
#include "libavcodec/avcodec.h"
#include "libavcodec/h264.h"
#include "libavutil/opt.h"
#include "fc_external.h"

//=============================================================================
//                              Constant Definition
//=============================================================================

//#define REDUCE_VIDEO_BUFFER

//=============================================================================
//                              Macro Definition
//=============================================================================

//=============================================================================
//                              Structure Definition
//=============================================================================
typedef struct AVC_FRAME_BUFFER_CONTROL_TAG
{
    MMP_UINT8  decodeBufSelect;
#ifdef SKIP_BROKEN_VIDEO_FRAME
    MMP_UINT8  fireBufSelect;
#endif

    MMP_UINT32 displayOrderQueue;

    MMP_UINT32 accumTime;
    MMP_UINT32 timeStamp[8];
    MMP_UINT32 frame_rate_code;
    MMP_BOOL   bUpdateTimeStamp;
    MMP_UINT8  DecodingQueue[8];
    MMP_INT32  FramePocQueue[8];
    MMP_UINT8  structure[8];

    MMP_UINT32 preVideoTimeStamp;
} AVC_FRAME_BUFFER_CONTROL;

typedef struct AVC_FRAME_PTS_INTERVAL_TAG
{
    MMP_UINT32 baseInterval;
    MMP_UINT32 remainder;
} AVC_FRAME_PTS_INTERVAL;

typedef struct DISPLAY_INFO_TAG
{
    MMP_UINT32 aspect_ratio_information;
    MMP_UINT32 active_format;
    MMP_UINT32 horizontal_size;
    MMP_UINT32 vertical_size;
} DISPLAY_INFO;

//=============================================================================
//                              Global Data Definition
//=============================================================================3

// ISO/IEC 13818-2 table 6-4 - frame_rate_value
// ms unit, for example, 41.667 ms frame interval for 24 fps
static AVC_FRAME_PTS_INTERVAL gFramePTS_Interval[12] =
{
    {  0,   0}, // 0: forbidden
    { 41, 667}, // 1: 23.976 fps
    { 41, 667}, // 2: 24 fps
    { 40,   0}, // 3: 25 fps
    { 33, 333}, // 4: 29.970 fps
    { 33, 333}, // 5: 30 fps
    { 20,   0}, // 6: 50 fps
    { 16, 667}, // 7: 60 fps
    { 16, 667}, // 8: 60 fps
    { 66, 667}, // 9: 14.985 fps
    { 66, 667}, //10: 15 fps
    {100,   0} //11: 10 fps
};




AVFrame     *pict;
H264DecInst decInst;
H264DecInfo decInfo;
uint32_t picDecodeNumber;
uint32_t picTimeStamp;
static uint32_t gp_video_buf_vram_addr  = 0;
static uint8_t* gp_video_buf_sys_addr   = NULL;
bool gbPictureGet;

//=============================================================================
//                              Private Function Declaration
//=============================================================================
static void
_VIDEO_DECODER_ReleaseValue(
    void);


//=============================================================================
//                              Public Function Definition
//=============================================================================




//=============================================================================
//                              Private Function Definition
//=============================================================================
unsigned int PalGetClock(void)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        printf("gettimeofday failed!\n");
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

unsigned long PalGetDuration(unsigned int clock)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) != 0)
        printf("gettimeofday failed!\n");
    return (unsigned int)(tv.tv_sec * 1000 + tv.tv_usec / 1000) - clock;
}

int decode_nal_units(H264Context *h, const MMP_UINT8 *buf, int buf_size)
{    
    H264DecRet ret, tmp;
    H264DecInput decInput;
    H264DecOutput decOutput;
    H264DecPicture decPicture;
    int result = 0;    		
	
    decInput.skipNonReference = 0;

    //printf("Data %x %x %x %x: len %d\n", *(buf + h->nal_length_size), *(buf + h->nal_length_size + 1), *(buf + h->nal_length_size + 2), *(buf + h->nal_length_size + 3), buf_size - h->nal_length_size);
    //gp_video_buf_sys_addr[0] = 0x00;
    //gp_video_buf_sys_addr[1] = 0x00;
    //gp_video_buf_sys_addr[2] = 0x00;
    //gp_video_buf_sys_addr[3] = 0x01;
    
    //if (*(buf + h->nal_length_size) == 0x6)
    //	return;
    memcpy(gp_video_buf_sys_addr, buf+h->nal_length_size, buf_size-h->nal_length_size);
      
    //printf("Data %x %x %x %x: Addr %x\n", *(gp_video_buf_sys_addr), *(gp_video_buf_sys_addr + 1), *(gp_video_buf_sys_addr + 2), *(gp_video_buf_sys_addr + 3), gp_video_buf_sys_addr);
        
#if defined(CFG_CPU_WB) && (!defined(WIN32))
    ithFlushDCacheRange((void*)gp_video_buf_sys_addr, buf_size); //-h->nal_length_size);
    ithFlushMemBuffer();
#endif
    // test error stream
    //if (picDecodeNumber == 10)
    //{
    //	*(gp_video_buf_sys_addr+20) = 0x00;
    //	*(gp_video_buf_sys_addr+21) = 0xff;		
    //}
	
    decInput.pStream = gp_video_buf_sys_addr;
    decInput.streamBusAddress = gp_video_buf_vram_addr;
    decInput.dataLen = buf_size - h->nal_length_size;
    
#ifdef _WIN32    
    ithUnmapVram(decInput.pStream, decInput.dataLen); //+4); //?
#endif    
    
    do
    {
        decInput.picId = picDecodeNumber;
        //*data_size = 0;
        gbPictureGet = false;
        
        ret = H264DecDecode(decInst, &decInput, &decOutput);
        //printf("De (%x)  Size %d\n", ret, decOutput.dataLeft);
	
        switch (ret)
        {
        case H264DEC_HDRS_RDY:
            tmp = H264DecGetInfo(decInst, &decInfo);

            if (tmp != H264DEC_OK)
            {
                printf("ERROR in getting stream info!\n");
                goto end;
            }

            printf("Width %d Height %d\n", decInfo.picWidth, decInfo.picHeight);
            printf("MonoChrome = %d\n", decInfo.monoChrome);
            printf("Interlaced = %d\n", decInfo.interlacedSequence);
            printf("DPB mode   = %d\n", decInfo.dpbMode);
            printf("Pictures in DPB = %d\n", decInfo.picBuffSize);
            printf("Pictures in Multibuffer PP = %d\n", decInfo.multiBuffPpSize);
            if (decInfo.outputFormat == H264DEC_TILED_YUV420)
                printf("Output format = H264DEC_TILED_YUV420\n");
            else if (decInfo.outputFormat == H264DEC_YUV400)
                printf("Output format = H264DEC_YUV400\n");
            else
                printf("Output format = H264DEC_SEMIPLANAR_YUV420\n");
            break;
        case H264DEC_PIC_DECODED:
            if (H264DecNextPicture(decInst, &decPicture, 0) == H264DEC_PIC_RDY)
            {
                //*data_size = 0;

                pict->width = decInfo.picWidth;
                pict->height = decInfo.picHeight;

                pict->data[0] = (uint8_t*)decPicture.outputPictureBusAddress; // pOutputPicture;
                pict->data[1] = (uint8_t*)(pict->data[0] + pict->width * pict->height);
                pict->data[2] = (uint8_t*)(pict->data[0] + pict->width * pict->height);
                pict->data[3] = (uint8_t*)(picDecodeNumber % 3); // 0; //buf index?

                pict->linesize[0] = decInfo.picWidth;
                pict->linesize[1] = decInfo.picWidth;

                // tmp solution, need to modify!
                pict->pts = picTimeStamp; //how to get the time stamp?

                //*data_size                              = sizeof(AVFrame);
                result = 1;
                gbPictureGet = true;
                //printf("%d %d PicType %d %d : %d %x\n", gbPictureGet, decOutput.dataLeft, decPicture.picCodingType[0], decPicture.picCodingType[1], decPicture.picId, decPicture.outputPictureBusAddress);               
            }

            //printf("PIC Decoding OK\n");
            picDecodeNumber++;
            picTimeStamp += 33;
            break;

        case H264DEC_STREAM_NOT_SUPPORTED:
            printf("ERROR in getting stream info!\n");
            goto end;

        case H264DEC_ADVANCED_TOOLS:
            printf("ERROR stream is using advanced tool!\n");
            goto end;

        case H264DEC_STRM_PROCESSED:
            if(result == 1) gbPictureGet = true;
            break;
        case H264DEC_NONREF_PIC_SKIPPED:
        case H264DEC_STRM_ERROR:
        case H264DEC_OK:
            /* nothing to do, just call again */
            break;

        case H264DEC_HW_TIMEOUT:
            printf("Timeout\n");
            goto end;
        default:
            printf("FATAL ERROR: %d\n", ret);
            goto end;
        }

        if (decOutput.dataLeft > 1)
        {
            decInput.dataLen = decOutput.dataLeft;
            decInput.pStream = decOutput.pStrmCurrPos;
            decInput.streamBusAddress = decOutput.strmCurrBusAddress;            
        }
    } while (decOutput.dataLeft > 1);
    //gbPictureGet                              = true;
end :    
    return 1;
}

int _DecodeExtradata(H264Context *h, const MMP_UINT8 *buf, int size)
{
    AVCodecContext *avctx = h->avctx;
    
    if (!buf || size <= 0)
        return -1;

    if (buf[0] == 1)
    {
        int                 i, cnt, nalsize;
        const unsigned char *p = buf;

        h->is_avc                        = 1;

        if (size < 7)
        {
            av_log(avctx, AV_LOG_ERROR, "avcC too short\n");
            return -1;
        }
        /* sps and pps in the avcC always have length coded with 2 bytes,
           so put a fake nal_length_size = 2 while parsing them */
        h->nal_length_size                = 2;
        //gptAVCVideoDecoder->NALUnitLength = 2;
        // Decode sps from avcC
        cnt                               = *(p + 5) & 0x1f; // Number of sps
        p                                += 6;
        for (i = 0; i < cnt; i++)
        {
            nalsize = AV_RB16(p) + 2;
            if (nalsize > size - (p - buf))
                return -1;
            if (decode_nal_units(h, p, nalsize) < 0)
            {
                av_log(avctx, AV_LOG_ERROR, "Decoding sps %d from avcC failed\n", i);
                return -1;
            }
            p += nalsize;
        }
        // Decode pps from avcC
        cnt = *(p++); // Number of pps
        for (i = 0; i < cnt; i++)
        {
            nalsize = AV_RB16(p) + 2;
            if (nalsize > size - (p - buf))
                return -1;
            if (decode_nal_units(h, p, nalsize) < 0)
            {
                av_log(avctx, AV_LOG_ERROR, "Decoding pps %d from avcC failed\n", i);
                return -1;
            }
            p += nalsize;
        }
        // Now store right nal length size, that will be use to parse all other nals
        h->nal_length_size                = (buf[4] & 0x03) + 1;
        //gptAVCVideoDecoder->NALUnitLength = h->nal_length_size;        
    }
    else
    {
        h->is_avc                        = 0;
        //gptAVCVideoDecoder->bIsNalFormat = 0;        
        if (decode_nal_units(h, buf, size) < 0)
            return -1;
    }
    return 0;
}

int iTE_h264_Dtv_decode_init(AVCodecContext *avctx)
{
    H264Context           *h = avctx->priv_data;
    //MpegEncContext *const s  = &h->s;

    //s->avctx = avctx;
    h->avctx = avctx;
#ifdef DEBUG
    //av_log_set_level(AV_LOG_DEBUG);
#endif

    //av_log(avctx, AV_LOG_INFO, "init\n");
    printf("Decode Init\n");
    //AVC_Video_Decoder_Init(avctx);

    //printf("Extra %d\n", avctx->extradata_size);
    //if (avctx->extradata_size > 0 && avctx->extradata &&
    //    AVC_Video_Decoder_DecodeExtradata(h, avctx->extradata, avctx->extradata_size))
    //    return -1;
    
    picDecodeNumber = 0;
    picTimeStamp = 0;

#ifdef REDUCE_VIDEO_BUFFER
    if (H264DecInit(&decInst, 0, 0, 0, DEC_REF_FRM_RASTER_SCAN)!= H264DEC_OK)
#else    
    if (H264DecInit(&decInst, 0, 0, 1, DEC_REF_FRM_RASTER_SCAN)!= H264DEC_OK)
#endif    	
    	return -1;
    
    if (!gp_video_buf_sys_addr)
    {
        gp_video_buf_vram_addr = itpVmemAlignedAlloc(8, 1*1024*1024);
        gp_video_buf_sys_addr = (uint8_t*) ithMapVram(gp_video_buf_vram_addr, 1*1024*1024, /*ITH_VRAM_READ |*/ ITH_VRAM_WRITE);        
    }
    
    h->nal_length_size = 0;
    
    if (avctx->extradata_size > 0 && avctx->extradata &&
        _DecodeExtradata(h, avctx->extradata, avctx->extradata_size))
        return -1;        
    
    return 0;
}

int iTE_h264_Dtv_decode_end(AVCodecContext *avctx)
{
    av_log(avctx, AV_LOG_INFO, "decode end\n");
    //AVC_Video_Decoder_End();
    printf("Decode End\n");
    if (gp_video_buf_sys_addr)
    {
        itpVmemFree(gp_video_buf_vram_addr);
        gp_video_buf_sys_addr  = NULL;
        gp_video_buf_vram_addr = 0;
    }
    
    H264DecRelease(decInst);
    return 0;
}

int iTE_h264_Dtv_decode_frame(AVCodecContext *avctx, void *data, int *data_size, AVPacket *avpkt)
{
    MMP_UINT8       *buf           = avpkt->data;
    int             buf_size       = avpkt->size;
    H264Context     *h             = avctx->priv_data;
    H264DecRet      ret, tmp;
    H264DecInput    decInput;
    H264DecOutput   decOutput;
    H264DecPicture  decPicture;
    int             result = 0;
    int             remainsize, len, offset;    
    pict          = data;
    
    *data_size = 0;
    //printf("VideoData %x %x %x %x %x %x %x %x %x\n", *buf, *(buf + 1), *(buf + 2), *(buf + 3), *(buf + 4), *(buf + 5), *(buf + 6), *(buf + 7), *(buf + 8));
    //printf("Len %x || D %x %x\n", avpkt->size-4, *(buf + 2), *(buf +3));
	
    //change stream to byte stream format
    offset = 0;
    remainsize = avpkt->size;
    do
    {
        //printf("VideoData2 %x %x %x %x %x %x %x %x %x\n", *(buf+offset), *(buf+offset + 1), *(buf+offset + 2), *(buf +offset+ 3), *(buf +offset+ 4), *(buf+offset + 5), *(buf+offset + 6), *(buf+offset + 7), *(buf+offset + 8));
        
        //check NAL or ByteStream format ?
        if (h->nal_length_size != 0)
        {
        	if (h->nal_length_size == 1)
        		len = buf[offset] + 1;
        		
        	if (h->nal_length_size == 2)
        		len =(buf[offset] << 8 | buf[offset + 1]) + 2;
        		
            if (h->nal_length_size == 3)
        		len = (buf[offset] << 16 | buf[offset + 1] << 8 | buf[offset + 2]) + 3;
        		
        	if (h->nal_length_size == 4)
        		len = (buf[offset] << 24 | buf[offset + 1] << 16 | buf[offset + 2] << 8 | buf[offset + 3]) + 4;        			
        		
        } else {
        	//0x00000001 or 0x000001 + NAL
            len = avpkt->size;        	
        }         
          		    
        //buf[offset] = 0x00;
        //buf[offset+1] = 0x00;
        //buf[offset+2] = 0x00;
        //buf[offset+3] = 0x01;
        //printf("R %d L %d O %d\n", remainsize, len, offset);
        //printf("Len %d : %x \n", len, buf[offset + 4]);
                  
        decode_nal_units(h, buf+offset, len);
        
        remainsize -= len;
        offset += len;		    	
    } while (remainsize != 0);

    if (gbPictureGet)
        *data_size = sizeof(AVFrame);        
    
    return (avpkt->size);
}

void iTE_h264_Dtv_decode_flush(AVCodecContext *avctx)
{
    av_log(avctx, AV_LOG_INFO, "flush dpb\n");
    //flush_dpb();
}

static const AVProfile profiles[] = {
    { FF_PROFILE_H264_BASELINE,             "Baseline"                              },
    { FF_PROFILE_H264_CONSTRAINED_BASELINE, "Constrained Baseline"                  },
    { FF_PROFILE_H264_MAIN,                 "Main"                                  },
    { FF_PROFILE_H264_EXTENDED,             "Extended"                              },
    { FF_PROFILE_H264_HIGH,                 "High"                                  },
    { FF_PROFILE_H264_HIGH_10,              "High 10"                               },
    { FF_PROFILE_H264_HIGH_10_INTRA,        "High 10 Intra"                         },
    { FF_PROFILE_H264_HIGH_422,             "High 4:2:2"                            },
    { FF_PROFILE_H264_HIGH_422_INTRA,       "High 4:2:2 Intra"                      },
    { FF_PROFILE_H264_HIGH_444,             "High 4:4:4"                            },
    { FF_PROFILE_H264_HIGH_444_PREDICTIVE,  "High 4:4:4 Predictive"                 },
    { FF_PROFILE_H264_HIGH_444_INTRA,       "High 4:4:4 Intra"                      },
    { FF_PROFILE_H264_CAVLC_444,            "CAVLC 4:4:4"                           },
    { FF_PROFILE_UNKNOWN },
};

static const AVOption h264_options[] = {
#if defined(WIN32)
    {"is_avc",          "is avc",          offsetof(H264Context, is_avc),          FF_OPT_TYPE_INT, { 0}, 0, 1, 0},
    {"nal_length_size", "nal_length_size", offsetof(H264Context, nal_length_size), FF_OPT_TYPE_INT, { 0}, 0, 4, 0},
#else
    {"is_avc",          "is avc",          offsetof(H264Context, is_avc),          FF_OPT_TYPE_INT, {.dbl = 0}, 0, 1, 0},
    {"nal_length_size", "nal_length_size", offsetof(H264Context, nal_length_size), FF_OPT_TYPE_INT, {.dbl = 0}, 0, 4, 0},
#endif
    {NULL}
};

static const AVClass h264_class = {
    "H264 Decoder",
    av_default_item_name,
    h264_options,
    LIBAVUTIL_VERSION_INT,
};

#if CONFIG_ITE_H264_DTV_DECODER
    #if defined(_MSC_VER)
AVCodec ff_iTE_h264_Dtv_decoder = {
    "iTE_h264_Dtv",
    AVMEDIA_TYPE_VIDEO,
    CODEC_ID_H264,
    sizeof(H264Context),
    iTE_h264_Dtv_decode_init,
    NULL,
    iTE_h264_Dtv_decode_end,
    iTE_h264_Dtv_decode_frame,
    /*CODEC_CAP_DRAW_HORIZ_BAND |*/ CODEC_CAP_DR1 | CODEC_CAP_DELAY |
    CODEC_CAP_SLICE_THREADS | CODEC_CAP_FRAME_THREADS,
    NULL,
    iTE_h264_Dtv_decode_flush /*flush_dpb*/,
    NULL,
    NULL,
    "H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10",
    NULL,
    NULL,
    NULL,
    0,
    &h264_class/*,
                  NULL_IF_CONFIG_SMALL(profiles),
                  ONLY_IF_THREADS_ENABLED(decode_init_thread_copy),
                  ONLY_IF_THREADS_ENABLED(decode_update_thread_context),*/
};
    #else // !defined (_MSC_VER)
AVCodec ff_iTE_h264_Dtv_decoder = {
    .name           = "iTE_h264_Dtv",
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = CODEC_ID_H264,
    .priv_data_size = sizeof(H264Context),
    .init           = iTE_h264_Dtv_decode_init,
    .close          = iTE_h264_Dtv_decode_end,
    .decode         = iTE_h264_Dtv_decode_frame,
    .capabilities   = /*CODEC_CAP_DRAW_HORIZ_BAND |*/ CODEC_CAP_DR1 | CODEC_CAP_DELAY |
                      CODEC_CAP_SLICE_THREADS | CODEC_CAP_FRAME_THREADS,
    .flush          = iTE_h264_Dtv_decode_flush,
    .long_name      = "H.264 / AVC / MPEG-4 AVC / MPEG-4 part 10",
    .priv_class     = &h264_class,
};
    #endif // _MSC_VER
#endif     // CONFIG_ITE_H264_DTV_DECODER