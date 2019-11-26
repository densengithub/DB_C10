#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "ite/ith.h"
#include "ite/itp.h"

#include "i2s/i2s.h"
#include "i2s_reg_970.h"

typedef signed   long long s64;
typedef signed   int       s32;
typedef signed   short     s16;
typedef signed   char      s8;
typedef unsigned long long u64;
typedef unsigned int       u32;
typedef unsigned short     u16;
typedef unsigned char      u8;

/* ************************************************************************** */
/* platform control */
#define I2S_SLAVE_MODE      0
#ifdef ENABLE_I2S_INTERNAL_CODEC
    #define I2S_INTERNAL_CODEC  1
#else
    #define I2S_INTERNAL_CODEC  0
#endif
#ifdef ENABLE_HDMI_GPIO_IN_SET
    #define I2S_HDMI_RX_CLK     1
#else
    #define I2S_HDMI_RX_CLK     0
#endif
//#define I2S_DEBUG_SET_CLOCK
//#define I2S_DEBUG_DEINIT_DAC_COST

/* ************************************************************************** */
#define TV_CAL_DIFF_MS(TIME2, TIME1) ((((TIME2.tv_sec * 1000000UL) + TIME2.tv_usec) - ((TIME1.tv_sec * 1000000UL) + TIME1.tv_usec)) / 1000)

//#define DEBUG_PRINT printf
#define DEBUG_PRINT(...)

#ifdef _WIN32
    #define asm()
#endif

/* ************************************************************************** */

static int _i2s_DA_running = 0;
static int _i2s_AD_running = 0;
static int _i2s_DA_mute    = 0;
static int _i2s_AD_mute    = 0;
static int _bar_mute_flag  = 0;
static u8 sample_width;
static pthread_mutex_t I2S_MUTEX = PTHREAD_MUTEX_INITIALIZER;

/* ************************************************************************** */
u32 I2S_DA32_GET_RP(void)
{
    return ithReadRegA(I2S_REG_OUT_RDPTR);
}

u32 I2S_DA32_GET_WP(void)
{
    return ithReadRegA(I2S_REG_OUT_WRPTR);
}

void I2S_DA32_SET_WP(u32 data32)
{
    ithWriteRegA(I2S_REG_OUT_WRPTR, data32);
}

void I2S_DA32_WAIT_RP_EQUAL_WP(int wait)
{
    u32 cnt=0;
    u32 WP,RP,dataout,bfferlen;

    if(wait){

        /* Enable the last pointer function */
        ithWriteRegMaskA(I2S_REG_OUT_CTRL, 1<<2, 1<<2);
        /*alignment 8bit*/
        bfferlen = ithReadRegA(I2S_REG_OUT_LEN)+1;
        /* if enable, then wait the RW=WP, */
        do
        {
            if(cnt++>100)   break;
            if(!(ithReadRegA(I2S_REG_OUT_CTRL)&0x4)) break;
            usleep(40000);
            WP=I2S_DA32_GET_WP();
            RP=I2S_DA32_GET_RP();
            dataout = (WP >= RP) ? (WP - RP):(bfferlen - RP + WP);
        }while(dataout >= 128);

        if(cnt++>5000) printf("I2S# waring, wait for RP=WP timeout.\n");

    }
    /* Disable this function,and then set WP */
    ithWriteRegMaskA(I2S_REG_OUT_CTRL, 0, 1<<2);


}

u32 I2S_AD32_GET_RP(void)
{
    return ithReadRegA(I2S_REG_IN1_RDPTR);
}

u32 I2S_AD32_GET_WP(void)
{
    return ithReadRegA(I2S_REG_IN1_WRPTR);;
}

void I2S_AD32_SET_RP(u32 data32)
{
    ithWriteRegA(I2S_REG_IN1_RDPTR, data32);
}

u32 I2S_AD32_GET_HDMI_RP(void)
{//HDMI RX
    return ithReadRegA(I2S_REG_IN2_RDPTR);
}

u32 I2S_AD32_GET_HDMI_WP(void)
{//HDMI RX
    return ithReadRegA(I2S_REG_IN2_WRPTR);
}

void I2S_AD32_SET_HDMI_RP(u32 data32)
{//HDMI RX
    ithWriteRegA(I2S_REG_IN2_RDPTR, data32);
}

u32 i2s_is_DAstarvation(void)
{
    if(ithReadRegA(I2S_REG_OUT_STATUS1) & 0x1) {
        return 1;
    }
    else {
        return 0;
    }
}

static void _i2s_power_on(void)
{
    u32 data32;
    /* enable audio clock for I2S modules */
    /* NOTE: we SHOULD enable audio clock before toggling 0x003A/3C/3E */
    /* enable PLL2 for audio */
    data32 = ithReadRegA(MMP_AUDIO_CLOCK_REG_40);
    data32 |= (1 << 17); /* enable AMCLK (system clock for wolfson chip) */
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_40, data32);

    data32 = ithReadRegA(MMP_AUDIO_CLOCK_REG_3C);  //Audio Clock Register 1
    data32 |= (1 << 19); /* enable ZCLK  (IIS DAC clock)                 */
    data32 |= (1 << 21); /* enable M7CLK (memory clock for DAC)          */
    data32 |= (1 << 23); /* enable M8CLK (memory clock for ADC)          */
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, data32);


    /* 970 */
    /* should enable audio clock by 0x003C, and enable PLL3 for audio */
}

