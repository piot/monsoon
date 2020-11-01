#include <opus/opusfile.h>
#include <stdint.h>

typedef struct Monsoon
{
    OggOpusFile *opusFile;
} Monsoon;

int monsoonInit(Monsoon *self, uint32_t frequency, const uint8_t *data, int octetCount);
int monsoonDecode(Monsoon *self, int16_t *sampleTarget, int sampleCount);
int monsoonMinimumSampleBufferSize(const Monsoon *self);
