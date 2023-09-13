#ifndef CUTIL_H
#define CUTIL_H

#include <stdarg.h>

char *cstrdup(const char *in);
char *cformat_va(const char *spec, va_list args);

#endif