static void _i2s_power_off(void)
{
    u32 data32;
    data32 = ithReadRegA(MMP_AUDIO_CLOCK_REG_40);
    data32 &= ~(1 << 17); /* disable AMCLK (system clock for wolfson chip) */
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_40, data32);

    /* disable audio clock for I2S modules */
    data32 = ithReadRegA(MMP_AUDIO_CLOCK_REG_3C);  //Audio Clock Register 1
    data32 &= ~(1<<19); /* disable ZCLK  (IIS DAC clock)                 */
    data32 &= ~(1<<21); /* disable M7CLK (memory clock for DAC)          */
    data32 &= ~(1<<23); /* disable M8CLK (memory clock for ADC)          */
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, data32);
}

static void _i2s_reset(void)
{
    u32 data32;

    if(_i2s_DA_running && itp_codec_get_DA_running()) {
        printf("I2S# DAC is running, skip reset I2S !\n");
        return;
    }
    if(_i2s_AD_running && itp_codec_get_AD_running()) {
        printf("I2S# ADC is running, skip reset I2S !\n");
        return;
    }

#if 1   //FixMe: should 970 use???
    /*Disable PLLL2*/
    data32 = ithReadRegA(0xD8000114);
    data32 |= (0x1 << 31);
    ithWriteRegA(0xD8000114,data32);

    ithDelay(100); /* FIXME: dummy loop */;

    //data32 = ithReadRegA(0xD800011C);
    //data32 |= (0x1 << 31);
    //ithWriteRegA(0xD800011C,data32);

    //ithDelay(100); /* FIXME: dummy loop */;

/*  data32 = ithReadRegA(MMP_AUDIO_CLOCK_REG_3C);
    data32 |= (0xF << 28);//reset DA & AD engine
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, data32);

    ithDelay(100); /* FIXME: dummy loop */;

    data32 = ithReadRegA(MMP_AUDIO_CLOCK_REG_3C);
    data32 &= ~(0xF << 28);//normal option DA & AD engine
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, data32);

    ithDelay(100); /* FIXME: dummy loop */;
#endif

    printf("I2S# %s\n", __func__);

}

static void _i2s_aws_sync()
{
    ithWriteRegA(I2S_REG_CODEC_SET,
    ( 1                                  << 31) //ADCWS/DACWS syn
    |( I2S_HDMI_RX_CLK                   <<  4) //CLK/WS/DATA1~4 from HDMI RX
    |( I2S_INTERNAL_CODEC                <<  2) //select External/Internal DAC
    |( I2S_SLAVE_MODE                    <<  1) //IIS Output Slave/Master Mode
    |( I2S_SLAVE_MODE                    <<  0) //IIS Input Slave/Master Mode
    );
    usleep(10);

    ithWriteRegMaskA(I2S_REG_CODEC_SET, 0, 1 << 31);
}

/***********************************************************************************
** Set CODEC Clock
** Assume the oversample rate is 256.
** Suggest system clock is (int)(CLK*2/256/sample_rate+1) / 2 * 256 * sample_rate

** For example:
** In 44100 sample rate, the system clock is 33MHz, the system clock should be
** (33000000*2/256/44100+1) / 2 * 256 * 44100 = 33868800.

** The PLL numurator is (int)(33868800 * 2 * 512 / 13000000 + 1) / 2 - 1 = 0x535.
***********************************************************************************/
#ifdef ENABLE_I2S_INTERNAL_CODEC
static void _i2s_set_clock(u32 demanded_sample_rate , u8 *demanded_sample_width)
{
    u8 data8=0x0;

    if(_i2s_DA_running && itp_codec_get_DA_running()) {
        printf("I2S# DAC is running, skip set clock !\n");
        return;
    }
    if(_i2s_AD_running && itp_codec_get_AD_running()) {
        printf("I2S# ADC is running, skip set clock !\n");
        return;
    }

    printf("I2S# %s, demanded_sample_rate: %u\n", __func__, demanded_sample_rate);
    #if I2S_SLAVE_MODE
    ithWriteRegA(0xD8000120, 0x20424001);
    ithWriteRegA(0xD8000124, 0x80000000);
    ithWriteRegA(0xD8000124, 0xF3000000);
    ithDelay(220);          //IDLE 220
    ithWriteRegA(0xD8000124, 0x80000000);

    //ZCLK,AMCLK (PLL3)
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, 0x80000000); // innter CodeC reset
    ithDelay(10);
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, 0x72A8C804); //i2sreg,i2sda,i2sad reset, 0x003C[31:28]
    ithDelay(10);
    //WRITE(0xD800003c, 0x02a8c804); //ZCLK, PLL2_OUT1 /4 ,[9:0]:zclk_ratio (i2s: 3.072MHz,WS=48K)
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, 0x02A00000);   // diable ZCLK
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_40, 0x0002C801);   //AMCLK PLL2_OUT1 /1 ,[6:0]:amclk_ratio (AMCLK: 12.288MHz)
    #else
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_40, 0x0002a801); //AMCLK PLL2_OUT1 /1 ,[9:0]:amclk_ratio (1)

    if(demanded_sample_rate == 32000){
        //12.288 MHz
        ithWriteRegA(0xD8000110,0xa02e2d01);
        ithWriteRegA(0xD8000114,0x800147ae);    //fraction 147ae
        ithWriteRegA(0xD8000114,0xF03147ae);    //durarion = 03
        ithDelay(220);
        ithWriteRegA(0xD8000114,0x800147ae);

    }else if(demanded_sample_rate == 44100){
        //11.2896 MHz
        ithWriteRegA(0xD8000110,0xa0525701);
        ithWriteRegA(0xD8000114,0x800D97F6);
        ithWriteRegA(0xD8000114,0xF03D97F6);
        ithDelay(220);
        ithWriteRegA(0xD8000114,0x800D97F6);
    }else{
        //12 MHz
        ithWriteRegA(0xD8000110,0x20404001);
        ithWriteRegA(0xD8000114,0x80000000);
        ithWriteRegA(0xD8000114,0xF3000000);
        ithDelay(220);
        ithWriteRegA(0xD8000114,0x80000000);
    }

    switch (demanded_sample_rate)
    {
        case 48000: data8 = 0x5 ;  *demanded_sample_width = 0x18; break;
        case 44100: data8 = 0x4 ;  *demanded_sample_width = 0x1F; break;
        case 32000: data8 = 0x6 ;  *demanded_sample_width = 0x1F; break;
        case 24000: data8 = 0xA ;  *demanded_sample_width = 0x18; break;
        case 22050: data8 = 0x8 ;  *demanded_sample_width = 0x21; break;
        case 16000: data8 = 0xF ;  *demanded_sample_width = 0x18; break;
        case 12000: data8 = 0xA ;  *demanded_sample_width = 0x31; break;
        case 11025: data8 = 0x10;  *demanded_sample_width = 0x21; break;
        case  8000: data8 = 0x1E;  *demanded_sample_width = 0x18; break;
        default   : printf("not support sampling rate : %d\n",demanded_sample_rate);break;
    }
    //printf("data8 = 0x%X demanded_sample_width =0x%X\n",data8,*demanded_sample_width);

    //if(_cold_init) ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C,(0xF2A88800|data8));//zclk_ratio
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C,(0xB2A88800|data8));//zclk_ratio

    ithDelay(10);
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C,(0x02A88800|data8));
    #endif
}

