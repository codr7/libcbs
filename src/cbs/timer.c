#include <errno.h>
#include "cbs/timer.h"
#include "cbs/time.h"

void ctimer_reset(struct ctimer *t) {
  t->start = cnow();
}

uint64_t ctimer_ms(const struct ctimer *t) {
  const struct timespec end = cnow();

  return
    (end.tv_sec - t->start.tv_sec) * 1000000 +
    (end.tv_nsec - t->start.tv_nsec) / 1000;
}

