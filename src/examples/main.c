/*---------------------------------------------------------------------------------------------
 *  Copyright (c) Peter Bjorklund. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/
#include <clog/clog.h>
#include <clog/console.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <monsoon/monsoon.h>

typedef struct context {
    Monsoon monsoon;
} context;

static context self;
clog_config g_clog;
char g_clog_temp_str[CLOG_TEMP_STR_SIZE];
int main(int argc, const char* argv[])
{
    g_clog.level = CLOG_TYPE_VERBOSE;
    g_clog.log = clog_console;
    (void) argc;
    (void) argv;

    CLOG_INFO("monsoon")

    FILE* f = fopen("sample.opus", "rb");
    if (f == 0) {
        CLOG_SOFT_ERROR("could not find sample.opus")
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
        CLOG_SOFT_ERROR("could not open sample.raw for writing")
        return -1;
    }

    int result = monsoonInit(&self.monsoon, 48000, data, fsize);
    if (result < 0) {
        CLOG_SOFT_ERROR("monsoonInit() failed")
        return -1;
    }
    CLOG_INFO("init: %d", result)

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

    CLOG_INFO("worked!")

    return 0;
}
