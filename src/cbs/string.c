#include <stdlib.h>
#include <string.h>
#include "cbs/string.h"

struct cstring *cstring(const char *data) {
  const size_t length = strlen(data);
  return cstring_init(malloc(CSTRING_SIZE(length)), data, length); 
}

struct cstring *cstring_init(struct cstring *s, const char *data, size_t length) {
  s->length = length;
  strncpy(s->data, data, length);
  s->data[length] = 0;
  return s;
}