#else
static void _i2s_set_clock(u32 demanded_sample_rate , u8 *demanded_sample_width)
{
    u8  zclk_ratio =0x0;
    u8  amclk_ratio=0x0;
    if(_i2s_DA_running && itp_codec_get_DA_running()) {
        printf("I2S# DAC is running, skip set clock !\n");
        return;
    }
    if(_i2s_AD_running && itp_codec_get_AD_running()) {
        printf("I2S# ADC is running, skip set clock !\n");
        return;
    }

    printf("I2S# %s, demanded_sample_rate: %u\n", __func__, demanded_sample_rate);
    #if I2S_SLAVE_MODE
    ithWriteRegA(0xD8000120, 0x20424001);
    ithWriteRegA(0xD8000124, 0x80000000);
    ithWriteRegA(0xD8000124, 0xF3000000);
    ithDelay(220);          //IDLE 220
    ithWriteRegA(0xD8000124, 0x80000000);

    //ZCLK,AMCLK (PLL3)
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, 0x80000000); // innter CodeC reset
    ithDelay(10);
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, 0x72A8C804); //i2sreg,i2sda,i2sad reset, 0x003C[31:28]
    ithDelay(10);
    //WRITE(0xD800003c, 0x02a8c804); //ZCLK, PLL2_OUT1 /4 ,[9:0]:zclk_ratio (i2s: 3.072MHz,WS=48K)
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C, 0x02A00000);   // diable ZCLK
    switch (demanded_sample_rate)
    {
        case 48000: amclk_ratio = 0x1; break;
        case 24000: amclk_ratio = 0x2; break;
        case 12000: amclk_ratio = 0x4; break;
        case 44100: amclk_ratio = 0x1; break;
        case 22050: amclk_ratio = 0x2; break;
        case 11025: amclk_ratio = 0x4; break;
        case 32000: amclk_ratio = 0x1; break;
        case 16000: amclk_ratio = 0x2; break;
        case  8000: amclk_ratio = 0x4; break;
        default   : printf("not support sampling rate : %d\n",demanded_sample_rate);break;
    }
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_40, 0x0002C800|amclk_ratio);   //AMCLK PLL2_OUT1 /1 ,[6:0]:amclk_ratio (AMCLK: 12.288MHz)
    itp_codec_set_DAC_slave_mode(0);//set DAC master mode;

    #else
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_40, 0x0002a800); //AMCLK PLL2_OUT1 /1 ,[9:0]:amclk_ratio (1)

    if(demanded_sample_rate==48000||demanded_sample_rate==24000||demanded_sample_rate==12000){
        //12.288 MHz
        ithWriteRegA(0xD8000110,0xa02e2d01);
        ithWriteRegA(0xD8000114,0x800147ae);
        ithWriteRegA(0xD8000114,0xF03147ae);
        ithDelay(220);
        ithWriteRegA(0xD8000114,0x800147ae);
    }else if(demanded_sample_rate==44100||demanded_sample_rate==22050||demanded_sample_rate==11025){
        //11.2896 MHz
        ithWriteRegA(0xD8000110,0xa0525701);
        ithWriteRegA(0xD8000114,0x800D97F6);
        ithWriteRegA(0xD8000114,0xF03D97F6);
        ithDelay(220);
        ithWriteRegA(0xD8000114,0x800D97F6);
    }else{//(demanded_sample_rate==32000||demanded_sample_rate==16000||demanded_sample_rate==8000)

        ithWriteRegA(0xD8000110,0xa03b5701);
        ithWriteRegA(0xD8000114,0x800645a1);  //fraction 645a1
        ithWriteRegA(0xD8000114,0xF03645a1);  //durarion = 03
        ithDelay(220);                     //IDLE 220
        ithWriteRegA(0xD8000114,0x800645a1);
    }

    switch (demanded_sample_rate)
    {
        case 48000: amclk_ratio = 0x1; zclk_ratio = 0x4; break;
        case 24000: amclk_ratio = 0x2; zclk_ratio = 0x8; break;
        case 12000: amclk_ratio = 0x4; zclk_ratio = 0x10; break;
        case 44100: amclk_ratio = 0x1; zclk_ratio = 0x4; break;
        case 22050: amclk_ratio = 0x2; zclk_ratio = 0x8; break;
        case 11025: amclk_ratio = 0x4; zclk_ratio = 0x10; break;
        case 32000: amclk_ratio = 0x1; zclk_ratio = 0x4; break;
        case 16000: amclk_ratio = 0x2; zclk_ratio = 0x8; break;
        case  8000: amclk_ratio = 0x4; zclk_ratio = 0x10; break;
        default   : printf("not support sampling rate : %d\n",demanded_sample_rate);break;
    }

    ithWriteRegA(MMP_AUDIO_CLOCK_REG_40, 0x0002a800|amclk_ratio);//amclk_ratio

    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C,(0xB2A88800|zclk_ratio));//zclk_ratio
    ithDelay(10);
    ithWriteRegA(MMP_AUDIO_CLOCK_REG_3C,(0x02A88800|zclk_ratio));
    itp_codec_set_DAC_slave_mode(1);//set DAC slave mode;
    #endif
    *demanded_sample_width =0x1F;
}
#endif

