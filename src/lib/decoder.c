#include <monsoon/monsoon.h>

/*
int monsoonInit(Monsoon *self, uint32_t frequency)
{
    const int channelCount = 2;

    int sizeToAllocate = opus_decoder_get_size(channelCount);

    self->opus = (OpusDecoder *)calloc(1, sizeToAllocate);

    int result = opus_decoder_init(self->opus, (opus_int32)frequency, channelCount);

    if (result != OPUS_OK)
    {
        return -1;
    }

    return 0;
}

int monsoonDecode(Monsoon *self, const uint8_t *data, uint32_t octetCount, int16_t *sampleTarget, int sampleCount)
{
    int frameSize = sampleCount;
    const int decodeForwardErrorCorrection = 0;

    return opus_decode(self->opus, data, octetCount, sampleTarget, frameSize, decodeForwardErrorCorrection);
}
*/

int monsoonInit(Monsoon *self, uint32_t frequency, const uint8_t *data, int octetCount)
{
    const int channelCount = 2;

    int errorCode;

    self->opusFile = op_open_memory(data, octetCount, &errorCode);

    if (errorCode != OPUS_OK)
    {
        return -1;
    }

    return 0;
}

int monsoonDecode(Monsoon *self, int16_t *sampleTarget, int sampleCount)
{
    int frameSize = sampleCount;
    const int decodeForwardErrorCorrection = 0;

    if (sampleCount < 5760)
    {
        return -1;
    }

    return op_read(self->opusFile, sampleTarget, sampleCount, 0);
}
