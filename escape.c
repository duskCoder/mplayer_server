#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "request.h"

char *real_escape_string(const byte *src, int size)
{
    char *escaped;
    char *pos;

    if (memchr(src, '\0', size - 1) || memchr(src, '\n', size) || memchr(src, '\r', size)) {
	fprintf(stderr, "cannot handle this string\n");
	return NULL;
    }

    pos = escaped = malloc(sizeof(char) * (size * 2 + 1));

    if (escaped == NULL) {
	fprintf(stderr, "malloc failed: %m\n");
	return NULL;
    }

    for (int i = 0; i < size; ++i) {
	if (!isalnum(src[i])) {
	    *escaped++ = '\\';
	}
	*escaped++ = src[i];
    }

    *escaped = '\0';

    return pos;
}
