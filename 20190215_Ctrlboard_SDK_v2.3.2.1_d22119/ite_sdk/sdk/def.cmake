# System
set(CFG_BUILD_ITH 1)
set(CFG_BUILD_ITP 1)

if (CFG_DEV_TEST)
    set(CFG_BUILD_GMOCK y)
    add_definitions(
        -DCFG_DEV_TEST
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DOORBELL_INDOOR)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DOORBELL_LOBBY)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DOORBELL_OUTDOOR)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DOORBELL_ADMIN)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AIRCONDITIONER)

if (CFG_BUILD_MONGOOSE)
    add_definitions(
        -DCS_PLATFORM=0
        -DMG_LOCALS
    )
endif()

if (CFG_BUILD_PILLOWTALK)
    set(CFG_BUILD_YAJL y)
    set(CFG_NET_HTTP y)
endif()

if (CFG_BUILD_OPUS)
    add_definitions(
        -DCFG_BUILD_OPUS
    )
endif()

if (CFG_BUILD_LEAF)
    add_definitions(
        -DCFG_LEAF_ENABLE
    )
endif()

if (DEFINED CFG_CHIP_REV_A0)
    add_definitions(
        -DCFG_CHIP_REV_A0
    )
elseif (DEFINED CFG_CHIP_REV_A1)
    add_definitions(
        -DCFG_CHIP_REV_A1
    )
endif()

if (DEFINED CFG_CHIP_PKG_IT9072)
    add_definitions(
        -DCFG_CHIP_PKG_IT9072
        -DCFG_OSC_CLK=30000000
        -DCFG_RTC_EXTCLK=30000000
    )
    set(CFG_CHIP_FAMILY 9070)

elseif (DEFINED CFG_CHIP_PKG_IT9076)
    add_definitions(
        -DCFG_CHIP_PKG_IT9076
        -DCFG_OSC_CLK=30000000
        -DCFG_RTC_EXTCLK=30000000
    )
    set(CFG_CHIP_FAMILY 9070)

elseif (DEFINED CFG_CHIP_PKG_IT9078)
    add_definitions(
        -DCFG_CHIP_PKG_IT9078
        -DCFG_OSC_CLK=30000000
        -DCFG_RTC_EXTCLK=32768
    )
    set(CFG_CHIP_FAMILY 9070)

elseif (DEFINED CFG_CHIP_PKG_IT9079)
    add_definitions(
        -DCFG_CHIP_PKG_IT9079
        -DCFG_OSC_CLK=30000000
        -DCFG_RTC_EXTCLK=30000000
    )
    set(CFG_CHIP_FAMILY 9070)

elseif (DEFINED CFG_CHIP_PKG_IT9079H)
    add_definitions(
        -DCFG_CHIP_PKG_IT9079H
        -DCFG_OSC_CLK=30000000
        -DCFG_RTC_EXTCLK=30000000
    )
    set(CFG_CHIP_FAMILY 9070)

elseif (DEFINED CFG_CHIP_PKG_IT9852)
    add_definitions(
        -DCFG_CHIP_PKG_IT9852
        -DCFG_OSC_CLK=12000000
        -DCFG_RTC_EXTCLK=12000000
    )
    set(CFG_CHIP_FAMILY 9850)

elseif (DEFINED CFG_CHIP_PKG_IT9854)
    add_definitions(
        -DCFG_CHIP_PKG_IT9854
        -DCFG_OSC_CLK=12000000
        -DCFG_RTC_EXTCLK=12000000
    )
    set(CFG_CHIP_FAMILY 9850)

elseif (DEFINED CFG_CHIP_PKG_IT9856)
    add_definitions(
        -DCFG_CHIP_PKG_IT9856
        -DCFG_OSC_CLK=12000000
        -DCFG_RTC_EXTCLK=12000000
    )
    set(CFG_CHIP_FAMILY 9850)

elseif (DEFINED CFG_CHIP_PKG_IT9910)
    add_definitions(
        -DCFG_CHIP_PKG_IT9910
        -DCFG_OSC_CLK=12000000
        -DCFG_RTC_EXTCLK=12000000
    )
    set(CFG_CHIP_FAMILY 9910)

elseif (DEFINED CFG_CHIP_PKG_IT976)
    add_definitions(
        -DCFG_CHIP_PKG_IT976
        -DCFG_OSC_CLK=12000000
        -DCFG_RTC_EXTCLK=32768
        -DCFG_AXI_DMA
    )
    set(CFG_CHIP_FAMILY 970)

elseif (DEFINED CFG_CHIP_PKG_IT977)
    add_definitions(
        -DCFG_CHIP_PKG_IT977
        -DCFG_OSC_CLK=12000000
        -DCFG_RTC_EXTCLK=32768
        -DCFG_AXI_DMA
    )
    set(CFG_CHIP_FAMILY 970)

elseif (DEFINED CFG_CHIP_PKG_IT972)
    add_definitions(
        -DCFG_CHIP_PKG_IT972
        -DCFG_OSC_CLK=12000000
        -DCFG_RTC_EXTCLK=32768
        -DCFG_AXI_DMA
    )
    set(CFG_CHIP_FAMILY 970)

elseif (DEFINED CFG_CHIP_PKG_IT960)
    add_definitions(
        -DCFG_CHIP_PKG_IT960
        -DCFG_OSC_CLK=12000000
        -DCFG_RTC_EXTCLK=12000000
        -DCFG_AXI_DMA
    )
    set(CFG_CHIP_FAMILY 960)

endif()

if (DEFINED CFG_CPU_SM32)
    if (DEFINED CFG_CPU_BIG_ENDIAN)
        set(CFG_GLAMOMEM_BIG_ENDIAN 1)
    else()
        set(CFG_GLAMOMEM_BIG_ENDIAN 0)
    endif()
    set(CFG_CPU_NAME sm32)
elseif (DEFINED CFG_CPU_RISCV)
    set(CFG_GLAMOMEM_BIG_ENDIAN 0)
    set(CFG_CPU_NAME riscv)
else()
    set(CFG_GLAMOMEM_BIG_ENDIAN 0)
    set(CFG_CPU_NAME fa626)
endif()

add_definitions(
    -DCFG_CHIP_FAMILY=${CFG_CHIP_FAMILY}
)

add_definitions(
    -DCFG_RAM_SIZE=${CFG_RAM_SIZE}
)

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_MEMDBG_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DCPS_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DPU_ENABLE)

if (DEFINED CFG_SPREAD_SPECTRUM_PLL1_ENABLE)
    add_definitions(
        -DCFG_SPREAD_SPECTRUM_PLL1_ENABLE
        -DCFG_SPREAD_SPECTRUM_PLL1_MODE=${CFG_SPREAD_SPECTRUM_PLL1_MODE}
        -DCFG_SPREAD_SPECTRUM_PLL1_WIDTH=${CFG_SPREAD_SPECTRUM_PLL1_WIDTH}
        -DCFG_SPREAD_SPECTRUM_PLL1_FREQ=${CFG_SPREAD_SPECTRUM_PLL1_FREQ}
    )
endif()

if (DEFINED CFG_SPREAD_SPECTRUM_PLL2_ENABLE)
    add_definitions(
        -DCFG_SPREAD_SPECTRUM_PLL2_ENABLE
        -DCFG_SPREAD_SPECTRUM_PLL2_MODE=${CFG_SPREAD_SPECTRUM_PLL2_MODE}
        -DCFG_SPREAD_SPECTRUM_PLL2_WIDTH=${CFG_SPREAD_SPECTRUM_PLL2_WIDTH}
        -DCFG_SPREAD_SPECTRUM_PLL2_FREQ=${CFG_SPREAD_SPECTRUM_PLL2_FREQ}
    )
endif()

if (DEFINED CFG_SPREAD_SPECTRUM_PLL3_ENABLE)
    add_definitions(
        -DCFG_SPREAD_SPECTRUM_PLL3_ENABLE
        -DCFG_SPREAD_SPECTRUM_PLL3_MODE=${CFG_SPREAD_SPECTRUM_PLL3_MODE}
        -DCFG_SPREAD_SPECTRUM_PLL3_WIDTH=${CFG_SPREAD_SPECTRUM_PLL3_WIDTH}
        -DCFG_SPREAD_SPECTRUM_PLL3_FREQ=${CFG_SPREAD_SPECTRUM_PLL3_FREQ}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_CPU_WB)

#if (DEFINED CFG_CPU_WRITE_BUFFER)
#    add_definitions(
#        -DCFG_CPU_WRITE_BUFFER
#    )
#endif()

if (DEFINED CFG_CPU_WB AND (DEFINED CFG_USB0_ENABLE OR DEFINED CFG_USB1_ENABLE OR DEFINED CFG_NET_ETHERNET))
    add_definitions(
        -DCFG_WT_SIZE=0x200000
    )
else()
    add_definitions(
        -DCFG_WT_SIZE=0
    )
endif()

if (DEFINED CFG_XCPU_MSGQ)
    add_definitions(
        -DCFG_XCPU_MSGQ
        -DCFG_XCPU_MSGQ_SIZE=${CFG_XCPU_MSGQ_SIZE}
    )
    set(CFG_BUILD_XCPU_MASTER 1)
endif()

add_definitions(
    -DCFG_MMAP_SIZE=${CFG_MMAP_SIZE}
)

if (CFG_BUILD_MEDIASTREAMER2)
    set(CFG_BUILD_MATROSKA y)
    set(CFG_BUILD_EBML y)
    set(CFG_BUILD_COREC y)
    set(CFG_BUILD_AUDIO_PREPROCESS y)

    #temp solution
    if (CFG_CHIP_FAMILY STREQUAL 9910)
        set(CFG_BUILD_SENSOR 1)
        set(CFG_BUILD_CAPTURE 1)
        set(CFG_BUILD_VP 1)
        set(CFG_BUILD_VIDEO_ENCODER 1)
        set(CFG_BUILD_ENCODER 1)
    endif()

    if (CFG_CHIP_FAMILY STREQUAL 970)
        set(CFG_BUILD_ISP 1)
        if(NOT DEFINED CFG_DOORBELL_INDOOR AND NOT DEFINED CFG_DOORBELL_ADMIN)
            set(CFG_BUILD_SENSOR 1)
            set(CFG_BUILD_CAPTURE 1)
            set(CFG_BUILD_H264_ENCODER 1)
            set(CFG_BUILD_VIDEO_ENCODER 1)
        endif()
    endif()

    if (DEFINED CFG_CHIP_PKG_IT9079)
        set(CFG_BUILD_VIDEO_ENCODER 1)
    endif()

    if (DEFINED CFG_CHIP_PKG_IT9856)
        set(CFG_BUILD_VIDEO_ENCODER 1)
    endif()
endif()

# Screen
if (DEFINED CFG_LCD_ENABLE)
    add_definitions(
        -DCFG_LCD_ENABLE
        -DCFG_LCD_WIDTH=${CFG_LCD_WIDTH}
        -DCFG_LCD_HEIGHT=${CFG_LCD_HEIGHT}
        -DCFG_LCD_PITCH=${CFG_LCD_PITCH}
        -DCFG_LCD_BPP=${CFG_LCD_BPP}
    )
else()
    add_definitions(
        -DCFG_LCD_WIDTH=0
        -DCFG_LCD_HEIGHT=0
        -DCFG_LCD_PITCH=0
        -DCFG_LCD_BPP=0
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_LCD_MULTIPLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_LCD_TRIPLE_BUFFER)

if (DEFINED CFG_HDMI_OUT_ENABLE)
    add_definitions(
        -DCFG_HDMI_OUT_ENABLE
    )
    set(CFG_BUILD_HDMITX 1)
endif()

if (DEFINED CFG_LCD_INIT_ON_BOOTING)
    add_definitions(
        -DCFG_LCD_INIT_ON_BOOTING
        -DCFG_LCD_BOOT_BGCOLOR=${CFG_LCD_BOOT_BGCOLOR}
    )
endif()

if (DEFINED CFG_BACKLIGHT_ENABLE)
    add_definitions(
        -DCFG_BACKLIGHT_ENABLE
        -DCFG_BACKLIGHT_FREQ=${CFG_BACKLIGHT_FREQ}
        -DCFG_BACKLIGHT_DUTY_CYCLES=${CFG_BACKLIGHT_DUTY_CYCLES}
        -DCFG_BACKLIGHT_DEFAULT_DUTY_CYCLE=${CFG_BACKLIGHT_DEFAULT_DUTY_CYCLE}
        -DCFG_BACKLIGHT_BOOTLODER_DELAY=${CFG_BACKLIGHT_BOOTLODER_DELAY}
    )
endif()

if (DEFINED CFG_TVOUT_ENABLE)
    add_definitions(
        -DCFG_TVOUT_ENABLE
    )
    set(CFG_BUILD_ISP 1)

    if (DEFINED CFG_TVOUT_HDMI)
        add_definitions(
            -DCFG_TVOUT_HDMI
        )
        set(CFG_BUILD_HDMI 1)
    endif()

endif()

if (DEFINED CFG_BL_SHOW_LOGO)
    add_definitions(
        -DCFG_BL_SHOW_LOGO
        -DCFG_LCD_BOOT_BGCOLOR=${CFG_LCD_BOOT_BGCOLOR}
    )
endif()

# Graphics
if (DEFINED CFG_CMDQ_ENABLE)
    add_definitions(
        -DCFG_CMDQ_ENABLE
        -DCFG_CMDQ_SIZE=${CFG_CMDQ_SIZE}
    )
else()
    add_definitions(
        -DCFG_CMDQ_SIZE=0
    )
endif()

if (DEFINED CFG_M2D_ENABLE)
    add_definitions(
        -DCFG_M2D_ENABLE
    )
    set(CFG_BUILD_M2D 1)
endif()

if (DEFINED CFG_JPEG_HW_ENABLE)
    add_definitions(
        -DCFG_JPEG_HW_ENABLE
    )
    set(CFG_BUILD_ISP 1)
    set(CFG_BUILD_JPG 1)
    set(CFG_BUILD_ASYNC_FILE 1)

    if (DEFINED CFG_PLAY_MJPEG_ON_BOOTING)
        add_definitions(
            -DCFG_PLAY_MJPEG_ON_BOOTING
            -DCFG_BOOT_MJPEG_FILE=${CFG_BOOT_MJPEG_FILE}
            -DCFG_BOOT_MJPEG_FILE_NAME="${CFG_BOOT_MJPEG_FILE}"
            -DCFG_BOOT_VIDEO_BGCOLOR=${CFG_BOOT_VIDEO_BGCOLOR}
            -DCFG_BOOT_VIDEO_VOLUME=${CFG_BOOT_VIDEO_VOLUME}
        )
    endif()
endif()

if (DEFINED CFG_TV_ENCODER_ENABLE)
    add_definitions(
        -DCFG_TV_ENCODER_ENABLE
    )
    set(CFG_BUILD_TV_ENCODER 1)
endif()

if (DEFINED CFG_UI_ENC_ENABLE)
    set(CFG_BUILD_UIENC 1)
endif()

if (DEFINED CFG_FONT_FILENAME)
    add_definitions(
        -DCFG_FONT_FILENAME="${CFG_FONT_FILENAME}"
    )
endif()

