#include <monsoon/monsoon.h>

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

#define MONSOON_MINIMUM_SAMPLE_COUNT (5760)

int monsoonMinimumSampleBufferSize(const Monsoon *self)
{
    return MONSOON_MINIMUM_SAMPLE_COUNT;
}

int monsoonDecode(Monsoon *self, int16_t *sampleTarget, int sampleCount)
{
    int frameSize = sampleCount;
    const int decodeForwardErrorCorrection = 0;

    if (sampleCount < MONSOON_MINIMUM_SAMPLE_COUNT)
    {
        return -1;
    }

    return op_read(self->opusFile, sampleTarget, sampleCount, 0);
}

int monsoonRewind(struct Monsoon* self)
{
    return op_raw_seek(self->opusFile, 0);
}
