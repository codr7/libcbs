#ifndef CTIMER_H
#define CTIMER_H

#include <stdint.h>
#include <time.h>

struct ctimer {
  struct timespec start;
};

void ctimer_reset(struct ctimer *t);
uint64_t ctimer_ms(const struct ctimer *t);

#endif