# Audio
if (DEFINED CFG_AUDIO_ENABLE)
    add_definitions(
        -DCFG_AUDIO_ENABLE
        -DAUDIO_PLUGIN_MESSAGE_QUEUE
        -DENABLE_CODECS_PLUGIN
        -DCONFIG_ITADRIVER
        -DCFG_RISC_ENABLE
    )

    if (DEFINED CFG_AUDIO_SAMPLING_RATE)
        add_definitions(
            -DCFG_AUDIO_SAMPLING_RATE=${CFG_AUDIO_SAMPLING_RATE}
        )
    else(DEFINED CFG_AUDIO_SAMPLING_RATE)
        add_definitions(
            -DCFG_AUDIO_SAMPLING_RATE=8000
        )
    endif()

    if (DEFINED CFG_AUDIO_ENGENEER_MODEL)
        add_definitions(
            -DENABLE_AUDIO_ENGENEER_MODEL
            -DENABLE_AUDIO_NOISE_GATE
            -DENABLE_AUDIO_SPK_EQUALIZER
            -DENABLE_AUDIO_MIC_EQUALIZER
            -DENABLE_AEC_ENABLE
        )
    endif()

    if (DEFINED CFG_AEC_ENABLE)
        add_definitions(
            -DENABLE_AEC_ENABLE
        )
    endif()

    if (DEFINED CFG_AEC_WITH_FILTER)
        add_definitions(
            -DENABLE_AEC_WITH_FILTER
        )
    endif()

    if (DEFINED CFG_AEC_DELAY_MS)
        add_definitions(
            -DCFG_AEC_DELAY_MS=${CFG_AEC_DELAY_MS}
        )
    else(DEFINED CFG_AEC_DELAY_MS)
        add_definitions(
            -DCFG_AEC_DELAY_MS=60
        )
    endif()

    if (DEFINED CFG_DUAL_MIC_ICA_SEPARATION)
        add_definitions(
            -DENABLE_DUAL_MIC_ICA_SEPARATION
        )
    endif()

    if (DEFINED CFG_AEC_WITH_DUAL_MIC)
        add_definitions(
            -DENABLE_AEC_WITH_DUAL_MIC
        )
    endif()

    if(DEFINED CFG_AEC_RUN_IN_ARM)
        add_definitions(
            -DAEC_RUN_IN_ARM
        )
    endif()

    if (DEFINED CFG_SPEAKER_GAIN)
        add_definitions(
            -DCFG_SPEAKER_GAIN=${CFG_SPEAKER_GAIN}
        )
    else(DEFINED CFG_SPEAKER_GAIN)
        add_definitions(
            -DCFG_SPEAKER_GAIN=0
        )
    endif()

    if(DEFINED CFG_ENABLE_SEND_FILTER)
        add_definitions(
            -DENABLE_SEND_FILTER
        )
    endif()

    if (DEFINED CFG_MIC_GAIN)
        add_definitions(
            -DCFG_MIC_GAIN=${CFG_MIC_GAIN}
        )
    else(DEFINED CFG_MIC_GAIN)
        add_definitions(
            -DCFG_MIC_GAIN=0
        )
    endif()

    if (DEFINED CFG_NOISE_GATE_ENABLE)
        add_definitions(
            -DENABLE_AUDIO_NOISE_GATE
        )
    endif()

    if (DEFINED CFG_NOISE_GATE_THRESHOLD)
        add_definitions(
            -DCFG_NOISE_GATE_THRESHOLD=${CFG_NOISE_GATE_THRESHOLD}
        )
    else(DEFINED CFG_NOISE_GATE_THRESHOLD)
        add_definitions(
            -DCFG_NOISE_GATE_THRESHOLD=0.3
        )
    endif()

    if (DEFINED CFG_SPK_EQUALIZER_ENABLE)
        add_definitions(
            -DENABLE_AUDIO_SPK_EQUALIZER
        )
    endif()

    if (DEFINED CFG_EQUALIZER_SPKGAIN_SET)
        add_definitions(
            -DCFG_EQUALIZER_SPKGAIN_SET=${CFG_EQUALIZER_SPKGAIN_SET}
        )
    else(DEFINED CFG_EQUALIZER_SPKGAIN_SET)
        add_definitions(
            -DCFG_EQUALIZER_SPKGAIN_SET=NULL
        )
    endif()

    if (DEFINED CFG_MIC_EQUALIZER_ENABLE)
        add_definitions(
            -DENABLE_AUDIO_MIC_EQUALIZER
        )
    endif()

    if (DEFINED CFG_EQUALIZER_MICGAIN_SET)
        add_definitions(
            -DCFG_EQUALIZER_MICGAIN_SET=${CFG_EQUALIZER_MICGAIN_SET}
        )
    else(DEFINED CFG_EQUALIZER_MICGAIN_SET)
        add_definitions(
            -DCFG_EQUALIZER_MICGAIN_SET=NULL
        )
    endif()

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_AACDEC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_AC3DEC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_AC3SPDIF)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_AMR)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_EAC3DEC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_MP3DEC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_MP2DEC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_WAV)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_WMADEC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_FLACDEC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_SBC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_G711_ALAW)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_G711_ULAW)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_OPUS)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_OPUS_DTX_ENABLE)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_OPUS_FEC_ENABLE)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_MP2ENC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_CODEC_AACENC)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_RISC_TS_DEMUX_PLUGIN)
    set(CFG_BUILD_AUDIO 1)
    set(CFG_BUILD_RISC 1)
endif()

if (DEFINED CFG_RISC_ENABLE)
    set(CFG_BUILD_RISC 1)
endif()

# Video
if (DEFINED CFG_VIDEO_ENABLE)
    add_definitions(
        -DCFG_STC_ENABLE
        -DCFG_VIDEO_ENABLE
        -DCFG_VIDEO_MAX_WIDTH=1920
        -DCFG_VIDEO_MAX_HEIGHT=1088
        -DCFG_VIDEO_BUFFER_PITCH=2048
        -DCFG_VIDEO_DECODE_MODE=1   # 0: general mode, 1: JEDI mode
        -DTWO_WAY_AUDIORECORD
    )
    set(CFG_BUILD_ISP 1)
    if (DEFINED CFG_MULTICAST_ENABLE)
        add_definitions(
            -DENABLE_VIDEO_MULTICAST
        )
    endif()
    if (DEFINED CFG_RTSP_CLIENT_ENABLE)
        add_definitions(
            -DCFG_RTSP_CLIENT_ENABLE
        )
        set(CFG_BUILD_LIVE555 1)
        set(CFG_BUILD_VIDEO_ENCODER 1)
        set(CFG_BUILD_H264_ENCODER 1)
        set(CFG_BUILD_ASYNC_FILE 1)
        set(CFG_BUILD_TS_AIRFILE 1)
    endif()
    #if (DEFINED CFG_LCD_ENABLE)
    set(CFG_BUILD_ITV 1)
    set(CFG_BUILD_FFMPEG 1)
    set(CFG_BUILD_AUDIO 1)  #ffmpeg needs audio API
    add_definitions(
        -DCFG_BUILD_ITV
    )
#endif()
if (DEFINED CFG_FFMPEG_H264_SW)
    set(CFG_BUILD_FFMPEG 1)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TEST_VIDEO)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TEST_RTSPCLIENT)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_VIDEO_FRAME_FUN_ENABLE)

if (DEFINED CFG_MJPEG_DEC_ENABLE)
    set(CFG_BUILD_MJPEG 1)
    add_definitions(
        -DCFG_MJPEG_DEC_ENABLE
    )
endif()

if (CFG_CHIP_FAMILY STREQUAL 970 OR CFG_CHIP_FAMILY STREQUAL 960)
    set(CFG_BUILD_CAPTURE 1)
    set(CFG_BUILD_SENSOR 1)
    set(CFG_BUILD_H264_DECODER 1)
endif()

endif()

# VP test
if (DEFINED CFG_VP_TEST_MODULE_ENABLE)
    set(CFG_BUILD_ISP 1)
    add_definitions(
        -DCFG_VP_TEST_MODULE_ENABLE
    )
endif()

# Ts Module
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TS_MODULE_ENABLE)
if (DEFINED CFG_PURE_TS_STREAM)
    set(CFG_BUILD_TSI 1)

    # tsi interface option
    if (DEFINED CFG_TSI_SERIAL_MODE)
        add_definitions(
            -DCFG_TSI_SERIAL_MODE
            -DCFG_TSI_BUF_SIZE=${CFG_TSI_BUF_SIZE}
        )
    elseif (DEFINED CFG_TSI_PARALLEL_MODE)
        add_definitions(
            -DCFG_TSI_PARALLEL_MODE
            -DCFG_TSI_BUF_SIZE=${CFG_TSI_BUF_SIZE}
        )
    endif()
elseif (DEFINED CFG_ISDB_STANDARDS)
    set(CFG_BUILD_TSI 1)
    set(CFG_BUILD_DEMOD_CTRL 1)

    set(CFG_BUILD_TS_DEMUXER 1)
    set(CFG_TS_DEMUX_ENABLE TRUE)

    set(CFG_DEMOD_ISDB_ENABLE TRUE)

    add_definitions(
        -DCFG_ISDB_ENABLE
        -DCFG_DEMOD_ENABLE
        -DCFG_TS_DEMUX_ENABLE
        -DCFG_DEMOD_SUPPORT_COUNT=${CFG_DEMOD_SUPPORT_COUNT}
    )

    # tsi interface option
    if (DEFINED CFG_TSI_SERIAL_MODE)
        add_definitions(
            -DCFG_TSI_SERIAL_MODE
            -DCFG_TSI_BUF_SIZE=${CFG_TSI_BUF_SIZE}
        )
    elseif (DEFINED CFG_TSI_PARALLEL_MODE)
        add_definitions(
            -DCFG_TSI_PARALLEL_MODE
            -DCFG_TSI_BUF_SIZE=${CFG_TSI_BUF_SIZE}
        )
    endif()

elseif (DEFINED CFG_DVB_STANDARDS)

    set(CFG_BUILD_TS_AIRFILE 1)
    set(CFG_BUILD_DEMOD_CTRL 1)
    set(CFG_BUILD_TSI 1)

    if (DEFINED CFG_TS_DEMUX_ENABLE)
        set(CFG_BUILD_TS_DEMUXER 1)
        add_definitions(
            -DCFG_TS_DEMUX_ENABLE
        )
    endif()

    if (DEFINED CFG_TS_EXTRACTOR_ENABLE)
        set(CFG_BUILD_TS_EXTRACTOR 1)
        add_definitions(
            -DCFG_TS_EXTRACTOR_ENABLE
        )
    endif()

    # demod option
    if (DEFINED CFG_DEMOD_NONE)
        add_definitions(
            -DCFG_DEMOD_SUPPORT_COUNT=-1
        )
    else()
        set(CFG_BUILD_IIC 1)
        set(CFG_DEMOD_ENABLE TRUE)

        add_definitions(
            -DCFG_DEMOD_ENABLE
            -DCFG_DEMOD_SUPPORT_COUNT=${CFG_DEMOD_SUPPORT_COUNT}
        )

        if (DEFINED CFG_DEMOD_IT9135)
            add_definitions(
                -DCFG_DEMOD_IT9135
            )
        elseif (DEFINED CFG_DEMOD_IT9137)
            add_definitions(
                -DCFG_DEMOD_IT9137
            )
        elseif (DEFINED CFG_DEMOD_IT9137_USB)
            add_definitions(
                -DCFG_DEMOD_IT9137_USB
            )

            if (DEFINED CFG_DEMOD_USB_INDEX)
                add_definitions(
                    -DCFG_USB_DEMOD_ENABLE
                    -DCFG_DEMOD_USB_INDEX=${CFG_DEMOD_USB_INDEX}
                )
                if (${CFG_DEMOD_USB_INDEX} STREQUAL 0)
                    set(CFG_BUILD_USB 1)
                    set(CFG_USB0_ENABLE 1)
                elseif (${CFG_DEMOD_USB_INDEX} STREQUAL 1)
                    set(CFG_BUILD_USB 1)
                    set(CFG_USB1_ENABLE 1)
                endif()
            else()
                add_definitions(
                    -DCFG_DEMOD_USB_INDEX=-1
                )
            endif()
        endif()

        # switch i2c addr for demod
        if (DEFINED CFG_DEMOD_SWITCH_GPIO_0)
            add_definitions(
                -DCFG_DEMOD_SWITCH_GPIO_0=${CFG_DEMOD_SWITCH_GPIO_0}
            )
        else()
            add_definitions(
                -DCFG_DEMOD_SWITCH_GPIO_0=-1
            )
        endif()
    endif()

    # aggre option
    if (DEFINED CFG_AGGRE_NONE)
        add_definitions(
            -DCFG_AGGRE_SUPPORT_COUNT=-1
        )
    else()
        set(CFG_BUILD_IIC 1)

        add_definitions(
            -DCFG_AGGRE_ENABLE
            -DCFG_AGGRE_SUPPORT_COUNT=${CFG_AGGRE_SUPPORT_COUNT}
        )
        if (DEFINED CFG_AGGRE_IT9300)
            add_definitions(
                -DCFG_AGGRE_IT9300
            )
        endif()

        if (DEFINED CFG_DEMOD_0_LINK_AGGRE_PORT)
            add_definitions(
                -DCFG_DEMOD_0_LINK_AGGRE_PORT=${CFG_DEMOD_0_LINK_AGGRE_PORT}
            )
        endif()

        if (DEFINED CFG_DEMOD_1_LINK_AGGRE_PORT)
            add_definitions(
                -DCFG_DEMOD_1_LINK_AGGRE_PORT=${CFG_DEMOD_1_LINK_AGGRE_PORT}
            )
        endif()

        if (DEFINED CFG_DEMOD_2_LINK_AGGRE_PORT)
            add_definitions(
                -DCFG_DEMOD_2_LINK_AGGRE_PORT=${CFG_DEMOD_2_LINK_AGGRE_PORT}
            )
        endif()

        if (DEFINED CFG_DEMOD_3_LINK_AGGRE_PORT)
            add_definitions(
                -DCFG_DEMOD_3_LINK_AGGRE_PORT=${CFG_DEMOD_3_LINK_AGGRE_PORT}
            )
        endif()

    endif()

    # tsi interface option
    if (DEFINED CFG_TSI_SERIAL_MODE)
        add_definitions(
            -DCFG_TSI_SERIAL_MODE
            -DCFG_TSI_BUF_SIZE=${CFG_TSI_BUF_SIZE}
        )
    elseif (DEFINED CFG_TSI_PARALLEL_MODE)
        add_definitions(
            -DCFG_TSI_PARALLEL_MODE
            -DCFG_TSI_BUF_SIZE=${CFG_TSI_BUF_SIZE}
        )
    endif()
endif()

# capture module
if (DEFINED CFG_CAPTURE_MODULE_ENABLE)
    add_definitions(
        -DCFG_CAPTURE_MODULE_ENABLE
        -DCFG_CAPTURE_MODULE=${CFG_CAPTURE_MODULE}
        -DCFG_CAPTURE_MODULE_NAME="${CFG_CAPTURE_MODULE}"
    )
    if (DEFINED CFG_SENCOND_CAPTURE_MODULE_ENABLE)
        add_definitions(
            -DCFG_SENCOND_CAPTURE_MODULE_ENABLE
            -DCFG_SENCOND_CAPTURE_MODULE=${CFG_SENCOND_CAPTURE_MODULE}
            -DCFG_SENCOND_CAPTURE_MODULE_NAME="${CFG_SENCOND_CAPTURE_MODULE}"
        )
    endif()
    set(CFG_BUILD_CAPTURE_MODULE 1)
endif()

#hdmi rx chip
if(DEFINED CFG_HDMI_RX_ENABLE)
    set(CFG_BUILD_HDMIRX 1)
    if(DEFINED CFG_IT6801)
        add_definitions(
            -DCFG_IT6801
            -DCFG_HDMI_OUTPUT_MODE=${CFG_HDMIRX_OUTPUTMODE}
        )
    endif()
    add_definitions(
        -DENABLE_HDMI_GPIO_IN_SET
    )
    #iic port sel
    if(DEFINED CFG_HDMIRX_IIC0)
        add_definitions(
            -DCFG_HDMIRX_IIC_PORT=0
        )
    elseif(DEFINED CFG_HDMIRX_IIC1)
        add_definitions(
            -DCFG_HDMIRX_IIC_PORT=1
        )
    elseif(DEFINED CFG_HDMIRX_IIC2)
        add_definitions(
            -DCFG_HDMIRX_IIC_PORT=2
        )
    elseif(DEFINED CFG_HDMIRX_IIC3)
        add_definitions(
            -DCFG_HDMIRX_IIC_PORT=3
        )
    endif()
