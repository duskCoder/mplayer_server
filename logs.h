#ifndef LOGS_H
#define LOGS_H

#include <stdarg.h>

__attribute__((format(printf, 1, 2)))
void _log(const char *fmt, ...);

#endif /* LOGS_H */
