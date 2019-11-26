/***************************************************************************
 * Copyright (c) 2008 SMedia Technology Corp. All Rights Reserved.
 *
 * @file
 * Codecs Startup Code
 *
 * @author Kuoping Hsu
 * @version 1.0
 *
 ***************************************************************************/
#include "codecs.h"
//#include "mmio.h"

char __tm_info[] = "Audio Codec Plug-Ins (c) 2008 SMedia Tech. Corp.";

// Put the *ci to the data section instead of bss section. Cause the
// bss section will clear in the codec_start function, it will
// reset the *ci to NULL.
struct _codec_api *ci __attribute__ ((section (".data"))) = 0;

extern unsigned char risc1_start_addr[];
extern unsigned char risc1_end_addr[];
extern unsigned char risc1_bss_start[];
extern unsigned char risc1_bss_end[];

//int codec_start(struct _codec_api *api) __attribute__ ( ( naked ) );
int codec_start(void) __attribute__ ( ( naked ) );
extern int codec_info();
//extern AUDIO_CODEC_STREAM* gpAudioStream;
#if 1 //defined(AUDIO_PLUGIN_MESSAGE_QUEUE)

struct _codec_header __header __attribute__ ((section (".codecs_header"))) = {
    CODEC_MAGIC, TARGET_ID, CODEC_API_VERSION,
    (unsigned char*)risc1_start_addr, (unsigned char*)risc1_end_addr,
    codec_start, 0xffffffff, codec_info
};
#else
const struct _codec_header __header __attribute__ ((section (".codecs_header"))) = {
    CODEC_MAGIC, TARGET_ID, CODEC_API_VERSION,
    (unsigned char*)risc1_start_addr, (unsigned char*)risc1_end_addr,
    codec_start, codec_info
};

#endif

void MMIO_Write(unsigned long addr, unsigned short data)
{
    *(volatile unsigned long *) (addr) = data;
}

unsigned int MMIO_Read(unsigned long addr)
{
    return *(volatile unsigned int *) (addr);
}
// startup entry
//int codec_start(struct _codec_api *api)
int codec_start(void)
{
    // Make sure the ci is not in .bss section. The .bss section will
    // clear later.
    //ci = api;

    asm volatile ("l.movhi r1,hi(_stack)");
    asm volatile ("l.ori   r1,r1,lo(_stack)");
    asm volatile ("l.addi  r2, r1, 0x0");
    asm volatile ("l.nop");
    
    ic_enable(); 
    dc_enable();

    // Clear BSS section of CODEC. Be carefull, it will clear all of
    // global variable which un-initialized or initialize with zero's
    // variable.
    #if 1
    {
        int *ptr = (int*)risc1_bss_start;
    do {
        *ptr++ = 0;
    } while((int)ptr <= (int)risc1_bss_end);
    }
    #endif

    //MMIO_Write(AUDIO_DECODER_START_FALG, 1);       

    main();

    // Never return by codec main functions.
    //taskSOFTWARE_BREAKPOINT();
    while(1) ;

    return 0;
}


// dummy __main function, do not remove it.
void __main(void) { /* dummy */ }
void free(void) { /* dummy */ }
