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

static callback_t callbacks_g[CALLBACK_COUNT] = {NULL};

int callbacks_init(void)
{
    callbacks_g[CALLBACK_LOAD_URL]      = callback_load_url;
    callbacks_g[CALLBACK_PAUSE]         = callback_pause;
    callbacks_g[CALLBACK_QUIT]          = callback_quit;
    callbacks_g[CALLBACK_SND_DOWN]      = callback_snd_down;
    callbacks_g[CALLBACK_SND_UP]        = callback_snd_up;
    callbacks_g[CALLBACK_FULLSCREEN]    = callback_fullscreen;
    callbacks_g[CALLBACK_MUTE]          = callback_mute;

    return 0;
}

/*
 * returns a new malloced() null terminated escaped string
 * if the string is considered dangerous, then the function returns NULL
 */

char *real_escape_string(const byte *buf, int size);

void *get_assoc_cb(int opcode)
{
    if (opcode >= 0 && opcode < CALLBACK_COUNT) {
        return callbacks_g[opcode];
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
