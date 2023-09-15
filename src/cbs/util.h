#ifndef CUTIL_H
#define CUTIL_H

#include <stdarg.h>

char *cformat_va(const char *spec, va_list args);
char *cstrdup(const char *in);

#endif
