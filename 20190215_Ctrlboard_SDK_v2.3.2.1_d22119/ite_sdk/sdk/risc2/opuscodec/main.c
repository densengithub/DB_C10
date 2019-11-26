#include <stdarg.h>
#include <string.h>

#include "risc2/opuscodec/opuscodec.h"
#include "opus.h"

#define ENDIAN_SWAP16(x) \
        (((x & 0x00FF) << 8) | ((x & 0xFF00) >> 8))

#define ENDIAN_SWAP32(x) \
        (((x & 0x000000FF) << 24) | \
        ((x & 0x0000FF00) <<  8) | \
        ((x & 0x00FF0000) >>  8) | \
        ((x & 0xFF000000) >> 24))

uint8_t* gpCmdExchangeAddr = NULL;
extern unsigned char risc2_start_addr[];

static void opus_create(void)
{
    int err = 0;
    OPUS_INIT_DATA *ptInitData = (OPUS_INIT_DATA*) gpCmdExchangeAddr;
    uint32_t sampleRate = ENDIAN_SWAP32(ptInitData->sampleRate);
    uint32_t application = ENDIAN_SWAP32(ptInitData->application);
    uint32_t channels = ENDIAN_SWAP32(ptInitData->channels);
    uint32_t mode = ENDIAN_SWAP32(ptInitData->mode);

    if (mode == OPUS_ENCODE_INIT)
    {
        OpusEncoder *encoder = (OpusEncoder *) (ENDIAN_SWAP32(ptInitData->codecAddr));
        err = opus_encoder_init(encoder, sampleRate, channels, application);
        if (err < 0) {
            ptInitData->err = ENDIAN_SWAP32(err);
            return;
        }
#ifdef CFG_OPUS_DTX_ENABLE
        opus_encoder_ctl(encoder, OPUS_SET_DTX(1));
#endif
#ifdef CFG_OPUS_FEC_ENABLE
        opus_encoder_ctl(encoder, OPUS_SET_BITRATE(22000));
        opus_encoder_ctl(encoder, OPUS_SET_INBAND_FEC(1));
        opus_encoder_ctl(encoder, OPUS_SET_PACKET_LOSS_PERC(25));
#endif
    }
    else if (mode == OPUS_DECODE_INIT)
    {
        OpusDecoder *decoder = (OpusDecoder *) (ENDIAN_SWAP32(ptInitData->codecAddr));
        err = opus_decoder_init(decoder, sampleRate, channels);
        if (err < 0) {
            ptInitData->err = ENDIAN_SWAP32(err);
        }
    }
}

static void opus_enc(void)
{
    OPUS_CONTEXT *ptOpusCtxt = (OPUS_CONTEXT*) gpCmdExchangeAddr;
    int i;
    uint32_t ctxtLen = ENDIAN_SWAP32(ptOpusCtxt->ctxtLen);
    uint16_t pcm_bytes[ctxtLen];
    uint8_t  cbits[ctxtLen];
    OpusEncoder *encoder = (OpusEncoder *) (ENDIAN_SWAP32(ptOpusCtxt->codecAddr));

    for (i = 0; i < ctxtLen; i++)
    {
        pcm_bytes[i] = ENDIAN_SWAP16(ptOpusCtxt->ctxtBuffer[i]);
    }
    int nbBytes = opus_encode(encoder, pcm_bytes, ctxtLen, cbits, ctxtLen);
    if (nbBytes < 0) {
        ptOpusCtxt->err = ENDIAN_SWAP32(nbBytes);
        return;
    }
    
    memcpy(ptOpusCtxt->ctxtBuffer, cbits, nbBytes);
    ptOpusCtxt->ctxtLen = ENDIAN_SWAP32(nbBytes);
}

static void opus_dec(void)
{
    OPUS_CONTEXT *ptOpusCtxt = (OPUS_CONTEXT*) gpCmdExchangeAddr;
    int i;
    uint32_t ctxtLen = ENDIAN_SWAP32(ptOpusCtxt->ctxtLen);
    uint32_t frameSize = ENDIAN_SWAP32(ptOpusCtxt->frameSize);
    int      fecDec =  ENDIAN_SWAP32(ptOpusCtxt->fecDec);
    uint8_t  cbits[ctxtLen];
    uint16_t out[frameSize];
    OpusDecoder *decoder = (OpusDecoder *) (ENDIAN_SWAP32(ptOpusCtxt->codecAddr));
    
    memcpy(cbits, ptOpusCtxt->ctxtBuffer, ctxtLen);

    int frame_size = opus_decode(decoder, cbits, ctxtLen, out, frameSize, fecDec);
    if (frame_size < 0) {
        ptOpusCtxt->err = ENDIAN_SWAP32(frame_size);
        return;
    }
    
    for(i = 0; i < frame_size; i++)
    {
        ptOpusCtxt->ctxtBuffer[i] = ENDIAN_SWAP16(out[i]);
    }

    ptOpusCtxt->ctxtLen = ENDIAN_SWAP32(frame_size);
}

int main(int argc, char **argv)
{
    int inputCmd = 0;
    struct _risc2_header *ptRisc2Header = (struct _risc2_header*) risc2_start_addr;
    gpCmdExchangeAddr = (uint8_t*) ENDIAN_SWAP32((uint32_t)ptRisc2Header->cmd_exchange_addr);

    while(1)
    {
        inputCmd = RISC2_COMMAND_REG_READ(REQUEST_CMD_REG);
        if (inputCmd && RISC2_COMMAND_REG_READ(RESPONSE_CMD_REG) == 0)
        {
            dc_invalidate();
            switch(inputCmd)
            {
                case OPUS_CREATE:
                    opus_create();
                    break;
                case OPUS_ENCODE:
                    opus_enc();
                    break;
                case OPUS_DECODE:
                    opus_dec();
                    break;
                default:
                    break;
            }
            RISC2_COMMAND_REG_WRITE(RESPONSE_CMD_REG, (uint16_t) inputCmd);
        }
    }
    
    return 0;
}
