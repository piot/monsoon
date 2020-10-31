#include <stdint.h>
#include <stdio.h>

#include <monsoon/monsoon.h>

typedef struct context
{
	Monsoon monsoon;
} context;

static context self;

void *g_breathe_init(int argc, const char *argv[], int width, int height)
{
	(void)argc;
	(void)argv;

	printf("monsoon\n");

	FILE *f = fopen("sample.opus", "rb");
	if (f == 0)
	{
		return;
	}
	fseek(f, 0, SEEK_END);
	long fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char *data = malloc(fsize);
	fread(data, 1, fsize, f);
	fclose(f);

	int result = monsoonInit(&self.monsoon, 48000, data, fsize);
	if (result < 0)
	{
		return;
	}
	printf("init: %d\n", result);

	int16_t tempSamples[5076 * 2];

	while (1)
	{
		result = monsoonDecode(&self.monsoon, tempSamples, 5076 * 2);
		printf("decode: %d\n", result);
		if (result <= 0)
		{
			return;
		}
	}

	return &self;
}

int g_breathe_draw(void *_self)
{
	(void)_self;

	return 0;
}

void g_breathe_close(void *app)
{
}