static void _i2s_enable_fading(u32 fading_step, u32 fading_duration)
{
    u32 data32;

    ithWriteRegMaskA(I2S_REG_OUT_FADE_SET, 1<<0, 1<<0); //Enable OUT_BASE1 fade
    data32 = ithReadRegA(I2S_REG_OUT_FADE_SET);
    data32 |= ((fading_step & 0xF) << 8);
    data32 |= ((fading_duration & 0xFF) << 16);
    ithWriteRegA(I2S_REG_OUT_FADE_SET, data32);
}

static void _i2s_disable_fading(void)
{
    ithWriteRegMaskA(I2S_REG_OUT_FADE_SET, 0, 1<<0);  //Disable OUT_BASE1 fade
}

static void _i2s_set_GPIO(void) /* GPIO settings for CFG2 */
{
#if (I2S_INTERNAL_CODEC == 0)
    //external codec
    ithGpioSetMode(20, ITH_GPIO_MODE4); //GPIO[20] //IIS_AMCLK
    ithGpioSetMode(21, ITH_GPIO_MODE4); //GPIO[21] //IIS_ZCLK
    ithGpioSetMode(22, ITH_GPIO_MODE4); //GPIO[22] //IIS_ZWS
    ithGpioSetMode(23, ITH_GPIO_MODE4); //GPIO[23] //IIS_ZDO
    ithGpioSetMode(24, ITH_GPIO_MODE4); //GPIO[24] //IIS_ZDI
#endif
#ifdef ENABLE_HDMI_GPIO_OUT_SET
    //HDMITX_1  (mode4 57~62)
    //ithWriteRegA(0xd10000EC, 0x04444440);    //GPIO[63:56]
    ithGpioSetMode(57, ITH_GPIO_MODE4); //GPIO[57] TX_ZCLK
    ithGpioSetMode(58, ITH_GPIO_MODE4); //GPIO[58] TX_ZWS
    ithGpioSetMode(59, ITH_GPIO_MODE4); //GPIO[59] TX_ZD0
    ithGpioSetMode(60, ITH_GPIO_MODE4); //GPIO[60] TX_ZD1
    ithGpioSetMode(61, ITH_GPIO_MODE4); //GPIO[61] TX_ZD2
    ithGpioSetMode(62, ITH_GPIO_MODE4); //GPIO[62] TX_ZD3
    ithGpioSetMode(63, ITH_GPIO_MODE4); //GPIO[63] TX_AMCLK

#endif
#ifdef ENABLE_HDMI_GPIO_IN_SET
    //HDMIRX (mode4 72~76) (EVB 77 78 no use)
    //ithWriteRegA(0xd1000164, 0x00044444);  //GPIO[79:72]
    ithGpioSetMode(40, ITH_GPIO_MODE4); //GPIO[72] RX_AMCLK
    ithGpioSetMode(41, ITH_GPIO_MODE4); //GPIO[73] RX_ZCLK
    ithGpioSetMode(42, ITH_GPIO_MODE4); //GPIO[74] RX_ZWS
    ithGpioSetMode(46, ITH_GPIO_MODE4); //GPIO[75] RX_ZD3
    ithGpioSetMode(45, ITH_GPIO_MODE4); //GPIO[76] RX_ZD2
    //ithGpioSetMode(77, ITH_GPIO_MODE4); //GPIO[76] RX_ZD1
    //ithGpioSetMode(78, ITH_GPIO_MODE4); //GPIO[76] RX_ZD0
#endif
}

