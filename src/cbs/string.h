#ifndef CSTRING_H
#define CSTRING_H

#include <stddef.h>

#define CSTRING_SIZE(_length)		\
  (sizeof(struct cstring) + _length + 1)

struct cstring {
  size_t length;
  char data[];
};

struct cstring *cstring(const char *data);
struct cstring *cstring_init(struct cstring *s, const char *data, size_t length);

#endif
