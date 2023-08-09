/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <monsoon/monsoon.h>

typedef struct context {
    Monsoon monsoon;
} context;

static context self;

int main(int argc, const char* argv[])
{
    (void) argc;
    (void) argv;

    printf("monsoon\n");

    FILE* f = fopen("sample.opus", "rb");
    if (f == 0) {
        return 0;
    }

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    const uint8_t* data = malloc(fsize);
    fread((void*) data, 1, fsize, f);
    fclose(f);

    FILE* of = fopen("sample.raw", "wb");
    if (of == 0) {
        return 0;
    }

    int result = monsoonInit(&self.monsoon, 48000, data, fsize);
    if (result < 0) {
        return 0;
    }
    printf("init: %d\n", result);

    int minimumSampleCount = monsoonMinimumSampleBufferSize(&self.monsoon);

    int16_t* tempSamples = malloc(sizeof(int16_t) * 2 * minimumSampleCount);

    while (1) {
        int decodedSamples = monsoonDecode(&self.monsoon, tempSamples, minimumSampleCount);
        // printf("decode: %d\n", decodedSamples);
        if (decodedSamples <= 0) {
            break;
        }
        fwrite(tempSamples, sizeof(int16_t) * 2, decodedSamples, of);
    }

    fclose(of);
    free(tempSamples);

    return &self;
}