endif()

#hdmi tx chip
if(DEFINED CFG_HDMI_TX_ENABLE)
    set(CFG_BUILD_HDMITX 1)
    add_definitions(
        -DENABLE_HDMI_GPIO_OUT_SET
    )
endif()

#canbus
if(DEFINED CFG_CANBUS_ENABLE)
    set(CFG_BUILD_CAN_BUS 1)
endif()

#canbus protocol
if(DEFINED CFG_OBD2_ENABLE)
    set(CFG_BUILD_OBD2 1)
endif()

#if run grabber
if (DEFINED CFG_GRABBER_ENABLE)
    set(CFG_BUILD_ASYNC_FILE 1)
    set(CFG_BUILD_MPEGENCODER 1)
    add_definitions(
        -DCFG_GRABBER_ENABLE=1
        -DENABLE_MENCODER=1
    )
endif ()

if (DEFINED CFG_BUILD_ITU)
    add_definitions(
        -DCFG_BUILD_ITU
    )
endif ()

# return channel option
if (DEFINED CFG_TSCAM_ENABLE)
    set(CFG_BUILD_TSCAM_CTRL 1)

    add_definitions(
        -DCFG_TSCAM_ENABLE
    )

    if (DEFINED CFG_TSCAM_PKT_FMT_CUSTOMER)
        add_definitions(
            -DCFG_TSCAM_PKT_FMT_CUSTOMER
        )
    elseif (DEFINED CFG_TSCAM_PKT_FMT_SIMPLEX_RS232)
        add_definitions(
            -DCFG_TSCAM_PKT_FMT_SIMPLEX_RS232
        )
    elseif (DEFINED CFG_TSCAM_PKT_FMT_DUPLEX_RS232)
        add_definitions(
            -DCFG_TSCAM_PKT_FMT_DUPLEX_RS232
        )
    endif()
endif()

# Storage
if (DEFINED CFG_SD0_ENABLE)
    add_definitions(
        -DCFG_SD0_ENABLE
    )

    if (DEFINED CFG_SD0_STATIC)
        add_definitions(
            -DCFG_SD0_STATIC
            -DCFG_SD0_RESERVED_SIZE=${CFG_SD0_RESERVED_SIZE}
        )
    endif()

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SDIO0_STATIC)
    set(CFG_BUILD_SD 1)
endif()

if (DEFINED CFG_SD1_ENABLE)
    add_definitions(
        -DCFG_SD1_ENABLE
    )

    if (DEFINED CFG_SD1_STATIC)
        add_definitions(
            -DCFG_SD1_STATIC
            -DCFG_SD1_RESERVED_SIZE=${CFG_SD1_RESERVED_SIZE}
        )
    endif()

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SDIO1_STATIC)
    set(CFG_BUILD_SD 1)
endif()

if (DEFINED CFG_MMC_ENABLE)
    add_definitions(
        -DCFG_MMC_ENABLE
    )
    set(CFG_BUILD_LINUX 1)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SDIO_ENABLE)

if (DEFINED CFG_MS_ENABLE)
    add_definitions(
        -DCFG_MS_ENABLE
    )
    set(CFG_BUILD_MSPRO 1)
endif()

if (DEFINED CFG_XD_ENABLE)
    add_definitions(
        -DCFG_XD_ENABLE
    )
    set(CFG_BUILD_XD 1)
endif()

if (DEFINED CFG_NAND_ENABLE)
    add_definitions(
        -DCFG_NAND_ENABLE
        -DCFG_NAND_RESERVED_SIZE=${CFG_NAND_RESERVED_SIZE}
        -DCFG_NAND_PAGE_SIZE=${CFG_NAND_PAGE_SIZE}
    )
    set(CFG_BUILD_NAND 1)

    if (DEFINED CFG_SPI_NAND)
        add_definitions(
            -DCFG_SPI_NAND
        )

        if (DEFINED CFG_AXISPI_ENABLE)
            add_definitions(
                -DCFG_AXISPI_ENABLE
            )
            set(CFG_BUILD_AXISPI 1)
        endif()
        set(CFG_BUILD_SPI 1)

        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_UPGRADE_FILE_FOR_NAND_PROGRAMMER)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SPI_NAND_BOOT)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SPI_NAND_USE_SPI0)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SPI_NAND_USE_SPI1)
    elseif (DEFINED CFG_PPI_NAND)
        add_definitions(
            -DCFG_PPI_NAND
        )
    endif()
endif()

if (DEFINED CFG_NOR_ENABLE)
    add_definitions(
        -DCFG_NOR_ENABLE
        -DCFG_NOR_RESERVED_SIZE=${CFG_NOR_RESERVED_SIZE}
        -DCFG_NOR_CACHE_SIZE=${CFG_NOR_CACHE_SIZE}
        -DCFG_NOR_CACHE_FLUSH_INTERVAL=${CFG_NOR_CACHE_FLUSH_INTERVAL}
    )

    if (DEFINED CFG_NOR_USE_AXISPI)
        add_definitions(
            -DCFG_NOR_USE_AXISPI
        )
        set(CFG_BUILD_AXISPI 1)
    endif()

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NOR_ENABLE_QUADMODE)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NOR_ENABLE_DTRMODE)

    if (DEFINED CFG_NOR_SHARE_GPIO)
        add_definitions(
            -DCFG_NOR_SHARE_GPIO
        )
    elseif ((DEFINED CFG_SPI_NAND) AND
            (NOT CFG_CHIP_FAMILY STREQUAL 9850))
        add_definitions(
            -DCFG_NOR_SHARE_SPI_NAND
        )
    endif()

    set(CFG_BUILD_SPI 1)
    set(CFG_BUILD_NOR 1)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_RAMDISK_ENABLE)

if ($ENV{CFG_PLATFORM} STREQUAL openrtos)
    if (DEFINED CFG_MSC_ENABLE)
        add_definitions(
            -DCFG_MSC_ENABLE
        )
        set(CFG_BUILD_MSC 1)
    endif()
    if (DEFINED CFG_UAS_ENABLE)
        add_definitions(
            -DCFG_UAS_ENABLE
        )
        set(CFG_BUILD_UAS 1)
    endif()
endif()

# File System
if (DEFINED CFG_FS_FAT)
    add_definitions(
        -DCFG_FS_FAT
    )
    set(CFG_BUILD_FAT 1)
endif()

if (DEFINED CFG_FS_NTFS)
    add_definitions(
        -DCFG_FS_NTFS
        -DCFG_FS_NTFS_CACHE_PAGE_COUNT=${CFG_FS_NTFS_CACHE_PAGE_COUNT}
        -DCFG_FS_NTFS_CACHE_PAGE_SIZE=${CFG_FS_NTFS_CACHE_PAGE_SIZE}
    )
    set(CFG_BUILD_NTFS 1)
endif()

add_definitions(
    -DCFG_PRIVATE_DRIVE="${CFG_PRIVATE_DRIVE}"
    -DCFG_PUBLIC_DRIVE="${CFG_PUBLIC_DRIVE}"
    -DCFG_TEMP_DRIVE="${CFG_TEMP_DRIVE}"
    -DCFG_BACKUP_DRIVE="${CFG_BACKUP_DRIVE}"
)

# BACKUP
if (DEFINED CFG_CHECK_FILES_CRC_ON_BOOTING)
    add_definitions(
        -DCFG_CHECK_FILES_CRC_ON_BOOTING
    )

    if (DEFINED CFG_BACKUP_ENABLE)
        add_definitions(
            -DCFG_BACKUP_ENABLE
        )
    endif()

endif()

if (DEFINED CFG_NOR_PARTITION3)
    add_definitions(
        -DCFG_NOR_PARTITION3
    )
endif()

# Peripheral
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_RTC_ENABLE)

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_RTC_I2C0)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_RTC_I2C1)

if (DEFINED CFG_RTC_REDUCE_IO_ACCESS_ENABLE)
    add_definitions(
        -DCFG_RTC_REDUCE_IO_ACCESS_ENABLE
        -DCFG_RTC_SYNC_PERIOD=${CFG_RTC_SYNC_PERIOD}
    )
endif()

add_definitions(
    -DCFG_RTC_DEFAULT_TIMESTAMP=${CFG_RTC_DEFAULT_TIMESTAMP}
)

add_definitions(
    -DCFG_INTERNAL_RTC_TIMER=${CFG_INTERNAL_RTC_TIMER}
)

if (DEFINED CFG_I2C0_ENABLE)
    add_definitions(
        -DCFG_I2C0_ENABLE
    )

    set(CFG_BUILD_IIC 1)
endif()

if (DEFINED CFG_I2C1_ENABLE)
    add_definitions(
        -DCFG_I2C1_ENABLE
        -DCFG_HW_I2C_ENABLE
    )

    set(CFG_BUILD_IIC 1)
endif()

if (DEFINED CFG_I2C2_ENABLE)
    add_definitions(
        -DCFG_I2C2_ENABLE
    )

    set(CFG_BUILD_IIC 1)
endif()

if (DEFINED CFG_I2C3_ENABLE)
    add_definitions(
        -DCFG_I2C3_ENABLE
    )

    set(CFG_BUILD_IIC 1)
endif()

if (DEFINED CFG_SW_I2C_ENABLE)
    add_definitions(
        -DCFG_SW_I2C_ENABLE
        -DCFG_SW_I2C_GPIO=${CFG_SW_I2C_GPIO}
    )
    set(CFG_BUILD_IIC_SW 1)
endif()

if (DEFINED CFG_AXISPI_ENABLE)
    add_definitions(
        -DCFG_AXISPI_ENABLE
        -DCFG_AXISPI_MOSI_GPIO=${CFG_AXISPI_MOSI_GPIO}
        -DCFG_AXISPI_MISO_GPIO=${CFG_AXISPI_MISO_GPIO}
        -DCFG_AXISPI_CLOCK_GPIO=${CFG_AXISPI_CLOCK_GPIO}
        -DCFG_AXISPI_CHIP_SEL_GPIO=${CFG_AXISPI_CHIP_SEL_GPIO}
        -DCFG_AXISPI_WP_GPIO=${CFG_AXISPI_WP_GPIO}
        -DCFG_AXISPI_HOLD_GPIO=${CFG_AXISPI_HOLD_GPIO}
    )
endif()

if (DEFINED CFG_SPI0_ENABLE)
    add_definitions(
        -DCFG_SPI0_ENABLE
        -DCFG_SPI0_MOSI_GPIO=${CFG_SPI0_MOSI_GPIO}
        -DCFG_SPI0_MISO_GPIO=${CFG_SPI0_MISO_GPIO}
        -DCFG_SPI0_CLOCK_GPIO=${CFG_SPI0_CLOCK_GPIO}
        -DCFG_SPI0_CHIP_SEL_GPIO=${CFG_SPI0_CHIP_SEL_GPIO}
    )
endif()

if (DEFINED CFG_SPI0_40MHZ_ENABLE)
    add_definitions(
        -DCFG_SPI0_40MHZ_ENABLE
        -DCFG_SPI0_NORCLK_PARA=${CFG_SPI0_NORCLK_PARA}
        -DCFG_SPI0_NANDCLK_PARA=${CFG_SPI0_NANDCLK_PARA}
    )
endif()

if (DEFINED CFG_SPI1_ENABLE)
    add_definitions(
        -DCFG_SPI1_ENABLE
        -DCFG_SPI1_MOSI_GPIO=${CFG_SPI1_MOSI_GPIO}
        -DCFG_SPI1_MISO_GPIO=${CFG_SPI1_MISO_GPIO}
        -DCFG_SPI1_CLOCK_GPIO=${CFG_SPI1_CLOCK_GPIO}
        -DCFG_SPI1_CHIP_SEL_GPIO=${CFG_SPI1_CHIP_SEL_GPIO}
    )
endif()

if (DEFINED CFG_GPIO_EXPANDER_ENABLE)
    add_definitions(
        -DCFG_GPIO_EXPANDER_ENABLE
        -DCFG_GPIO_EXPANDER=${CFG_GPIO_EXPANDER}
    )

    if (DEFINED CFG_SECOND_GPIO_EXPANDER_ENABLE)
        add_definitions(
            -DCFG_SECOND_GPIO_EXPANDER_ENABLE
            -DCFG_SECOND_GPIO_EXPANDER=${CFG_SECOND_GPIO_EXPANDER}
        )
    endif()

    set(CFG_BUILD_GPIO_EX 1)
endif()

if (DEFINED CFG_RGBTOMIPI_ENABLE)
    add_definitions(
        -DCFG_RGBTOMIPI_ENABLE
    )

    if (DEFINED CFG_RGBTOMIPI_SPI0)
        add_definitions(
            -DCFG_RGBTOMIPI_SPI0
        )
    elseif (DEFINED CFG_RGBTOMIPI_SPI1)
        add_definitions(
            -DCFG_RGBTOMIPI_SPI1
        )
    endif()
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_IOEX_ENABLE)

if (DEFINED CFG_SWUART_CODEC_ENABLE)
    add_definitions(
        -DCFG_RISC_ENABLE
        -DCFG_SWUART_CODEC_ENABLE
        -DCFG_SWUART_CODEC_BAUDRATE=${CFG_SWUART_CODEC_BAUDRATE}
        -DCFG_SWUARTTX_CODEC_GPIO=${CFG_SWUARTTX_CODEC_GPIO}
        -DCFG_SWUARTRX_CODEC_GPIO=${CFG_SWUARTRX_CODEC_GPIO}
    )
endif()

if (DEFINED CFG_DBG_SWUART_CODEC)
    add_definitions(
        -DCFG_RISC_ENABLE
        -DCFG_DBG_SWUART_CODEC
        -DCFG_SWUARTDBGPRINTF_BAUDRATE=${CFG_SWUARTDBGPRINTF_BAUDRATE}
        -DCFG_SWUARTDBGPRINTF_GPIO=${CFG_SWUARTDBGPRINTF_GPIO}
    )
endif()

if (DEFINED CFG_SW_WIEGAND_ENABLE)
    add_definitions(
        -DCFG_RISC_ENABLE
        -DCFG_SW_WIEGAND_ENABLE
    )
endif()

if (DEFINED CFG_WIEGAND0_ENABLE)
    add_definitions(
        -DCFG_WIEGAND0_ENABLE
        -DCFG_WIEGAND0_GPIO0=${CFG_WIEGAND0_GPIO0}
        -DCFG_WIEGAND0_GPIO1=${CFG_WIEGAND0_GPIO1}
    )
    if (DEFINED CFG_WIEGAND0_BIT_26)
        add_definitions(
            -DWIEGAND0_BIT_COUNT=26
        )
    elseif (DEFINED CFG_WIEGAND0_BIT_34)
        add_definitions(
            -DWIEGAND0_BIT_COUNT=34
        )
    elseif (DEFINED CFG_WIEGAND0_BIT_37)
        add_definitions(
            -DWIEGAND0_BIT_COUNT=37
        )
    endif()
    set(CFG_BUILD_WIEGAND 1)
endif()