void GPIO_switch_set(AudioInOutCase enable){
    int CHANEL_SEL = 15;
    int CHANEL_ENB = 16;
    ithGpioClear(CHANEL_SEL);
    ithGpioClear(CHANEL_ENB);
    switch (enable)
    {
    case HANDFREE:
        {/* hand free */
            itp_codec_playback_init(1);//Faraday DAC ONLY Left
            itp_codec_rec_init(1);
            //ithWriteRegMaskA(I2S_REG_OUT_CTRL, 1<<6||0<<4, 1<<6||1<<4);//l channels ,left high
            ithWriteRegMaskA(I2S_REG_IN_CTRL, 0<<6|0<<4, 1<<6|1<<4);
            ithGpioClear(CHANEL_ENB);
            ithGpioClear(CHANEL_SEL);
            break;
        }
    case HEADSET:
        {/* head set */
            itp_codec_playback_init(0);//Faraday DAC ONLY right
            itp_codec_rec_init(0);
            //ithWriteRegMaskA(I2S_REG_IN_CTRL, 1<<6||0<<4, 1<<6||1<<4);//l channels ,right high
            ithWriteRegMaskA(I2S_REG_IN_CTRL, 1<<6|0<<4, 1<<6|1<<4);
            ithGpioSetOut(CHANEL_ENB);
            ithGpioSet(CHANEL_SEL);
            break;
        }
    case TELEPHONETUBE:
        {/* telephone tube */
            itp_codec_playback_init(0);//Faraday DAC ONLY right
            itp_codec_rec_init(0);
            //ithWriteRegMaskA(I2S_REG_OUT_CTRL, 1<<6||0<<4, 1<<6||1<<4);//l channels ,right high
            ithWriteRegMaskA(I2S_REG_IN_CTRL, 1<<6|0<<4, 1<<6|1<<4);
            ithGpioSetOut(CHANEL_ENB);
            ithGpioSetOut(CHANEL_SEL);
            break;
        }
    case DOUBLE_HEADSET:
        {
            itp_codec_playback_init(2);//Faraday DAC LEFT/RIGHT
            itp_codec_rec_init(0);
            //ithWriteRegMaskA(I2S_REG_OUT_CTRL, 0<<6||1<<4, 1<<6||1<<4);//2 channel ,left high
            ithWriteRegMaskA(I2S_REG_IN_CTRL, 1<<6|0<<4, 1<<6|1<<4);
            ithGpioSetOut(CHANEL_ENB);
            ithGpioSet(CHANEL_SEL);
            break;
        }
    case DOUBLE_TELETUBE:
        {
            itp_codec_playback_init(2);//Faraday DAC LEFT/RIGHT
            itp_codec_rec_init(0);
            //ithWriteRegMaskA(I2S_REG_OUT_CTRL, 0<<6||1<<4, 1<<6||1<<4);//2 channel ,left high
            ithWriteRegMaskA(I2S_REG_IN_CTRL, 1<<6|0<<4, 1<<6|1<<4);
            ithGpioSetOut(CHANEL_ENB);
            ithGpioSetOut(CHANEL_SEL);
            break;
        }
    default:
        {
            itp_codec_playback_init(1);
            itp_codec_rec_init(1);
            //ithWriteRegMaskA(I2S_REG_OUT_CTRL, 0<<6, 0<<6);
            ithGpioClear(CHANEL_ENB);
            ithGpioClear(CHANEL_SEL);
            break;
        }
    }
    printf("Gpio%d:%d Gpio%d:%d\n",CHANEL_SEL,ithGpioGet(CHANEL_SEL),CHANEL_ENB,ithGpioGet(CHANEL_ENB));

}

/* ************************************************************************** */

void i2s_volume_up(void)
{
    // amp use. useless function
}

void i2s_volume_down(void)
{
    // amp use. useless function
}

void i2s_pause_ADC(int pause)
{
    printf("I2S# %s(%d)\n", __func__, pause);

    if(pause)
    {
        ithWriteRegMaskA(I2S_REG_IN_CTRL, 0, 1<<0);
        itp_codec_rec_mute();

    }
    else /* resume */
    {
        ithWriteRegMaskA(I2S_REG_IN_CTRL, 1<<0, 1<<0);
        itp_codec_rec_unmute();

    }
}

void i2s_enable_fading(int yesno){
    if(yesno){
//      _i2s_enable_fading(0xF, 0x01); /* fast response */
//      _i2s_enable_fading(0x7, 0x7F); /* moderato */
        _i2s_enable_fading(0x1, 0xFF); /* slow response */
        ithWriteRegA(I2S_REG_OUT_VOLUME, 0x7F0100);
    }else{
        _i2s_disable_fading();
    }
}

void i2s_pause_DAC(int pause)
{
    printf("I2S# %s(%d)\n", __func__, pause);

    if(pause)
    {
        ithWriteRegMaskA(I2S_REG_OUT_CTRL, 0, 1<<0);
        itp_codec_playback_mute();
    }
    else /* resume */
    {
        ithWriteRegMaskA(I2S_REG_OUT_CTRL, 1<<0, 1<<0);

        if(!_i2s_DA_mute)
            itp_codec_playback_unmute();
    }
}

void i2s_deinit_ADC(void)
{
    u32 data32;

    if(0 == _i2s_AD_running) {
        printf("I2S# ADC is 'NOT' running, skip deinit ADC !\n");
        return;
    }

    printf("I2S# %s +\n", __func__);

    pthread_mutex_lock(&I2S_MUTEX);

    itp_codec_rec_deinit();

    /* disable hardware I2S */
    {
        data32 = ithReadRegA(I2S_REG_IN_CTRL);
        data32 &= ~(3 << 0);
        ithWriteRegA(I2S_REG_IN_CTRL, data32);
    }

    _i2s_AD_running = 0; /* put before _i2s_reset() */
    _i2s_reset();

    _i2s_power_off();

    pthread_mutex_unlock(&I2S_MUTEX);
    printf("I2S# %s -\n", __func__);

}

