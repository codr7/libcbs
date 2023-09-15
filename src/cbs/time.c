#include <assert.h>
#include "cbs/time.h"

struct timespec cnow() {
  struct timespec t;
  assert(timespec_get(&t, TIME_UTC));
  return t;
}

void cput_timestamp(struct timespec ts, FILE *out) {
  char buf[20] = {0};
  struct tm tm;
  localtime_r(&(ts.tv_sec), &tm);
  strftime(buf, 20, "%F %T", &tm);
  fputs(buf, out);
}