if (DEFINED CFG_WIEGAND1_ENABLE)
    add_definitions(
        -DCFG_WIEGAND1_ENABLE
        -DCFG_WIEGAND1_GPIO0=${CFG_WIEGAND1_GPIO0}
        -DCFG_WIEGAND1_GPIO1=${CFG_WIEGAND1_GPIO1}
    )
    if (DEFINED CFG_WIEGAND1_BIT_26)
        add_definitions(
            -DWIEGAND1_BIT_COUNT=26
        )
    elseif (DEFINED CFG_WIEGAND1_BIT_34)
        add_definitions(
            -DWIEGAND1_BIT_COUNT=34
        )
    elseif (DEFINED CFG_WIEGAND1_BIT_37)
        add_definitions(
            -DWIEGAND1_BIT_COUNT=37
        )
    endif()
    set(CFG_BUILD_WIEGAND 1)
endif()

if (DEFINED CFG_I2S_ENABLE)
    add_definitions(
        -DCFG_I2S_ENABLE
    )

    #ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_I2S_OUTPUT_PIN_ENABLE)
    #ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_I2S_SPDIF_ENABLE)

    if (DEFINED CFG_I2S_INTERNAL_CODEC)
        add_definitions(
            -DENABLE_I2S_INTERNAL_CODEC
        )
    endif()

    if (DEFINED CFG_AXISPI_ENABLE)
        add_definitions(
            -DCFG_AXISPI_ENABLE
        )
        set(CFG_BUILD_AXISPI 1)
    endif()
    if (DEFINED CFG_DAC_port_I2C0)
        add_definitions(
            -DCFG_DAC_port_I2C0
        )
    elseif (DEFINED CFG_DAC_port_I2C1)
        add_definitions(
            -DCFG_DAC_port_I2C1
        )
    endif()

    set(CFG_BUILD_I2S 1)
    add_definitions(
        -DCFG_BUILD_I2S
    )
endif()

if (DEFINED CFG_SPI_ENABLE)
    add_definitions(
        -DCFG_SPI_ENABLE
    )
    set(CFG_BUILD_SPI 1)
endif()

if (DEFINED CFG_UART0_ENABLE)
    add_definitions(
        -DCFG_UART0_ENABLE
        -DCFG_UART0_BAUDRATE=${CFG_UART0_BAUDRATE}
    )
endif()

if (DEFINED CFG_UART1_ENABLE)
    add_definitions(
        -DCFG_UART1_ENABLE
        -DCFG_UART1_BAUDRATE=${CFG_UART1_BAUDRATE}
    )
endif()

if (DEFINED CFG_UART2_ENABLE)
    add_definitions(
        -DCFG_UART2_ENABLE
        -DCFG_UART2_BAUDRATE=${CFG_UART2_BAUDRATE}
    )
endif()

if (DEFINED CFG_UART3_ENABLE)
    add_definitions(
        -DCFG_UART3_ENABLE
        -DCFG_UART3_BAUDRATE=${CFG_UART3_BAUDRATE}
    )
endif()

if (DEFINED CFG_UART4_ENABLE)
    add_definitions(
        -DCFG_UART4_ENABLE
        -DCFG_UART4_BAUDRATE=${CFG_UART4_BAUDRATE}
    )
endif()

if (DEFINED CFG_UART5_ENABLE)
    add_definitions(
        -DCFG_UART5_ENABLE
        -DCFG_UART5_BAUDRATE=${CFG_UART5_BAUDRATE}
    )
endif()

if (DEFINED CFG_UART_INTR)
    add_definitions(
        -DCFG_UART_INTR
        -DCFG_UART_BUF_SIZE=${CFG_UART_BUF_SIZE}
    )
endif()

if (DEFINED CFG_UART_DMA)
    add_definitions(
        -DCFG_UART_DMA
        -DCFG_UART_DMA_BUF_SIZE=${CFG_UART_DMA_BUF_SIZE}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_UART_FORCE_FLUSH)

if (DEFINED CFG_RS485_0_ENABLE)
    add_definitions(
        -DCFG_RS485_0_ENABLE
        -DCFG_RS485_0_BAUDRATE=${CFG_RS485_0_BAUDRATE}
    )
endif()

if (DEFINED CFG_RS485_1_ENABLE)
    add_definitions(
        -DCFG_RS485_1_ENABLE
        -DCFG_RS485_1_BAUDRATE=${CFG_RS485_1_BAUDRATE}
    )
endif()

if (DEFINED CFG_RS485_2_ENABLE)
    add_definitions(
        -DCFG_RS485_2_ENABLE
        -DCFG_RS485_2_BAUDRATE=${CFG_RS485_2_BAUDRATE}
    )
endif()

if (DEFINED CFG_RS485_3_ENABLE)
    add_definitions(
        -DCFG_RS485_3_ENABLE
        -DCFG_RS485_3_BAUDRATE=${CFG_RS485_3_BAUDRATE}
    )
endif()

if (DEFINED CFG_RS485_4_ENABLE)
    add_definitions(
        -DCFG_RS485_4_ENABLE
        -DCFG_RS485_4_BAUDRATE=${CFG_RS485_4_BAUDRATE}
    )
endif()

if (DEFINED CFG_RS485_5_ENABLE)
    add_definitions(
        -DCFG_RS485_5_ENABLE
        -DCFG_RS485_5_BAUDRATE=${CFG_RS485_5_BAUDRATE}
    )
endif()

if (DEFINED CFG_RS485_INTR)
    add_definitions(
        -DCFG_RS485_INTR
        -DCFG_RS485_BUF_SIZE=${CFG_RS485_BUF_SIZE}
    )
endif()

if (DEFINED CFG_RS485_DMA)
    add_definitions(
        -DCFG_RS485_DMA
        -DCFG_RS485_DMA_BUF_SIZE=${CFG_RS485_DMA_BUF_SIZE}
    )
endif()

#if (DEFINED CFG_MODBUS_ENABLE)
#    add_definitions(
#        -DCFG_MODBUS_ENABLE
#        -DCFG_MODBUS_PARITY="${CFG_MODBUS_PARITY}"
#        -DCFG_MODBUS_DATA_BIT=${CFG_MODBUS_DATA_BIT}
#        -DCFG_MODBUS_STOP_BIT=${CFG_MODBUS_STOP_BIT}
#        )
#    set(CFG_BUILD_MODBUS 1)
#endif()

if ($ENV{CFG_PLATFORM} STREQUAL openrtos)
    if (DEFINED CFG_USB0_ENABLE)
        add_definitions(
            -DCFG_USB0_ENABLE
        )
        set(CFG_BUILD_USB 1)
        set(CFG_BUILD_LINUX 1)
    endif()

    if (DEFINED CFG_USB1_ENABLE)
        add_definitions(
            -DCFG_USB1_ENABLE
        )
        set(CFG_BUILD_USB 1)
        set(CFG_BUILD_LINUX 1)
    endif()


    if (DEFINED CFG_USB_HID_ENABLE)
        add_definitions(
            -DCFG_USB_HID_ENABLE
        )

        if (DEFINED CFG_USB_MOUSE)
            add_definitions(
                -DCFG_USB_MOUSE
            )
        endif()

        if (DEFINED CFG_USB_KBD)
            add_definitions(
                -DCFG_USB_KBD
            )
        endif()

        set(CFG_BUILD_HID 1)
    endif()

    if (DEFINED CFG_UVC_ENABLE)
        add_definitions(
            -DCFG_UVC_ENABLE
        )
        set(CFG_BUILD_UVC 1)
    endif()

endif()

if (DEFINED CFG_IRDA_ENABLE)
    add_definitions(
        -DCFG_IRDA_ENABLE
    )
    set(CFG_BUILD_IRDA 1)
endif()

if (DEFINED CFG_IR_ENABLE)
    add_definitions(
        -DCFG_IR_ENABLE
        -DCFG_IR_VENDOR_CODE=${CFG_IR_VENDOR_CODE}
        -DCFG_IR_PRESS_INTERVAL=${CFG_IR_PRESS_INTERVAL}
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_IR_REPEAT)
endif()

if (DEFINED CFG_KEYPAD_ENABLE)
    add_definitions(
        -DCFG_KEYPAD_ENABLE
        -DCFG_KEYPAD_MODULE=${CFG_KEYPAD_MODULE}
        -DCFG_KEYPAD_PRESS_INTERVAL=${CFG_KEYPAD_PRESS_INTERVAL}
        -DCFG_KEYPAD_PROBE_INTERVAL=${CFG_KEYPAD_PROBE_INTERVAL}
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_KEYPAD_REPEAT)

    if (DEFINED CFG_TOUCH_KEYPAD_SET_KEY_NUMBER_ENABLE)
        add_definitions(
            -DCFG_TOUCH_KEY_NUM=${CFG_TOUCH_KEY_NUM}
        )
    endif()

    if (DEFINED CFG_TOUCH_KEYPAD_I2C0)
        add_definitions(
            -DCFG_TOUCH_KEYPAD_I2C0
        )
    elseif (DEFINED CFG_TOUCH_KEYPAD_I2C1)
        add_definitions(
            -DCFG_TOUCH_KEYPAD_I2C1
        )
    elseif (DEFINED CFG_TOUCH_KEYPAD_I2C2)
        add_definitions(
            -DCFG_TOUCH_KEYPAD_I2C2
        )
    endif()
endif()

if (DEFINED CFG_TOUCH_ENABLE)
    add_definitions(
        -DCFG_TOUCH_ENABLE
    )

    if (DEFINED CFG_TOUCH_MODULE)
        list (LENGTH CFG_TOUCH_MODULE count)
        add_definitions(
            -DCFG_TOUCH_MODULE_COUNT=${count}
        )
    endif()

    if (DEFINED CFG_TOUCH_I2C0)
        add_definitions(
            -DCFG_TOUCH_I2C0
        )
    elseif (DEFINED CFG_TOUCH_I2C1)
        add_definitions(
            -DCFG_TOUCH_I2C1
        )
    elseif (DEFINED CFG_TOUCH_I2C2)
        add_definitions(
            -DCFG_TOUCH_I2C2
        )
    elseif (DEFINED CFG_TOUCH_I2C3)
        add_definitions(
            -DCFG_TOUCH_I2C3
        )
    elseif (DEFINED CFG_TOUCH_SPI)
        add_definitions(
            -DCFG_TOUCH_SPI
        )
    endif()

    if (DEFINED CFG_TOUCH_CAPACITIVE)
        add_definitions(
            -DCFG_TOUCH_CAPACITIVE
        )
    elseif (DEFINED CFG_TOUCH_RESISTIVE)
        add_definitions(
            -DCFG_TOUCH_RESISTIVE
        )
    endif()

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TOUCH_INTR)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TOUCH_MULTI_FINGER)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TOUCH_BUTTON)

    if (DEFINED CFG_TOUCH_X_MAX_VALUE)
        add_definitions(
            -DCFG_TOUCH_X_MAX_VALUE=${CFG_TOUCH_X_MAX_VALUE}
        )
    endif()

    if (DEFINED CFG_TOUCH_Y_MAX_VALUE)
        add_definitions(
            -DCFG_TOUCH_Y_MAX_VALUE=${CFG_TOUCH_Y_MAX_VALUE}
        )
    endif()

    if (DEFINED CFG_TOUCH_ADVANCE_CONFIG)
        add_definitions(
            -DCFG_TOUCH_ADVANCE_CONFIG
        )

        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TOUCH_SWAP_XY)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TOUCH_REVERSE_X)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TOUCH_REVERSE_Y)
    endif()

    set(CFG_BUILD_TSLIB 1)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_FFMPEG_H264_SW)

if (DEFINED CFG_BATTERY_ENABLE)
    add_definitions(
        -DCFG_BATTERY_ENABLE
        -DCFG_BATTERY_MODULE="${CFG_BATTERY_MODULE}"
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_BATTERY_CHARGE_DETECT)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_GSENSOR_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AMPLIFIER_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_LED_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SWITCH_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TUNER_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIOAMP_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SENSOR_ENABLE)

if (DEFINED CFG_DUAL_STREAM_ENABLE)
    add_definitions(
        -DCFG_DUAL_STREAM
    )
endif()

if (DEFINED CFG_SENSOR_PIXELPLUS_PO3100)
    add_definitions(
        -DSENSOR_PIXELPLUS_PO3100
        -DSENSOR_WIDTH=${CFG_SENSOR_RESOLUTION_W}
        -DSENSOR_HEIGHT=${CFG_SENSOR_RESOLUTION_H}
        -DSENSOR_BITRATE=${CFG_SENSOR_BITRATE}
        -DSENSOR_WIDTH_2=${CFG_SENSOR_RESOLUTION_W_2}
        -DSENSOR_HEIGHT_2=${CFG_SENSOR_RESOLUTION_H_2}
        -DSENSOR_BITRATE_2=${CFG_SENSOR_BITRATE_2}
    )
    set(SENSOR_FW_NAME PIXELPLUS_PO3100)
    if (DEFINED CFG_SENSOR_FLICK_50HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_50HZ
        )
    elseif (DEFINED CFG_SENSOR_FLICK_60HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_60HZ
        )
    elseif (DEFINED CFGSENSOR_FLICK_AUTO)
        add_definitions(
            -DCFG_SENSOR_FLICK_AUTO
        )
    endif()
elseif (DEFINED CFG_SENSOR_NOVATEK_NT99141)
    add_definitions(
        -DSENSOR_NOVATEK_NT99141
        -DSENSOR_WIDTH=${CFG_SENSOR_RESOLUTION_W}
        -DSENSOR_HEIGHT=${CFG_SENSOR_RESOLUTION_H}
        -DSENSOR_BITRATE=${CFG_SENSOR_BITRATE}
        -DSENSOR_WIDTH_2=${CFG_SENSOR_RESOLUTION_W_2}
        -DSENSOR_HEIGHT_2=${CFG_SENSOR_RESOLUTION_H_2}
        -DSENSOR_BITRATE_2=${CFG_SENSOR_BITRATE_2}
    )
    set(SENSOR_FW_NAME NOVATEK_NT99141)
    if (DEFINED CFG_SENSOR_FLICK_50HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_50HZ
        )
    elseif (DEFINED CFG_SENSOR_FLICK_60HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_60HZ
        )
    elseif (DEFINED CFGSENSOR_FLICK_AUTO)
        add_definitions(
            -DCFG_SENSOR_FLICK_AUTO
        )
    endif()
elseif (DEFINED CFG_SENSOR_HIMAX_HM1375)
    add_definitions(
        -DSENSOR_HIMAX_HM1375
        -DSENSOR_WIDTH=${CFG_SENSOR_RESOLUTION_W}
        -DSENSOR_HEIGHT=${CFG_SENSOR_RESOLUTION_H}
        -DSENSOR_BITRATE=${CFG_SENSOR_BITRATE}
        -DSENSOR_WIDTH_2=${CFG_SENSOR_RESOLUTION_W_2}
        -DSENSOR_HEIGHT_2=${CFG_SENSOR_RESOLUTION_H_2}
        -DSENSOR_BITRATE_2=${CFG_SENSOR_BITRATE_2}
    )
    set(SENSOR_FW_NAME HIMAX_HM1375)
    if (DEFINED CFG_SENSOR_FLICK_50HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_50HZ
        )
    elseif (DEFINED CFG_SENSOR_FLICK_60HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_60HZ
        )
    endif()
elseif (DEFINED CFG_SENSOR_OMNIVISION_OV7725)
    add_definitions(
        -DSENSOR_OMNIVISION_OV7725
        -DSENSOR_WIDTH=${CFG_SENSOR_RESOLUTION_W}
        -DSENSOR_HEIGHT=${CFG_SENSOR_RESOLUTION_H}
        -DSENSOR_BITRATE=${CFG_SENSOR_BITRATE}
        -DSENSOR_WIDTH_2=${CFG_SENSOR_RESOLUTION_W_2}
        -DSENSOR_HEIGHT_2=${CFG_SENSOR_RESOLUTION_H_2}
        -DSENSOR_BITRATE_2=${CFG_SENSOR_BITRATE_2}
    )
    set(SENSOR_FW_NAME OMNIVISION_OV7725)
    if (DEFINED CFG_SENSOR_FLICK_50HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_50HZ
        )
    elseif (DEFINED CFG_SENSOR_FLICK_60HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_60HZ
        )
    endif()