void i2s_deinit_DAC(void)
{
    u32 data32;
//  u32 out_status;
//  u32 i2s_idle;
//  u32 pipe_idle;
//  u32 i2s_memcnt;
#ifdef I2S_DEBUG_DEINIT_DAC_COST
    static struct timeval tv_pollS;
    static struct timeval tv_pollE;
#endif

#ifdef I2S_DEBUG_DEINIT_DAC_COST
    gettimeofday(&tv_pollS, NULL); //-*-
#endif

    pthread_mutex_lock(&I2S_MUTEX);

    if(!_i2s_DA_running) {
        printf("I2S# DAC is 'NOT' running, skip deinit DAC !\n");
        pthread_mutex_unlock(&I2S_MUTEX);
        return;
    }

    printf("I2S# %s +\n", __func__);

    /*cost 2ms prevent pop sound */
    itp_codec_playback_mute();
    ithWriteRegMaskA(I2S_REG_OUT_CTRL, 1<<0, 1<<0);
    ithDelay(2000);

    /*deinit dac*/
    itp_codec_playback_deinit();

    /* disable I2S_OUT_FIRE & I2S_OUTPUT_EN */
    data32 = ithReadRegA(I2S_REG_OUT_CTRL);
    data32 &= ~(0x7);
    ithWriteRegA(I2S_REG_OUT_CTRL, data32);

    _i2s_DA_running = 0; /* put before _i2s_reset() */
    _i2s_reset();

    _i2s_power_off();

    printf("I2S# %s -\n", __func__);
    pthread_mutex_unlock(&I2S_MUTEX);

#ifdef I2S_DEBUG_DEINIT_DAC_COST
    gettimeofday(&tv_pollE, NULL); //-*-
    printf("I2S# DEINIT_DAC_COST: %ld (ms)\n", TV_CAL_DIFF_MS(tv_pollE, tv_pollS));
#endif

}

void i2s_init_DAC(STRC_I2S_SPEC *i2s_spec)
{
    int param_fail = 0;
    int i;
    u32 data32 = 0;
    u8 resolution_type;
    u8 hdmi_num;

    if(_i2s_DA_running && itp_codec_get_DA_running()) {
        printf("I2S# DAC is running, skip re-init !\n");
        return;
    }
    if(((u32)i2s_spec->base_i2s % 8) || (i2s_spec->buffer_size % 8)) {
        printf("ERROR# I2S, bufbase/bufsize must be 8-Bytes alignment !\n");
        return;
    }
    if((i2s_spec->channels != 1) && (i2s_spec->channels != 2)) {
        printf("ERROR# I2S, only support single or two channels !\n");
        return;
    }

    printf("I2S# %s Start\n", __func__);
    pthread_mutex_lock(&I2S_MUTEX);

    _i2s_power_on();

    _i2s_set_GPIO();

    _i2s_set_clock(i2s_spec->sample_rate , &sample_width);

    //_i2s_reset();

    /* mastor mode & AWS & ZWS sync */
    switch(i2s_spec->sample_size) {
        case 16: { resolution_type = 0; break; }
        case 24: { resolution_type = 1; break; }
        case 32: { resolution_type = 2; break; }
        case 8 : { resolution_type = 3; break; }
        default: { resolution_type = 2; break; }
    }

    data32 = (resolution_type   << 28) /* ADC resolution bits, 00:16bits; 01:24bits; 10:32bits; 11:8bits */
            |(1                 << 24) /* WS mute check, 0:Disable 1:Enable */
            |(0xF               << 16) /* WS mute check times */
            |(sample_width      <<  0); /* sample width (in bits unit) */
    ithWriteRegA(I2S_REG_DAC_SRATE_SET,data32);
    ithWriteRegA(I2S_REG_ADC_SRATE_SET,data32);

    /* 970 remove PCM support*/
    _i2s_aws_sync();

    /*Faraday audio init*/
    if(i2s_spec->enable_Speaker) itp_codec_playback_init(1);// 0:HPR only 1:HPL only 2:HPRL both
    itp_codec_set_i2s_sample_rate(i2s_spec->sample_rate);

    /* buffer base */
    for (i = 0; i<4; i++) {
        if(i2s_spec->base_hdmi[i] == NULL) {i2s_spec->base_hdmi[i] = i2s_spec->base_i2s;}
    }
    ithWriteRegA(I2S_REG_OUT_BASE1, (u32)i2s_spec->base_i2s    );/*(IIS /SPDIF out) base 1*/
    ithWriteRegA(I2S_REG_OUT_BASE2, (u32)i2s_spec->base_hdmi[0]);/*(HDMI Data0 out) base 2*/
    ithWriteRegA(I2S_REG_OUT_BASE3, (u32)i2s_spec->base_hdmi[1]);/*(HDMI Data1 out) base 3*/
    ithWriteRegA(I2S_REG_OUT_BASE4, (u32)i2s_spec->base_hdmi[2]);/*(HDMI Data2 out) base 4*/
    ithWriteRegA(I2S_REG_OUT_BASE5, (u32)i2s_spec->base_hdmi[3]);/*(HDMI Data3 out) base 5*/

    /* buffer length */
    ithWriteRegA(I2S_REG_OUT_LEN, i2s_spec->buffer_size - 1);

    /* DA starvation interrupt threshold */
    ithWriteRegA(I2S_REG_OUT_RdWrGAP, 0x00000080);   //OutRdWrGap[31:0]

    /* output path */
    hdmi_num = 0;
    switch(i2s_spec->num_hdmi_audio_buffer){
        case 1: { hdmi_num |=  1; break; } /* buf 1 (HDMI Data0) */
        case 2: { hdmi_num |=  3; break; }
        case 3: { hdmi_num |=  7; break; }
        case 4: { hdmi_num |= 15; break; }
        case 0:
        default: break;
    }

    /*should 970 set W0OutRqSize[3:0]=0b1000 ?*/
    ithWriteRegA(I2S_REG_OUT_CTRL,
    (  hdmi_num                          << 9) /* HDMI DATA Output*/
    | (1                                 << 8) /* Enable IIS 1/SPDIF Data Output */
    | (0                                 << 6) /* Output Channel active level, 0:Low for Left; 1:High for Left */
    | (0                                 << 5) /* Output Interface Format, 0:IIS Mode; 1:MSB Left-Justified Mode */
    | ((i2s_spec->channels - 1)          << 4) /* Output Channel Select, 0:Single Channel; 1:Two Channels */
    | ((i2s_spec->is_big_endian ? 1 : 0) << 3) /* 0:Little Endian; 1:Big Endian */
    | (0                                 << 2) /* 0:NOT_LAST_WPTR; 1:LAST_WPTR */
    | (1                                 << 1) /* Fire the IIS for Audio Output */
    );

    if(!i2s_spec->postpone_audio_output) {
        i2s_pause_DAC(0); /* Enable Audio Output */
    }
    else
        i2s_pause_DAC(1);

    _i2s_DA_running = 1;

    printf("I2S# %s End\n", __func__);
    pthread_mutex_unlock(&I2S_MUTEX);

}

