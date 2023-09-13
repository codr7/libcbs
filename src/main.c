#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbs/fail.h"
#include "cbs/slab.h"
#include "cbs/timer.h"

static void fail_test() {
  int line = __LINE__+1;
  struct cerror *e = CPUSH_ERROR(1234, "foo %s", "bar");
  assert(e->line == line);
  assert(e->code == 1234);
  assert(strcmp(e->message, "foo bar") == 0);
  assert(cpop_error() == e);
  free(cerror_deinit(e));
}

#define SLAB_BENCH_MAX 1000000

static void slab_test() {
  static int *ps[SLAB_BENCH_MAX];

  struct cslab_alloc a;
  cslab_alloc_init(&a, 64, sizeof(int));
  int *p1 = cslab_malloc(&a);
  int *p2 = cslab_malloc(&a);
  cslab_free(&a, p1);
  cslab_free(&a, p2);
  int *p3 = cslab_malloc(&a);
  assert(p3 == p2);
  p3 = cslab_malloc(&a);
  assert(p3 == p1);

  struct ctimer t;
  ctimer_reset(&t);
  
  for (int i = 0; i < SLAB_BENCH_MAX; i++) {
    ps[i] = malloc(sizeof(int));
    *ps[i] = i;
  }

  for (int i = 0; i < SLAB_BENCH_MAX; i++) { free(ps[i]); }
  printf("%" PRIu64 "ms\n", ctimer_ms(&t));
  ctimer_reset(&t);
  
  for (int i = 0; i < SLAB_BENCH_MAX; i++) {
    ps[i] = cslab_malloc(&a);
    *ps[i] = i;
  }

  for (int i = 0; i < SLAB_BENCH_MAX; i++) { cslab_free(&a, ps[i]); }
  printf("%" PRIu64 "ms\n", ctimer_ms(&t));
  cslab_alloc_deinit(&a);
}

int main() {
  fail_test();
  slab_test();
}