elseif (DEFINED CFG_SENSOR_TI_TVP5150)
    add_definitions(
        -DSENSOR_TI_TVP5150
        -DSENSOR_WIDTH=${CFG_SENSOR_RESOLUTION_W}
        -DSENSOR_HEIGHT=${CFG_SENSOR_RESOLUTION_H}
        -DSENSOR_BITRATE=${CFG_SENSOR_BITRATE}
    )
    set(SENSOR_FW_NAME TI_TVP5150)
    if (DEFINED CFG_SENSOR_FLICK_50HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_50HZ
        )
    elseif (DEFINED CFG_SENSOR_FLICK_60HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_60HZ
        )
    endif()
elseif (DEFINED CFG_SENSOR_AR0130)
    add_definitions(
        -DSENSOR_AR0130
        -DSENSOR_WIDTH=${CFG_SENSOR_RESOLUTION_W}
        -DSENSOR_HEIGHT=${CFG_SENSOR_RESOLUTION_H}
        -DSENSOR_BITRATE=${CFG_SENSOR_BITRATE}
    )
    set(SENSOR_FW_NAME AR0130)
    if (DEFINED CFG_SENSOR_FLICK_50HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_50HZ
        )
    elseif (DEFINED CFG_SENSOR_FLICK_60HZ)
        add_definitions(
            -DCFG_SENSOR_FLICK_60HZ
        )
    endif()
endif()
#SENSOR PIN SEL
if (DEFINED CFG_SENSOR_ENABLE)
    add_definitions(
        -DCFG_SN1_GPIO_RST=${CFG_GPIO_SN1_RST}
        -DCFG_SN1_GPIO_PWN=${CFG_GPIO_SN1_PWN}
        -DCFG_SN1_GPIO_LED=${CFG_GPIO_SN1_LED}
    )
    if(DEFINED CFG_SENSOR_IIC0)
        add_definitions(
            -DCFG_SENSOR_IIC_PORT=0
        )
    elseif(DEFINED CFG_SENSOR_IIC1)
        add_definitions(
            -DCFG_SENSOR_IIC_PORT=1
        )
    elseif(DEFINED CFG_SENSOR_IIC2)
        add_definitions(
            -DCFG_SENSOR_IIC_PORT=2
        )
    elseif(DEFINED CFG_SENSOR_IIC3)
        add_definitions(
            -DCFG_SENSOR_IIC_PORT=3
        )
    endif()
endif()

if (DEFINED CFG_STNLCD_ENABLE)
    add_definitions(
        -DCFG_STNLCD_ENABLE
        -DCFG_STNLCD_WIDTH=${CFG_STNLCD_WIDTH}
        -DCFG_STNLCD_HEIGHT=${CFG_STNLCD_HEIGHT}
    )
endif()


# Power
if (DEFINED CFG_POWER_ON)
    add_definitions(
        -DCFG_POWER_ON
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_POWER_OFF)
endif()

if (DEFINED CFG_POWER_STANDBY)
    add_definitions(
        -DCFG_POWER_STANDBY
        -DCFG_POWER_STANDBY_CPU_FREQ_RATIO=${CFG_POWER_STANDBY_CPU_FREQ_RATIO}
        -DCFG_POWER_STANDBY_BUS_FREQ_RATIO=${CFG_POWER_STANDBY_BUS_FREQ_RATIO}
    )
endif()

if (DEFINED CFG_POWER_SLEEP)
    add_definitions(
        -DCFG_POWER_SLEEP
    )

    if (DEFINED CFG_POWER_WAKEUP_DELAY)
        add_definitions(
            -DCFG_POWER_WAKEUP_DELAY
            -DCFG_POWER_WAKEUP_DELAYLOOP=${CFG_POWER_WAKEUP_DELAYLOOP}
        )
    elseif (DEFINED CFG_POWER_WAKEUP_TOUCH_ONE_CLICK)
        add_definitions(
            -DCFG_POWER_WAKEUP_TOUCH_ONE_CLICK
        )
    elseif (DEFINED CFG_POWER_WAKEUP_TOUCH_DOUBLE_CLICK)
        add_definitions(
            -DCFG_POWER_WAKEUP_TOUCH_DOUBLE_CLICK
            -DCFG_POWER_WAKEUP_DOUBLE_CLICK_INTERVAL=${CFG_POWER_WAKEUP_DOUBLE_CLICK_INTERVAL}
        )
    elseif (DEFINED CFG_POWER_WAKEUP_KEYPAD)
        add_definitions(
            -DCFG_POWER_WAKEUP_KEYPAD
            -DCFG_POWER_WAKEUP_KEYNUM=${CFG_POWER_WAKEUP_KEYNUM}
        )
    elseif (DEFINED CFG_POWER_WAKEUP_GPIO)
        add_definitions(
            -DCFG_POWER_WAKEUP_GPIO
            -DCFG_POWER_WAKEUP_GPIO_PIN=${CFG_POWER_WAKEUP_GPIO_PIN}
        )
    elseif (DEFINED CFG_POWER_WAKEUP_IR)
        add_definitions(
            -DCFG_POWER_WAKEUP_IR
        )
    elseif (DEFINED CFG_POWER_WAKEUP_UART)
        add_definitions(
            -DCFG_POWER_WAKEUP_UART
        )
    elseif (DEFINED CFG_POWER_WAKEUP_WOL)
        add_definitions(
            -DCFG_POWER_WAKEUP_WOL
        )
    endif()

endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_POWER_HIBERNATION)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_POWER_SUSPEND)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_POWER_DOZE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_POWER_TICKLESS_IDLE)

if (DEFINED CFG_ALT_CPU_ENABLE)
    set(CFG_BUILD_RISC 1)
    set(CFG_BUILD_ALT_CPU 1)
    add_definitions(
        -DCFG_RISC_ENABLE
        -DCFG_ALT_CPU_ENABLE
    )
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_RSL_MASTER)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_RSL_SLAVE)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_PATTERN_GEN)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_OLED_CTRL)
    if (DEFINED CFG_SW_PWM)
        add_definitions(
            -DCFG_SW_PWM
        )
        if (DEFINED CFG_SW_PWM0_ENABLE)
            add_definitions(
                -DCFG_SW_PWM0_ENABLE
                -DCFG_SW_PWM0_CLOCK=${CFG_SW_PWM0_CLOCK}
                -DCFG_SW_PWM0_DUTY_CYCLE=${CFG_SW_PWM0_DUTY_CYCLE}
                -DCFG_GPIO_SW_PWM0=${CFG_GPIO_SW_PWM0}
            )
        endif()

        if (DEFINED CFG_SW_PWM1_ENABLE)
            add_definitions(
                -DCFG_SW_PWM1_ENABLE
                -DCFG_SW_PWM1_CLOCK=${CFG_SW_PWM1_CLOCK}
                -DCFG_SW_PWM1_DUTY_CYCLE=${CFG_SW_PWM1_DUTY_CYCLE}
                -DCFG_GPIO_SW_PWM1=${CFG_GPIO_SW_PWM1}
            )
        endif()

        if (DEFINED CFG_SW_PWM2_ENABLE)
            add_definitions(
                -DCFG_SW_PWM2_ENABLE
                -DCFG_SW_PWM2_CLOCK=${CFG_SW_PWM2_CLOCK}
                -DCFG_SW_PWM2_DUTY_CYCLE=${CFG_SW_PWM2_DUTY_CYCLE}
                -DCFG_GPIO_SW_PWM2=${CFG_GPIO_SW_PWM2}
            )
        endif()

        if (DEFINED CFG_SW_PWM3_ENABLE)
            add_definitions(
                -DCFG_SW_PWM3_ENABLE
                -DCFG_SW_PWM3_CLOCK=${CFG_SW_PWM3_CLOCK}
                -DCFG_SW_PWM3_DUTY_CYCLE=${CFG_SW_PWM3_DUTY_CYCLE}
                -DCFG_GPIO_SW_PWM3=${CFG_GPIO_SW_PWM3}
            )
        endif()
    endif()

    if (DEFINED CFG_SW_UART)
        add_definitions(
            -DCFG_SW_UART
            -DCFG_SW_UART_BAUDRATE=${CFG_SW_UART_BAUDRATE}
            -DCFG_GPIO_SW_UART_TX=${CFG_GPIO_SW_UART_TX}
            -DCFG_GPIO_SW_UART_RX=${CFG_GPIO_SW_UART_RX}
        )
    endif()

    if (DEFINED CFG_SW_SERIAL_PORT)
        add_definitions(
            -DCFG_SW_SERIAL_PORT
        )
        if (DEFINED CFG_SW_RS232_0_ENABLE)
            add_definitions(
                -DCFG_SW_RS232_0_ENABLE
                -DCFG_SW_RS232_0_BAUDRATE=${CFG_SW_RS232_0_BAUDRATE}
                -DCFG_GPIO_SW_RS232_0_TX=${CFG_GPIO_SW_RS232_0_TX}
                -DCFG_GPIO_SW_RS232_0_RX=${CFG_GPIO_SW_RS232_0_RX}
            )
        endif()

        if (DEFINED CFG_SW_RS232_1_ENABLE)
            add_definitions(
                -DCFG_SW_RS232_1_ENABLE
                -DCFG_SW_RS232_1_BAUDRATE=${CFG_SW_RS232_1_BAUDRATE}
                -DCFG_GPIO_SW_RS232_1_TX=${CFG_GPIO_SW_RS232_1_TX}
                -DCFG_GPIO_SW_RS232_1_RX=${CFG_GPIO_SW_RS232_1_RX}
            )
        endif()

        if (DEFINED CFG_SW_RS485_0_ENABLE)
            add_definitions(
                -DCFG_SW_RS485_0_ENABLE
                -DCFG_SW_RS485_0_BAUDRATE=${CFG_SW_RS485_0_BAUDRATE}
                -DCFG_GPIO_SW_RS485_0_TX=${CFG_GPIO_SW_RS485_0_TX}
                -DCFG_GPIO_SW_RS485_0_TX_ENABLE=${CFG_GPIO_SW_RS485_0_TX_ENABLE}
                -DCFG_GPIO_SW_RS485_0_RX=${CFG_GPIO_SW_RS485_0_RX}
            )
        endif()

        if (DEFINED CFG_SW_RS485_1_ENABLE)
            add_definitions(
                -DCFG_SW_RS485_1_ENABLE
                -DCFG_SW_RS485_1_BAUDRATE=${CFG_SW_RS485_1_BAUDRATE}
                -DCFG_GPIO_SW_RS485_1_TX=${CFG_GPIO_SW_RS485_1_TX}
                -DCFG_GPIO_SW_RS485_1_TX_ENABLE=${CFG_GPIO_SW_RS485_1_TX_ENABLE}
                -DCFG_GPIO_SW_RS485_1_RX=${CFG_GPIO_SW_RS485_1_RX}
            )
        endif()
    endif()

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_ALT_CPU_CUSTOM_DEVICE)
endif()

if (DEFINED CFG_RISC2_ENABLE)
    set(CFG_BUILD_RISC 1)
    set(CFG_BUILD_RISC2 1)
    add_definitions(
        -DCFG_RISC_ENABLE
        -DCFG_RISC2_ENABLE
    )
    if (DEFINED CFG_RISC2_TEST_DEVICE)
        add_definitions(
            -DCFG_RISC2_TEST_DEVICE
        )
    endif()
    if (DEFINED CFG_RISC2_OPUS_CODEC)
        add_definitions(
            -DCFG_RISC2_OPUS_CODEC
        )
    endif()
endif()
#endif

# SARADC
if (DEFINED CFG_SARADC_ENABLE)
    set(CFG_BUILD_SARADC 1)
    add_definitions(
        -DCFG_SARADC_ENABLE
        -DCFG_SARADC_VALID_XAIN=${CFG_SARADC_VALID_XAIN}
        -DCFG_GPIO_XAIN_0_RX=${CFG_GPIO_XAIN_0_RX}
        -DCFG_GPIO_XAIN_1_RX=${CFG_GPIO_XAIN_1_RX}
        -DCFG_GPIO_XAIN_2_RX=${CFG_GPIO_XAIN_2_RX}
        -DCFG_GPIO_XAIN_3_RX=${CFG_GPIO_XAIN_3_RX}
    )
endif()

# GPIO
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_GPIO_CARD_DETECT_INTR)

if (DEFINED CFG_GPIO_SD0_CARD_DETECT)
    add_definitions(
        -DCFG_GPIO_SD0_CARD_DETECT=${CFG_GPIO_SD0_CARD_DETECT}
        -DCFG_GPIO_SD0_POWER_ENABLE=${CFG_GPIO_SD0_POWER_ENABLE}
        -DCFG_GPIO_SD0_WRITE_PROTECT=${CFG_GPIO_SD0_WRITE_PROTECT}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SD0_CARD_DETECT_ACTIVE_HIGH)

if (DEFINED CFG_GPIO_SD0_IO)
    add_definitions(
        -DCFG_GPIO_SD0_IO=${CFG_GPIO_SD0_IO}
    )
endif()

if (DEFINED CFG_GPIO_SD1_CARD_DETECT)
    add_definitions(
        -DCFG_GPIO_SD1_CARD_DETECT=${CFG_GPIO_SD1_CARD_DETECT}
        -DCFG_GPIO_SD1_POWER_ENABLE=${CFG_GPIO_SD1_POWER_ENABLE}
        -DCFG_GPIO_SD1_WRITE_PROTECT=${CFG_GPIO_SD1_WRITE_PROTECT}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SD1_CARD_DETECT_ACTIVE_HIGH)

if (DEFINED CFG_GPIO_SD1_IO)
    add_definitions(
        -DCFG_GPIO_SD1_IO=${CFG_GPIO_SD1_IO}
    )
endif()

if (DEFINED CFG_GPIO_CF_CARD_DETECT)
    add_definitions(
        -DCFG_GPIO_CF_CARD_DETECT=${CFG_GPIO_CF_CARD_DETECT}
        -DCFG_GPIO_CF_POWER_ENABLE=${CFG_GPIO_CF_POWER_ENABLE}
    )
endif()

if (DEFINED CFG_EXT_WATCHDOG_ENABLE)
    add_definitions(
        -DCFG_EXT_WATCHDOG_ENABLE=${CFG_EXT_WATCHDOG_ENABLE}
        -DCFG_GPIO_EXT_WATCHDOG=${CFG_GPIO_EXT_WATCHDOG}
    )
endif()

if (DEFINED CFG_GPIO_BACKLIGHT_PWM)
    add_definitions(
        -DCFG_GPIO_BACKLIGHT_PWM=${CFG_GPIO_BACKLIGHT_PWM}
        -DCFG_GPIO_PWM_NUMBER=${CFG_GPIO_PWM_NUMBER}
        -DCFG_GPIO_PWM_MODE=${CFG_GPIO_PWM_MODE}
    )
endif()

if (DEFINED CFG_GPIO_UART0_TX)
    add_definitions(
        -DCFG_GPIO_UART0_TX=${CFG_GPIO_UART0_TX}
        -DCFG_GPIO_UART0_RX=${CFG_GPIO_UART0_RX}
    )
endif()

if (DEFINED CFG_GPIO_UART1_TX)
    add_definitions(
        -DCFG_GPIO_UART1_TX=${CFG_GPIO_UART1_TX}
        -DCFG_GPIO_UART1_RX=${CFG_GPIO_UART1_RX}
    )
