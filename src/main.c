#include <assert.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cbs/deque.h"
#include "cbs/fail.h"
#include "cbs/malloc.h"
#include "cbs/slog.h"
#include "cbs/timer.h"

static void deque_test() {
  struct cdeque d;
  cdeque_init(&d, 2, sizeof(int));
  const int n = 20;

  for (int i = n/2-1; i >= 0; i--) { *(int *)cdeque_push_front(&d) = i; }
  assert(d.length == n/2);

  for (int i = n/2; i < n; i++) { *(int *)cdeque_push_back(&d) = i; }
  assert(d.length == n);

  for (int i = 0; i < n; i++) { assert(*(int *)cdeque_get(&d, i) == i); }

  for (int i = 0; i < 3; i++) { assert(*(int *)cdeque_pop_front(&d) == i); }
  for (int i = n-1; i >= n-3; i--) { assert(*(int *)cdeque_pop_back(&d) == i); }
  assert(d.length == n-6);

  for (int i = 2; i >= 0; i--) { *(int *)cdeque_push_front(&d) = i; }
  for (int i = 0; i < n-3; i++) { assert(*(int *)cdeque_get(&d, i) == i); }

  cdeque_deinit(&d);
}

static void fail_test() {
  int line = __LINE__+1;
  struct cerror *e = CPUSH_ERROR(1234, "foo %s", "bar");
  assert(e->line == line);
  assert(e->code == 1234);
  assert(strcmp(e->message, "foo bar") == 0);
  assert(cpop_error() == e);
  free(cerror_deinit(e));
}

struct list_item {
  struct clist list;
  int value;
};
  
static void list_test() {
  struct clist head;
  clist_init(&head);

  const int n = 10;
  struct list_item items[n];
  
  for (int i = 0; i < n; i++) {
    items[i].value = i;
    clist_push_back(&head, &items[i].list);
  }

  int i = 0;
  
  CLIST_DO(&head, il) {
    assert(CBASEOF(il, struct list_item, list)->value == i++);
  }
}

static void malloc_test() {
  struct cmalloc m;
  cmalloc_init(&m, 64, sizeof(int));

  int *p1 = cmalloc_acquire(&m);
  int *p2 = cmalloc_acquire(&m);
  cmalloc_release(&m, p1);
  cmalloc_release(&m, p2);
  int *p3 = cmalloc_acquire(&m);
  assert(p3 == p2);
  p3 = cmalloc_acquire(&m);
  assert(p3 == p1);

  cmalloc_deinit(&m);
}

static void slog_test() {
  struct cslog s;
  cslog_init(&s, stdout, CSLOG_TEXT);
  CSLOG(&s, "hello slog", cslog_int("int", 42));
  cslog_deinit(&s);
}

#define MALLOC_BENCH_MAX 1000000

static void malloc_bench() {
  static int *ps[MALLOC_BENCH_MAX];

  struct cmalloc m;
  cmalloc_init(&m, 64, sizeof(int));

  struct ctimer t;
  ctimer_reset(&t);
  
  for (int i = 0; i < MALLOC_BENCH_MAX; i++) {
    ps[i] = malloc(sizeof(int));
    *ps[i] = i;
  }

  for (int i = 0; i < MALLOC_BENCH_MAX; i++) { free(ps[i]); }
  printf("%" PRIu64 "ms\n", ctimer_ms(&t));
  ctimer_reset(&t);
  
  for (int i = 0; i < MALLOC_BENCH_MAX; i++) {
    ps[i] = cmalloc_acquire(&m);
    *ps[i] = i;
  }

  for (int i = 0; i < MALLOC_BENCH_MAX; i++) { cmalloc_release(&m, ps[i]); }
  printf("%" PRIu64 "ms\n", ctimer_ms(&t));
  cmalloc_deinit(&m);
}

int main() {
  deque_test();
  fail_test();
  list_test();
  malloc_test();
  slog_test();
  
  malloc_bench();
}
