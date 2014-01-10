#ifndef PUB_CALLBACKS_H
#define PUB_CALLBACKS_H

#include <stdint.h>

typedef uint8_t byte;

typedef struct {
    byte opcode;

    byte data[512];
} request_t;

enum {
    CALLBACK_LOAD_URL,
    CALLBACK_PAUSE,
    CALLBACK_QUIT,
    CALLBACK_SND_UP,
    CALLBACK_SND_DOWN,
    CALLBACK_FULLSCREEN,
    CALLBACK_MUTE,

    CALLBACK_COUNT
};

#endif /* PUB_CALLBACKS_H */