endif()

if (DEFINED CFG_GPIO_UART2_TX)
    add_definitions(
        -DCFG_GPIO_UART2_TX=${CFG_GPIO_UART2_TX}
        -DCFG_GPIO_UART2_RX=${CFG_GPIO_UART2_RX}
    )
endif()

if (DEFINED CFG_GPIO_UART3_TX)
    add_definitions(
        -DCFG_GPIO_UART3_TX=${CFG_GPIO_UART3_TX}
        -DCFG_GPIO_UART3_RX=${CFG_GPIO_UART3_RX}
    )
endif()

if (DEFINED CFG_GPIO_UART4_TX)
    add_definitions(
        -DCFG_GPIO_UART4_TX=${CFG_GPIO_UART4_TX}
        -DCFG_GPIO_UART4_RX=${CFG_GPIO_UART4_RX}
    )
endif()

if (DEFINED CFG_GPIO_UART5_TX)
    add_definitions(
        -DCFG_GPIO_UART5_TX=${CFG_GPIO_UART5_TX}
        -DCFG_GPIO_UART5_RX=${CFG_GPIO_UART5_RX}
    )
endif()

if (DEFINED CFG_GPIO_RS485_0_TX)
    add_definitions(
        -DCFG_GPIO_RS485_0_TX=${CFG_GPIO_RS485_0_TX}
        -DCFG_GPIO_RS485_0_TX_ENABLE=${CFG_GPIO_RS485_0_TX_ENABLE}
        -DCFG_GPIO_RS485_0_RX=${CFG_GPIO_RS485_0_RX}
    )
endif()

if (DEFINED CFG_GPIO_RS485_1_TX)
    add_definitions(
        -DCFG_GPIO_RS485_1_TX=${CFG_GPIO_RS485_1_TX}
        -DCFG_GPIO_RS485_1_TX_ENABLE=${CFG_GPIO_RS485_1_TX_ENABLE}
        -DCFG_GPIO_RS485_1_RX=${CFG_GPIO_RS485_1_RX}
    )
endif()

if (DEFINED CFG_GPIO_RS485_2_TX)
    add_definitions(
        -DCFG_GPIO_RS485_2_TX=${CFG_GPIO_RS485_2_TX}
        -DCFG_GPIO_RS485_2_TX_ENABLE=${CFG_GPIO_RS485_2_TX_ENABLE}
        -DCFG_GPIO_RS485_2_RX=${CFG_GPIO_RS485_2_RX}
    )
endif()

if (DEFINED CFG_GPIO_RS485_3_TX)
    add_definitions(
        -DCFG_GPIO_RS485_3_TX=${CFG_GPIO_RS485_3_TX}
        -DCFG_GPIO_RS485_3_TX_ENABLE=${CFG_GPIO_RS485_3_TX_ENABLE}
        -DCFG_GPIO_RS485_3_RX=${CFG_GPIO_RS485_3_RX}
    )
endif()

if (DEFINED CFG_GPIO_RS485_4_TX)
    add_definitions(
        -DCFG_GPIO_RS485_4_TX=${CFG_GPIO_RS485_4_TX}
        -DCFG_GPIO_RS485_4_TX_ENABLE=${CFG_GPIO_RS485_4_TX_ENABLE}
        -DCFG_GPIO_RS485_4_RX=${CFG_GPIO_RS485_4_RX}
    )
endif()

if (DEFINED CFG_GPIO_IIC0_CLK)
    add_definitions(
        -DCFG_GPIO_IIC0_CLK=${CFG_GPIO_IIC0_CLK}
        -DCFG_GPIO_IIC0_DATA=${CFG_GPIO_IIC0_DATA}
        -DCFG_GPIO_IIC0_MODE=${CFG_GPIO_IIC0_MODE}
    )
endif()

if (DEFINED CFG_GPIO_IIC1_CLK)
    add_definitions(
        -DCFG_GPIO_IIC1_CLK=${CFG_GPIO_IIC1_CLK}
        -DCFG_GPIO_IIC1_DATA=${CFG_GPIO_IIC1_DATA}
        -DCFG_GPIO_IIC1_MODE=${CFG_GPIO_IIC1_MODE}
    )
endif()

if (DEFINED CFG_GPIO_IIC2_CLK)
    add_definitions(
        -DCFG_GPIO_IIC2_CLK=${CFG_GPIO_IIC2_CLK}
        -DCFG_GPIO_IIC2_DATA=${CFG_GPIO_IIC2_DATA}
    )
endif()

if (DEFINED CFG_GPIO_IIC3_CLK)
    add_definitions(
        -DCFG_GPIO_IIC3_CLK=${CFG_GPIO_IIC3_CLK}
        -DCFG_GPIO_IIC3_DATA=${CFG_GPIO_IIC3_DATA}
    )
endif()

if (DEFINED CFG_GPIO_IR)
    add_definitions(
        -DCFG_GPIO_IR=${CFG_GPIO_IR}
    )
endif()

if (DEFINED CFG_GPIO_IR_TX)
    add_definitions(
        -DCFG_GPIO_IR_TX=${CFG_GPIO_IR_TX}
    )
endif()

if (DEFINED CFG_GPIO_KEYPAD)
    add_definitions(
        -DCFG_GPIO_KEYPAD=${CFG_GPIO_KEYPAD}
    )
endif()

if (DEFINED CFG_GPIO_TOUCH_INT)
    add_definitions(
        -DCFG_GPIO_TOUCH_INT=${CFG_GPIO_TOUCH_INT}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_GPIO_TOUCH_INT_ACTIVE_HIGH)

if (DEFINED CFG_GPIO_TOUCH_WAKE)
    add_definitions(
        -DCFG_GPIO_TOUCH_WAKE=${CFG_GPIO_TOUCH_WAKE}
    )
endif()

if (DEFINED CFG_GPIO_TOUCH_RESET)
    add_definitions(
        -DCFG_GPIO_TOUCH_RESET=${CFG_GPIO_TOUCH_RESET}
    )
endif()

if (DEFINED CFG_GPIO_POWER_ON)
    add_definitions(
        -DCFG_GPIO_POWER_ON=${CFG_GPIO_POWER_ON}
    )
endif()

if (DEFINED CFG_GPIO_POWER_OFF_DETECT)
    add_definitions(
        -DCFG_GPIO_POWER_OFF_DETECT=${CFG_GPIO_POWER_OFF_DETECT}
    )
endif()

if (DEFINED CFG_GPIO_BATTERY)
    add_definitions(
        -DCFG_GPIO_BATTERY=${CFG_GPIO_BATTERY}
    )
endif()

if (DEFINED CFG_GPIO_BATTERY_CHARGE_DETECT)
    add_definitions(
        -DCFG_GPIO_BATTERY_CHARGE_DETECT=${CFG_GPIO_BATTERY_CHARGE_DETECT}
    )
endif()

if (DEFINED CFG_GPIO_GSENSOR)
    add_definitions(
        -DCFG_GPIO_GSENSOR=${CFG_GPIO_GSENSOR}
    )
endif()

if (DEFINED CFG_GPIO_HEADSET)
    add_definitions(
        -DCFG_GPIO_HEADSET=${CFG_GPIO_HEADSET}
    )
endif()

if (DEFINED CFG_GPIO_SPEAKER)
    add_definitions(
        -DCFG_GPIO_SPEAKER=${CFG_GPIO_SPEAKER}
    )
endif()

if (DEFINED CFG_GPIO_AMPLIFIER_ENABLE)
    add_definitions(
        -DCFG_GPIO_AMPLIFIER_ENABLE=${CFG_GPIO_AMPLIFIER_ENABLE}
    )
endif()

if (DEFINED CFG_GPIO_AMPLIFIER_MUTE)
    add_definitions(
        -DCFG_GPIO_AMPLIFIER_MUTE=${CFG_GPIO_AMPLIFIER_MUTE}
    )
endif()

if (DEFINED CFG_GPIO_ETHERNET_LINK)
    add_definitions(
        -DCFG_GPIO_ETHERNET_LINK=${CFG_GPIO_ETHERNET_LINK}
    )
else()
    add_definitions(
        -DCFG_GPIO_ETHERNET_LINK=-1
    )
endif()

if (DEFINED CFG_GPIO_ETHERNET_WAKE_ON_LAN)
    add_definitions(
        -DCFG_GPIO_ETHERNET_WAKE_ON_LAN=${CFG_GPIO_ETHERNET_WAKE_ON_LAN}
    )
endif()

if (DEFINED CFG_GPIO_LED)
    add_definitions(
        -DCFG_GPIO_LED=${CFG_GPIO_LED}
    )
endif()

if (DEFINED CFG_GPIO_SWITCH)
    add_definitions(
        -DCFG_GPIO_SWITCH=${CFG_GPIO_SWITCH}
    )
endif()

if (DEFINED CFG_GPIO_TUNER_ENABLE)
    add_definitions(
        -DCFG_GPIO_TUNER_ENABLE=${CFG_GPIO_TUNER_ENABLE}
    )
endif()

if (DEFINED CFG_GPIO_STNLCD_A0)
    add_definitions(
        #        -DCFG_GPIO_STNLCD_CS=${CFG_GPIO_STNLCD_CS}
        -DCFG_GPIO_STNLCD_RST=${CFG_GPIO_STNLCD_RST}
        -DCFG_GPIO_STNLCD_A0=${CFG_GPIO_STNLCD_A0}
        #        -DCFG_GPIO_STNLCD_SCLK=${CFG_GPIO_STNLCD_SCLK}
        #        -DCFG_GPIO_STNLCD_SID=${CFG_GPIO_STNLCD_SID}
        -DCFG_GPIO_STNLCD_WR=${CFG_GPIO_STNLCD_WR}
        -DCFG_GPIO_STNLCD_RD=${CFG_GPIO_STNLCD_RD}
        -DCFG_GPIO_STNLCD_DATA=${CFG_GPIO_STNLCD_DATA}
    )
endif()

if (DEFINED CFG_GPIO_MIPI_RESET)
    add_definitions(
        -DCFG_GPIO_MIPI_RESET=${CFG_GPIO_MIPI_RESET}
    )
endif()

if (DEFINED CFG_GPIO_SWUART)
    add_definitions(
        -DCFG_GPIO_SWUART=${CFG_GPIO_SWUART}
    )
endif()

if (DEFINED CFG_SWUART_TIMER)
    add_definitions(
        -DCFG_SWUART_TIMER=${CFG_SWUART_TIMER}
    )
endif()

if (DEFINED CFG_GPIO_DEMOD_SUSPEND)
    add_definitions(
        -DCFG_GPIO_DEMOD_SUSPEND=${CFG_GPIO_DEMOD_SUSPEND}
    )
endif()

if (DEFINED CFG_IIC0_GPIO_CONFIG_1)
    add_definitions(
        -DCFG_IIC0_GPIO_CONFIG_1=${CFG_IIC0_GPIO_CONFIG_1}
    )
elseif (DEFINED CFG_IIC0_GPIO_CONFIG_2)
    add_definitions(
        -DCFG_IIC0_GPIO_CONFIG_2=${CFG_IIC0_GPIO_CONFIG_2}
    )
elseif (DEFINED CFG_IIC0_GPIO_CONFIG_3)
    add_definitions(
        -DCFG_IIC0_GPIO_CONFIG_3=${CFG_IIC0_GPIO_CONFIG_3}
    )
endif()

if (DEFINED CFG_IIC1_GPIO_CONFIG_1)
    add_definitions(
        -DCFG_IIC1_GPIO_CONFIG_1=${CFG_IIC1_GPIO_CONFIG_1}
    )
elseif (DEFINED CFG_IIC1_GPIO_CONFIG_2)
    add_definitions(
        -DCFG_IIC1_GPIO_CONFIG_2=${CFG_IIC1_GPIO_CONFIG_2}
    )
elseif (DEFINED CFG_IIC1_GPIO_CONFIG_3)
    add_definitions(
        -DCFG_IIC1_GPIO_CONFIG_3=${CFG_IIC1_GPIO_CONFIG_3}
    )
endif()

if (DEFINED CFG_I2S_USE_GPIO_MODE_3)
    add_definitions(
        -DCFG_I2S_USE_GPIO_MODE_3=${CFG_I2S_USE_GPIO_MODE_3}
    )
elseif (DEFINED CFG_I2S_USE_GPIO_MODE_2)
    add_definitions(
        -DCFG_I2S_USE_GPIO_MODE_2=${CFG_I2S_USE_GPIO_MODE_2}
    )
elseif (DEFINED CFG_I2S_USE_GPIO_MODE_1)
    add_definitions(
        -DCFG_I2S_USE_GPIO_MODE_1=${CFG_I2S_USE_GPIO_MODE_1}
    )
endif()

if (DEFINED CFG_DAC_USE_EXTERNAL_DEPOP_CIRCUIT)
    add_definitions(
        -DCFG_DAC_USE_EXTERNAL_DEPOP_CIRCUIT=${CFG_DAC_USE_EXTERNAL_DEPOP_CIRCUIT}
        -DCFG_DAC_EXT_DEPOP_GPIO_PIN=${CFG_DAC_EXT_DEPOP_GPIO_PIN}
        -DCFG_DAC_EXT_DEPOP_DELAY_MS_T1=${CFG_DAC_EXT_DEPOP_DELAY_MS_T1}
        -DCFG_DAC_EXT_DEPOP_DELAY_MS_T2=${CFG_DAC_EXT_DEPOP_DELAY_MS_T2}
    )
endif()

if (DEFINED CFG_RSL_MASTER)
    add_definitions(
        -DCFG_GPIO_RSL_MASTER_CLK=${CFG_GPIO_RSL_MASTER_CLK}
        -DCFG_GPIO_RSL_MASTER_DATA=${CFG_GPIO_RSL_MASTER_DATA}
    )
endif()

# HTTP Server
if (DEFINED CFG_BUILD_HTTPSERVER)
    set(CFG_BUILD_MICROHTTPD 1)
    set(CFG_BUILD_TRE 1)
endif()

if (DEFINED CFG_BUILD_MICROHTTPD)
    set(CFG_NET_ENABLE 1)
endif()

if (DEFINED CFG_BUILD_IOT)
    add_definitions(
        -DCFG_IOT_ENABLE
    )
endif()

# Network
if (DEFINED CFG_NET_ENABLE)
    set(CFG_BUILD_LWIP 1)

    add_definitions(
        -DCFG_NET_ENABLE
    )

    if (DEFINED CFG_NET_ETHERNET)
        add_definitions(
            -DCFG_NET_ETHERNET
            -DCFG_GPIO_ETHERNET=${CFG_GPIO_ETHERNET}
            -DCFG_NET_ETHERNET_POLL_INTERVAL=${CFG_NET_ETHERNET_POLL_INTERVAL}
            -DCFG_NET_ETHERNET_COUNT=${CFG_NET_ETHERNET_COUNT}
        )

        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_ETHERNET_LINK_INTR)

        if (DEFINED CFG_NET_ETHERNET_PHY_ADDR)
            add_definitions(
                -DCFG_NET_ETHERNET_PHY_ADDR=${CFG_NET_ETHERNET_PHY_ADDR}
            )
        else()
            add_definitions(
                -DCFG_NET_ETHERNET_PHY_ADDR=-1
            )
        endif()

        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_ETHERNET_WAKE_ON_LAN)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_ETHERNET_DHCP)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_ETHERNET_MULTI_INTERFACE_DHCP)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_ETHERNET_DHCP_BY_DNS)
        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_ETHERNET_AUTOIP)

        if (NOT DEFINED CFG_NET_ETHERNET_DHCP AND NOT DEFINED CFG_NET_ETHERNET_AUTOIP)
            add_definitions(
                -DCFG_NET_ETHERNET_IPADDR="${CFG_NET_ETHERNET_IPADDR}"
                -DCFG_NET_ETHERNET_NETMASK="${CFG_NET_ETHERNET_NETMASK}"
                -DCFG_NET_ETHERNET_GATEWAY="${CFG_NET_ETHERNET_GATEWAY}"
            )
        endif()

        ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_ETHERNET_MULTI_INTERFACE)
        #        if (DEFINED CFG_NET_ETHERNET_MULTI_INTERFACE)
        #            add_definitions(
        #                -DCFG_NET_ETHERNET_INTERFACE_0="${CFG_NET_ETHERNET_INTERFACE_0}"
        #                -DCFG_NET_ETHERNET_INTERFACE_1="${CFG_NET_ETHERNET_INTERFACE_1}"
    #            )
