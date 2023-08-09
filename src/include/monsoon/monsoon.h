/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <opusfile.h>
#include <stdint.h>

typedef struct Monsoon {
    OggOpusFile* opusFile;
    int isFirstDecode;
} Monsoon;

int monsoonInit(Monsoon* self, uint32_t frequency, const uint8_t* data, size_t octetCount);
int monsoonDecode(Monsoon* self, int16_t* sampleTarget, size_t sampleCount);
int monsoonDecodeAll(Monsoon* self, int16_t* sampleTarget, size_t sampleCount);
int monsoonMinimumSampleBufferSize(const Monsoon* self);
int monsoonRewind(struct Monsoon* self);