void i2s_init_ADC(STRC_I2S_SPEC *i2s_spec)
{
    int i;
    u32 data32 = 0;
    u8 resolution_type;
    u8 hdmi_num;

    if(_i2s_AD_running && itp_codec_get_DA_running()) {
        printf("I2S# ADC is running, skip re-init ADC !\n");
        return;
    }
    if(((u32)i2s_spec->base_i2s % 8) || (i2s_spec->buffer_size % 8)) {
        printf("ERROR# I2S, bufbase/bufsize must be 8-Bytes alignment !\n");
        return;
    }
    if((i2s_spec->channels != 1) && (i2s_spec->channels != 2)) {
        printf("ERROR# I2S, only support single or two channels !\n");
        return;
    }

    printf("I2S# %s Start\n", __func__);
    pthread_mutex_lock(&I2S_MUTEX);

    _i2s_power_on();

    _i2s_set_GPIO();

    _i2s_set_clock(i2s_spec->sample_rate, &sample_width);

    //_i2s_reset();

    itp_codec_set_i2s_sample_rate(i2s_spec->sample_rate);

    /* mastor mode & AWS & ZWS sync */
    switch(i2s_spec->sample_size) {
        case 16: { resolution_type = 0; break; }
        case 24: { resolution_type = 1; break; }
        case 32: { resolution_type = 2; break; }
        case 8 : { resolution_type = 3; break; }
        default: { resolution_type = 2; break; }
    }

    /* config AD */
    data32 = (resolution_type   << 28) /* ADC resolution bits, 00:16bits; 01:24bits; 10:32bits; 11:8bits */
            |(1                 << 24) /* WS mute check, 0:Disable 1:Enable */
            |(0xF               << 16) /* WS mute check times */
            |(sample_width      <<  0); /* sample width (in bits unit) */
    ithWriteRegA(I2S_REG_ADC_SRATE_SET, data32);
    ithWriteRegA(I2S_REG_DAC_SRATE_SET, data32);

    _i2s_aws_sync();

    if(i2s_spec->from_MIC_IN) itp_codec_rec_init(i2s_spec->channels);//0:micR only 1:micL only 2:micRLboth


    /* buffer base */
    for (i = 0; i<4; i++) {
        if(i2s_spec->base_hdmi[i] == NULL) {i2s_spec->base_hdmi[i] = i2s_spec->base_i2s;}
    }

    ithWriteRegA(I2S_REG_IN1_BASE1, (u32)i2s_spec->base_i2s    ); /*(I2S  Data  in ) IN1 base1*/
    ithWriteRegA(I2S_REG_IN2_BASE4, (u32)i2s_spec->base_hdmi[3]); /*(HDMI Data4 in)  IN2 base4 :RX_ZD3*/
    ithWriteRegA(I2S_REG_IN2_BASE3, (u32)i2s_spec->base_hdmi[2]); /*(HDMI Data3 in)  IN2 base3 :RX_ZD2*/
    ithWriteRegA(I2S_REG_IN2_BASE2, (u32)i2s_spec->base_hdmi[1]); /*(HDMI Data2 in)  IN2 base2 :RX_ZD1*/
    ithWriteRegA(I2S_REG_IN2_BASE1, (u32)i2s_spec->base_hdmi[0]); /*(HDMI Data1 in)  IN2 base1 :RX_ZD0*/

    /* buffer length */
    ithWriteRegA(I2S_REG_IN_LEN, i2s_spec->buffer_size - 1);


    /* DA starvation interrupt threshold */
    ithWriteRegA(I2S_REG_IN_RdWrGAP, 0x00000040);   //OutRdWrGap[31:0]


    hdmi_num = 0;
    switch (i2s_spec->num_hdmi_audio_buffer){
        case  1: { hdmi_num |= 1 ; break; } /* buf 1 (HDMI Data0) */
        case  2: { hdmi_num |= 3 ; break; }
        case  3: { hdmi_num |= 7 ; break; }
        case  4: { hdmi_num |= 15; break; }
        default: { hdmi_num |= 0 ; break; }
    }

    ithWriteRegA(I2S_REG_IN_CTRL,
    (  hdmi_num                          <<12) /* Eanble HDMI Data Input */
    | (1                                 << 8) /* Eanble Input1 IIS Data Input */
    | (0                                 << 6) /* Input Channel active level, 0:Low for Left; 1:High for Left */
    | (0                                 << 5) /* Input Interface Format, 0:IIS Mode; 1:MSB Left-Justified Mode */
    | ((i2s_spec->channels - 1)          << 4) /* Input Channel, 0:Single Channel; 1:Two Channels */
    | ((i2s_spec->is_big_endian ? 1 : 0) << 3) /* 0:Little Endian; 1:Big Endian */
    | ((i2s_spec->record_mode ? 1 : 0)   << 2) /* 0:AV Sync Mode (wait capture to start); 1:Only Record Mode */
    | (1                                 << 1) /* Fire the IIS for Audio Input */
    | (1                                 << 0) /* Enable Audio Input */
    );


    _i2s_AD_running = 1;

    pthread_mutex_unlock(&I2S_MUTEX);
    printf("I2S# %s End\n", __func__);

}