#        endif()

if (DEFINED CFG_NET_ETHERNET_MAC_ADDR_RANDOM)
    add_definitions(
        -DCFG_NET_ETHERNET_MAC_ADDR_RANDOM
    )
else()
    add_definitions(
        -DCFG_NET_ETHERNET_MAC_ADDR_DEFAULT=${CFG_NET_ETHERNET_MAC_ADDR_DEFAULT}
    )
endif()

if (DEFINED CFG_NET_ETHERNET_MAC_ADDR_STORAGE)
    add_definitions(
        -DCFG_NET_ETHERNET_MAC_ADDR_STORAGE
        -DCFG_NET_ETHERNET_MAC_ADDR_POS=${CFG_NET_ETHERNET_MAC_ADDR_POS}
        -DCFG_NET_ETHERNET_MAC_ADDR_VERIFY_CODE="${CFG_NET_ETHERNET_MAC_ADDR_VERIFY_CODE}"
    )

    if (DEFINED CFG_NET_ETHERNET_MAC_ADDR_NAND)
        add_definitions(
            -DCFG_NET_ETHERNET_MAC_ADDR_NAND
        )
    elseif (DEFINED CFG_NET_ETHERNET_MAC_ADDR_NOR)
        add_definitions(
            -DCFG_NET_ETHERNET_MAC_ADDR_NOR
        )
    elseif (DEFINED CFG_NET_ETHERNET_MAC_ADDR_SD0)
        add_definitions(
            -DCFG_NET_ETHERNET_MAC_ADDR_SD0
        )
    elseif (DEFINED CFG_NET_ETHERNET_MAC_ADDR_SD1)
        add_definitions(
            -DCFG_NET_ETHERNET_MAC_ADDR_SD1
        )
    endif()

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_ETHERNET_MAC_ADDR_UPGRADE)
endif()

if ($ENV{CFG_PLATFORM} STREQUAL openrtos)
    set(CFG_BUILD_MAC 1)

    add_definitions(
        -DCFG_NET_MAC_CLOCK_INVERSE=${CFG_NET_MAC_CLOCK_INVERSE}
        -DCFG_NET_MAC_CLOCK_DELAY=${CFG_NET_MAC_CLOCK_DELAY}
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_RTL8201FL)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_IP101GR)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_LAN8720)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_RTL8211F)

    if (DEFINED CFG_NET_RTL8304MB)
        add_definitions(
            -DCFG_NET_RTL8304MB
        )
        set(CFG_BUILD_RTL8304MB 1)
    endif()

    if (DEFINED CFG_NET_RTL8363NB)
        add_definitions(
            -DCFG_NET_RTL8363NB
        )
        set(CFG_BUILD_RTL8363NB 1)
    endif()

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_MAC_INIT_ON_BOOTLOADER)
endif()

if (DEFINED CFG_NET_AMEBA_SDIO)
    add_definitions(
        -DCFG_NET_AMEBA_SDIO
    )
    set(CFG_BUILD_NET 1)
    set(CFG_BUILD_AMEBA_SDIO 1)
endif()
else()
    add_definitions(
        -DCFG_NET_ETHERNET_PHY_ADDR=-1
        -DCFG_NET_ETHERNET_COUNT=1
    )
endif()

if (DEFINED CFG_NET_WIFI)
    add_definitions(
        -DCFG_NET_WIFI
        -DCFG_NET_WIFI_POLL_INTERVAL=${CFG_NET_WIFI_POLL_INTERVAL}
    )

    if (DEFINED CFG_NET_WIFI_8188CUS)
        set(CFG_BUILD_WIFI 1)
        add_definitions(
        -DCFG_NET_WIFI_8188CUS)
    elseif (DEFINED CFG_NET_WIFI_8188EUS)
        set(CFG_BUILD_WIFI_EUS 1)
        add_definitions(
        -DCFG_NET_WIFI_8188EUS)
	elseif (DEFINED CFG_NET_WIFI_8188EUS_NEW)
        set(CFG_BUILD_WIFI_EUS_NEW 1)
        add_definitions(
        -DCFG_NET_WIFI_8188EUS_NEW)
    elseif (DEFINED CFG_NET_WIFI_SDIO)
        set(CFG_BUILD_WIFI_SDIO 1)
        add_definitions(
        -DCFG_NET_WIFI_SDIO)
    elseif (DEFINED CFG_NET_WIFI_5G)
        set(CFG_BUILD_WIFI_5G 1)
        add_definitions(
        -DCFG_NET_WIFI_5G)
	elseif (DEFINED CFG_NET_WIFI_8188FTV)
        set(CFG_BUILD_WIFI_FTV 1)
        add_definitions(
        -DCFG_NET_WIFI_8188FTV)
    elseif (DEFINED CFG_NET_WIFI_MARVELL)
        set(CFG_BUILD_WIFI_MARVELL 1)
        add_definitions(
        -DCFG_NET_WIFI_MARVELL)
    endif()
endif()

if (DEFINED CFG_NET_WIFI_WPA)
    add_definitions(
        -DCFG_NET_WIFI_WPA
    )
    set(CFG_BUILD_WIFI_WPA 1)
    set(CFG_BUILD_OPENSSL 1)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_WIFI_MAY_NOT_EXIST)

if (DEFINED CFG_NET_WIFI_HOSTAPD)
    add_definitions(
        -DCFG_NET_WIFI_HOSTAPD
    )
    set(CFG_BUILD_WIFI_HOSTAPD 1)
    set(CFG_BUILD_OPENSSL 1)
    set(CFG_BUILD_DHCPS 1)
endif()

if (DEFINED CFG_NET_WIFI_HOSTAPD AND DEFINED CFG_NET_WIFI)
    add_definitions(
        -DCFG_NET_WIFI_IPADDR="${CFG_NET_WIFI_IPADDR}"
        -DCFG_NET_WIFI_NETMASK="${CFG_NET_WIFI_NETMASK}"
        -DCFG_NET_WIFI_GATEWAY="${CFG_NET_WIFI_GATEWAY}"
        -DCFG_NET_WIFI_SSID="${CFG_NET_WIFI_SSID}"
        -DCFG_NET_WIFI_CHANNEL="${CFG_NET_WIFI_CHANNEL}"
        -DCFG_NET_WIFI_HIDDENSSID="${CFG_NET_WIFI_HIDDENSSID}"
    )
endif()

if (DEFINED CFG_NET_WIFI_WPA_ENABLE)
    add_definitions(
        -DCFG_NET_WIFI_PW="${CFG_NET_WIFI_PW}"
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_WIFI_FIND_BEST_CHANNEL)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_WIFI_TRANS_LED)

if (DEFINED CFG_NET_WIFI_TRANS_LED_NUM)
    add_definitions(
        -DCFG_NET_WIFI_TRANS_LED_NUM=${CFG_NET_WIFI_TRANS_LED_NUM}
    )
endif()

if (DEFINED CFG_NET_WIFI_MP_SSID)
    add_definitions(
        -DCFG_NET_WIFI_MP_SSID="${CFG_NET_WIFI_MP_SSID}"
    )
endif()

if (DEFINED CFG_NET_WIFI_MP_PASSWORD)
    add_definitions(
        -DCFG_NET_WIFI_MP_PASSWORD="${CFG_NET_WIFI_MP_PASSWORD}"
    )
endif()

if (DEFINED CFG_NET_WIFI_MP_SECURITY)
    add_definitions(
        -DCFG_NET_WIFI_MP_SECURITY="${CFG_NET_WIFI_MP_SECURITY}"
    )
endif()

if (DEFINED CFG_NET_WIFI_MGR)
    add_definitions(
        -DCFG_BUILD_WIFI_MGR
    )
    set(CFG_BUILD_WIFI_MGR 1)
endif()

if (DEFINED CFG_NET_WIFI_80211N)
    add_definitions(
        -DCFG_NET_WIFI_80211N="${CFG_NET_WIFI_80211N}"
    )
endif()

if (DEFINED CFG_NET_HTTP)
    add_definitions(
        -DCFG_NET_HTTP
    )
    set(CFG_BUILD_POLARSSL 1)
    set(CFG_BUILD_CURL 1)
    set(CFG_BUILD_ZLIB 1)
endif()

if (DEFINED CFG_NET_FTP)
    add_definitions(
        -DCFG_NET_FTP
    )
    set(CFG_BUILD_CURL 1)
    set(CFG_BUILD_ZLIB 1)

    if (DEFINED CFG_NET_FTP_SERVER)
        add_definitions(
            -DCFG_NET_FTP_SERVER
        )
    endif()
endif()

if (DEFINED CFG_NET_TFTP)
    add_definitions(
        -DCFG_NET_TFTP
    )
    set(CFG_BUILD_CURL 1)
    set(CFG_BUILD_ZLIB 1)
endif()

if (DEFINED CFG_NET_SMTP)
    add_definitions(
        -DCFG_NET_SMTP
    )
    set(CFG_BUILD_CURL 1)
    set(CFG_BUILD_ZLIB 1)
endif()

if (DEFINED CFG_NET_UPNP)
    add_definitions(
        -DCFG_NET_UPNP
    )
    set(CFG_BUILD_UPNP 1)
endif()

if (DEFINED CFG_NET_URENDER)
    add_definitions(
        -DCFG_NET_URENDER
    )
    set(CFG_BUILD_URENDER  1)
endif()

if (DEFINED CFG_BUILD_URENDER)
    add_definitions(
        -DCFG_NET_URENDER
        -DCFG_NET_URENDER_CONFIG="${CFG_NET_URENDER_CONFIG}"
        -DCFG_NET_URENDER_SYSTEMNAME="${CFG_NET_URENDER_SYSTEMNAME}"
        -DCFG_NET_URENDER_PORT="${CFG_NET_URENDER_PORT}"
        -DCFG_NET_URENDER_FRIENDLYNAME="${CFG_NET_URENDER_FRIENDLYNAME}"
        -DCFG_NET_URENDER_MENUFACTURER="${CFG_NET_URENDER_MENUFACTURER}"
        -DCFG_NET_URENDER_MENUFACTURERURL="${CFG_NET_URENDER_MENUFACTURERURL}"
        -DCFG_NET_URENDER_MODELDESCRIPTION="${CFG_NET_URENDER_MODELDESCRIPTION}"
        -DCFG_NET_URENDER_MODELNAME="${CFG_NET_URENDER_MODELNAME}"
        -DCFG_NET_URENDER_MODELNUMBER="${CFG_NET_URENDER_MODELNUMBER}"
        -DCFG_NET_URENDER_MODELURL="${CFG_NET_URENDER_MODELURL}"
        -DCFG_NET_URENDER_SERIALNUMBER="${CFG_NET_URENDER_SERIALNUMBER}"
        -DCFG_NET_URENDER_WEBSERVER_HOMEPAGE="${CFG_NET_URENDER_WEBSERVER_HOMEPAGE}"
        -DCFG_NET_URENDER_CONFIG_INI_FILE="${CFG_NET_URENDER_CONFIG_INI_FILE}"
    )
endif()

if (DEFINED CFG_NET_WAC)
    add_definitions(
        -DCFG_NET_WAC
    )
    set(CFG_BUILD_WAC 1)
endif()

if (DEFINED CFG_NET_SIPROXD)
    add_definitions(
        -DCFG_NET_SIPROXD
    )
    set(CFG_BUILD_SIPROXD 1)
endif()

if (DEFINED CFG_NET_DNS_SD)
    add_definitions(
        -DCFG_NET_DNS_SD
    )
    set(CFG_BUILD_DNS_SD 1)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DNS_SD_RESPONDER)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DNS_SD_DAEMON)
if (DEFINED CFG_NET_SHAIRPORT)
    add_definitions(
        -DCFG_NET_SHAIRPORT
    )
    set(CFG_BUILD_AO 1)
    set(CFG_BUILD_OPENSSL 1)
    set(CFG_BUILD_SHAIRPORT 1)
endif()

if (DEFINED CFG_NET_SHAIRPORT_DACP)
    add_definitions(
        -DCFG_NET_SHAIRPORT_DACP
    )
    set(CFG_BUILD_AO 1)
    set(CFG_BUILD_OPENSSL 1)
    set(CFG_BUILD_SHAIRPORT_DACP 1)
endif()
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_BLOCK_UDP)

if (DEFINED CFG_SIP_SERVER_TEST)
    add_definitions(
        -DCFG_SIP_SERVER_TEST
        -DCFG_REGISTER_ACCOUNT="${CFG_REGISTER_ACCOUNT}"
        -DCFG_REGISTER_DOMAIN="${CFG_REGISTER_DOMAIN}"
        -DCFG_REGISTER_PWD="${CFG_REGISTER_PWD}"
        -DCFG_REGISTER_ACCOUNT_2="${CFG_REGISTER_ACCOUNT_2}"
        -DCFG_REGISTER_DOMAIN_2="${CFG_REGISTER_DOMAIN_2}"
        -DCFG_REGISTER_PWD_2="${CFG_REGISTER_PWD_2}"
        -DCFG_SIP_CALLOUT="${CFG_SIP_CALLOUT}"
    )
    if (DEFINED CFG_SIP_INDOOR_TEST)
        add_definitions(
            -DCFG_SIP_INDOOR_TEST
        )
    elseif (DEFINED CFG_SIP_LOBBY_TEST)
        add_definitions(
            -DCFG_SIP_LOBBY_TEST
        )
    elseif (DEFINED CFG_SIP_OUTDOOR_TEST)
        add_definitions(
            -DCFG_SIP_OUTDOOR_TEST
        )
    endif()
endif()

if (DEFINED CFG_PING)
    add_definitions(
        -DCFG_PING
    )
    set(CFG_BUILD_PING  1)
endif()

if (DEFINED CFG_JSON)
    add_definitions(
        -DCFG_JSON
    )
    set(CFG_BUILD_JSON  1)
endif()

if (DEFINED CFG_P2P)
    add_definitions(
        -DCFG_P2P
        -DCFG_P2P_CALLOUT="${CFG_P2P_CALLOUT}"
        -DCFG_P2P_DOMAIN="${CFG_P2P_DOMAIN}"
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_SIP_PROXD_TEST)
else()
    add_definitions(
        -DCFG_NET_ETHERNET_PHY_ADDR=-1
        -DCFG_NET_ETHERNET_COUNT=1
    )
endif()

# Task
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_TASK_DRIVE_PROBE)

if (DEFINED CFG_TASK_HDMI_PROBE)
    add_definitions(
        -DCFG_TASK_HDMI_PROBE
        -DCFG_TASK_HDMI_PROBE_PERIOD
    )
endif()

if (DEFINED CFG_BUILD_GTEST)
    add_definitions(
        -DGTEST_HAS_POSIX_RE=0
    )
endif()

# Debug
if (DEFINED CFG_DBG_PRINTBUF_SIZE)
    add_definitions(
        -DCFG_DBG_PRINTBUF_SIZE=${CFG_DBG_PRINTBUF_SIZE}
    )
