#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbs/util.h"

char *cstrdup(const char *in) {
  const size_t n = strlen(in)+1;
  char *out = malloc(n);
  memcpy(out, in, n);
  return out;
}

char *cformat_va(const char *spec, va_list args) {
  va_list as;
  va_copy(as, args);
  int n = vsnprintf(NULL, 0, spec, as);
  assert(n > 0);
  va_end(as);

  char *out = malloc(n+1);
  assert(vsnprintf(out, n+1, spec, args) == n);
  return out;
}
