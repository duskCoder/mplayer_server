#include <stdio.h>
#include <stdlib.h>

#include "mplayer_server.h"
#include "request.h"

#define CB(_name) int callback_##_name(const byte *buffer __attribute__((unused)), int size __attribute__((unused)))

CB(load_url);
CB(pause);
CB(quit);
CB(snd_down);
CB(snd_up);
CB(fullscreen);
CB(mute);

extern FILE *stream_g;

typedef struct {
    int opcode;
    int (*cb)(const byte *, int);
} callback_t;

static callback_t callbacks_g[] = {
    {0, callback_load_url},
    {1, callback_pause},
    {2, callback_quit},
    {3, callback_snd_up},
    {4, callback_snd_down},
    {5, callback_fullscreen},
    {6, callback_mute},
};

/*
 * returns a new malloced() null terminated escaped string
 * if the string is considered dangerous, then the function returns NULL
 */

char *real_escape_string(const byte *buf, int size);

void *get_assoc_cb(int opcode)
{
    for (size_t i = 0; i < sizeof(callbacks_g) / sizeof(callbacks_g[0]); ++i) {
        if (callbacks_g[i].opcode == opcode) {
            return callbacks_g[i].cb;
        }
    }

    return NULL;
}

#define SEND_CMD(_fmt, ...) \
    do { \
	_log(_fmt, ##__VA_ARGS__); \
	fprintf(stream_g, _fmt "\n", ##__VA_ARGS__); \
    } while (0)

/* TODO refuse the execution of commands with arguments when they do not need */
CB(load_url)
{
    /* XXX beware, someone may perform arbitrary code injection */
    char *escaped = real_escape_string(buffer, size);

    if (escaped == NULL) {
	return -1;
    }

    SEND_CMD("loadfile '%s'", escaped);

    free(escaped);
    return 0;
}

CB(pause)
{
    SEND_CMD("pause");
    return 0;
}

CB(quit)
{
    SEND_CMD("quit");
    return 0;
}

CB(snd_down)
{
    SEND_CMD("volume -5");
    return 0;
}

CB(snd_up)
{
    SEND_CMD("volume +5");
    return 0;
}

CB(fullscreen)
{
    SEND_CMD("vo_fullscreen");
    return 0;
}

CB(mute)
{
    SEND_CMD("mute");
    return 0;
}