else()
    add_definitions(
        -DCFG_DBG_PRINTBUF_SIZE=0
    )
endif()

if (DEFINED CFG_DBG_PRINTBUF)
    add_definitions(
        -DCFG_DBG_PRINTBUF
    )
elseif (DEFINED CFG_DBG_SWUART)
    add_definitions(
        -DCFG_DBG_SWUART
        -DCFG_SWUART_BAUDRATE=${CFG_SWUART_BAUDRATE}
    )

#elseif (DEFINED CFG_DBG_SWUART_CODEC)
#    add_definitions(
#        -DCFG_DBG_SWUART_CODEC
#        -DCFG_SWUART_CODEC_BAUDRATE=${CFG_SWUART_CODEC_BAUDRATE}
#        -DCFG_SWUART_CODEC_GPIO=${CFG_SWUART_CODEC_GPIO}
#    )

elseif (DEFINED CFG_DBG_UART0)
    add_definitions(
        -DCFG_DBG_UART0
    )
elseif (DEFINED CFG_DBG_UART1)
    add_definitions(
        -DCFG_DBG_UART1
    )
elseif (DEFINED CFG_DBG_LCDCONSOLE)
    add_definitions(
        -DCFG_DBG_LCDCONSOLE
    )
elseif (DEFINED CFG_DBG_OSDCONSOLE)
    add_definitions(
        -DCFG_DBG_OSDCONSOLE
    )
endif()

if (DEFINED CFG_DBG_NETCONSOLE)
    add_definitions(
        -DCFG_DBG_NETCONSOLE
        -DCFG_DBG_NETCONSOLE_PORT=${CFG_DBG_NETCONSOLE_PORT}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_BLUESCREEN)

if (DEFINED CFG_DBG_MEMLEAK)
    add_definitions(
        -DCFG_DBG_MEMLEAK
        -DCFG_DBG_MEMLEAK_LEN=${CFG_DBG_MEMLEAK_LEN}
    )
endif()

if (DEFINED CFG_DBG_RMALLOC)
    add_definitions(
        -DCFG_DBG_RMALLOC
        -DMALLOC_DEBUG
    )
endif()

if (DEFINED CFG_DBG_STATS)
    add_definitions(
        -DCFG_DBG_STATS
        -DCFG_DBG_STATS_PERIOD=${CFG_DBG_STATS_PERIOD}
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_STATS_HEAP)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_STATS_TASK_LIST)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_STATS_TASK_TIME)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_STATS_MEM_BANDWIDTH)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_STATS_TCPIP)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_STATS_FAT)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_STATS_NTFS)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_TRACE_ANALYZER)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_VCD)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_TRACE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_CLI)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_ENABLE_UART_CLI)

if (DEFINED CFG_DBG_NETCLI_PORT)
    add_definitions(
        -DCFG_DBG_NETCLI_PORT=${CFG_DBG_NETCLI_PORT}
    )
endif()

# Upgrade
add_definitions(
    -DCFG_UPGRADE_FILENAME="${CFG_UPGRADE_FILENAME}"
    -DCFG_UPGRADE_FILENAME_LIST="${CFG_UPGRADE_FILENAME_LIST}"
    -DCFG_UPGRADE_ENC_KEY=${CFG_UPGRADE_ENC_KEY}
)

if (DEFINED CFG_UPGRADE_BOOTLOADER OR DEFINED CFG_UPGRADE_IMAGE OR DEFINED CFG_UPGRADE_DATA)
    set(CFG_BUILD_UPGRADE 1)
endif()

if (DEFINED CFG_UPGRADE_PRESSKEY)
    add_definitions(
        -DCFG_UPGRADE_PRESSKEY
        -DCFG_UPGRADE_PRESSKEY_NUM=${CFG_UPGRADE_PRESSKEY_NUM}
        -DCFG_UPGRADE_PRESSKEY_DELAY=${CFG_UPGRADE_PRESSKEY_DELAY}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_UPGRADE_OPEN_FILE)

if (DEFINED CFG_UPGRADE_IMAGE_NAND)
    add_definitions(
        -DCFG_UPGRADE_IMAGE_NAND
    )

elseif (DEFINED CFG_UPGRADE_IMAGE_NOR)
    add_definitions(
        -DCFG_UPGRADE_IMAGE_NOR
    )
elseif (DEFINED CFG_UPGRADE_IMAGE_SD0)
    add_definitions(
        -DCFG_UPGRADE_IMAGE_SD0
    )
elseif (DEFINED CFG_UPGRADE_IMAGE_SD1)
    add_definitions(
        -DCFG_UPGRADE_IMAGE_SD1
    )
endif()

if (DEFINED CFG_UPGRADE_IMAGE)
    add_definitions(
        -DCFG_UPGRADE_IMAGE_POS=${CFG_UPGRADE_IMAGE_POS}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_UPGRADE_DELETE_PKGFILE_AFTER_FINISH)

if (DEFINED CFG_UPGRADE_DELAY_AFTER_FINISH)
    add_definitions(
        -DCFG_UPGRADE_DELAY_AFTER_FINISH=${CFG_UPGRADE_DELAY_AFTER_FINISH}
    )
endif()

if (DEFINED CFG_UPGRADE_USB_TIMEOUT)
    add_definitions(
        -DCFG_UPGRADE_USB_DETECT_TIMEOUT=${CFG_UPGRADE_USB_DETECT_TIMEOUT}
        -DCFG_UPGRADE_USB_TIMEOUT=${CFG_UPGRADE_USB_TIMEOUT}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_UPGRADE_USB_DEVICE)

if (DEFINED CFG_UPGRADE_RECOVERY)
    add_definitions(
        -DCFG_UPGRADE_RECOVERY
        -DCFG_UPGRADE_RECOVERY_PRESSKEY_NUM=${CFG_UPGRADE_RECOVERY_PRESSKEY_NUM}
        -DCFG_UPGRADE_RECOVERY_PRESSKEY_DELAY=${CFG_UPGRADE_RECOVERY_PRESSKEY_DELAY}
        -DCFG_UPGRADE_RECOVERY_SERVER_ADDR="${CFG_UPGRADE_RECOVERY_SERVER_ADDR}"
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_UPGRADE_RECOVERY_TFTP)
endif()

if (DEFINED CFG_UPGRADE_RECOVERY_SSID)
    add_definitions(
        -DCFG_UPGRADE_RECOVERY_SSID="${CFG_UPGRADE_RECOVERY_SSID}"
    )
endif()

if (DEFINED CFG_UPGRADE_RECOVERY_LED)
    add_definitions(
        -DCFG_UPGRADE_RECOVERY_LED
        -DCFG_UPGRADE_RECOVERY_LED_NUM="${CFG_UPGRADE_RECOVERY_LED_NUM}"
    )
endif()

if (DEFINED CFG_UPGRADE_RESET_FACTORY)
    add_definitions(
        -DCFG_UPGRADE_RESET_FACTORY
        -DCFG_UPGRADE_RESET_FACTORY_PRESSKEY_NUM=${CFG_UPGRADE_RESET_FACTORY_PRESSKEY_NUM}
        -DCFG_UPGRADE_RESET_FACTORY_PRESSKEY_DELAY=${CFG_UPGRADE_RESET_FACTORY_PRESSKEY_DELAY}
    )
endif()

if (DEFINED CFG_UPGRADE_BACKUP_PACKAGE)
    add_definitions(
        -DCFG_UPGRADE_BACKUP_PACKAGE
        -DCFG_UPGRADE_BACKUP_PACKAGE_POS=${CFG_UPGRADE_BACKUP_PACKAGE_POS}
    )
endif()

if (DEFINED CFG_UPGRADE_MARK_POS)
    add_definitions(
        -DCFG_UPGRADE_MARK_POS=${CFG_UPGRADE_MARK_POS}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED_VALUE(CFG_UPGRADE_FROM_UART)
ITE_ADD_DEFINITIONS_IF_DEFINED_VALUE(CFG_UPGRADE_UART0)
ITE_ADD_DEFINITIONS_IF_DEFINED_VALUE(CFG_UPGRADE_UART1)

# user app
ITE_ADD_DEFINITIONS_IF_DEFINED_VALUE(CFG_LCD_POWER_ENABLE)
ITE_ADD_DEFINITIONS_IF_DEFINED_VALUE(CFG_GPIO_LCD_POWER_CTL ${CFG_GPIO_LCD_POWER_CTL})
ITE_ADD_DEFINITIONS_IF_DEFINED_VALUE(CFG_GPIO_LCD_RESET_CTL ${CFG_GPIO_LCD_RESET_CTL})

# Gadget
if (DEFINED CFG_USB_DEVICE)
    add_definitions(
        -DCFG_USB_DEVICE
        -DCFG_GPIO_USB_ID_PIN=${CFG_GPIO_USB_ID_PIN}
        -DCFG_USB_VENDOR_ID=${CFG_USB_VENDOR_ID}
        -DCFG_USB_PRODUCT_ID=${CFG_USB_PRODUCT_ID}
        -DCFG_USBD_STR_MANUFACTURER="${CFG_USBD_STR_MANUFACTURER}"
        -DCFG_USBD_STR_PRODUCT="${CFG_USBD_STR_PRODUCT}"
        -DCFG_USBD_STR_SERIAL_NUMBER="${CFG_USBD_STR_SERIAL_NUMBER}"
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_USB_DEVICE_USB0)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_USB_DEVICE_USB1)
    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_USB_DEVICE_NEW)

    if (DEFINED CFG_USB_DEVICE_DRIVE)
        add_definitions(
            -DCFG_USB_DEVICE_DRIVE="${CFG_USB_DEVICE_DRIVE}"
        )
    endif()

endif()

if (DEFINED CFG_USBD_MASS_STORAGE)
    add_definitions(
        -DCFG_USBD_MASS_STORAGE
    )

    add_definitions(
        -DCFG_USB_DEVICE_DISKS=${CFG_USB_DEVICE_DISKS}
        -DCFG_USBD_LUN_VENDOR_ID=${CFG_USBD_LUN_VENDOR_ID}
        -DCFG_USBD_LUN0_PRODUCT_ID=${CFG_USBD_LUN0_PRODUCT_ID}
        -DCFG_USBD_LUN1_PRODUCT_ID=${CFG_USBD_LUN1_PRODUCT_ID}
        -DCFG_USBD_LUN2_PRODUCT_ID=${CFG_USBD_LUN2_PRODUCT_ID}
        -DCFG_USBD_LUN3_PRODUCT_ID=${CFG_USBD_LUN3_PRODUCT_ID}
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_USBD_ACM)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_DBG_USB)

if (DEFINED CFG_USBD_IDB AND DEFINED CFG_USB_DEVICE_NEW)
    add_definitions(
        -DCFG_USBD_IDB
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_USBD_UVC)


if (DEFINED CFG_USBD_FILE_STORAGE)
    add_definitions(
        -DCFG_USBD_FILE_STORAGE
    )

    add_definitions(
        -DCFG_USB_DEVICE_DISKS=${CFG_USB_DEVICE_DISKS}
        -DCFG_USBD_LUN_VENDOR_ID=${CFG_USBD_LUN_VENDOR_ID}
        -DCFG_USBD_LUN0_PRODUCT_ID=${CFG_USBD_LUN0_PRODUCT_ID}
        -DCFG_USBD_LUN1_PRODUCT_ID=${CFG_USBD_LUN1_PRODUCT_ID}
        -DCFG_USBD_LUN2_PRODUCT_ID=${CFG_USBD_LUN2_PRODUCT_ID}
        -DCFG_USBD_LUN3_PRODUCT_ID=${CFG_USBD_LUN3_PRODUCT_ID}
    )

    ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_USBD_IDB)
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_COMPRESS_AUDIO_PLUGIN)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_BUILD_AUDIO_MGR)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_MGR_M4A)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_MGR_WAV_HD)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_MGR_LRC)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_MGR_RESAMPLE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_AUDIO_MGR_PARSING_MP3)

if (NOT DEFINED CFG_CHIP_PKG_IT9852)
    add_definitions(
        -DCFG_NORMAL_BUFFER_MODE
    )
endif()

ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_CTRLBOARD_REDUCE_PBUFSIZE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_NET_WIFI_REDEFINE)
ITE_ADD_DEFINITIONS_IF_DEFINED(CFG_ENABLE_ROTATE)

# SDK
if (DEFINED CFG_GENERATE_VERSION_TWEAK)
    string(TIMESTAMP CFG_VERSION_TWEAK %Y%m%d)
    #message("CFG_VERSION_TWEAK=${CFG_VERSION_TWEAK}")
endif()

if (DEFINED CFG_BUILD_DEBUG)
    set(CMAKE_BUILD_TYPE "Debug")
    add_definitions(
        -DCFG_BUILD_DEBUG
    )
elseif (DEFINED CFG_BUILD_DEBUGREL)
    set(CMAKE_BUILD_TYPE "RelWithDebInfo")
    add_definitions(
        -DCFG_BUILD_DEBUGREL
    )
elseif (DEFINED CFG_BUILD_RELEASE)
    set(CMAKE_BUILD_TYPE "Release")
    add_definitions(
        -DCFG_BUILD_RELEASE
    )
elseif (DEFINED CFG_BUILD_MINSIZEREL)
    set(CMAKE_BUILD_TYPE "MinSizeRel")
    add_definitions(
        -DCFG_BUILD_MINSIZEREL
    )
endif()

add_definitions(
    -DCFG_VERSION_MAJOR=${CFG_VERSION_MAJOR}
    -DCFG_VERSION_MINOR=${CFG_VERSION_MINOR}
    -DCFG_VERSION_PATCH=${CFG_VERSION_PATCH}
    -DCFG_VERSION_CUSTOM=${CFG_VERSION_CUSTOM}
    -DCFG_VERSION_TWEAK=${CFG_VERSION_TWEAK}
    -DCFG_VERSION_MAJOR_STR="${CFG_VERSION_MAJOR}"
    -DCFG_VERSION_MINOR_STR="${CFG_VERSION_MINOR}"
    -DCFG_VERSION_PATCH_STR="${CFG_VERSION_PATCH}"
    -DCFG_VERSION_CUSTOM_STR="${CFG_VERSION_CUSTOM}"
    -DCFG_VERSION_TWEAK_STR="${CFG_VERSION_TWEAK}"
    -DCFG_SYSTEM_NAME="${CFG_SYSTEM_NAME}"
    -DCFG_MANUFACTURER="${CFG_MANUFACTURER}"
    -DCFG_PROJECT_NAME="${CMAKE_PROJECT_NAME}"
)

# Global include paths
include_directories(
    ${PROJECT_SOURCE_DIR}/$ENV{CFG_PLATFORM}/include
    ${PROJECT_SOURCE_DIR}/sdk/include
)

if ($ENV{CFG_PLATFORM} STREQUAL win32)
    link_directories(
        ${PROJECT_SOURCE_DIR}/build/$ENV{CFG_PLATFORM}/lib
    )
else()
    link_directories(
        ${PROJECT_SOURCE_DIR}/build/$ENV{CFG_PLATFORM}/lib/${CFG_CPU_NAME}
    )
endif()

# data
file(MAKE_DIRECTORY
    ${CMAKE_BINARY_DIR}/data/private/backup/${CFG_PUBLIC_DRIVE}
    ${CMAKE_BINARY_DIR}/data/public
    ${CMAKE_BINARY_DIR}/data/temp
)

#IPTV
if (DEFINED CFG_IPTV_TX)
    add_definitions(
        -DCFG_IPTV_TX
    )
endif()

if (DEFINED CFG_IPTV_RX)
    add_definitions(
        -DCFG_IPTV_RX
    )
endif()
