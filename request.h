#ifndef REQUEST_H
#define REQUEST_H

#include <stdint.h>

typedef uint8_t byte;

typedef struct {
    byte opcode;

    byte data[512];
} request_t;

#endif /* REQUEST_H */
