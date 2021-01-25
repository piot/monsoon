#include <clog/clog.h>
#include <monsoon/monsoon.h>

int monsoonInit(Monsoon* self, uint32_t frequency, const uint8_t* data, int octetCount)
{
    int errorCode;

    self->opusFile = op_open_memory(data, octetCount, &errorCode);

    if (errorCode != OPUS_OK) {
        return -1;
    }

    self->isFirstDecode = 1;

    return 0;
}

#define MONSOON_MINIMUM_SAMPLE_COUNT (5760)

int monsoonMinimumSampleBufferSize(const Monsoon* self)
{
    return MONSOON_MINIMUM_SAMPLE_COUNT;
}

int monsoonDecode(Monsoon* self, int16_t* sampleTarget, int sampleCount)
{
    if (sampleCount < MONSOON_MINIMUM_SAMPLE_COUNT) {
        return -1;
    }

    int listIndex;

    int readSampleCount = op_read(self->opusFile, sampleTarget, sampleCount, &listIndex);

#if CONFIGURATION_DEBUG
    if (self->isFirstDecode) {
        const OpusHead* head = op_head(self->opusFile, listIndex);
        if (head->channel_count != 2) {
            CLOG_SOFT_ERROR("wrong channel count");
            return -2;
        }
        self->isFirstDecode = 0;
    }
#endif

    return readSampleCount;
}

int monsoonDecodeAll(Monsoon* self, int16_t* sampleTarget, int maxSampleCountInStereo)
{
    int16_t* target = sampleTarget;
    int remaining = maxSampleCountInStereo;
    int samplesRead = 0;

    while (remaining > 0) {
        int decodedSamplesInStereo = monsoonDecode(self, target, remaining);
        if (decodedSamplesInStereo < 0) {
            return decodedSamplesInStereo;
        }

        if (decodedSamplesInStereo == 0) {
            break;
        }

        samplesRead += decodedSamplesInStereo;
        if (samplesRead > maxSampleCountInStereo) {
            return -1;
        }

        remaining -= decodedSamplesInStereo;
        target += decodedSamplesInStereo * 2;
        // CLOG_VERBOSE("read %d samples (%d) %d", decodedSamplesInStereo, samplesRead, remaining);
    }

    return samplesRead;
}

int monsoonRewind(struct Monsoon* self)
{
    return op_raw_seek(self->opusFile, 0);
}
