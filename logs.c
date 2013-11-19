#include <stdarg.h>
#include <stdio.h>

void _log(const char *fmt, ...)
{
#ifdef LOGS
    va_list ap;

    va_start(ap, fmt);

    vfprintf(stderr, fmt, ap);
    fputc('\n', stderr);

    va_end(ap);
#else
    (void) fmt;
#endif
}