int  i2s_get_DA_running(void)
{
    return itp_codec_get_DA_running();
}

int  i2s_get_AD_running(void)
{
    return itp_codec_get_AD_running();
}

void i2s_mute_DAC(int mute)
{

    DEBUG_PRINT("I2S# %s(%d)\n", __func__, mute);
    if(mute)
    {
        itp_codec_playback_mute();
        _i2s_DA_mute = 1;
    }
    else /* resume */
    {
        itp_codec_playback_unmute();
        _i2s_DA_mute = 0;
    }

}

void i2s_set_direct_volperc(unsigned volperc)
{   /*volperc: 0~100*/
    itp_codec_playback_set_direct_volperc(volperc);
    if(!volperc)
        i2s_mute_volume(1);//mute
    else if(!_i2s_DA_mute && volperc && _bar_mute_flag)
        i2s_mute_volume(0);//unmute
    else{
        //do nothing
    }
}
void i2s_set_direct_volstep(unsigned int volstep)
{   /*volstep: 0x00~0xFF,0x100;*/
    //itp_codec_playback_set_direct_vol(volstep);
}

unsigned i2s_get_current_volperc(void)
{
    unsigned currvolperc = 0;
    itp_codec_playback_get_currvol(&currvolperc);
    return currvolperc;
}

void i2s_ADC_set_rec_volperc(unsigned recperc)
{
    /*recperc:0~100*/
    itp_codec_rec_set_direct_volperc(recperc);
    if(!recperc)
        i2s_mute_ADC(1);//mute
    else if(_i2s_AD_mute && recperc)
        i2s_mute_ADC(0);//unmute
    else{
        //do nothing
    }

}

void i2s_ADC_set_direct_volstep(unsigned int  recstep)
{   /*volstep: 0x00~0xFF,0x100;*/
    //itp_codec_rec_set_direct_vol(volstep);
}

unsigned i2s_ADC_get_current_volstep(void)
{
    unsigned currmicperc = 0;
    itp_codec_rec_get_currvol(&currmicperc);
    return currmicperc;
}

void i2s_ADC_get_volstep_range(unsigned *max, unsigned *normal, unsigned *min)
{
    itp_codec_rec_get_vol_range(max, normal, min);
}

void i2s_mute_ADC(int mute)
{
    DEBUG_PRINT("I2S# %s(%d)\n", __func__, mute);

    if(mute) { itp_codec_rec_mute(); }
    else     { itp_codec_rec_unmute(); }

    _i2s_AD_mute = mute;
}

void i2s_set_linein_bypass(int bypass)
{

}

static void _init_spdif(u32 sample_rate)
{
}

static void _deinit_spdif(void)
{
}

static void _show_i2s_spec(STRC_I2S_SPEC *i2s_spec)
{
    printf("    @channels   %x\n",i2s_spec->channels);
    printf("    @sample_rate    %x\n",i2s_spec->sample_rate);
    printf("    @buffer_size    %x\n",i2s_spec->buffer_size);
    printf("    @is_big_endian  %x\n",i2s_spec->is_big_endian);
    printf("    @sample_size    %x\n",i2s_spec->sample_size);
    printf("    @from_LineIN    %x\n",i2s_spec->from_LineIN);
    printf("    @from_MIC_IN    %x\n",i2s_spec->from_MIC_IN);
    printf("    @num_hdmi_buf   %x\n",i2s_spec->num_hdmi_audio_buffer);
    printf("    @base_i2s   %x\n",i2s_spec->base_i2s);
    printf("    @spdif_same_buf %x\n",i2s_spec->is_dac_spdif_same_buffer);
    printf("    @base_spdif %x\n",i2s_spec->base_spdif);
    printf("    @aud_out    %x\n",i2s_spec->postpone_audio_output);
    printf("    @record_mode    %x\n",i2s_spec->record_mode);
    printf("    @Speaker    %x\n",i2s_spec->enable_Speaker);
    printf("    @HeadPhone  %x\n",i2s_spec->enable_HeadPhone);
}

void i2s_mute_volume(int mute)
{
    if(mute){
        _bar_mute_flag = 1;
        itp_codec_playback_mute();
    }
    else{
        _bar_mute_flag = 0;
        itp_codec_playback_unmute();
    }

}
