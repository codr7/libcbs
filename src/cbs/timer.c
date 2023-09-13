#include <errno.h>
#include "cbs/fail.h"
#include "cbs/timer.h"

static void ctime_get(struct timespec *ts) {
  if (!timespec_get(ts, TIME_UTC)) {
    CPUSH_ERROR(0, "Failed getting time: %d", errno);
  }
}

void ctimer_reset(struct ctimer *t) {
  ctime_get(&t->start);
}

uint64_t ctimer_ms(const struct ctimer *t) {
  struct timespec end;
  ctime_get(&end);

  return
    (end.tv_sec - t->start.tv_sec) * 1000000 +
    (end.tv_nsec - t->start.tv_nsec) / 1000;
}

